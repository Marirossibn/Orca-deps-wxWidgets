///////////////////////////////////////////////////////////////////////////////
// Name:        bombs1.cpp
// Purpose:     Bombs game
// Author:      P. Foggia 1996
// Modified by: Wlodzimierz Skiba (ABX) 2003
// Created:     1996
// RCS-ID:      $Id$
// Copyright:   (c) 1996 P. Foggia
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/*
 * implementation of the methods DrawField and OnEvent of the
 * class BombsCanvas
 */

#ifdef __GNUG__
#   pragma implementation
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif //precompiled headers

#include "bombs.h"

// Draws the field on the device context dc
// xc1,yc1 etc. are the (inclusive) limits of the area to be drawn,
// expressed in cells.
void BombsCanvas::DrawField(wxDC *dc, int xc1, int yc1, int xc2, int yc2)
{
    wxString buf;
    long chw, chh;

    wxColour wxBlack = wxTheColourDatabase->Find(wxT("BLACK"));
    wxColour wxWhite = wxTheColourDatabase->Find(wxT("WHITE"));
    wxColour wxRed = wxTheColourDatabase->Find(wxT("RED"));
    wxColour wxBlue = wxTheColourDatabase->Find(wxT("BLUE"));
    wxColour wxGrey = wxTheColourDatabase->Find(wxT("LIGHT GREY"));
    wxColour wxFocused = wxTheColourDatabase->Find(wxT("GREY"));
    wxColour wxGreen = wxTheColourDatabase->Find(wxT("GREEN"));

    wxPen *blackPen = wxThePenList->FindOrCreatePen(wxBlack, 1, wxSOLID);
    wxPen *redPen = wxThePenList->FindOrCreatePen(wxRed, 1, wxSOLID);
    wxPen *bluePen = wxThePenList->FindOrCreatePen(wxBlue, 1, wxSOLID);
    wxBrush *whiteBrush = wxTheBrushList->FindOrCreateBrush(wxWhite, wxSOLID);
    wxBrush *greyBrush = wxTheBrushList->FindOrCreateBrush(wxGrey, wxSOLID);
    wxBrush *focusedBrush = wxTheBrushList->FindOrCreateBrush(wxFocused, wxSOLID);
    wxBrush *redBrush = wxTheBrushList->FindOrCreateBrush(wxRed, wxSOLID);

    dc->SetPen(* blackPen);

    int x, y;
    int xMax = this->GetGridSizeInPixels().GetWidth();
    int yMax = this->GetGridSizeInPixels().GetHeight();
    for(x=xc1; x<=xc2; x++)
        dc->DrawLine(x*m_cellWidth*X_UNIT, 0, x*m_cellWidth*X_UNIT, yMax);
    for(y=xc1; y<=yc2; y++)
        dc->DrawLine(0, y*m_cellHeight*Y_UNIT, xMax, y*m_cellHeight*Y_UNIT);


    wxFont font= BOMBS_FONT;
    dc->SetFont(font); 

    for(x=xc1; x<=xc2; x++)
        for(y=yc1; y<=yc2; y++)
        {
            if (m_game->IsMarked(x,y))
            {
                dc->SetPen(* blackPen);

                if (m_game->IsFocussed(x, y))
                    dc->SetBrush(* focusedBrush);
                else
                    dc->SetBrush(* greyBrush);

                dc->DrawRectangle( x*m_cellWidth*X_UNIT, y*m_cellHeight*Y_UNIT,
                    m_cellWidth*X_UNIT+1, m_cellHeight*Y_UNIT+1);
                buf = wxT("M");
                if (!m_game->IsHidden(x,y) && m_game->IsBomb(x,y))
                    dc->SetTextForeground(wxBlue);
                else
                    dc->SetTextForeground(wxRed);

                dc->SetTextBackground(wxGrey);
                dc->GetTextExtent(buf, &chw, &chh);
                dc->DrawText( buf,
                    x*m_cellWidth*X_UNIT + (m_cellWidth*X_UNIT-chw)/2,
                    y*m_cellHeight*Y_UNIT + (m_cellHeight*Y_UNIT-chh)/2 );

                if (!m_game->IsHidden(x,y) && m_game->IsBomb(x,y))
                {
                    dc->SetPen(*redPen);
                    dc->DrawLine(x*m_cellWidth*X_UNIT, y*m_cellHeight*Y_UNIT,
                        (x+1)*m_cellWidth*X_UNIT, (y+1)*m_cellHeight*Y_UNIT);
                    dc->DrawLine(x*m_cellWidth*X_UNIT, (y+1)*m_cellHeight*Y_UNIT,
                        (x+1)*m_cellWidth*X_UNIT, y*m_cellHeight*Y_UNIT);
                }
            }
            else if (m_game->IsHidden(x,y))
            {
                dc->SetPen(*blackPen);
                if (m_game->IsFocussed(x, y))
                    dc->SetBrush(* focusedBrush);
                else
                    dc->SetBrush(*greyBrush);

                dc->DrawRectangle( x*m_cellWidth*X_UNIT, y*m_cellHeight*Y_UNIT,
                    m_cellWidth*X_UNIT+1, m_cellHeight*Y_UNIT+1);
            }
            else if (m_game->IsBomb(x,y))
            {
                dc->SetPen(* blackPen);
                dc->SetBrush(* redBrush);
                dc->DrawRectangle( x*m_cellWidth*X_UNIT, y*m_cellHeight*Y_UNIT,
                    m_cellWidth*X_UNIT+1, m_cellHeight*Y_UNIT+1);
                buf = wxT("B");
                dc->SetTextForeground(wxBlack);
                dc->SetTextBackground(wxRed);
                dc->GetTextExtent(buf, &chw, &chh);
                dc->DrawText( buf,
                    x*m_cellWidth*X_UNIT + (m_cellWidth*X_UNIT-chw)/2,
                    y*m_cellHeight*Y_UNIT + (m_cellHeight*Y_UNIT-chh)/2);
                if (m_game->IsExploded(x,y))
                {
                    dc->SetPen(* bluePen);
                    dc->DrawLine(x*m_cellWidth*X_UNIT, y*m_cellHeight*Y_UNIT,
                        (x+1)*m_cellWidth*X_UNIT, (y+1)*m_cellHeight*Y_UNIT);
                    dc->DrawLine(x*m_cellWidth*X_UNIT, (y+1)*m_cellHeight*Y_UNIT,
                        (x+1)*m_cellWidth*X_UNIT, y*m_cellHeight*Y_UNIT);
                }
            }
            else   // Display a digit
            {
                dc->SetPen(* blackPen);
                if (m_game->IsFocussed(x, y))
                    dc->SetBrush(* focusedBrush);
                else
                    dc->SetBrush(* whiteBrush);
                dc->DrawRectangle( x*m_cellWidth*X_UNIT, y*m_cellHeight*Y_UNIT,
                    m_cellWidth*X_UNIT+1, m_cellHeight*Y_UNIT+1);

                int digit_value = m_game->Get(x,y) & BG_MASK;
                switch(digit_value)
                {
                case 0:
                    buf = wxT("0");
                    dc->SetTextForeground(wxGreen);
                    break;
                case 1: 
                    buf = wxT("1");
                    dc->SetTextForeground(wxBlue);
                    break;
                default:
                    buf.Printf(wxT("%d"),digit_value);
                    dc->SetTextForeground(wxBlack);
                    break;
            }
            dc->GetTextExtent(buf, &chw, &chh);
            dc->SetTextBackground(wxWhite);
            dc->DrawText( buf,
                x*m_cellWidth*X_UNIT + (m_cellWidth*X_UNIT-chw)/2,
                y*m_cellHeight*Y_UNIT + (m_cellHeight*Y_UNIT-chh)/2);
        }
    }
    dc->SetFont(wxNullFont);

#if wxUSE_LOG
    wxLogStatus(wxT("%d bombs  %d remaining cells"),
        m_game->GetNumBombs(), m_game->GetNumRemainingCells() );
#endif
}

