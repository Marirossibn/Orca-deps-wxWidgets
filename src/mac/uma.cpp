#include "wx/defs.h"
#include "wx/dc.h"
#include "wx/mac/uma.h"
#include <MacTextEditor.h>

#ifndef __DARWIN__
  #include <Navigation.h>
#endif

// since we have decided that we only support 8.6 upwards we are
// checking for these minimum requirements in the startup code of
// the application so all wxWindows code can safely assume that appearance 1.1
// windows manager, control manager, navigation services etc. are 
// present

#define wxUSE_MLTE 0

static bool	sUMAHasAppearance = false ;
static long sUMAAppearanceVersion = 0 ;
extern int gAGABackgroundColor ;
bool UMAHasAppearance() { return sUMAHasAppearance ; }
long UMAGetAppearanceVersion() { return sUMAAppearanceVersion ; }

static bool	sUMAHasWindowManager = false ;
static long sUMAWindowManagerAttr = 0 ;

bool UMAHasWindowManager() { return sUMAHasWindowManager ; }
long UMAGetWindowManagerAttr() { return sUMAWindowManagerAttr ; }

void UMACleanupToolbox()
{
	if ( sUMAHasAppearance )
	{
		UnregisterAppearanceClient() ;
	}
	if ( NavServicesAvailable() )
	{
		NavUnload() ;
	}
#if wxUSE_MLTE
	TXNTerminateTextension( ) ;
#endif
}
void UMAInitToolbox( UInt16 inMoreMastersCalls )
{
#if !TARGET_CARBON
	::MaxApplZone();
	for (long i = 1; i <= inMoreMastersCalls; i++)
		::MoreMasters();

	::InitGraf(&qd.thePort);
	::InitFonts();
	::InitMenus();
	::TEInit();
	::InitDialogs(0L);
	::FlushEvents(everyEvent, 0);
	::InitCursor();
	long total,contig;
	PurgeSpace(&total, &contig);
#else
	InitCursor();
#endif

	long theAppearance ;
	if ( Gestalt( gestaltAppearanceAttr, &theAppearance ) == noErr )
	{
		sUMAHasAppearance = true ;
		RegisterAppearanceClient();
		if ( Gestalt( gestaltAppearanceVersion, &theAppearance ) == noErr )
		{
			sUMAAppearanceVersion = theAppearance ;
		}
		else
		{
			sUMAAppearanceVersion = 0x0100 ;
		}
	}
	if ( Gestalt( gestaltWindowMgrAttr, &sUMAWindowManagerAttr ) == noErr )
	{
		sUMAHasWindowManager = sUMAWindowManagerAttr & gestaltWindowMgrPresent ;
	}
		
#ifndef __DARWIN__
#if TARGET_CARBON
// Call currently implicitely done :		InitFloatingWindows() ;
#else
	if ( sUMAHasWindowManager )
		InitFloatingWindows() ;
	else
		InitWindows();
#endif
#endif

	if ( NavServicesAvailable() )
	{
		NavLoad() ;
	}

#if wxUSE_MLTE
  TXNMacOSPreferredFontDescription defaults;
  defaults.fontID = kFontIDGeneva ;
  defaults.pointSize = (10 << 16) ;
  defaults.fontStyle = kTXNDefaultFontStyle;
  defaults.encoding = kTXNSystemDefaultEncoding;
	TXNInitTextension(&defaults,  1, (kTXNAlwaysUseQuickDrawTextMask | kTXNWantMoviesMask | kTXNWantSoundMask | kTXNWantGraphicsMask));
#endif
}

// process manager
long UMAGetProcessMode() 
{
	OSErr err ; 
	ProcessInfoRec processinfo;
	ProcessSerialNumber procno ;
	
	procno.highLongOfPSN = NULL ;
	procno.lowLongOfPSN = kCurrentProcess ;
	processinfo.processInfoLength = sizeof(ProcessInfoRec);
	processinfo.processName = NULL;
	processinfo.processAppSpec = NULL;

	err = ::GetProcessInformation( &procno , &processinfo ) ;
	wxASSERT( err == noErr ) ;
	return processinfo.processMode ;
}

bool UMAGetProcessModeDoesActivateOnFGSwitch() 
{
	return UMAGetProcessMode() & modeDoesActivateOnFGSwitch ;
}

// menu manager

