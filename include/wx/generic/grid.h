/////////////////////////////////////////////////////////////////////////////
// Name:        grid.h
// Purpose:     wxGrid and related classes
// Author:      Michael Bedward (based on code by Julian Smart, Robin Dunn)
// Modified by:
// Created:     1/08/1999
// RCS-ID:      $Id$
// Copyright:   (c) Michael Bedward
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#include "wx/defs.h"

#if !defined(wxUSE_NEW_GRID) || !(wxUSE_NEW_GRID)
#include "gridg.h"
#else

#ifndef __WXGRID_H__
#define __WXGRID_H__

#ifdef __GNUG__
#pragma interface "grid.h"
#endif

#include "wx/panel.h"
#include "wx/string.h"
#include "wx/scrolbar.h"
#include "wx/event.h"
#include "wx/textctrl.h"
#include "wx/combobox.h"
#include "wx/dynarray.h"


// Default parameters for wxGrid
//
#define WXGRID_DEFAULT_NUMBER_ROWS            10
#define WXGRID_DEFAULT_NUMBER_COLS            10
#ifdef __WXMSW__
#define WXGRID_DEFAULT_ROW_HEIGHT             25
#else
#define WXGRID_DEFAULT_ROW_HEIGHT             30
#endif  // __WXMSW__
#define WXGRID_DEFAULT_COL_WIDTH              80
#define WXGRID_DEFAULT_COL_LABEL_HEIGHT       32
#define WXGRID_DEFAULT_ROW_LABEL_WIDTH        82
#define WXGRID_LABEL_EDGE_ZONE                 5
#define WXGRID_MIN_ROW_HEIGHT                 15
#define WXGRID_MIN_COL_WIDTH                  15
#define WXGRID_DEFAULT_SCROLLBAR_WIDTH        16
#define WXGRID_DEFAULT_TOPEDIT_WIDTH          300
#define WXGRID_DEFAULT_TOPEDIT_HEIGHT         60


class wxGrid;


//////////////////////////////////////////////////////////////////////
//
//  Grid table classes
//
//////////////////////////////////////////////////////////////////////


class wxGridTableBase : public wxObject
{
    wxGrid * m_view;
    
  public:
    wxGridTableBase();
    virtual ~wxGridTableBase();

    // You must override these functions in a derived table class
    //
    virtual long GetNumberRows() = 0;
    virtual long GetNumberCols() = 0;
    virtual wxString GetValue( int row, int col ) = 0;
    virtual void SetValue( int row, int col, const wxString& s ) = 0;
    virtual bool IsEmptyCell( int row, int col ) = 0;
    
    // Overriding these is optional
    //
    virtual void SetView( wxGrid *grid ) { m_view = grid; }
    virtual wxGrid * GetView() const { return m_view; }

    virtual void Clear() {}
    virtual bool InsertRows( size_t pos = 0, size_t numRows = 1 );
    virtual bool AppendRows( size_t numRows = 1 );
    virtual bool DeleteRows( size_t pos = 0, size_t numRows = 1 );
    virtual bool InsertCols( size_t pos = 0, size_t numCols = 1 );
    virtual bool AppendCols( size_t numCols = 1 );
    virtual bool DeleteCols( size_t pos = 0, size_t numCols = 1 );

    virtual wxString GetRowLabelValue( int row );
    virtual wxString GetColLabelValue( int col );
    virtual void SetRowLabelValue( int row, const wxString& ) {}
    virtual void SetColLabelValue( int col, const wxString& ) {}
    
    DECLARE_ABSTRACT_CLASS( wxGridTableBase );
};



// IDs for messages sent from grid table to view
//
enum wxGridTableRequest {
    wxGRIDTABLE_REQUEST_VIEW_GET_VALUES = 2000,
    wxGRIDTABLE_REQUEST_VIEW_SEND_VALUES,
    wxGRIDTABLE_NOTIFY_ROWS_INSERTED,
    wxGRIDTABLE_NOTIFY_ROWS_APPENDED,
    wxGRIDTABLE_NOTIFY_ROWS_DELETED,
    wxGRIDTABLE_NOTIFY_COLS_INSERTED,
    wxGRIDTABLE_NOTIFY_COLS_APPENDED,
    wxGRIDTABLE_NOTIFY_COLS_DELETED
};

class wxGridTableMessage
{
    wxGridTableBase *m_table;
    int m_id;
    int m_comInt1;
    int m_comInt2;
    
  public:
    wxGridTableMessage();
    wxGridTableMessage( wxGridTableBase *table, int id,
                        int comInt1 = -1,
                        int comInt2 = -1 );

