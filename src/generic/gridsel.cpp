///////////////////////////////////////////////////////////////////////////
// Name:        src/generic/gridsel.cpp
// Purpose:     wxGridSelection
// Author:      Stefan Neis
// Modified by:
// Created:     20/02/1999
// Copyright:   (c) Stefan Neis (Stefan.Neis@t-online.de)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if wxUSE_GRID

#include "wx/generic/gridsel.h"
#include "wx/dynarray.h"


namespace
{

// The helper function to compare wxIntSortedArray elements.
int CompareInts(int n1, int n2)
{
    return n1 - n2;
}

}

WX_DEFINE_SORTED_ARRAY(int, wxIntSortedArray);


wxGridSelection::wxGridSelection( wxGrid * grid,
                                  wxGrid::wxGridSelectionModes sel )
{
    m_grid = grid;
    m_selectionMode = sel;
}

bool wxGridSelection::IsSelection()
{
    return !m_selection.empty();
}

bool wxGridSelection::IsInSelection( int row, int col )
{
    // Check whether the given cell is contained in one of the selected blocks.
    //
    // Note that this algorithm is O(N) in number of selected blocks, not in
    // number of cells in the grid, so it should be reasonably efficient even
    // for very large grids, as the user shouldn't be able to select too many
    // blocks. If we still run into problems with this, we should find a more
    // efficient way of storing the selection, e.g. using k-d trees.
    const size_t count = m_selection.size();
    for ( size_t n = 0; n < count; n++ )
    {
        if ( m_selection[n].ContainsCell(wxGridCellCoords(row, col)) )
            return true;
    }

    return false;
}

// Change the selection mode
void wxGridSelection::SetSelectionMode( wxGrid::wxGridSelectionModes selmode )
{
    // if selection mode is unchanged return immediately
    if (selmode == m_selectionMode)
        return;

    if ( m_selectionMode != wxGrid::wxGridSelectCells )
    {
        // if changing form row to column selection
        // or vice versa, clear the selection.
        if ( selmode != wxGrid::wxGridSelectCells )
            ClearSelection();

        m_selectionMode = selmode;
    }
    else
    {
        // Preserve only fully selected rows/columns when switching from cell
        // selection mode and discard the selected blocks that are invalid in
        // the new selection mode.
        const int lastCol = m_grid->GetNumberCols() - 1;
        const int lastRow = m_grid->GetNumberRows() - 1;
        for ( size_t n = m_selection.size(); n > 0; )
        {
            n--;
            const wxGridBlockCoords& block = m_selection[n];
            const int topRow = block.GetTopRow();
            const int leftCol = block.GetLeftCol();
            const int bottomRow = block.GetBottomRow();
            const int rightCol = block.GetRightCol();

            bool valid = false;
            switch ( selmode )
            {
                case wxGrid::wxGridSelectCells:
                    wxFAIL_MSG("unreachable");
                    break;

                case wxGrid::wxGridSelectRows:
                    valid = leftCol == 0 && rightCol == lastCol;
                    break;

                case wxGrid::wxGridSelectColumns:
                    valid = topRow == 0 && bottomRow == lastRow;
                    break;

                case wxGrid::wxGridSelectRowsOrColumns:
                    valid = (leftCol == 0 && rightCol == lastCol) ||
                            (topRow == 0 && bottomRow == lastRow);
                    break;
            }

            if ( !valid )
                m_selection.erase(m_selection.begin() + n);
        }

        m_selectionMode = selmode;
    }
}

void wxGridSelection::SelectRow(int row, const wxKeyboardState& kbd)
{
    if ( m_selectionMode == wxGrid::wxGridSelectColumns )
        return;

    Select(wxGridBlockCoords(row, 0, row, m_grid->GetNumberCols() - 1),
           kbd, true);
}

void wxGridSelection::SelectCol(int col, const wxKeyboardState& kbd)
{
    if ( m_selectionMode == wxGrid::wxGridSelectRows )
        return;

    Select(wxGridBlockCoords(0, col, m_grid->GetNumberRows() - 1, col),
           kbd, true);
}