// Refreshes the field image
// xc1,yc1 etc. are the (inclusive) limits of the area to be drawn,
// expressed in cells.
void BombsCanvas::RefreshField(int xc1, int yc1, int xc2, int yc2)
{
    wxClientDC dc(this);
    DrawField(& dc, xc1, yc1, xc2, yc2);
    if (m_bmp)
    {
        wxMemoryDC memDC;
        memDC.SelectObject(*m_bmp);
        DrawField(&memDC, xc1, yc1, xc2, yc2);
        memDC.SelectObject(wxNullBitmap);
    }
}

// Called when uncovering a cell.
void BombsCanvas::Uncover(int x, int y)
{
    m_game->Unhide(x,y);
    RefreshField(x, y, x, y);

    const int gridWidth = m_game->GetWidth();
    const int gridHeight = m_game->GetHeight();

    const bool hasWon = m_game->GetNumRemainingCells() == 0;
    if (m_game->IsBomb(x,y) || hasWon)
    {
        wxBell();
        if (hasWon)
        {
            wxMessageBox(wxT("Nice! You found all the bombs!"),
                wxT("wxWin Bombs"), wxOK|wxCENTRE);
        }
        else // x,y is a bomb
        {
            m_game->Explode(x, y);
        }

        for(x=0; x<gridWidth; x++)
            for(y=0; y<gridHeight; y++)
                m_game->Unhide(x,y);
                    RefreshField(0, 0, gridWidth-1, gridHeight-1);
    }
    else if (!m_game->Get(x, y))
    {
        int left = ( x > 0 ) ? x-1 : 0;
        int right = ( x < gridWidth - 1 )
            ? x+1
            : gridWidth - 1;
        int top = ( y > 0 ) ? y-1 : 0;
        int bottom = ( y < gridHeight - 1 )
            ? y+1
            : gridHeight - 1;

        int i, j;
        for (j=top; j<=bottom; j++)
            for (i=left; i<=right; i++)
                if ( (i != x || j != y) && m_game->IsHidden(i, j)
                    && !m_game->IsMarked(i, j) )
                {
                    Uncover(i, j);
                }
    }
}