    void SetTableObject( wxGridTableBase *table ) { m_table = table; }
    wxGridTableBase * GetTableObject() const { return m_table; }
    void SetId( int id ) { m_id = id; }
    int  GetId() { return m_id; }
    void SetCommandInt( int comInt1 ) { m_comInt1 = comInt1; }
    int  GetCommandInt() { return m_comInt1; }
    void SetCommandInt2( int comInt2 ) { m_comInt2 = comInt2; }
    int  GetCommandInt2() { return m_comInt2; }    
};



// ------ wxGridStringArray
// A 2-dimensional array of strings for data values
//

WX_DECLARE_OBJARRAY(wxArrayString, wxGridStringArray);


// ------ wxGridStringTable
//
// Simplest type of data table for a grid for small tables of strings
// that are stored in memory
//

class wxGridStringTable : public wxGridTableBase
{
    wxGridStringArray m_data;

    // These only get used if you set your own labels, otherwise the
    // GetRow/ColLabelValue functions return wxGridTableBase defaults
    //
    wxArrayString     m_rowLabels;
    wxArrayString     m_colLabels;
    
  public:
    wxGridStringTable();
    wxGridStringTable( int numRows, int numCols );
    ~wxGridStringTable();

    // these are pure virtual in wxGridTableBase
    //
    long GetNumberRows();
    long GetNumberCols();
    wxString GetValue( int row, int col );
    void SetValue( int row, int col, const wxString& s );
    bool IsEmptyCell( int row, int col );
    
    // overridden functions from wxGridTableBase
    //
    void Clear();
    bool InsertRows( size_t pos = 0, size_t numRows = 1 );
    bool AppendRows( size_t numRows = 1 );
    bool DeleteRows( size_t pos = 0, size_t numRows = 1 );
    bool InsertCols( size_t pos = 0, size_t numCols = 1 );
    bool AppendCols( size_t numCols = 1 );
    bool DeleteCols( size_t pos = 0, size_t numCols = 1 );

    void SetRowLabelValue( int row, const wxString& );
    void SetColLabelValue( int col, const wxString& );
    wxString GetRowLabelValue( int row );
    wxString GetColLabelValue( int col );
    
    DECLARE_DYNAMIC_CLASS( wxGridStringTable )
};



//////////////////////////////////////////////////////////////////////
//
//  Grid view classes
//
//////////////////////////////////////////////////////////////////////

class wxGridCellCoords
{
    long m_row;
    long m_col;
    
  public:
    wxGridCellCoords() { m_row = m_col = -1; }
    wxGridCellCoords( int r, int c ) { m_row = r; m_col = c; }

    // default copy ctor is ok

    long GetRow() const { return m_row; }
    void SetRow( long n ) { m_row = n; }
    long GetCol() const { return m_col; }
    void SetCol( long n ) { m_col = n; }
    void Set( long row, long col ) { m_row = row; m_col = col; }
    
    wxGridCellCoords& operator=( const wxGridCellCoords& other )
    {
        if ( &other != this )
        {
            m_row=other.m_row;
            m_col=other.m_col;
        }
        return *this;
    }

    bool operator==( const wxGridCellCoords& other )
    {
        return (m_row == other.m_row  &&  m_col == other.m_col);
    }

    bool operator!=( const wxGridCellCoords& other )
    {
        return (m_row != other.m_row  ||  m_col != other.m_col);
    }

    bool operator!()
    {
        return (m_row == -1 && m_col == -1 );
    }
};


// For comparisons...
//
extern wxGridCellCoords wxGridNoCellCoords;
extern wxRect           wxGridNoCellRect;


// This set of classes is to provide for the use of different types of
// cell edit controls in the grid while avoiding the wx class info 
// system in deference to wxPython

class wxGridTextCtrl : public wxTextCtrl
{
    // TRUE for controls placed over cells,
    // FALSE for a control on a grid control panel
    bool m_isCellControl;  

    wxString startValue;

    void OnKeyDown( wxKeyEvent& );
    
  public:
    wxGridTextCtrl() {}
    wxGridTextCtrl( wxWindow *,
                    bool isCellControl,
                    wxWindowID id,
                    const wxString& value = wxEmptyString,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0 );

    void     SetStartValue( const wxString& );
    wxString GetStartValue() { return startValue; }
    
    DECLARE_DYNAMIC_CLASS( wxGridTextCtrl )
    DECLARE_EVENT_TABLE()
};


class wxGrid : public wxPanel
{
    DECLARE_DYNAMIC_CLASS( wxGrid )

  private:
    bool m_created;

    wxGridTableBase *m_table;

    int m_left;
    int m_top;
    int m_right;
    int m_bottom;
    
    int m_numRows;
    int m_numCols;

    wxGridCellCoords m_currentCellCoords;
    bool             m_currentCellHighlighted;

    wxGridCellCoords m_selectedTopLeft;
    wxGridCellCoords m_selectedBottomRight;
        