void wxGridSelection::SelectBlock( int topRow, int leftCol,
                                   int bottomRow, int rightCol,
                                   const wxKeyboardState& kbd,
                                   bool sendEvent )
{
    // Fix the coordinates of the block if needed.
    int allowed = -1;
    switch ( m_selectionMode )
    {
        case wxGrid::wxGridSelectCells:
            // In this mode arbitrary blocks can be selected.
            allowed = 1;
            break;

        case wxGrid::wxGridSelectRows:
            leftCol = 0;
            rightCol = m_grid->GetNumberCols() - 1;
            allowed = 1;
            break;

        case wxGrid::wxGridSelectColumns:
            topRow = 0;
            bottomRow = m_grid->GetNumberRows() - 1;
            allowed = 1;
            break;

        case wxGrid::wxGridSelectRowsOrColumns:
            // block selection doesn't make sense for this mode, we could only
            // select the entire grid but this wouldn't be useful
            allowed = 0;
            break;
    }

    wxASSERT_MSG(allowed != -1, "unknown selection mode?");
    if ( !allowed )
        return;

    Select(wxGridBlockCoords(topRow, leftCol, bottomRow, rightCol).Canonicalize(),
           kbd, sendEvent);
}

void
wxGridSelection::DeselectBlock(const wxGridBlockCoords& block,
                               const wxKeyboardState& kbd,
                               bool sendEvent)
{
    const wxGridBlockCoords canonicalizedBlock = block.Canonicalize();

    size_t count, n;

    // If the selected block intersects with the deselection block, split it
    // in up to 4 new parts, that don't contain the block to be selected, like
    // this (for rows):
    // |---------------------------|
    // |                           |
    // |           part 1          |
    // |                           |
    // |---------------------------|
    // | part 3 |    x    | part 4 |
    // |---------------------------|
    // |                           |
    // |           part 2          |
    // |                           |
    // |---------------------------|
    // And for columns:
    // |---------------------------|
    // |        |         |        |
    // |        | part 3  |        |
    // |        |         |        |
    // |        |---------|        |
    // | part 1 |    x    | part 2 |
    // |        |---------|        |
    // |        |         |        |
    // |        | part 4  |        |
    // |        |         |        |
    // |---------------------------|
    //   (The x marks the newly deselected block).
    // Note: in row/column selection mode, we only need part1 and part2

    // Blocks to refresh.
    wxVectorGridBlockCoords refreshBlocks;
    // Add the deselected block.
    refreshBlocks.push_back(canonicalizedBlock);

    count = m_selection.size();
    for ( n = 0; n < count; n++ )
    {
        const wxGridBlockCoords& selBlock = m_selection[n];

        // Whether blocks intersect.
        if ( !m_selection[n].Intersects(canonicalizedBlock) )
            continue;

        int splitOrientation = wxHORIZONTAL;
        switch ( m_selectionMode )
        {
        case wxGrid::wxGridSelectCells:
            if ( selBlock.GetLeftCol() == 0 &&
                 selBlock.GetRightCol() == m_grid->GetNumberCols() - 1 )
                break;

            if ( selBlock.GetTopRow() == 0 &&
                 selBlock.GetBottomRow() == m_grid->GetNumberRows() - 1 )
                splitOrientation = wxVERTICAL;

            break;

        case wxGrid::wxGridSelectColumns:
            splitOrientation = wxVERTICAL;
            break;

        case wxGrid::wxGridSelectRowsOrColumns:
            if ( selBlock.GetLeftCol() == 0 &&
                 selBlock.GetRightCol() == m_grid->GetNumberCols() - 1 )
                break;

            splitOrientation = wxVERTICAL;
            break;
        }

        // remove the block
        m_selection.erase(m_selection.begin() + n);
        n--;
        count--;

        wxGridBlockDiffResult result =
            selBlock.Difference(canonicalizedBlock, splitOrientation);

        for ( int i = 0; i < 2; ++i )
        {
            const wxGridBlockCoords& part = result.m_parts[i];
            if ( part != wxGridNoBlockCoords )
                SelectBlockNoEvent(part);
        }

        for ( int i = 2; i < 4; ++i )
        {
            const wxGridBlockCoords& part = result.m_parts[i];
            if ( part != wxGridNoBlockCoords )
            {
                // Add part[2] and part[3] only in the cells selection mode.
                if ( m_selectionMode == wxGrid::wxGridSelectCells )
                    SelectBlockNoEvent(part);
                else
                    MergeOrAddBlock(refreshBlocks, part);
            }
        }
    }

    // Refresh the screen and send events.
    count = refreshBlocks.size();
    for ( n = 0; n < count; n++ )
    {
        const wxGridBlockCoords& refBlock = refreshBlocks[n];

        if ( !m_grid->GetBatchCount() )
        {
            m_grid->RefreshBlock(refBlock.GetTopRow(), refBlock.GetLeftCol(),
                                 refBlock.GetBottomRow(), refBlock.GetRightCol());
        }

        if ( sendEvent )
        {
            wxGridRangeSelectEvent gridEvt(m_grid->GetId(),
                                           wxEVT_GRID_RANGE_SELECT,
                                           m_grid,
                                           refBlock.GetTopLeft(),
                                           refBlock.GetBottomRight(),
                                           false,
                                           kbd);
            m_grid->GetEventHandler()->ProcessEvent(gridEvt);
        }
    }
}