void UMASetMenuTitle( MenuRef menu , StringPtr title )
{
#if !TARGET_CARBON
	long 			size = GetHandleSize( (Handle) menu ) ;
	const long 		headersize = 14 ;
	int				oldlen = (**menu).menuData[0] + 1;
	int				newlen = title[0] + 1 ;
	
	if ( oldlen < newlen )
	{
		// enlarge before adjusting
		SetHandleSize( (Handle) menu , size + (newlen - oldlen ) );
	}

	if ( oldlen != newlen )
		memmove( (char*) (**menu).menuData + newlen , (char*) (**menu).menuData + oldlen , size - headersize - oldlen ) ;

	memcpy( (char*) (**menu).menuData , title ,  newlen ) ;
	if ( oldlen > newlen )
	{
		// shrink after
		SetHandleSize( (Handle) menu , size + (newlen - oldlen ) ) ;
	}
#else
	SetMenuTitle( menu , title ) ;
#endif
}

UInt32 UMAMenuEvent( EventRecord *inEvent )
{
		return MenuEvent( inEvent ) ;
}

void 			UMAEnableMenuItem( MenuRef inMenu , MenuItemIndex inItem ) 
{
	EnableMenuItem( inMenu , inItem ) ;
}

void 			UMADisableMenuItem( MenuRef inMenu , MenuItemIndex inItem ) 
{
	DisableMenuItem( inMenu , inItem ) ;
}

void UMAAppendSubMenuItem( MenuRef menu , StringPtr l , SInt16 id ) 
{
	Str255 label ;
	memcpy( label , l , l[0]+1 ) ;
	// hardcoded adding of the submenu combination for mac

	int theEnd = label[0] + 1; 
	if (theEnd > 251) 
		theEnd = 251; // mac allows only 255 characters
	label[theEnd++] = '/';
	label[theEnd++] = hMenuCmd; 
	label[theEnd++] = '!';
	label[theEnd++] = id ; 
	label[theEnd] = 0x00;
	label[0] = theEnd;
	MacAppendMenu(menu, label);
}

void UMAInsertSubMenuItem( MenuRef menu , StringPtr l , MenuItemIndex item , SInt16 id  ) 
{
	Str255 label ;
	memcpy( label , l , l[0]+1 ) ;
	// hardcoded adding of the submenu combination for mac

	int theEnd = label[0] + 1; 
	if (theEnd > 251) 
		theEnd = 251; // mac allows only 255 characters
	label[theEnd++] = '/';
	label[theEnd++] = hMenuCmd; 
	label[theEnd++] = '!';
	label[theEnd++] = id; 
	label[theEnd] = 0x00;
	label[0] = theEnd;
	MacInsertMenuItem(menu, label , item);
}

void UMAAppendMenuItem( MenuRef menu , StringPtr l , SInt16 key, UInt8 modifiers ) 
{
	Str255 label ;
	memcpy( label , l , l[0]+1 ) ;
	if ( key )
	{
			int pos = label[0] ;
			label[++pos] = '/';
			label[++pos] = toupper( key );
			label[0] = pos ;
	}
	MacAppendMenu( menu , label ) ;
}

void UMAInsertMenuItem( MenuRef menu , StringPtr l , MenuItemIndex item , SInt16 key, UInt8 modifiers ) 
{
	Str255 label ;
	memcpy( label , l , l[0]+1 ) ;
	if ( key )
	{
			int pos = label[0] ;
			label[++pos] = '/';
			label[++pos] = toupper( key );
			label[0] = pos ;
	}
	MacInsertMenuItem( menu , label , item) ;
}

// quickdraw

int gPrOpenCounter = 0 ;

#if TARGET_CARBON && PM_USE_SESSION_APIS
OSStatus UMAPrOpen(PMPrintSession *macPrintSession)
#else
OSStatus UMAPrOpen()
#endif
{
#if !TARGET_CARBON
	OSErr err = noErr ;
	++gPrOpenCounter ;
	if ( gPrOpenCounter == 1 )
	{
		PrOpen() ;
		err = PrError() ;
		wxASSERT( err == noErr ) ;
	}
	return err ;
#else
	OSStatus err = noErr ;
	++gPrOpenCounter ;
	if ( gPrOpenCounter == 1 )
	{
  #if PM_USE_SESSION_APIS
	    err = PMCreateSession(macPrintSession) ;
  #else
	    err = PMBegin() ;
  #endif
		wxASSERT( err == noErr ) ;
	}
	return err ;
#endif
}

#if TARGET_CARBON && PM_USE_SESSION_APIS
OSStatus UMAPrClose(PMPrintSession *macPrintSession)
#else
OSStatus UMAPrClose()
#endif
{
#if !TARGET_CARBON
	OSErr err = noErr ;
	wxASSERT( gPrOpenCounter >= 1 ) ;
	if ( gPrOpenCounter == 1 )
	{
		PrClose() ;
		err = PrError() ;
		wxASSERT( err == noErr ) ;
	}
	--gPrOpenCounter ;
	return err ;
#else
	OSStatus err = noErr ;
	wxASSERT( gPrOpenCounter >= 1 ) ;
	if ( gPrOpenCounter == 1 )
	{
  #if PM_USE_SESSION_APIS
	    err = PMRelease(*macPrintSession) ;
	    *macPrintSession = kPMNoReference;
  #else
	    err = PMEnd() ;
  #endif
	}
	--gPrOpenCounter ;
	return err ;
#endif
}