    int        m_defaultRowHeight;
    wxArrayInt m_rowHeights;
    wxArrayInt m_rowBottoms;

    int        m_defaultColWidth;
    wxArrayInt m_colWidths;
    wxArrayInt m_colRights;

    int m_rowLabelWidth;
    int m_colLabelHeight;

    wxColour   m_labelBackgroundColour;
    wxColour   m_labelTextColour;
    wxFont     m_labelFont;
    
    int        m_rowLabelHorizAlign;
    int        m_rowLabelVertAlign;
    int        m_colLabelHorizAlign;
    int        m_colLabelVertAlign;

    bool       m_defaultRowLabelValues;
    bool       m_defaultColLabelValues;

    wxColour   m_gridLineColour;
    bool       m_gridLinesEnabled;

    wxFont     m_defaultCellFont;
    
    wxScrollBar * m_horizScrollBar;
    wxScrollBar * m_vertScrollBar;
    int m_scrollBarWidth;
    int m_scrollPosX;
    int m_scrollPosY;
    int m_wholeColsVisible;
    int m_wholeRowsVisible;

    bool m_inOnKeyDown;
    bool m_inOnText;
    bool m_firstPaint;
    int  m_batchCount;

    int  m_cursorMode;
    enum { WXGRID_CURSOR_DEFAULT,
           WXGRID_CURSOR_SELECT_CELL,
           WXGRID_CURSOR_RESIZE_ROW,
           WXGRID_CURSOR_RESIZE_COL,
           WXGRID_CURSOR_SELECT_ROW,
           WXGRID_CURSOR_SELECT_COL
    };

    int  m_dragLastPos;
    int  m_dragRowOrCol;
    bool m_isDragging;

    wxGridCellCoords m_selectionStart;
    
    wxCursor m_rowResizeCursor;
    wxCursor m_colResizeCursor;

    bool       m_editable;  // applies to whole grid
    int        m_editCtrlType;  // for current cell
    wxWindow*  m_cellEditCtrl;
    bool       m_cellEditCtrlEnabled;
    wxWindow*  m_topEditCtrl;
    bool       m_topEditCtrlEnabled;
    
    // ------ internal init and update functions
    //
    void Create();                 
    void Init();
    void CalcDimensions();
    bool IsOnScreen();    
    bool Redimension( wxGridTableMessage& );
    
    // ------ event processing
    //
    bool SendEvent( const wxEventType,
                    int row, int col,
                    wxMouseEvent& );
    
    bool SendEvent( const wxEventType,
                    int row, int col );
    
    void OnPaint( wxPaintEvent& );
    void OnSize( wxSizeEvent& );
    void OnMouse( wxMouseEvent& );
    void OnKeyDown( wxKeyEvent& );
    void OnText( wxKeyEvent& );
    void OnGridScroll( wxScrollEvent& );

    void SelectCell( const wxGridCellCoords& coords );
    void SelectCell( int row, int col )
        { SelectCell( wxGridCellCoords(row, col) ); }


    // ------ edit controls
    //
    void ShowCellEditControl();
    void HideCellEditControl();
    void SaveEditControlValue();

    
    // ------ grid location functions
    //
    int  XYToArea( int x, int y );  // returns one of the following...
    enum { WXGRID_NOAREA,
           WXGRID_ROWLABEL,
           WXGRID_ROWLABEL_EDGE,
           WXGRID_COLLABEL,
           WXGRID_COLLABEL_EDGE,
           WXGRID_CORNERLABEL,
           WXGRID_CELL };
    
    void XYToCell( int x, int y, wxGridCellCoords& );
    int  YToRow( int y );
    int  XToCol( int x );

    int  YToEdgeOfRow( int y );
    int  XToEdgeOfCol( int x );

    wxRect CellToRect( int row, int col );
    wxRect CellToRect( const wxGridCellCoords& coords )
        { return CellToRect( coords.GetRow(), coords.GetCol() ); }

    bool MoveCursorUp();
    bool MoveCursorDown();
    bool MoveCursorLeft();
    bool MoveCursorRight();
    bool MovePageDown();
    bool MovePageUp();
    bool MoveCursorUpBlock();
    bool MoveCursorDownBlock();
    bool MoveCursorLeftBlock();
    bool MoveCursorRightBlock();
    
    
    // ------ label drawing functions
    //
    void DrawLabelAreas( wxDC& dc );
    void DrawColLabelBorders( wxDC& dc );
    void DrawColLabels( wxDC& dc );
    void DrawColLabel( wxDC& dc, const wxRect&, int col );
    void DrawRowLabelBorders( wxDC& dc );
    void DrawRowLabels( wxDC& dc );
    void DrawRowLabel( wxDC& dc, const wxRect&, int col );