void wxGridSelection::ClearSelection()
{
    size_t n;
    wxRect r;
    wxGridCellCoords coords1, coords2;

    // deselect all blocks and update the screen
    while ( ( n = m_selection.size() ) > 0)
    {
        n--;
        const wxGridBlockCoords& block = m_selection[n];
        coords1 = block.GetTopLeft();
        coords2 = block.GetBottomRight();
        m_selection.erase(m_selection.begin() + n);
        if ( !m_grid->GetBatchCount() )
        {
            m_grid->RefreshBlock(coords1, coords2);

#ifdef __WXMAC__
            m_grid->UpdateGridWindows();
#endif
        }
    }

    // One deselection event, indicating deselection of _all_ cells.
    // (No finer grained events for each of the smaller regions
    //  deselected above!)
    wxGridRangeSelectEvent gridEvt( m_grid->GetId(),
                                    wxEVT_GRID_RANGE_SELECT,
                                    m_grid,
                                    wxGridCellCoords( 0, 0 ),
                                    wxGridCellCoords(
                                        m_grid->GetNumberRows() - 1,
                                        m_grid->GetNumberCols() - 1 ),
                                    false );

    m_grid->GetEventHandler()->ProcessEvent(gridEvt);
}


void wxGridSelection::UpdateRows( size_t pos, int numRows )
{
    size_t count = m_selection.size();
    size_t n;

    for ( n = 0; n < count; n++ )
    {
        wxGridBlockCoords& block = m_selection[n];
        wxCoord row1 = block.GetTopRow();
        wxCoord row2 = block.GetBottomRow();

        if ((size_t)row2 >= pos)
        {
            if (numRows > 0)
            {
                // If rows inserted, increase row counter where necessary
                block.SetBottomRow( row2 + numRows );
                if ((size_t)row1 >= pos)
                    block.SetTopRow( row1 + numRows );
            }
            else if (numRows < 0)
            {
                // If rows deleted ...
                if ((size_t)row2 >= pos - numRows)
                {
                    // ...either decrement row counter (if row still exists)...
                    block.SetBottomRow( row2 + numRows );
                    if ((size_t)row1 >= pos)
                        block.SetTopRow( wxMax(row1 + numRows, (int)pos) );

                }
                else
                {
                    if ((size_t)row1 >= pos)
                    {
                        // ...or remove the attribute
                        m_selection.erase(m_selection.begin() + n);
                        n--;
                        count--;
                    }
                    else
                        block.SetBottomRow( pos );
                }
            }
        }
    }
}


void wxGridSelection::UpdateCols( size_t pos, int numCols )
{
    size_t count = m_selection.size();
    size_t n;

    for ( n = 0; n < count; n++ )
    {
        wxGridBlockCoords& block = m_selection[n];
        wxCoord col1 = block.GetLeftCol();
        wxCoord col2 = block.GetRightCol();

        if ((size_t)col2 >= pos)
        {
            if (numCols > 0)
            {
                // If rows inserted, increase row counter where necessary
                block.SetRightCol(col2 + numCols);
                if ((size_t)col1 >= pos)
                    block.SetLeftCol(col1 + numCols);
            }
            else if (numCols < 0)
            {
                // If cols deleted ...
                if ((size_t)col2 >= pos - numCols)
                {
                    // ...either decrement col counter (if col still exists)...
                    block.SetRightCol(col2 + numCols);
                    if ( (size_t) col1 >= pos)
                        block.SetLeftCol( wxMax(col1 + numCols, (int)pos) );

                }
                else
                {
                    if ((size_t)col1 >= pos)
                    {
                        // ...or remove the attribute
                        m_selection.erase(m_selection.begin() + n);
                        n--;
                        count--;
                    }
                    else
                        block.SetRightCol(pos);
                }
            }
        }
    }
}

