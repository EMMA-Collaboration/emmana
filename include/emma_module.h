///
/// \file emma_module.h
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

struct EmmaConfig {
   bool fVerboseV1190 = false;
   bool fVerboseMesadc32 = false;
}; // end EmmaConfig

class EmmaModule: public TARunObject {
public:
   EmmaConfig* fConfig = NULL;
   int fCounter;

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
   int ct;
   int ach[6] = {0, 1, 2, 16, 18, 20};

   TH1D *fHTdcTrig;
   TH1D *fHTdcRaw[64];
   TH1D *fHAdcRaw[32];
   TH1D *hSienergy;
   TH1D *hADC_used[6];
   TH1D *hATenergy;
   TH1D *hAMenergy;
   TH1D *hABenergy;
   TH1D *hPGACenergy;
   TH2D *hdE_E;
   TH1D *x_y_diff[2];
   TH1D *x_y_diff_Gated[2];
   TH1D *x_y_sum[2];
   TH2D *x_y_diff_vs_sum[2];
   TH1D *hXPosition;
   TH1D *hXPosition_Gated;
   TH1D *hYPosition;
   TH1D *hYPosition_Gated;
   TH2D *hXYPosition;
   TH2D *hXYPosition_Gated;
   TH1D *hRF;
   TH1D *hsbl;
   TH1D *hsbr;

   TH1D *hmulti_at;
   TH1D *hmulti_am;
   TH1D *hmulti_ab;
   TH1D *hmulti_xr;
   TH1D *hmulti_xl;
   TH1D *hmulti_yt;
   TH1D *hmulti_yb;
   TH1D *hmulti_trig;

   TCanvas* fCanvasTdcRaw;
   TCanvas* fCanvasTdcUsed;
   TCanvas* fCanvasAdcRaw;
   TCanvas* fCanvasXYDiffs;
   TCanvas* fCanvasXYDiffsGated;
   TCanvas* fCanvasXYSums;
   TCanvas* fCanvasDiffVsSum;
   TCanvas* fCanvasXPosition;
   TCanvas* fCanvasXPosition_Gated;
   TCanvas* fCanvasYPosition;
   TCanvas* fCanvasYPosition_Gated;
   TCanvas* fCanvasXYPositions;
   TCanvas* fCanvasXYPositions_Gated;
   TCanvas* fCanvasEnergy;
   TCanvas* fCanvasdE_E;
   TCanvas* fCanvasAnodeMulti;
   TCanvas* fCanvasCathodeMulti;
   TCanvas* fCanvasRF;
   TCanvas* fCanvasSSB;

   TH1D*    fHTdcNhits = NULL;
   TH1D*    fHAdcNhits = NULL;
   TH1D*    fHAdcTime0 = NULL;
   TH1D*    fHTdcTime0 = NULL;
   TH1D*    fHAdcTime1 = NULL;
   TH1D*    fHTdcTime1 = NULL;
   TH1D*    fHAdcTime2 = NULL;
   TH1D*    fHTdcTime2 = NULL;
   TH1D*    fHAdcTdcTime = NULL;

   Double_t at;
   Double_t am;
   Double_t ab;
   Double_t anode;
   Double_t xl;
   Double_t xr;
   Double_t yb;
   Double_t yt;
   Double_t trig;
   Double_t Sienergy;
   Double_t ATenergy;
   Double_t AMenergy;
   Double_t ABenergy;
   Double_t PGACenergy;
   Double_t trf;
   Double_t trf_next;
   Double_t sbl_ene;
   Double_t sbr_ene;

   Int_t multi_at;
   Int_t multi_am;
   Int_t multi_ab;
   Int_t multi_xr;
   Int_t multi_xl;
   Int_t multi_yt;
   Int_t multi_yb;
   Int_t multi_trig;

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