    // ------ cell drawing functions
    //
    void DrawCellArea( wxDC& dc );
    void DrawGridLines( wxDC& dc );
    void DrawCells( wxDC& dc );
    void DrawCellBackground( wxDC& dc, const wxRect&, int row, int col );
    void DrawCellValue( wxDC& dc, const wxRect&, int row, int col );

    // this one is useful when you just need to draw one or a few
    // cells
    void DrawCell( int row, int col );
    void DrawCell( const wxGridCellCoords& coords )
        { DrawCell( coords.GetRow(), coords.GetCol() ); }
    
    void DrawCellHighlight( wxDC& dc, int row, int col );
    void DrawCellHighlight( wxDC& dc, wxGridCellCoords& coords )
        { DrawCellHighlight( dc, coords.GetRow(), coords.GetCol() ); }
    void ShowCurrentCellHighlight( wxDC& dc );
    void HideCurrentCellHighlight( wxDC& dc );
            

    // ------ generic drawing functions
    //
    void DrawTextRectangle( wxDC& dc, const wxString&, const wxRect&,
                            int horizontalAlignment = wxLEFT,
                            int verticalAlignment = wxTOP );

    // Split a string containing newline chararcters into an array of
    // strings and return the number of lines
    //
    void StringToLines( const wxString& value, wxArrayString& lines );
    
    void GetTextBoxSize( wxDC& dc,
                         wxArrayString& lines,
                         long *width, long *height );

    
    // ------ functions to get/send data (see also public functions)
    //
    bool GetModelValues();
    bool SetModelValues();


    ////////////////////// Public section ////////////////////
    
  public:
    wxGrid()
        { Create(); }
    
    wxGrid( wxWindow *parent,
            wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = 0,
            const wxString& name = wxPanelNameStr )
        : wxPanel( parent, id, pos, size, style, name )
        {
            Create();
        }
        
    ~wxGrid();    

    bool CreateGrid( int numRows = WXGRID_DEFAULT_NUMBER_ROWS,
                     int numCols = WXGRID_DEFAULT_NUMBER_COLS );

    wxGridTableBase * GetTable() const { return m_table; }
    void SetTable( wxGridTableBase *table ) { m_table = table; }

    void ClearGrid();
    bool InsertRows( int pos = 0, int numRows = 1, bool updateLabels=TRUE );
    bool AppendRows( int numRows = 1, bool updateLabels=TRUE );
    bool DeleteRows( int pos = 0, int numRows = 1, bool updateLabels=TRUE );
    bool InsertCols( int pos = 0, int numCols = 1, bool updateLabels=TRUE );
    bool AppendCols( int numCols = 1, bool updateLabels=TRUE );
    bool DeleteCols( int pos = 0, int numCols = 1, bool updateLabels=TRUE );    

    // ------ editing and edit controls
    //
    bool IsEditable() { return m_editable; }
    void EnableEditing( bool edit );

    void     EnableTopEditControl( bool enable );
    bool     IsTopEditControlEnabled()
             { return (m_topEditCtrl && m_topEditCtrlEnabled); }
    void     EnableCellEditControl( bool enable );
    bool     IsCellEditControlEnabled()
             { return (m_cellEditCtrl && m_cellEditCtrlEnabled); }
    void     SetEditControlValue( const wxString& s = wxEmptyString );
    

    // ------ grid dimensions
    //
    int      GetNumberRows() { return  m_numRows; }
    int      GetNumberCols() { return  m_numCols; }
    int      GetNumberVisibleRows() { return m_wholeRowsVisible; }
    int      GetNumberVisibleCols() { return m_wholeColsVisible; }

    
    // ------
    // Code that does a lot of grid modification can be enclosed
    // between BeginBatch() and EndBatch() calls to avoid screen
    // flicker
    //
    void     BeginBatch() { m_batchCount++; }
    void     EndBatch() { if ( m_batchCount > 0 ) m_batchCount--; }
    int      GetBatchCount() { return m_batchCount; }

    
    // ------ label and gridline formatting
    //
    int      GetDefaultRowLabelSize() { return WXGRID_DEFAULT_ROW_LABEL_WIDTH; }
    int      GetRowLabelSize() { return m_rowLabelWidth; }
    int      GetDefaultColLabelSize() { return WXGRID_DEFAULT_COL_LABEL_HEIGHT; }    
    int      GetColLabelSize() { return m_colLabelHeight; }
    wxColour GetLabelBackgroundColour() { return m_labelBackgroundColour; }
    wxColour GetLabelTextColour() { return m_labelTextColour; }
    wxFont   GetLabelFont() { return m_labelFont; }
    void     GetRowLabelAlignment( int *horiz, int *vert );
    void     GetColLabelAlignment( int *horiz, int *vert );
    wxString GetRowLabelValue( int row );
    wxString GetColLabelValue( int col );
    wxColour GetGridLineColour() { return m_gridLineColour; }

