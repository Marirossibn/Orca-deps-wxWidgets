/////////////////////////////////////////////////////////////////////////////
// Name:        metafile.cpp
// Purpose:     wxMetaFileDC etc.
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "metafile.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/setup.h"
#endif

#if USE_METAFILE

#ifndef WX_PRECOMP
#include "wx/utils.h"
#include "wx/app.h"
#endif

#include "wx/metafile.h"
#include "wx/clipbrd.h"
#include "wx/msw/private.h"

#include <stdio.h>
#include <string.h>

extern bool wxClipboardIsOpen;

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxMetaFile, wxObject)
IMPLEMENT_ABSTRACT_CLASS(wxMetaFileDC, wxDC)
#endif

/*
 * Metafiles - Windows 3.1 only
 * Currently, the only purpose for making a metafile is to put
 * it on the clipboard.
 */

wxMetaFile::wxMetaFile(const wxString& file)
{
  m_windowsMappingMode = MM_ANISOTROPIC;
  m_metaFile = 0;
  if (!file.IsNull() && file == "")
    m_metaFile = (WXHANDLE) GetMetaFile(file);
}

wxMetaFile::~wxMetaFile(void)
{
  if (m_metaFile)
    { DeleteMetaFile((HANDLE) m_metaFile); m_metaFile = 0; }
}

bool wxMetaFile::SetClipboard(int width, int height)
{
  bool alreadyOpen=wxClipboardOpen();
  if (!alreadyOpen)
  {
    wxOpenClipboard();
    if (!wxEmptyClipboard()) return FALSE;
  }
  bool success = wxSetClipboardData(wxCF_METAFILE,this, width,height);
  if (!alreadyOpen) wxCloseClipboard();
  return (bool) success;
}

bool wxMetaFile::Play(wxDC *dc)
{
  dc->BeginDrawing();

  if (dc->GetHDC() && m_metaFile)
    PlayMetaFile((HDC) dc->GetHDC(), (HANDLE) m_metaFile);

  dc->EndDrawing();

  return TRUE;
}

/*
 * Metafile device context
 *
 */

// Original constructor that does not takes origin and extent. If you use this,
// *DO* give origin/extent arguments to wxMakeMetaFilePlaceable.
wxMetaFileDC::wxMetaFileDC(const wxString& file)
{
  m_metaFile = NULL;
  m_minX = 10000;
  m_minY = 10000;
  m_maxX = -10000;
  m_maxY = -10000;
//  m_title = NULL;

  if (!file.IsNull() && wxFileExists(file))
    wxRemoveFile(file);
  m_hDC = (WXHDC) CreateMetaFile(file);

  m_ok = TRUE;

  // Actual Windows mapping mode, for future reference.
  m_windowsMappingMode = MM_TEXT;
  
  SetMapMode(MM_TEXT); // NOTE: does not set HDC mapmode (this is correct)
}

// New constructor that takes origin and extent. If you use this, don't
// give origin/extent arguments to wxMakeMetaFilePlaceable.
wxMetaFileDC::wxMetaFileDC(const wxString& file, int xext, int yext, int xorg, int yorg)
{
  m_minX = 10000;
  m_minY = 10000;
  m_maxX = -10000;
  m_maxY = -10000;
  if (file != "" && wxFileExists(file)) wxRemoveFile(file);
  m_hDC = (WXHDC) CreateMetaFile(file);

  m_ok = TRUE;

  ::SetWindowOrgEx((HDC) m_hDC,xorg,yorg, NULL);
  ::SetWindowExtEx((HDC) m_hDC,xext,yext, NULL);

  // Actual Windows mapping mode, for future reference.
  m_windowsMappingMode = MM_ANISOTROPIC;
  
  SetMapMode(MM_TEXT); // NOTE: does not set HDC mapmode (this is correct)
}

wxMetaFileDC::~wxMetaFileDC(void)
{
  m_hDC = 0;
}

void wxMetaFileDC::GetTextExtent(const wxString& string, float *x, float *y,
                                 float *descent, float *externalLeading, wxFont *theFont, bool use16bit)
{
  wxFont *fontToUse = theFont;
  if (!fontToUse)
    fontToUse = &m_font;

  HDC dc = GetDC(NULL);

  SIZE sizeRect;
  TEXTMETRIC tm;
  GetTextExtentPoint(dc, (char *)(const char *) string, strlen((char *)(const char *) string), &sizeRect);
  GetTextMetrics(dc, &tm);

  ReleaseDC(NULL, dc);

  *x = (float)XDEV2LOGREL(sizeRect.cx);
  *y = (float)YDEV2LOGREL(sizeRect.cy);
  if (descent) *descent = (float)tm.tmDescent;
  if (externalLeading) *externalLeading = (float)tm.tmExternalLeading;
}

wxMetaFile *wxMetaFileDC::Close(void)
{
  SelectOldObjects(m_hDC);
  HANDLE mf = CloseMetaFile((HDC) m_hDC);
  m_hDC = 0;
  if (mf)
  {
    wxMetaFile *wx_mf = new wxMetaFile;
    wx_mf->SetHMETAFILE((WXHANDLE) mf);
    wx_mf->SetWindowsMappingMode(m_windowsMappingMode);
    return wx_mf;
  }
  return NULL;
}