#if !TARGET_CARBON

pascal QDGlobalsPtr GetQDGlobalsPtr (void) ;
pascal QDGlobalsPtr GetQDGlobalsPtr (void)
{
	return QDGlobalsPtr (* (Ptr*) LMGetCurrentA5 ( ) - 0xCA);
}

#endif

void UMAShowWatchCursor() 
{
	OSErr err = noErr;

	CursHandle watchFob = GetCursor (watchCursor);

	if (!watchFob)
		err = nilHandleErr;
	else
	{
	#if TARGET_CARBON
//		Cursor preservedArrow;
//		GetQDGlobalsArrow (&preservedArrow);
//		SetQDGlobalsArrow (*watchFob);
//		InitCursor ( );
//		SetQDGlobalsArrow (&preservedArrow);
		SetCursor (*watchFob);	
	#else
		SetCursor (*watchFob);	
	#endif
	}
}

void			UMAShowArrowCursor() 
{
#if TARGET_CARBON
	Cursor arrow;
	SetCursor (GetQDGlobalsArrow (&arrow));
#else
	SetCursor (&(qd.arrow));
#endif
}

// window manager

GrafPtr		UMAGetWindowPort( WindowRef inWindowRef ) 
{
	wxASSERT( inWindowRef != NULL ) ;
#if TARGET_CARBON 
	return GetWindowPort( inWindowRef ) ; 
#else
	return (GrafPtr) inWindowRef ;
#endif
}

void		 	UMADisposeWindow( WindowRef inWindowRef ) 
{
	wxASSERT( inWindowRef != NULL ) ;
	DisposeWindow( inWindowRef ) ;
}

void 			UMASetWTitleC( WindowRef inWindowRef , const char *title ) 
{
	Str255	ptitle ;
	strncpy( (char*)ptitle , title , 96 ) ;
	ptitle[96] = 0 ;
#if TARGET_CARBON
	c2pstrcpy( ptitle, (char *)ptitle ) ;
#else
	c2pstr( (char*)ptitle ) ;
#endif
	SetWTitle( inWindowRef , ptitle ) ;
}

void 			UMAGetWTitleC( WindowRef inWindowRef , char *title ) 
{
	GetWTitle( inWindowRef , (unsigned char*)title ) ;
#if TARGET_CARBON
	p2cstrcpy( title, (unsigned char *)title ) ;
#else
	p2cstr( (unsigned char*)title ) ;
#endif
}

// appearance additions

void UMAActivateControl( ControlHandle inControl ) 
{
      if ( !IsControlActive( inControl ) )
      {
        bool visible = IsControlVisible( inControl ) ;
        if ( visible )
          SetControlVisibility( inControl , false , false ) ;
        ::ActivateControl( inControl ) ;
        if ( visible ) {
          SetControlVisibility( inControl , true , false ) ;
          Rect ctrlBounds ;
          InvalWindowRect(GetControlOwner(inControl),GetControlBounds(inControl,&ctrlBounds) ) ;
        }
       }
}

void UMADrawControl( ControlHandle inControl ) 
{
    WindowRef theWindow = GetControlOwner(inControl) ;
    RgnHandle updateRgn = NewRgn() ;
    GetWindowUpdateRgn( theWindow , updateRgn ) ;
	Point zero = { 0 , 0 } ;
	LocalToGlobal( &zero ) ;
	OffsetRgn( updateRgn , -zero.h , -zero.v ) ;
  ::DrawControlInCurrentPort( inControl ) ;
  InvalWindowRgn( theWindow, updateRgn) ;
	DisposeRgn( updateRgn ) ;

}

void UMAMoveControl( ControlHandle inControl , short x , short y ) 
{
  bool visible = IsControlVisible( inControl ) ;
  if ( visible ) {
    SetControlVisibility( inControl , false , false ) ;
    Rect ctrlBounds ;
    InvalWindowRect(GetControlOwner(inControl),GetControlBounds(inControl,&ctrlBounds) ) ;
  }
  ::MoveControl( inControl , x , y ) ;
  if ( visible ) {
    SetControlVisibility( inControl , true , false ) ;
    Rect ctrlBounds ;
    InvalWindowRect(GetControlOwner(inControl),GetControlBounds(inControl,&ctrlBounds) ) ;
  }
}

