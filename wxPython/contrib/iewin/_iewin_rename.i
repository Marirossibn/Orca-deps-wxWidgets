// A bunch of %rename directives generated by ./distrib/build_renamers.py
// in order to remove the wx prefix from all global scope names.

#ifndef BUILDING_RENAMERS

%rename(MSHTMLEvent)                        wxMSHTMLEvent;
%rename(IEHTML_REFRESH_NORMAL)              wxIEHTML_REFRESH_NORMAL;
%rename(IEHTML_REFRESH_IFEXPIRED)           wxIEHTML_REFRESH_IFEXPIRED;
%rename(IEHTML_REFRESH_CONTINUE)            wxIEHTML_REFRESH_CONTINUE;
%rename(IEHTML_REFRESH_COMPLETELY)          wxIEHTML_REFRESH_COMPLETELY;
%rename(IEHtmlWin)                          wxIEHtmlWin;

#endif
