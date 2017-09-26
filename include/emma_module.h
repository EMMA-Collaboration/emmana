///
/// \file emma_module.h
/// \authors M. Williams, D. Connolly
/// \brief Defines emma analyzer classes
///

#include "manalyzer.h"
#include "midasio.h"

#include <assert.h> // assert()

#include "TCanvas.h"
#include "TStyle.h"
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

struct EmmaConfig {
   bool fVerboseV1190 = false;
   bool fVerboseMesadc32 = false;
}; // end EmmaConfig

class EmmaModule: public TARunObject {
public:
   EmmaConfig* fConfig = NULL;
   int fCounter;

public:
   static const Double_t MAX_TIME  = 999999;
   static const Double_t Si_E_low  = 800;
   static const Double_t Si_E_high = 1100;
   static const Double_t An_E_low  = 1100;
   static const Double_t An_E_high = 1300;
   static const Double_t rft_low   = 1900;
   static const Double_t rft_high  = 2200;

public:
   EmmaModule(TARunInfo* runinfo, EmmaConfig* config);
   ~EmmaModule();
   void ResetHistograms();
   void PlotHistograms(TARunInfo* runinfo);
   void UpdateHistograms(TARunInfo* runinfo, const v1190event* tdc_data, const mesadc32event* adc_data);
   void BeginRun(TARunInfo* runinfo);
   void EndRun(TARunInfo* runinfo);
   void PauseRun(TARunInfo* runinfo) { printf("PauseRun, run %d\n", runinfo->fRunNo); }
   void ResumeRun(TARunInfo* runinfo) { printf("ResumeRun, run %d\n", runinfo->fRunNo); }
   TAFlowEvent* Analyze(TARunInfo* runinfo, TMEvent* event, TAFlags* flags, TAFlowEvent* flow);
   void AnalyzeSpecialEvent(TARunInfo* runinfo, TMEvent* event);

public:
   // EMMA things go below here:
   Int_t ct;
   Int_t ach[6] = {0, 1, 2, 16, 18, 20};
   // Int_t multi_at;
   // Int_t multi_am;
   // Int_t multi_ab;
   // Int_t multi_xl;
   // Int_t multi_xr;
   // Int_t multi_yt;
   // Int_t multi_yb;
   // Int_t multi_rf;

   Double_t at;
   Double_t am;
   Double_t ab;
   Double_t anode;

   Double_t xl;
   Double_t xr;
   Double_t xsum;
   Double_t xdiff;
   Double_t xpos;

   Double_t yb;
   Double_t yt;
   Double_t ysum;
   Double_t ydiff;
   Double_t ypos;

   Double_t trig;

   Double_t Sienergy;
   Double_t ATenergy;
   Double_t AMenergy;
   Double_t ABenergy;
   Double_t AnodeEnergy;
   Double_t sbl_ene;
   Double_t sbr_ene;

   Double_t trf;
   Double_t trf_next;
   Double_t rftof;
   Double_t rft;

   TH1D *fHTdcRaw[64];
   TH1D *fHAdcRaw[32];
   TH1D *hAnodeEnergy;
   TH1D *hSum[2];
   TH1D *h1DPosition[2];
   TH1D *h1DPositionSiliconGated[2];
   TH1D *h1DPositionAnodeGated[2];
   TH1D *h1DPositionRFTOFGated[2];
   TH1D *h1DPositionPID2Gated[2];
   TH1D *hrft;
   TH1D *hMulti[8];
   TH2D *h2DPositionSiliconGated;
   TH2D *h2DPositionAnodeGated;
   TH2D *h2DPositionRFTOFGated;
   TH2D *h2DPositionPID2Gated;
   TH2D *hSiliconAnodeEnergy;
   TH2D *hAnodeEnergyRFTOF;
   TH2D *h2DPosition;

   TCanvas* fCanvasSums;
   TCanvas* fCanvas1DPosition;
   TCanvas* fCanvas2DPosition;
   TCanvas* fCanvas1DPositionSiliconGated;
   TCanvas* fCanvas2DPositionSiliconGated;
   TCanvas* fCanvas1DPositionAnodeGated;
   TCanvas* fCanvas2DPositionAnodeGated;
   TCanvas* fCanvas1DPositionRFTOFGated;
   TCanvas* fCanvas2DPositionRFTOFGated;
   TCanvas* fCanvas1DPositionPID2Gated;
   TCanvas* fCanvas2DPositionPID2Gated;
   TCanvas* fCanvasEnergySpectra;
   TCanvas* fCanvasPID1;
   TCanvas* fCanvasPID2;
   TCanvas* fCanvasMulti;

   TTree *t1;

}; // end EmmaModule


class EmmaModuleFactory: public TAFactory {
public:
   EmmaConfig* fConfig = new EmmaConfig;

public:
   void Finish() { printf("Finish!\n"); }
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
