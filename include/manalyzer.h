/// EMMA MIDAS analyzer
///
/// \file: manalyzer.h
/// \author K.Olchanski, D. Connolly
/// \brief Defines classes used by EMMA MIDAS analyzer
///

#ifndef MANALYZER_H
#define MANALYZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <assert.h>
#include <signal.h>
#include <string>
#include <vector>
#include <deque>

#include "manalyzer.h"
#include "midasio.h"
#include "rootana_config.h"
#include "midasio.h"
#include "VirtualOdb.h"

#ifdef HAVE_MIDAS
#include "TMidasOnline.h"
#endif

#ifdef HAVE_ROOT
#include "TSystem.h"
#include <TGMenu.h>
#include <TGButton.h>
#include <TBrowser.h>
#endif

#ifdef HAVE_XMLSERVER
#include "xmlServer.h"
#include "TROOT.h"
#endif

#ifdef HAVE_ROOT_XML
#include "XmlOdb.h"
#endif

#ifdef XHAVE_LIBNETDIRECTORY
#include "netDirectoryServer.h"
#endif

#ifdef HAVE_THTTP_SERVER
#include "THttpServer.h"
#endif


#define CTRL_QUIT 1
#define CTRL_NEXT 2
#define CTRL_CONTINUE 3
#define CTRL_PAUSE    4
#define CTRL_TBROWSER 11

class TARootHelper;

class TARunInfo
{
public:
   int fRunNo;
   std::string fFileName;
   VirtualOdb* fOdb;
   TARootHelper* fRoot;
   std::vector<std::string> fArgs;

public:
   TARunInfo(int runno, const char* filename, const std::vector<std::string>& args);
   ~TARunInfo();

private:
   TARunInfo() {}; // hidden default constructor
};

class TAFlowEvent
{
public:
   TAFlowEvent* fNext;

public:
   TAFlowEvent(TAFlowEvent*);
   virtual ~TAFlowEvent();

   template<class T> T* Find()
   {
      TAFlowEvent* f = this;
      while (f) {
         T *ptr = dynamic_cast<T*>(f);
         if (ptr) return ptr;
         f = f->fNext;
      }
      return NULL;
   }

private:
   TAFlowEvent() {}; // hidden default constructor
};

typedef int TAFlags;

#define TAFlag_OK          0
#define TAFlag_SKIP    (1<<0)
#define TAFlag_QUIT    (1<<1)
#define TAFlag_WRITE   (1<<2)
#define TAFlag_DISPLAY (1<<3)

class TARunObject
{
public:
   TARunObject(TARunInfo* runinfo); // ctor
   virtual ~TARunObject() {}; // dtor

public:
   virtual void BeginRun(TARunInfo* runinfo); // begin of run
   virtual void EndRun(TARunInfo* runinfo); // end of run
   virtual void NextSubrun(TARunInfo* runinfo); // next subrun file

   virtual void PauseRun(TARunInfo* runinfo); // pause of run (if online)
   virtual void ResumeRun(TARunInfo* runinfo); // resume of run (if online)

   virtual void PreEndRun(TARunInfo* runinfo, std::deque<TAFlowEvent*>* flow_queue); // generate flow events before end of run

   virtual TAFlowEvent* Analyze(TARunInfo* runinfo, TMEvent* event, TAFlags* flags, TAFlowEvent* flow);
   virtual TAFlowEvent* AnalyzeFlowEvent(TARunInfo* runinfo, TAFlags* flags, TAFlowEvent* flow);
   virtual void AnalyzeSpecialEvent(TARunInfo* runinfo, TMEvent* event);

private:
   TARunObject(); // hidden default constructor
};

class TAFactory
{
public:
   TAFactory() {}; // ctor
   virtual ~TAFactory() {}; // dtor

public:
   virtual TARunObject* NewRunObject(TARunInfo* runinfo) = 0; // factory for Run objects

public:
   virtual void Init(const std::vector<std::string> &args); // start of analysis
   virtual void Finish(); // end of analysis
};

template<class T> class TAFactoryTemplate: public TAFactory
{
   T* NewRunObject(TARunInfo* runinfo)
   {
      return new T(runinfo);
   }
};

class TARegister
{
public:
   TARegister(TAFactory* m);
   //static void Register(TAModuleInterface* m);
   //static std::vector<TAModuleInterface*>* fgModules;
   //static std::vector<TAModuleInterface*>* Get() const;
};

#ifdef HAVE_ROOT

#include "TFile.h"
#include "TDirectory.h"
#include "TApplication.h"

class XmlServer;
class THttpServer;

class TARootHelper
{
public:
   TFile* fOutputFile;
   static TDirectory*   fgDir;
   static TApplication* fgApp;
   static XmlServer*    fgXmlServer;
   static THttpServer*  fgHttpServer;

public:
   TARootHelper(const TARunInfo*);
   ~TARootHelper(); // dtor

private:
   TARootHelper() { }; // hidden default constructor
};
#endif


// ==================== Class EmptyOdb ==================== //