    void     SetRowLabelSize( int width );
    void     SetColLabelSize( int height );
    void     SetLabelBackgroundColour( const wxColour& );
    void     SetLabelTextColour( const wxColour& );
    void     SetLabelFont( const wxFont& );
    void     SetRowLabelAlignment( int horiz, int vert );
    void     SetColLabelAlignment( int horiz, int vert );
    void     SetRowLabelValue( int row, const wxString& );
    void     SetColLabelValue( int col, const wxString& );
    void     SetGridLineColour( const wxColour& );

    void     EnableGridLines( bool enable = TRUE );
    bool     GridLinesEnabled() { return m_gridLinesEnabled; }

    
    // ------ row and col formatting
    //
    int      GetDefaultRowSize();
    int      GetRowSize( int row );
    int      GetDefaultColSize();
    int      GetColSize( int col );
    wxColour GetDefaultCellBackgroundColour();
    wxColour GetCellBackgroundColour( int row, int col );
    wxColour GetDefaultCellTextColour();
    wxColour GetCellTextColour( int row, int col );
    wxColour GetCellHighlightColour();
    wxFont   GetDefaultCellFont();
    wxFont   GetCellFont( int row, int col );
    void     GetDefaultCellAlignment( int *horiz, int *vert );
    void     GetCellAlignment( int row, int col, int *horiz, int *vert );
    
    void     SetDefaultRowSize( int height, bool resizeExistingRows = FALSE );
    void     SetRowSize( int row, int height );
    void     SetDefaultColSize( int width, bool resizeExistingCols = FALSE );
    void     SetColSize( int col, int width );
    void     SetDefaultCellBackgroundColour( const wxColour& );
    void     SetCellBackgroundColour( int row, int col, const wxColour& );
    void     SetDefaultCellTextColour( const wxColour& );
    void     SetCellTextColour( int row, int col, const wxColour& );
    void     SetCellHighlightColour( const wxColour& );
    void     SetDefaultCellFont( const wxFont& );
    void     SetCellFont( int row, int col, const wxFont& );
    void     SetDefaultCellAlignment( int horiz, int vert );
    void     SetCellAlignment( int row, int col, int horiz, int vert );

    
    // ------ cell value accessors
    //
    wxString GetCellValue( int row, int col )
    {
        if ( m_table )
        {
            return m_table->GetValue( row, col );
        }
        else
        {
            return wxEmptyString;
        }
    }

    wxString GetCellValue( const wxGridCellCoords& coords )
        { return GetCellValue( coords.GetRow(), coords.GetCol() ); }
        
    void SetCellValue( int row, int col, const wxString& s );
    void SetCellValue( const wxGridCellCoords& coords, const wxString& s )
        { SetCellValue( coords.GetRow(), coords.GetCol(), s ); }
            

    // ------ interaction with data model
    //
    bool ProcessTableMessage( wxGridTableMessage& );
    


    // ------ grid location functions
    //
    
    int  GetGridCursorRow() { return m_currentCellCoords.GetRow(); }
    int  GetGridCursorCol() { return m_currentCellCoords.GetCol(); }
    int  GetHorizontalScrollPos() { return m_scrollPosX; }
    int  GetVerticalScrollPos() { return m_scrollPosY; }
    
    bool IsVisible( const wxGridCellCoords& );
    void MakeCellVisible( int row, int col );
    void MakeCellVisible( const wxGridCellCoords& coords )
        { MakeCellVisible( coords.GetRow(), coords.GetCol() ); }
                          
    void SetGridCursor( int row, int col )
        { SelectCell( wxGridCellCoords(row, col) ); }

    void SetHorizontalScrollPos( int leftMostCol );
    void SetVerticalScrollPos( int topMostRow );


    // ------ selections of blocks of cells
    //
    void SelectRow( int row, bool addToSelected = FALSE );
    void SelectCol( int col, bool addToSelected = FALSE );
    
    void SelectBlock( int topRow, int leftCol, int bottomRow, int rightCol );

    void SelectBlock( const wxGridCellCoords& topLeft,
                      const wxGridCellCoords& bottomRight )
        { SelectBlock( topLeft.GetRow(), topLeft.GetCol(),
                       bottomRight.GetRow(), bottomRight.GetCol() ); }

    void SelectAll();
    
    bool IsSelection()
        { return ( m_selectedTopLeft != wxGridNoCellCoords &&
                   m_selectedBottomRight != wxGridNoCellCoords );
        }

    void ClearSelection();