// Called when the canvas receives a mouse event.
void BombsCanvas::OnMouseEvent(wxMouseEvent& event)
{
    const int gridWidth = m_game->GetWidth();
    const int gridHeight = m_game->GetHeight();

    wxCoord fx, fy;
    event.GetPosition(&fx, &fy);
    int x = fx/(m_cellWidth*X_UNIT);
    int y = fy/(m_cellHeight*Y_UNIT);
    if (x<gridWidth && y<gridHeight)
    {
        if ( (event.RightDown() || (event.LeftDown() && event.ShiftDown()))
           && (m_game->IsHidden(x,y)
               || !m_game->GetNumRemainingCells() ) )
        { 
            // store previous and current field
            int prevFocusX = m_game->m_gridFocusX;
            int prevFocusY = m_game->m_gridFocusY;
            m_game->m_gridFocusX = x;
            m_game->m_gridFocusY = y;
            RefreshField(prevFocusX, prevFocusY, prevFocusX, prevFocusY);
            m_game->Mark(x, y);
            RefreshField(x, y, x, y);
            return;
        }
        else if (event.LeftDown() && m_game->IsHidden(x,y)
            && !m_game->IsMarked(x,y))
        {
            // store previous and current field
            int prevGridFocusX = m_game->m_gridFocusX;
            int prevGridFocusY = m_game->m_gridFocusY;
            m_game->m_gridFocusX = x;
            m_game->m_gridFocusY = y;
            RefreshField(prevGridFocusX, prevGridFocusY,
                prevGridFocusX, prevGridFocusY);
            Uncover(x, y);
            return;
        }
    }
}

void BombsCanvas::OnChar(wxKeyEvent& event)
{
    int keyCode = event.GetKeyCode();
    int prevGridFocusX = m_game->m_gridFocusX;
    int prevGridFocusY = m_game->m_gridFocusY;

    const int gridWidth = m_game->GetWidth();
    const int gridHeight = m_game->GetHeight();

    switch(keyCode)
    {

    case WXK_RIGHT:
        m_game->m_gridFocusX++;
        if (m_game->m_gridFocusX >= gridWidth) m_game->m_gridFocusX = 0;
        break;

    case WXK_LEFT:
        m_game->m_gridFocusX--;
        if (m_game->m_gridFocusX<0) m_game->m_gridFocusX = gridWidth-1;
        break;

    case WXK_DOWN:
        m_game->m_gridFocusY++;
        if (m_game->m_gridFocusY >= gridHeight) m_game->m_gridFocusY = 0;
        break;

    case WXK_UP:
        m_game->m_gridFocusY--;
        if (m_game->m_gridFocusY<0) m_game->m_gridFocusY = gridHeight-1;
        break;

    case WXK_RETURN:
        if ( (prevGridFocusX == m_game->m_gridFocusX)
            && (prevGridFocusY == m_game->m_gridFocusY)
            && (m_game->IsHidden(m_game->m_gridFocusX, m_game->m_gridFocusY)) )
        {
            m_game->Mark(m_game->m_gridFocusX, m_game->m_gridFocusY);
            if (!m_game->IsMarked(m_game->m_gridFocusX, m_game->m_gridFocusY))
            {
                Uncover(m_game->m_gridFocusX, m_game->m_gridFocusY);
            }
            RefreshField(m_game->m_gridFocusX, m_game->m_gridFocusY,
                m_game->m_gridFocusX, m_game->m_gridFocusY);
        }
        break;

    default:
        event.Skip();

    }

    if ((prevGridFocusX != m_game->m_gridFocusX)
        || (prevGridFocusY != m_game->m_gridFocusY))
    {
        // cause focused field to be visible after first key hit after launching new game
        if( m_game->m_gridFocusX < 0 ) m_game->m_gridFocusX = 0;
        if( m_game->m_gridFocusY < 0 ) m_game->m_gridFocusY = 0;

        // refresh previous field and focused field
        RefreshField(prevGridFocusX, prevGridFocusY,
            prevGridFocusX, prevGridFocusY);
        RefreshField(m_game->m_gridFocusX, m_game->m_gridFocusY,
            m_game->m_gridFocusX, m_game->m_gridFocusY);
    }
}