void wxMetaFileDC::SetMapMode(int mode)
{
  m_mappingMode = mode;

//  int pixel_width = 0;
//  int pixel_height = 0;
//  int mm_width = 0;
//  int mm_height = 0;

  float mm2pixelsX = 10.0;
  float mm2pixelsY = 10.0;

  switch (mode)
  {
    case MM_TWIPS:
    {
      m_logicalScaleX = (float)(twips2mm * mm2pixelsX);
      m_logicalScaleY = (float)(twips2mm * mm2pixelsY);
      break;
    }
    case MM_POINTS:
    {
      m_logicalScaleX = (float)(pt2mm * mm2pixelsX);
      m_logicalScaleY = (float)(pt2mm * mm2pixelsY);
      break;
    }
    case MM_METRIC:
    {
      m_logicalScaleX = mm2pixelsX;
      m_logicalScaleY = mm2pixelsY;
      break;
    }
    case MM_LOMETRIC:
    {
      m_logicalScaleX = (float)(mm2pixelsX/10.0);
      m_logicalScaleY = (float)(mm2pixelsY/10.0);
      break;
    }
    default:
    case MM_TEXT:
    {
      m_logicalScaleX = 1.0;
      m_logicalScaleY = 1.0;
      break;
    }
  }
  m_windowExtX = 100;
  m_windowExtY = 100;
}

#ifdef __WIN32__
struct RECT32
{
  short left;
  short top;
  short right;
  short bottom;
};

struct mfPLACEABLEHEADER {
	DWORD	key;
	short	hmf;
	RECT32	bbox;
	WORD	inch;
	DWORD	reserved;
	WORD	checksum;
};
#else
struct mfPLACEABLEHEADER {
	DWORD	key;
	HANDLE	hmf;
	RECT	bbox;
	WORD	inch;
	DWORD	reserved;
	WORD	checksum;
};
#endif

/*
 * Pass filename of existing non-placeable metafile, and bounding box.
 * Adds a placeable metafile header, sets the mapping mode to anisotropic,
 * and sets the window origin and extent to mimic the MM_TEXT mapping mode.
 *
 */
 
bool wxMakeMetaFilePlaceable(const wxString& filename, float scale)
{
  return wxMakeMetaFilePlaceable(filename, 0, 0, 0, 0, scale, FALSE);
}

bool wxMakeMetaFilePlaceable(const wxString& filename, int x1, int y1, int x2, int y2, float scale, bool useOriginAndExtent)
{
  // I'm not sure if this is the correct way of suggesting a scale
  // to the client application, but it's the only way I can find.
  int unitsPerInch = (int)(576/scale);
  
  mfPLACEABLEHEADER header;
  header.key = 0x9AC6CDD7L;
  header.hmf = 0;
  header.bbox.left = (int)(x1);
  header.bbox.top = (int)(y1);
  header.bbox.right = (int)(x2);
  header.bbox.bottom = (int)(y2);
  header.inch = unitsPerInch;
  header.reserved = 0;

  // Calculate checksum  
  WORD *p;
  mfPLACEABLEHEADER *pMFHead = &header;
  for (p =(WORD *)pMFHead,pMFHead -> checksum = 0;
	p < (WORD *)&pMFHead ->checksum; ++p)
       pMFHead ->checksum ^= *p;

  FILE *fd = fopen((char *)(const char *)filename, "rb");
  if (!fd) return FALSE;
  
  char tempFileBuf[256];
  wxGetTempFileName("mf", tempFileBuf);
  FILE *fHandle = fopen(tempFileBuf, "wb");
  if (!fHandle)
    return FALSE;
  fwrite((void *)&header, sizeof(unsigned char), sizeof(mfPLACEABLEHEADER), fHandle);

  // Calculate origin and extent
  int originX = x1;
  int originY = y1;
  int extentX = x2 - x1;
  int extentY = (y2 - y1);

  // Read metafile header and write
  METAHEADER metaHeader;
  fread((void *)&metaHeader, sizeof(unsigned char), sizeof(metaHeader), fd);
  
  if (useOriginAndExtent)
    metaHeader.mtSize += 15;
  else
    metaHeader.mtSize += 5;
    
  fwrite((void *)&metaHeader, sizeof(unsigned char), sizeof(metaHeader), fHandle);

  // Write SetMapMode, SetWindowOrigin and SetWindowExt records
  char modeBuffer[8];
  char originBuffer[10];
  char extentBuffer[10];
  METARECORD *modeRecord = (METARECORD *)&modeBuffer;

  METARECORD *originRecord = (METARECORD *)&originBuffer;
  METARECORD *extentRecord = (METARECORD *)&extentBuffer;

  modeRecord->rdSize = 4;
  modeRecord->rdFunction = META_SETMAPMODE;
  modeRecord->rdParm[0] = MM_ANISOTROPIC;

  originRecord->rdSize = 5;
  originRecord->rdFunction = META_SETWINDOWORG;
  originRecord->rdParm[0] = originY;
  originRecord->rdParm[1] = originX;

  extentRecord->rdSize = 5;
  extentRecord->rdFunction = META_SETWINDOWEXT;
  extentRecord->rdParm[0] = extentY;
  extentRecord->rdParm[1] = extentX;

  fwrite((void *)modeBuffer, sizeof(char), 8, fHandle);
  
  if (useOriginAndExtent)
  {
    fwrite((void *)originBuffer, sizeof(char), 10, fHandle);
    fwrite((void *)extentBuffer, sizeof(char), 10, fHandle);
  }

  int ch = -2;
  while (ch != EOF)
  {
    ch = getc(fd);
    if (ch != EOF)
    {
      putc(ch, fHandle);
    }
  }
  fclose(fHandle);
  fclose(fd);
  wxRemoveFile(filename);
  wxCopyFile(tempFileBuf, filename);
  wxRemoveFile(tempFileBuf);
  return TRUE;
}

#endif // USE_METAFILE