    bool IsInSelection( int row, int col )
        { return ( IsSelection() &&
                   row >= m_selectedTopLeft.GetRow() &&
                   col >= m_selectedTopLeft.GetCol() &&
                   row <= m_selectedBottomRight.GetRow() &&
                   col <= m_selectedBottomRight.GetCol() );
        }

    bool IsInSelection( const wxGridCellCoords& coords )
        { return IsInSelection( coords.GetRow(), coords.GetCol() ); }

    void GetSelection( int* topRow, int* leftCol, int* bottomRow, int* rightCol )
        {
            // these will all be -1 if there is no selected block
            //
            *topRow    = m_selectedTopLeft.GetRow();
            *leftCol   = m_selectedTopLeft.GetCol();
            *bottomRow = m_selectedBottomRight.GetRow();
            *rightCol  = m_selectedBottomRight.GetCol();
        }

    // get coordinates of selected block edges for repainting etc.
    //
    wxRect SelectionToRect();
    

    // ------ For compatibility with previous wxGrid only...
    //
    //  ************************************************
    //  **  Don't use these in new code because they  **
    //  **  are liable to disappear in a future       **
    //  **  revision                                  **
    //  ************************************************
    //

    wxGrid( wxWindow *parent,
            int x = -1, int y = -1, int w = -1, int h = -1,
            long style = 0,
            const wxString& name = wxPanelNameStr )
        : wxPanel( parent, -1, wxPoint(x,y), wxSize(w,h), style, name )
        { 
            Create();
        }    

    void SetCellValue( const wxString& val, int row, int col )
        { SetCellValue( row, col, val ); }
    
    void AdjustScrollbars()
        { CalcDimensions(); }

    void UpdateDimensions()
        { CalcDimensions(); }

    int GetRows() { return GetNumberRows(); }
    int GetCols() { return GetNumberCols(); }
    int GetCursorRow() { return GetGridCursorRow(); }
    int GetCursorColumn() { return GetGridCursorCol(); }
    int GetScrollPosX() { return GetHorizontalScrollPos(); }
    int GetScrollPosY() { return GetVerticalScrollPos(); }
    
    void SetScrollX( int x ) { SetHorizontalScrollPos( x ); }
    void SetScrollY( int y ) { SetVerticalScrollPos( y ); }

    void SetColumnWidth( int col, int width )
        { SetColSize( col, width ); }
        
    int GetColumnWidth( int col )
        { return GetColSize( col ); }
        
    void SetRowHeight( int row, int height )
        { SetRowSize( row, height ); }
        
    int GetRowHeight( int row )
        { return GetRowSize( row ); }
        
    int GetViewHeight()
        { return m_wholeRowsVisible; }
    
    int GetViewWidth()
        { return m_wholeColsVisible; }

    void SetLabelSize( int orientation, int sz )
        {
            if ( orientation == wxHORIZONTAL )
                SetColLabelSize( sz );
            else
                SetRowLabelSize( sz );
        }

    int GetLabelSize( int orientation )
        {
            if ( orientation == wxHORIZONTAL )
                return GetColLabelSize();
            else
                return GetRowLabelSize();
        }

    void SetLabelAlignment( int orientation, int align )
        {
            if ( orientation == wxHORIZONTAL )
                SetColLabelAlignment( align, -1 );
            else
                SetRowLabelAlignment( align, -1 );
        }

    int GetLabelAlignment( int orientation, int align )
        {
            int h, v;
            if ( orientation == wxHORIZONTAL )
            {
                GetColLabelAlignment( &h, &v );
                return h;
            }
            else
            {
                GetRowLabelAlignment( &h, &v );
                return h;
            }
        }

    void SetLabelValue( int orientation, const wxString& val, int pos )
        {
            if ( orientation == wxHORIZONTAL )
                SetColLabelValue( pos, val );
            else
                SetRowLabelValue( pos, val );
        }
    
    wxString GetLabelValue( int orientation, int pos)
        {
            if ( orientation == wxHORIZONTAL )
                return GetColLabelValue( pos );
            else
                return GetRowLabelValue( pos );
        }

    wxFont GetCellTextFont() const 
        { return m_defaultCellFont; }
        
    wxFont GetCellTextFont(int row, int col) const
        { return m_defaultCellFont; }
        
    void SetCellTextFont(const wxFont& fnt)
        { SetDefaultCellFont( fnt ); }
        
    void SetCellTextFont(const wxFont& fnt, int row, int col)
        { SetCellFont( row, col, fnt ); }
        
    void SetCellTextColour(const wxColour& val, int row, int col)
        { SetCellTextColour( row, col, val ); }
        
    void SetCellTextColour(const wxColour& col)
        { SetDefaultCellTextColour( col ); }
        
    void SetCellBackgroundColour(const wxColour& col)
        { SetDefaultCellBackgroundColour( col ); }
        