void UMASizeControl( ControlHandle inControl , short x , short y ) 
{
  bool visible = IsControlVisible( inControl ) ;
  if ( visible ) {
    SetControlVisibility( inControl , false , false ) ;
    Rect ctrlBounds ;
    InvalWindowRect(GetControlOwner(inControl),GetControlBounds(inControl,&ctrlBounds) ) ;
  } 
  ::SizeControl( inControl , x , y ) ;
  if ( visible ) {
    SetControlVisibility( inControl , true , false ) ;
    Rect ctrlBounds ;
    InvalWindowRect(GetControlOwner(inControl),GetControlBounds(inControl,&ctrlBounds) ) ;
  }
}

void UMADeactivateControl( ControlHandle inControl ) 
{
   if ( IsControlActive( inControl ) )
   {
     bool visible = IsControlVisible( inControl ) ;
     if ( visible )
       SetControlVisibility( inControl , false , false ) ;
     ::DeactivateControl( inControl ) ;
     if ( visible ) {
       SetControlVisibility( inControl , true , false ) ;
       Rect ctrlBounds ;
       InvalWindowRect(GetControlOwner(inControl),GetControlBounds(inControl,&ctrlBounds) ) ;
     }
    }
}
// shows the control and adds the region to the update region
void UMAShowControl						(ControlHandle 			inControl)
{
        SetControlVisibility( inControl , true , false ) ;
        Rect ctrlBounds ;
        InvalWindowRect(GetControlOwner(inControl),GetControlBounds(inControl,&ctrlBounds) ) ;
}

// keyboard focus
OSErr UMASetKeyboardFocus				(WindowPtr 				inWindow,
								 ControlHandle 			inControl,
								 ControlFocusPart 		inPart)
{
	OSErr err = noErr;
	GrafPtr port ;
	GetPort( &port ) ;

	SetPortWindowPort( inWindow ) ;

	SetOrigin( 0 , 0 ) ;
  err = SetKeyboardFocus( inWindow , inControl , inPart ) ;
	SetPort( port ) ;
	return err ;
}




// events
void UMAUpdateControls( WindowPtr inWindow , RgnHandle inRgn ) 
{
  RgnHandle updateRgn = NewRgn() ;
  GetWindowUpdateRgn( inWindow , updateRgn ) ;

	Point zero = { 0 , 0 } ;
	LocalToGlobal( &zero ) ;
	OffsetRgn( updateRgn , -zero.h , -zero.v ) ;

  UpdateControls( inWindow , inRgn ) ;
  InvalWindowRgn( inWindow, updateRgn) ;
	DisposeRgn( updateRgn ) ;

}

bool UMAIsWindowFloating( WindowRef inWindow ) 
{
	WindowClass cl ;
	
	GetWindowClass( inWindow , &cl ) ;
	return cl == kFloatingWindowClass ;
}

bool UMAIsWindowModal( WindowRef inWindow ) 
{
	WindowClass cl ;
	
	GetWindowClass( inWindow , &cl ) ;
	return cl < kFloatingWindowClass ;
}

// others

void UMAHighlightAndActivateWindow( WindowRef inWindowRef , bool inActivate )
{
	if ( inWindowRef )
	{
//		bool isHighlighted = IsWindowHighlited( inWindowRef ) ;
//		if ( inActivate != isHightlited )
		GrafPtr port ;
		GetPort( &port ) ;
		SetPortWindowPort( inWindowRef ) ;
		SetOrigin( 0 , 0 ) ;
		HiliteWindow( inWindowRef , inActivate ) ;
		ControlHandle control = NULL ;
		::GetRootControl( inWindowRef , & control ) ;
		if ( control )
		{
			if ( inActivate )
				UMAActivateControl( control ) ;
			else
				UMADeactivateControl( control ) ;
		}	
		SetPort( port ) ;
	}
}
OSStatus UMADrawThemePlacard( const Rect *inRect , ThemeDrawState inState ) 
{
  return ::DrawThemePlacard( inRect , inState ) ;
}

static OSStatus helpMenuStatus = noErr ;
static MenuRef helpMenuHandle = NULL ;
static MenuItemIndex firstCustomItemIndex = 0 ;

OSStatus UMAGetHelpMenu(
  MenuRef *        outHelpMenu,
  MenuItemIndex *  outFirstCustomItemIndex)
{
#if TARGET_CARBON
  return HMGetHelpMenu( outHelpMenu , outFirstCustomItemIndex ) ;
#else
  if ( helpMenuHandle == NULL )
  {
    helpMenuStatus = HMGetHelpMenuHandle( &helpMenuHandle ) ;
    if ( helpMenuStatus == noErr )
    {
      firstCustomItemIndex = CountMenuItems( helpMenuHandle ) + 1 ;
    }
  }
  if ( outFirstCustomItemIndex )
  {
    *outFirstCustomItemIndex = firstCustomItemIndex ;
  }
  *outHelpMenu = helpMenuHandle ;
  return helpMenuStatus ;
#endif
}
