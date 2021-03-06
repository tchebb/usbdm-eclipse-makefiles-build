/*! \file
    \brief Memory Dump Application

    MemoryDumpApp.cpp

    \verbatim
    USBDM
    Copyright (C) 2009  Peter O'Donoghue

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    \endverbatim

    \verbatim
   Change History
   -=========================================================================================
   | 15 Mar 2015 | Complete redesign using wxFormBuilder                   - pgo V4.10.6.260
   +=========================================================================================
   \endverbatim
*/
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/wx.h>
#include <wx/cmdline.h>
#include <wx/stdpaths.h>

#include <wx/wx.h>
#include <wx/cmdline.h>
#include <wx/stdpaths.h>

#include "AppSettings.h"
#include "MemoryDumpDialogue.h"
#include "UsbdmSystem.h"
#include "Common.h"

using namespace std;

#define CONFIG_FILE_NAME "MemoryDump_"

class Logger {
public:
   Logger(const char *name) {
      UsbdmSystem::Log::openLogFile(name, "Memory Dump");
      UsbdmSystem::Log::setLoggingLevel(100);
   }
   ~Logger() {
      UsbdmSystem::Log::closeLogFile();
   }
};

// Declare the application class
class MemoryDumpApp : public wxApp {
   DECLARE_CLASS( MemoryDumpApp )
   DECLARE_EVENT_TABLE()

private:
   Logger          logger;
   AppSettingsPtr  appsettings;

public:
   MemoryDumpApp();

   // Called on application startup
   virtual bool OnInit();
   virtual int  OnExit();
   virtual int  OnRun();
   virtual ~MemoryDumpApp();
};

// Implements MemoryDumpApp& GetApp()
DECLARE_APP(MemoryDumpApp)
IMPLEMENT_APP(MemoryDumpApp)

/*
 * MemoryDumpApp type definition
 */
IMPLEMENT_CLASS(MemoryDumpApp, wxApp)

/*
 * MemoryDumpApp event table definition
 */
BEGIN_EVENT_TABLE( MemoryDumpApp, wxApp )

END_EVENT_TABLE()

MemoryDumpApp::MemoryDumpApp() :
   logger("MemoryDump.log") {
}

bool MemoryDumpApp::OnInit() {
   LOGGING_E;

#ifndef _WIN32
   // Otherwise wxWidgets doesn't look in the correct location
   ((wxStandardPaths&)wxStandardPaths::Get()).SetInstallPrefix(_(USBDM_INSTALL_DIRECTORY));
#endif

   SetAppName(_("usbdm")); // So application files are kept in the correct directory

   // call for default command parsing behaviour
   if (!wxApp::OnInit()) {
      log.error("Failed OnInit()\n");
      return false;
   }

   appsettings.reset(new AppSettings("MemoryDump.cfg", "Memory Dump Configuration"));
   appsettings->load();
//   appsettings->printToLog();

   // Create and display the main application window
   MemoryDumpDialogue *dialogue = new MemoryDumpDialogue(NULL, appsettings);
   SetTopWindow((wxWindow*)dialogue);
   dialogue->ShowModal();
   appsettings->save();
   dialogue->Destroy();

   return true;
}

int MemoryDumpApp::OnRun(void) {
   LOGGING_E;
   return wxApp::OnRun();
}

int MemoryDumpApp::OnExit(void) {
   LOGGING_E;
   return wxApp::OnExit();
}

MemoryDumpApp::~MemoryDumpApp() {
}