    void SetCellBackgroundColour(const wxColour& colour, int row, int col)
        { SetCellBackgroundColour( row, col, colour ); }
     
    bool GetEditable() { return IsEditable(); }
    void SetEditable( bool edit = TRUE ) { EnableEditing( edit ); }
    bool GetEditInPlace() { return IsCellEditControlEnabled(); }
    void SetEditInPlace(bool edit = TRUE) { EnableCellEditControl( edit ); }

            
    // ******** End of compatibility functions **********

        
    // ------ control IDs
    enum { wxGRID_HORIZSCROLL = 2000,
           wxGRID_VERTSCROLL,
           wxGRID_CELLCTRL,
           wxGRID_TOPCTRL };

    // ------ control types
    enum { wxGRID_TEXTCTRL = 100,
           wxGRID_CHECKBOX,
           wxGRID_CHOICE,
           wxGRID_COMBOBOX };

    
    DECLARE_EVENT_TABLE()
};





//
// ------ Grid event class and event types
//

class WXDLLEXPORT wxGridEvent : public wxNotifyEvent
{
    DECLARE_DYNAMIC_CLASS(wxGridEvent)

  private:
    int         m_row;
    int         m_col;
    int         m_x;
    int         m_y;
    bool        m_control;
    bool        m_meta;
    bool        m_shift;
    bool        m_alt;
        
  public:
    wxGridEvent()
        : wxNotifyEvent(), m_row(-1), m_col(-1), m_x(-1), m_y(-1),
        m_control(0), m_meta(0), m_shift(0), m_alt(0)
        {
        }

    wxGridEvent(int id, wxEventType type, wxObject* obj,
                int row=-1, int col=-1, int x=-1, int y=-1,
                bool control=FALSE, bool shift=FALSE, bool alt=FALSE, bool meta=FALSE);

    virtual int GetRow() { return m_row; }
    virtual int GetCol() { return m_col; }
    wxPoint     GetPosition() { return wxPoint( m_x, m_y ); }
    bool        ControlDown() { return m_control; }
    bool        MetaDown() { return m_meta; }
    bool        ShiftDown() { return m_shift; }
    bool        AltDown() { return m_alt; }
};


class WXDLLEXPORT wxGridSizeEvent : public wxNotifyEvent
{
    DECLARE_DYNAMIC_CLASS(wxGridSizeEvent)

  private:
    int         m_rowOrCol;
    int         m_x;
    int         m_y;
    bool        m_control;
    bool        m_meta;
    bool        m_shift;
    bool        m_alt;
        
  public:
    wxGridSizeEvent()
        : wxNotifyEvent(), m_rowOrCol(-1), m_x(-1), m_y(-1),
        m_control(0), m_meta(0), m_shift(0), m_alt(0)
        {
        }

    wxGridSizeEvent(int id, wxEventType type, wxObject* obj,
                int rowOrCol=-1, int x=-1, int y=-1,
                bool control=FALSE, bool shift=FALSE, bool alt=FALSE, bool meta=FALSE);

    int         GetRowOrCol() { return m_rowOrCol; }
    wxPoint     GetPosition() { return wxPoint( m_x, m_y ); }
    bool        ControlDown() { return m_control; }
    bool        MetaDown() { return m_meta; }
    bool        ShiftDown() { return m_shift; }
    bool        AltDown() { return m_alt; }
};


class WXDLLEXPORT wxGridRangeSelectEvent : public wxNotifyEvent
{
    DECLARE_DYNAMIC_CLASS(wxGridRangeSelectEvent)

  private:
    wxGridCellCoords  m_topLeft;
    wxGridCellCoords  m_bottomRight;
    bool              m_control;
    bool              m_meta;
    bool              m_shift;
    bool              m_alt;

  public:
    wxGridRangeSelectEvent()
	: wxNotifyEvent()
	{
	    m_topLeft     = wxGridNoCellCoords;
	    m_bottomRight = wxGridNoCellCoords;
	    m_control     = FALSE;
	    m_meta        = FALSE;
	    m_shift       = FALSE;
	    m_alt         = FALSE;
	}

    wxGridRangeSelectEvent(int id, wxEventType type, wxObject* obj,
			   const wxGridCellCoords& topLeft,
			   const wxGridCellCoords& bottomRight,
			   bool control=FALSE, bool shift=FALSE,
			   bool alt=FALSE, bool meta=FALSE);