bool wxGridSelection::ExtendOrCreateCurrentBlock(const wxGridCellCoords& blockStart,
                                                 const wxGridCellCoords& blockEnd,
                                                 const wxKeyboardState& kbd)
{
    if ( m_selection.empty() )
    {
        SelectBlock(blockStart, blockEnd);
        return true;
    }

    wxGridBlockCoords& block = *m_selection.rbegin();
    wxGridBlockCoords newBlock = block;

    bool editBlock = false;

    if ( blockEnd.GetRow() != -1 )
    {
        if ( newBlock.GetTopRow() == blockStart.GetRow() )
        {
            newBlock.SetBottomRow(blockEnd.GetRow());
            editBlock = true;
        }
        else if ( newBlock.GetBottomRow() == blockStart.GetRow() )
        {
            newBlock.SetTopRow(blockEnd.GetRow());
            editBlock = true;
        }
    }
    if ( blockEnd.GetCol() != -1 )
    {
        if ( newBlock.GetLeftCol() == blockStart.GetCol() )
        {
            newBlock.SetRightCol(blockEnd.GetCol());
            editBlock = true;
        }
        else if ( newBlock.GetRightCol() == blockStart.GetCol() )
        {
            newBlock.SetLeftCol(blockEnd.GetCol());
            editBlock = true;
        }
    }

    newBlock = newBlock.Canonicalize();

    const bool endCoordsSet =
        blockEnd.GetRow() != -1 && blockEnd.GetCol() != -1;

    if ( editBlock )
    {
        if ( newBlock == block )
            return false;

        // Update View.
        if ( !m_grid->GetBatchCount() )
        {
            wxGridBlockDiffResult refreshBlocks = block.SymDifference(newBlock);
            for ( int i = 0; i < 4; ++i )
            {
                const wxGridBlockCoords& refreshBlock = refreshBlocks.m_parts[i];
                m_grid->RefreshBlock(refreshBlock.GetTopLeft(),
                                     refreshBlock.GetBottomRight());
            }
        }

        // Edit the current block.
        block = newBlock;

        // Send Event.
        wxGridRangeSelectEvent gridEvt(m_grid->GetId(),
                                        wxEVT_GRID_RANGE_SELECT,
                                        m_grid,
                                        newBlock.GetTopLeft(),
                                        newBlock.GetBottomRight(),
                                        true,
                                        kbd);
        m_grid->GetEventHandler()->ProcessEvent(gridEvt);
        return true;
    }
    else if ( endCoordsSet )
    {
        // Select the new one.
        SelectBlock(newBlock.GetTopLeft(), newBlock.GetBottomRight(), kbd);
        return true;
    }
    return false;
}

int wxGridSelection::GetCurrentBlockCornerRow() const
{
    if ( m_selection.empty() )
        return -1;

    const wxGridBlockCoords& block = *m_selection.rbegin();
    if ( block.GetTopRow() == m_grid->m_currentCellCoords.GetRow() )
        return block.GetBottomRow();
    if ( block.GetBottomRow() == m_grid->m_currentCellCoords.GetRow() )
        return block.GetTopRow();

    return -1;
}

int wxGridSelection::GetCurrentBlockCornerCol() const
{
    if ( m_selection.empty() )
        return -1;

    const wxGridBlockCoords& block = *m_selection.rbegin();
    if ( block.GetLeftCol() == m_grid->m_currentCellCoords.GetCol() )
        return block.GetRightCol();
    if ( block.GetRightCol() == m_grid->m_currentCellCoords.GetCol() )
        return block.GetLeftCol();

    return -1;
}

wxGridCellCoordsArray wxGridSelection::GetCellSelection() const
{
    if ( m_selectionMode != wxGrid::wxGridSelectCells )
        return wxGridCellCoordsArray();

    wxGridCellCoordsArray cells;
    const size_t count = m_selection.size();
    cells.reserve(count);
    for ( size_t n = 0; n < count; n++ )
    {
        const wxGridBlockCoords& block = m_selection[n];
        if ( block.GetTopRow() == block.GetBottomRow() &&
             block.GetLeftCol() == block.GetRightCol() )
        {
            cells.push_back(block.GetTopLeft());
        }
    }
    return cells;
}

wxGridCellCoordsArray wxGridSelection::GetBlockSelectionTopLeft() const
{
    // return blocks only in wxGridSelectCells selection mode
    if ( m_selectionMode != wxGrid::wxGridSelectCells )
        return wxGridCellCoordsArray();

    wxGridCellCoordsArray coords;
    const size_t count = m_selection.size();
    coords.reserve(count);
    for ( size_t n = 0; n < count; n++ )
    {
        coords.push_back(m_selection[n].GetTopLeft());
    }
    return coords;
}

wxGridCellCoordsArray wxGridSelection::GetBlockSelectionBottomRight() const
{
    if ( m_selectionMode != wxGrid::wxGridSelectCells )
        return wxGridCellCoordsArray();

    wxGridCellCoordsArray coords;
    const size_t count = m_selection.size();
    coords.reserve(count);
    for ( size_t n = 0; n < count; n++ )
    {
        coords.push_back(m_selection[n].GetBottomRight());
    }
    return coords;
}