class EmptyOdb: public VirtualOdb
{
public:
   int      odbReadArraySize(const char*name) { return 0; }
   int      odbReadAny(   const char*name, int index, int tid,void* buf, int bufsize = 0) { return 0; };
   int      odbReadInt(   const char*name, int index = 0, int      defaultValue = 0) { return defaultValue; }
   uint32_t odbReadUint32(const char*name, int index = 0, uint32_t defaultValue = 0) { return defaultValue; }
   float     odbReadFloat(const char*name, int index = 0, float   defaultValue = 0) { return defaultValue; }
   double   odbReadDouble(const char*name, int index = 0, double   defaultValue = 0) { return defaultValue; }
   bool     odbReadBool(  const char*name, int index = 0, bool     defaultValue = false) { return defaultValue; }
   const char* odbReadString(const char*name, int index = 0,const char* defaultValue = NULL) { return defaultValue; }
};

// ==================== Class RunHanler ==================== //

class RunHandler
{
public:
   TARunInfo* fRunInfo;
   std::vector<TARunObject*> fRunRun;
   std::vector<std::string>  fArgs;

public:
   RunHandler(const std::vector<std::string>& args); //ctor
   ~RunHandler(); // dtor
   void CreateRun(int run_number, const char* file_name);
   void BeginRun();
   void EndRun();
   void NextSubrun();
   void DeleteRun();
   void AnalyzeSpecialEvent(TMEvent* event);
   void AnalyzeEvent(TMEvent* event, TAFlags* flags, TMWriterInterface *writer);
};


// ==================== Class OnlineHanler ==================== //

class OnlineHandler: public TMHandlerInterface
{
public:
   RunHandler fRun;
   int fNumAnalyze;
   TMWriterInterface* fWriter;
   bool fQuit;

public:
   OnlineHandler(int num_analyze, TMWriterInterface* writer, const std::vector<std::string>& args); //ctor
   ~OnlineHandler(); // dtor
   void StartRun(int run_number);
   void Transition(int transition, int run_number, int transition_time);
   void Event(const void* data, int data_size);
};

// ==================== Class EventDumpModule ==================== //

class EventDumpModule: public TARunObject
{
public:
   EventDumpModule(TARunInfo* runinfo);
   ~EventDumpModule();
   void BeginRun(TARunInfo* runinfo);
   void EndRun(TARunInfo* runinfo);
   void NextSubrun(TARunInfo* runinfo);
   void PauseRun(TARunInfo* runinfo);
   void ResumeRun(TARunInfo* runinfo);
   TAFlowEvent* Analyze(TARunInfo* runinfo, TMEvent* event, TAFlags* flags, TAFlowEvent* flow);
   void AnalyzeSpecialEvent(TARunInfo* runinfo, TMEvent* event);
};

// ==================== Class EventDumpModuleFactory ==================== //

class EventDumpModuleFactory: public TAFactory
{
public:
   void Init(const std::vector<std::string> &args);
   void Finish();
   TARunObject* NewRunObject(TARunInfo* runinfo);
};


class XCtrl
{
public:
   int fValue;

public:
   XCtrl() { fValue = 0; }
};

class XButton: public TGTextButton
{
public:
   XCtrl* fCtrl;
   int    fValue;

public:
   XButton(TGWindow*p, const char* text, XCtrl* ctrl, int value);
#if 0
   void Pressed()  { printf("Pressed!\n"); }
   void Released() { printf("Released!\n"); }
#endif
   void Clicked();
};

class MainWindow: public TGMainFrame
{
private:
   TGPopupMenu*		fMenu;
   TGMenuBar*		fMenuBar;
   TGLayoutHints*	menuBarLayout;
   TGLayoutHints*	menuBarItemLayout;

public:
   XCtrl* fCtrl;

public:
   MainWindow(const TGWindow*w, int s1, int s2, XCtrl* ctrl);
   ~MainWindow(); // dtor // Closing the control window closes the whole program
   void CloseWindow();
   Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
};

class InteractiveModule: public TARunObject
{
public:
   bool fContinue;
   int  fSkip;
#ifdef HAVE_ROOT
   static XCtrl* fgCtrl;
   static MainWindow *fgCtrlWindow;
#endif

public:
   InteractiveModule(TARunInfo* runinfo);
   ~InteractiveModule();
   void BeginRun(TARunInfo* runinfo) { printf("InteractiveModule::BeginRun, run %d\n", runinfo->fRunNo); }
   void PauseRun(TARunInfo* runinfo) { printf("InteractiveModule::PauseRun, run %d\n", runinfo->fRunNo); }
   void ResumeRun(TARunInfo* runinfo) { printf("InteractiveModule::ResumeRun, run %d\n", runinfo->fRunNo); }
   void EndRun(TARunInfo* runinfo);
   TAFlowEvent* Analyze(TARunInfo* runinfo, TMEvent* event, TAFlags* flags, TAFlowEvent* flow);
   void AnalyzeSpecialEvent(TARunInfo* runinfo, TMEvent* event);
};


class InteractiveModuleFactory: public TAFactory
{
public:
   void Init(const std::vector<std::string> &args);
   void Finish();
   TARunObject* NewRunObject(TARunInfo* runinfo);
};



int manalyzer_main(int argc, char* argv[]);


#endif

/* emacs
 * Local Variables:
 * tab-width: 8
 * c-basic-offset: 3
 * indent-tabs-mode: nil
 * End:
 */
