/////////////////////////////////////////////////////////////////////////////
// Name:        thread.h
// Purpose:     topic overview
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/*!

 @page overview_thread Multithreading overview

 Classes: #wxThread, #wxMutex, #wxCriticalSection, #wxCondition

 wxWidgets provides a complete set of classes encapsulating objects necessary in
 multithreaded (MT) programs: the wxThread class itself and different
 synchronization objects: mutexes (see wxMutex) and critical sections (see 
 wxCriticalSection)  with conditions (see wxCondition). The thread API in wxWidgets 
 resembles to POSIX1.c threads API (a.k.a. pthreads), although several functions have
 different names and some features inspired by Win32 thread API are there as well.

 These classes will hopefully make writing MT programs easier and they also
 provide some extra error checking (compared to the native (be it Win32 or Posix)
 thread API), however it is still a non-trivial undertaking especially for large
 projects. Before starting an MT application (or starting to add MT features to
 an existing one) it is worth asking oneself if there is no easier and safer way
 to implement the same functionality. Of course, in some situations threads
 really make sense (classical example is a server application which launches a
 new thread for each new client), but in others it might be a very poor choice
 (example: launching a separate thread when doing a long computation to show a
 progress dialog). Other implementation choices are available: for the progress
 dialog example it is far better to do the calculations in the idle handler
 (see @ref wxIdleEvent) or even simply do everything at once but call wxWindow::Update() 
 periodically to update the screen.

 If you do decide to use threads in your application, it is strongly recommended
 that no more than one thread calls GUI functions. The thread sample shows that
 it @e is possible for many different threads to call GUI functions at once
 (all the threads created in the sample access GUI), but it is a very poor design
 choice for anything except an example. The design which uses one GUI thread and
 several worker threads which communicate with the main one using events is much
 more robust and will undoubtedly save you countless problems (example: under
 Win32 a thread can only access GDI objects such as pens, brushes, c created by
 itself and not by the other threads).

 For communication between secondary threads and the main thread, you may use
 wxEvtHandler::AddPendingEvent or its short version #wxPostEvent. These functions
 have a thread-safe implementation so that they can be used as they are for
 sending events from one thread to another. However there is no built in method
 to send messages to the worker threads and you will need to use the available
 synchronization classes to implement the solution which suits your needs
 yourself. In particular, please note that it is not enough to derive
 your class from #wxThread and #wxEvtHandler to send messages to it: 
 in fact, this does not work at all.

*/

