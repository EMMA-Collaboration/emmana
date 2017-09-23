///
/// \file emma_module.hxx
/// \authors M. Williams, D. Connolly
/// \brief Defines emma analyzer classes
///

#include "manalyzer.h"
#include "midasio.h"

#include <assert.h> // assert()

#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TMath.h"
#include "TTree.h"
#include "TBranch.h"

#include "v1190unpack.h"
#include "mesadc32unpack.h"
#include "Alpha16.h"

#define DELETE(p) if (p) { delete(p); (p)=NULL; }

struct EmmaConfig
{
  bool fVerboseV1190 = false;
  bool fVerboseMesadc32 = false;
}; // end EmmaConfig

class EmmaModuleFactory: public TAFactory {
public:
  EmmaConfig* fConfig = new EmmaConfig;

public:
  void Finish();
  void Init(const std::vector<std::string> &args);
  TARunObject* NewRunObject(TARunInfo* runinfo);
}; // end EmmaModuleFactory

TARegister tarm(new EmmaModuleFactory);


// emacs
// Local Variables:
// tab-width: 8
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