    wxGridCellCoords GetTopLeftCoords() { return m_topLeft; }
    wxGridCellCoords GetBottomRightCoords() { return m_bottomRight; }
    int         GetTopRow()    { return m_topLeft.GetRow(); }
    int         GetBottomRow() { return m_bottomRight.GetRow(); }
    int         GetLeftCol()   { return m_topLeft.GetCol(); }
    int         GetRightCol()  { return m_bottomRight.GetCol(); }
    bool        ControlDown()  { return m_control; }
    bool        MetaDown()     { return m_meta; }
    bool        ShiftDown()    { return m_shift; }
    bool        AltDown()      { return m_alt; }
};


const wxEventType EVT_GRID_CELL_LEFT_CLICK    = wxEVT_FIRST + 1580;
const wxEventType EVT_GRID_CELL_RIGHT_CLICK   = wxEVT_FIRST + 1581;
const wxEventType EVT_GRID_CELL_LEFT_DCLICK   = wxEVT_FIRST + 1582;
const wxEventType EVT_GRID_CELL_RIGHT_DCLICK  = wxEVT_FIRST + 1583;
const wxEventType EVT_GRID_LABEL_LEFT_CLICK   = wxEVT_FIRST + 1584;
const wxEventType EVT_GRID_LABEL_RIGHT_CLICK  = wxEVT_FIRST + 1585;
const wxEventType EVT_GRID_LABEL_LEFT_DCLICK  = wxEVT_FIRST + 1586;
const wxEventType EVT_GRID_LABEL_RIGHT_DCLICK = wxEVT_FIRST + 1587;
const wxEventType EVT_GRID_ROW_SIZE           = wxEVT_FIRST + 1588;
const wxEventType EVT_GRID_COL_SIZE           = wxEVT_FIRST + 1589;
const wxEventType EVT_GRID_RANGE_SELECT       = wxEVT_FIRST + 1590;
const wxEventType EVT_GRID_CELL_CHANGE        = wxEVT_FIRST + 1591;
const wxEventType EVT_GRID_SELECT_CELL        = wxEVT_FIRST + 1592;


typedef void (wxEvtHandler::*wxGridEventFunction)(wxGridEvent&);
typedef void (wxEvtHandler::*wxGridSizeEventFunction)(wxGridSizeEvent&);
typedef void (wxEvtHandler::*wxGridRangeSelectEventFunction)(wxGridRangeSelectEvent&);

#define EVT_GRID_CELL_LEFT_CLICK(fn)     { EVT_GRID_CELL_LEFT_CLICK,    -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_CELL_RIGHT_CLICK(fn)    { EVT_GRID_CELL_RIGHT_CLICK,   -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_CELL_LEFT_DCLICK(fn)    { EVT_GRID_CELL_LEFT_DCLICK,   -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_CELL_RIGHT_DCLICK(fn)   { EVT_GRID_CELL_RIGHT_DCLICK,  -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_LABEL_LEFT_CLICK(fn)    { EVT_GRID_LABEL_LEFT_CLICK,   -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_LABEL_RIGHT_CLICK(fn)   { EVT_GRID_LABEL_RIGHT_CLICK,  -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_LABEL_LEFT_DCLICK(fn)   { EVT_GRID_LABEL_LEFT_DCLICK,  -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_LABEL_RIGHT_DCLICK(fn)  { EVT_GRID_LABEL_RIGHT_DCLICK, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_ROW_SIZE(fn)            { EVT_GRID_ROW_SIZE,           -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridSizeEventFunction) &fn, NULL },
#define EVT_GRID_COL_SIZE(fn)            { EVT_GRID_COL_SIZE,           -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridSizeEventFunction) &fn, NULL },
#define EVT_GRID_RANGE_SELECT(fn)        { EVT_GRID_RANGE_SELECT,       -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridRangeSelectEventFunction) &fn, NULL },
#define EVT_GRID_CELL_CHANGE(fn)         { EVT_GRID_CELL_CHANGE,        -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL }, 
#define EVT_GRID_SELECT_CELL(fn)         { EVT_GRID_SELECT_CELL,        -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },


#if 0  // TODO: implement these ?  others ?

const wxEventType EVT_GRID_CREATE_CELL      = wxEVT_FIRST + 1576;
const wxEventType EVT_GRID_CHANGE_LABELS    = wxEVT_FIRST + 1577;
const wxEventType EVT_GRID_CHANGE_SEL_LABEL = wxEVT_FIRST + 1578;

#define EVT_GRID_CREATE_CELL(fn)      { EVT_GRID_CREATE_CELL,      -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_CHANGE_LABELS(fn)    { EVT_GRID_CHANGE_LABELS,    -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },
#define EVT_GRID_CHANGE_SEL_LABEL(fn) { EVT_GRID_CHANGE_SEL_LABEL, -1, -1, (wxObjectEventFunction) (wxEventFunction) (wxGridEventFunction) &fn, NULL },

#endif

#endif  // #ifndef __WXGRID_H__

#endif  // ifndef wxUSE_NEW_GRID
