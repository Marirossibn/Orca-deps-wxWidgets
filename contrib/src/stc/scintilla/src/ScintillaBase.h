// Scintilla source code edit control
// ScintillaBase.h - defines an enhanced subclass of Editor with calltips, autocomplete and context menu
// Copyright 1998-2000 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SCINTILLABASE_H
#define SCINTILLABASE_H

class ScintillaBase : public Editor {
protected:
	// Enumeration of commands and child windows
	enum {
		idCallTip=1,
		idAutoComplete=2,
		
		idcmdUndo=10,
		idcmdRedo=11,
		idcmdCut=12,
		idcmdCopy=13,
		idcmdPaste=14,
		idcmdDelete=15,
		idcmdSelectAll=16
	};

	Menu popup;
	AutoComplete ac;

	CallTip ct;

#ifdef SCI_LEXER
	int lexLanguage;
	PropSet props;
	enum {numWordLists=5};
	WordList *keyWordLists[numWordLists];
	void Colourise(int start, int end);
#endif

	ScintillaBase();
	virtual ~ScintillaBase();
	virtual void Initialise() = 0;
	virtual void Finalise() = 0;

	virtual void RefreshColourPalette(Palette &pal, bool want);
	
	virtual void AddChar(char ch);
	void Command(int cmdId);
	virtual int KeyCommand(UINT iMessage);
	
	void AutoCompleteStart(int lenEntered, const char *list);
	void AutoCompleteCancel();
	void AutoCompleteMove(int delta);
	void AutoCompleteChanged(char ch=0);
	void AutoCompleteCompleted();

	virtual void CreateCallTipWindow(PRectangle rc) = 0;
		
	virtual void AddToPopUp(const char *label, int cmd=0, bool enabled=true) = 0;
	void ContextMenu(Point pt);
	
	virtual void ButtonDown(Point pt, unsigned int curTime, bool shift, bool ctrl, bool alt);

	virtual void NotifyStyleNeeded(int endStyleNeeded);
public:
	// Public so scintilla_send_message can use it
	virtual LRESULT WndProc(UINT iMessage, WPARAM wParam, LPARAM lParam);
};

#endif