// For compatibility with the existing code, try reconstructing the selected
// rows/columns from the selected blocks we store internally. Of course, this
// only works well in the corresponding selection mode in which the user can
// only select the entire lines in the first place, as otherwise it's difficult to
// efficiently determine that a line is selected because all of its cells
// were selected one by one. But this should work well enough in practice and
// is, anyhow, the best we can do.
wxArrayInt wxGridSelection::GetRowSelection() const
{
    if ( m_selectionMode == wxGrid::wxGridSelectColumns )
        return wxArrayInt();

    wxIntSortedArray uniqueRows(&CompareInts);
    const size_t count = m_selection.size();
    for ( size_t n = 0; n < count; ++n )
    {
        const wxGridBlockCoords& block = m_selection[n];
        if ( block.GetLeftCol() == 0 &&
             block.GetRightCol() == m_grid->GetNumberCols() - 1 )
        {
            for ( int r = block.GetTopRow(); r <= block.GetBottomRow(); ++r )
            {
                uniqueRows.Add(r);
            }
        }
    }

    wxArrayInt result;
    result.reserve(uniqueRows.size());
    for( size_t i = 0; i < uniqueRows.size(); ++i )
    {
        result.push_back(uniqueRows[i]);
    }
    return result;
}

// See comments for GetRowSelection().
wxArrayInt wxGridSelection::GetColSelection() const
{
    if ( m_selectionMode == wxGrid::wxGridSelectRows )
        return wxArrayInt();

    wxIntSortedArray uniqueRows(&CompareInts);
    const size_t count = m_selection.size();
    for ( size_t n = 0; n < count; ++n )
    {
        const wxGridBlockCoords& block = m_selection[n];
        if ( block.GetTopRow() == 0 &&
             block.GetBottomRow() == m_grid->GetNumberRows() - 1 )
        {
            for ( int c = block.GetLeftCol(); c <= block.GetRightCol(); ++c )
            {
                uniqueRows.Add(c);
            }
        }
    }

    wxArrayInt result;
    result.reserve(uniqueRows.size());
    for( size_t i = 0; i < uniqueRows.size(); ++i )
    {
        result.push_back(uniqueRows[i]);
    }
    return result;
}

int wxGridSelection::BlockContain( int topRow1, int leftCol1,
                                   int bottomRow1, int rightCol1,
                                   int topRow2, int leftCol2,
                                   int bottomRow2, int rightCol2 )
// returns 1, if Block1 contains Block2,
//        -1, if Block2 contains Block1,
//         0, otherwise
{
    if ( topRow1 <= topRow2 && bottomRow2 <= bottomRow1 &&
         leftCol1 <= leftCol2 && rightCol2 <= rightCol1 )
        return 1;
    else if ( topRow2 <= topRow1 && bottomRow1 <= bottomRow2 &&
              leftCol2 <= leftCol1 && rightCol1 <= rightCol2 )
        return -1;

    return 0;
}

void
wxGridSelection::Select(const wxGridBlockCoords& block,
                        const wxKeyboardState& kbd, bool sendEvent)
{
    if (m_grid->GetNumberRows() == 0 || m_grid->GetNumberCols() == 0)
        return;

    m_selection.push_back(block);

    // Update View:
    if ( !m_grid->GetBatchCount() )
    {
        m_grid->RefreshBlock(block.GetTopLeft(), block.GetBottomRight());
    }

    // Send Event, if not disabled.
    if ( sendEvent )
    {
        wxGridRangeSelectEvent gridEvt( m_grid->GetId(),
            wxEVT_GRID_RANGE_SELECT,
            m_grid,
            block.GetTopLeft(),
            block.GetBottomRight(),
            true,
            kbd);
        m_grid->GetEventHandler()->ProcessEvent( gridEvt );
    }
}

void wxGridSelection::MergeOrAddBlock(wxVectorGridBlockCoords& blocks,
                                      const wxGridBlockCoords& newBlock)
{
    size_t count = blocks.size();
    for ( size_t n = 0; n < count; n++ )
    {
        const wxGridBlockCoords& block = blocks[n];

        if ( block.ContainsBlock(newBlock) )
            return;

        if ( newBlock.ContainsBlock(block) )
        {
            blocks.erase(blocks.begin() + n);
            n--;
            count--;
        }
    }

    blocks.push_back(newBlock);
}

#endif
