/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/gridsel.h
// Purpose:     wxGridSelection
// Author:      Stefan Neis
// Modified by:
// Created:     20/02/2000
// Copyright:   (c) Stefan Neis
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_GRIDSEL_H_
#define _WX_GENERIC_GRIDSEL_H_

#include "wx/defs.h"

#if wxUSE_GRID

#include "wx/grid.h"

#include "wx/vector.h"

typedef wxVector<wxGridBlockCoords> wxVectorGridBlockCoords;

class WXDLLIMPEXP_CORE wxGridSelection
{
public:
    wxGridSelection(wxGrid *grid,
                    wxGrid::wxGridSelectionModes sel = wxGrid::wxGridSelectCells);

    bool IsSelection();
    bool IsInSelection(int row, int col);
    bool IsInSelection(const wxGridCellCoords& coords)
    {
        return IsInSelection(coords.GetRow(), coords.GetCol());
    }

    void SetSelectionMode(wxGrid::wxGridSelectionModes selmode);
    wxGrid::wxGridSelectionModes GetSelectionMode() { return m_selectionMode; }
    void SelectRow(int row, const wxKeyboardState& kbd = wxKeyboardState());
    void SelectCol(int col, const wxKeyboardState& kbd = wxKeyboardState());
    void SelectBlock(int topRow, int leftCol,
                     int bottomRow, int rightCol,
                     const wxKeyboardState& kbd = wxKeyboardState(),
                     bool sendEvent = true );
    void SelectBlock(const wxGridCellCoords& topLeft,
                     const wxGridCellCoords& bottomRight,
                     const wxKeyboardState& kbd = wxKeyboardState(),
                     bool sendEvent = true )
    {
        SelectBlock(topLeft.GetRow(), topLeft.GetCol(),
                    bottomRight.GetRow(), bottomRight.GetCol(),
                    kbd, sendEvent);
    }

    // This function replaces all the existing selected blocks (which become
    // redundant) with a single block covering the entire grid.
    void SelectAll();

    void DeselectBlock(const wxGridBlockCoords& block,
                       const wxKeyboardState& kbd = wxKeyboardState(),
                       bool sendEvent = true );

    // Note that this method refreshes the previously selected blocks and sends
    // an event about the selection change.
    void ClearSelection();

    void UpdateRows( size_t pos, int numRows );
    void UpdateCols( size_t pos, int numCols );

    // Extend (or shrink) the current selection block to the one specified by
    // the start and end coordinates of its opposite corners (which don't have
    // to be in top/bottom left/right order).
    //
    // Note that blockStart is equal to wxGrid::m_currentCellCoords almost
    // always, but not always (the exception is when we scrolled out from
    // the top of the grid and select a column or scrolled right and select
    // a row: in this case the lowest visible row/column will be set as
    // current, not the first one).
    //
    // Both components of both blockStart and blockEnd must be valid.
    //
    // Return true if the current block was actually changed.
    bool ExtendOrCreateCurrentBlock(const wxGridCellCoords& blockStart,
                                    const wxGridCellCoords& blockEnd,
                                    const wxKeyboardState& kbd);


    // Return the row of the current selection block if it exists and we can
    // edit the block vertically. Otherwise return -1.
    int GetCurrentBlockCornerRow() const;
    // Return the column of the current selection block if it exists and we can
    // edit the block horizontally. Otherwise return -1.
    int GetCurrentBlockCornerCol() const;

    wxGridCellCoordsArray GetCellSelection() const;
    wxGridCellCoordsArray GetBlockSelectionTopLeft() const;
    wxGridCellCoordsArray GetBlockSelectionBottomRight() const;
    wxArrayInt GetRowSelection() const;
    wxArrayInt GetColSelection() const;

    wxVectorGridBlockCoords& GetBlocks() { return m_selection; }

private:
    void SelectBlockNoEvent(const wxGridBlockCoords& block)
    {
        SelectBlock(block.GetTopRow(), block.GetLeftCol(),
                    block.GetBottomRow(), block.GetRightCol(),
                    wxKeyboardState(), false);
    }

    // Really select the block and don't check for the current selection mode.
    void Select(const wxGridBlockCoords& block,
                const wxKeyboardState& kbd, bool sendEvent);

    // Ensure that the new "block" becomes part of "blocks", adding it to them
    // if necessary and, if we do it, also removing any existing elements of
    // "blocks" that become unnecessary because they're entirely contained in
    // the new "block". However note that we may also not to have to add it at
    // all, if it's already contained in one of the existing blocks.
    //
    // We don't currently check if the new block is contained by several
    // existing blocks, as this would be more difficult and doesn't seem to be
    // really needed in practice.
    void MergeOrAddBlock(wxVectorGridBlockCoords& blocks,
                         const wxGridBlockCoords& block);

    // All currently selected blocks. We expect there to be a relatively small
    // amount of them, even for very large grids, as each block must be
    // selected by the user, so we store them unsorted.
    //
    // Selection may be empty, but if it isn't, the last block is special, as
    // it is the current block, which is affected by operations such as
    // extending the current selection from keyboard.
    wxVectorGridBlockCoords             m_selection;

    wxGrid                              *m_grid;
    wxGrid::wxGridSelectionModes        m_selectionMode;

    wxDECLARE_NO_COPY_CLASS(wxGridSelection);
};

#endif  // wxUSE_GRID
#endif  // _WX_GENERIC_GRIDSEL_H_
