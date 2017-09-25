///
/// \file emma_module.cxx
/// \authors M. Williams, D. Connolly
/// \brief implements emma_module.hxx
///

#include "emma_module.h"


EmmaModule::EmmaModule(TARunInfo* runinfo, EmmaConfig* config):
   TARunObject(runinfo)
{
   printf("EmmaModule::ctor!\n");

   fConfig = config;

   // initialize canvases

   fCanvasTdcRaw = new TCanvas("TDC raw data");
   fCanvasTdcUsed = new TCanvas("TDC raw data (in-use channels)");
   //fCanvasAdcRaw = new TCanvas("ADC raw data");
   fCanvasXYDiffs = new TCanvas("XY Differences");
   fCanvasXYDiffsGated = new TCanvas("XY Differences Gated");
   fCanvasXYSums = new TCanvas("XY Sums");
   //fCanvasDiffVsSum = new TCanvas("Cathode Differences VS Sums");
   fCanvasXYPositions = new TCanvas("XY Positions");
   fCanvasXYPositions_Gated = new TCanvas("Silicon Gated XY Positions");
   fCanvasXPosition = new TCanvas("X Position");
   fCanvasXPosition_Gated = new TCanvas("X Position Gated");
   fCanvasYPosition = new TCanvas("Y Position");
   fCanvasYPosition_Gated = new TCanvas("Y Position Gated");
   fCanvasEnergy = new TCanvas("Energy Spectra");
   fCanvasdE_E = new TCanvas("#Delta E-E");
   fCanvasAnodeMulti = new TCanvas("Anode Multiplicity");
   fCanvasCathodeMulti = new TCanvas("Cathode Multiplicity");
   fCanvasRF = new TCanvas("RF");
   fCanvasSSB = new TCanvas("Surface Barrier Detectors");

   // initialize histograms

   fHTdcNhits = new TH1D("TDC_nhits", "TDC_nhits", 100, 0, 100);
   fHAdcNhits = new TH1D("ADC_nhits", "ADC_nhits", 16, 0, 16);

   fHAdcTime0 = new TH1D("ADC_time0", "ADC time between events, usec", 100, 0, 200000);
   fHTdcTime0 = new TH1D("TDC_time0", "TDC time between events, usec", 100, 0, 200000);
   fHAdcTime1 = new TH1D("ADC_time1", "ADC time between events, usec", 1000, 0, 15000);
   fHTdcTime1 = new TH1D("TDC_time1", "TDC time between events, usec", 1000, 0, 15000);
   fHAdcTime2 = new TH1D("ADC_time2", "ADC time between events, usec", 1000, 0, 1000);
   fHTdcTime2 = new TH1D("TDC_time2", "TDC time between events, usec", 1000, 0, 1000);
   fHAdcTdcTime = new TH1D("ADC_TDC_time", "ADC-TDC time mismatch, usec", 101, -50, 50);

   if (runinfo->fRunNo < 202) {
      fHTdcTrig = new TH1D("TDC_trig", "TDC trigger signal", 100, 20000, 21000);
   } else {
      fHTdcTrig = new TH1D("TDC_trig", "TDC trigger signal", 100, 19000, 20000);
   }

   for (int i=0; i<64; i++) {
      char title[256];
      sprintf(title, "TDC_%d", i);
      fHTdcRaw[i] = new TH1D(title, title, 4000, 0, 40000);
   }

   for (int i=0; i<32; i++) {
      char title[256];
      sprintf(title, "ADC_%d", i);
      fHAdcRaw[i] = new TH1D(title, title, 4096, 0, 4096);
   }
   /*
     for (int i=0; i<9; i++) {
     char title[256];
     sprintf(title, "TDC_%d", i);
     hTdcUsed[i] = new TH1D(title, title, 40000, 0, 40000);
     }
   */
   // **************************************
   // INITIALIZATION
   // **************************************
   ct = 0;
   // initialize histograms
   char name[100];
   Float_t diffmin=-100;
   Float_t diffmax=100;
   Float_t diffbin=200;

   Float_t summin=0;
   Float_t summax=8000;
   Float_t sumbin=800;

   {
      // initialize x_y_diff histogram
      const char* title[] = {"XL - XR", "YB - YT"};
      for(int i = 0; i < 2; i++){
         sprintf(name,"x_y_diff_%i",i);
         if(i == 0)
            x_y_diff[i] = new TH1D(name,title[i],diffbin,diffmin,diffmax);
         if(i == 1)
            x_y_diff[i] = new TH1D(name,title[i],diffbin,diffmin,diffmax);

         x_y_diff[i]->SetXTitle("Calibrated Time Difference (mm)");
      }
   }

   {
      // initialize x_y_diff histogram
      const char* title[] = {"XL - XR (Silicon Gated)", "YB - YT (Silicon Gated)"};
      for(int i = 0; i < 2; i++){
         sprintf(name,"x_y_diff_Gated_%i",i);
         if(i == 0)
            x_y_diff_Gated[i] = new TH1D(name,title[i],diffbin,diffmin,diffmax);
         if(i == 1)
            x_y_diff_Gated[i] = new TH1D(name,title[i],diffbin,diffmin,diffmax);

         x_y_diff_Gated[i]->SetXTitle("Calibrated Time Difference (mm)");
      }
   }

   {
      // initialize x_y_sum histogram
      const char* title[] = {"XR + XL", "YT + YB"};
      for(int i = 0; i < 2; i++){
         sprintf(name,"x_y_sum_%i",i);
         if(i == 0)
            x_y_sum[i] = new TH1D(name,title[i],sumbin,summin,summax);
         if(i == 1)
            x_y_sum[i] = new TH1D(name,title[i],sumbin,summin,summax);

         x_y_sum[i]->SetXTitle("Time Sums (chan)");
      }
   }
   /*
     {
     // initialize x_y_diff_vs_sum histogram
     const char*title[] = {"X1R/X1L Diff vs Sum", "Y1B/Y1T Diff vs Sum", "X2R/X2L Diff vs Sum", "Y2B/Y2T Diff vs Sum"};
     for(int i = 0; i < 2; i++){
     sprintf(name,"x_y_diff_vs_sum_%i",i);
     if(i == 0)
     x_y_diff_vs_sum[i] = new TH2D(name,title[i],sumbin,summin,summax,diffbin,diffmin,diffmax);
     if(i == 1)
     x_y_diff_vs_sum[i] = new TH2D(name,title[i],sumbin,summin,summax,diffbin,diffmin,diffmax);

     x_y_diff_vs_sum[i]->SetXTitle("Cathode Time Sum (chan)");
     x_y_diff_vs_sum[i]->SetYTitle("Cathode Time Differecnce (chan)");
     }
     }
   */
   {
      // initialize x position histogram
      hXPosition = new TH1D("hXPosition","XPosition",166,-83,83);
   }

   {
      // initialize y position histogram
      hYPosition = new TH1D("hYPosition","YPosition",66,-33,33);
   }

   {
      // initialize xy position histogram
      hXYPosition = new TH2D("hXYPosition","XYPosition",166,-83,83,66,-33,33);
   }

   {
      // initialize x position histogram
      hXPosition_Gated = new TH1D("hXPosition_Gated","XPosition Silicon Gated",166,-83,83);
   }

   {
      // initialize y position histogram
      hYPosition_Gated = new TH1D("hYPosition_Gated","YPosition Silicon Gated",66,-33,33);
   }


   {
      // initialize xy position histogram
      hXYPosition_Gated = new TH2D("hXYPosition_Gated","XYPosition Silicon Gated",166,-83,83,66,-33,33);
   }

   {
      hSienergy = new TH1D("hSienergy","Si_Energy",4096,-1,4096);
   }

   {
      const char*title[] = {"Anode Top Energy", "Anode Middle Energy", "Anode Bottom Energy", "Silicon Energy","SB Left","SB Right"};
      for(int i = 0; i < 6; i++){
         sprintf(name,"ADC_Used_%i",i);

         hADC_used[i] = new TH1D(name,title[i],511,1,2048);
         hADC_used[i]->SetXTitle("Energy");
         hADC_used[i]->SetYTitle("Counts");
      }
   }

   {
      hdE_E = new TH2D("hdE_E","#Delta E-E",512,-1,2047,512,-1,2047);
   }

   {
      hmulti_at = new TH1D("hmulti_at","Anode Top Multiplicity",20,0,20);
   }

   {
      hmulti_am = new TH1D("hmulti_am","Anode Middle Multiplicity",20,0,20);
   }

   {
      hmulti_ab = new TH1D("hmulti_ab","Anode Bottom Multiplicity",20,0,20);
   }

   {
      hmulti_xr = new TH1D("hmulti_xr","Cathode Right Multiplicity",20,0,20);
   }

   {
      hmulti_xl = new TH1D("hmulti_xl","Cathode Left Multiplicity",20,0,20);
   }

   {
      hmulti_yt = new TH1D("hmulti_yt","Cathode Top Multiplicity",20,0,20);
   }

   {
      hmulti_yb = new TH1D("hmulti_yb","Cathode Bottom Multiplicity",20,0,20);
   }

   {
      hmulti_trig = new TH1D("hmulti_trig","Trigger Multiplicity",20,0,20);
   }

   {
      hRF = new TH1D("hRF","RF",1000,15000,25000);
   }

   {
      hsbr = new TH1D("hsbr","SB Right",2000,0,2000);
   }

   {
      hsbl = new TH1D("hsbr","SB Left",2000,0,2000);
   }

} //end EmmaModule

EmmaModule::~EmmaModule()
{
   printf("EmmaModule::dtor!\n");

   DELETE(fCanvasTdcRaw);
   DELETE(fCanvasTdcUsed);
   //      DELETE(fCanvasAdcRaw);
   DELETE(fCanvasXYDiffs);
   DELETE(fCanvasXYDiffsGated);
   DELETE(fCanvasXYSums);
   //      DELETE(fCanvasDiffVsSum);
   DELETE(fCanvasXYPositions);
   DELETE(fCanvasXYPositions_Gated);
   DELETE(fCanvasXPosition);
   DELETE(fCanvasXPosition_Gated);
   DELETE(fCanvasYPosition);
   DELETE(fCanvasYPosition_Gated);
   DELETE(fCanvasEnergy);
   DELETE(fCanvasdE_E);
   DELETE(fCanvasAnodeMulti);
   DELETE(fCanvasCathodeMulti);
   DELETE(fCanvasRF);
   DELETE(fCanvasSSB);

} //end ~EmmaModule

void EmmaModule::ResetHistograms()
{
   for (int i=0; i<64; i++) {
      fHTdcRaw[i]->Reset();
   }

   for (int i=0; i<32; i++) {
      fHAdcRaw[i]->Reset();
   }

   //  for (int i=0; i<9; i++) {
   //     hTdcUsed[i]->Reset();
   // }

   for(int i =0; i < 2; i++) {
      x_y_diff[i]->Reset();
      x_y_diff_Gated[i]->Reset();
      x_y_sum[i]->Reset();
      x_y_diff_vs_sum[i]->Reset();
   }

   for(int i =0; i < 6; i++) {
      hADC_used[i]->Reset();
   }

   hSienergy->Reset();
   hdE_E->Reset();
   hXPosition->Reset();
   hXPosition_Gated->Reset();
   hYPosition->Reset();
   hYPosition_Gated->Reset();
   hXYPosition->Reset();
   hXYPosition_Gated->Reset();
   hRF->Reset();
   hsbl->Reset();
   hsbr->Reset();

   hmulti_at->Reset();
   hmulti_am->Reset();
   hmulti_ab->Reset();
   hmulti_xr->Reset();
   hmulti_xl->Reset();
   hmulti_yt->Reset();
   hmulti_yb->Reset();
   hmulti_trig->Reset();

} //end ResetHistograms

void EmmaModule::UpdateHistograms(TARunInfo* runinfo, const v1190event* tdc_data, const mesadc32event* adc_data)
{
   double adc_dt = 0;
   double tdc_dt = 0;

   if (1) {
      double ts = tdc_data->ettt/1.25;
      static double prevts = 0;
      if (prevts == 0) {
         prevts = ts;
      } else {
         double dt = ts - prevts;
         //printf("TDC ts %8d, dt %5d\n", (int)ts, (int)dt);
         //fHAdcTime->Fill(dt);
         prevts = ts;
         tdc_dt = dt;
      }
   }

   if (1) {
      static int prevts = 0;
      int ts = adc_data->time_stamp;
      if (prevts == 0) {
         prevts = ts;
      } else {
         int dt = ts - prevts;
         //printf("ts %8d, dt %5d\n", ts, dt);
         prevts = ts;
         adc_dt = dt;
      }
   }

   printf("tscheck: ADC %.0f, TDC %.0f\n", adc_dt, tdc_dt);

   fHAdcTime0->Fill(adc_dt);
   fHTdcTime0->Fill(tdc_dt);
   fHAdcTime1->Fill(adc_dt);
   fHTdcTime1->Fill(tdc_dt);
   fHAdcTime2->Fill(adc_dt);
   fHTdcTime2->Fill(tdc_dt);
   fHAdcTdcTime->Fill(adc_dt - tdc_dt);

   std::vector<int> anode_pathway(9,0);
   std::vector<double> earliest_times(64,999999);
   std::vector<int> counts(64,0);
   Double_t datum[64][20] = {{0}};


   //double tdc_bin = 0.01; // 100ps V1190
   int tdc_trig_chan = 7;
   Double_t xloffset = 40.0; // 4 ns cable delay for XL
   Double_t xroffset = 20.0; // 2 ns cable delay for XR
   Double_t yboffset = 20.0; // 2 ns cable delay for YB
   Double_t ytoffset = 10.0; // 1 ns cable delay for YT
   Int_t hit = 0;
   Int_t tdchit = 0; // for TDC

   if (runinfo->fRunNo >= 202) {
      //tdc_bin = 0.025; // 25ps V1290
      tdc_trig_chan = 7*4; // 28
   }

   int tdc_trig = 0;
   for (unsigned int i=0; i<tdc_data->hits.size(); i++) {
      if (tdc_data->hits[i].trailing) // skip trailing edge hits
         continue;
      int chan = tdc_data->hits[i].channel;
      if (chan != tdc_trig_chan) // skip if not TDC trigger channel
         continue;
      tdc_trig = tdc_data->hits[i].measurement;
      break;
   }

   printf("tdc_trig %d\n", tdc_trig);

   fHTdcTrig->Fill(tdc_trig);

   int chan = -1;
   // Seems to be some noise in the measurements.  In the case of multiple
   // measurements for the same channel, get the earliest measurement.
   //    double a1m_earliest = 9999999.0, a2m_earliest = 9999999.0;
   // Vector of the earliest TDC time for each channel
   //    std::vector<double> a1_pulse_time(10,-1.0);
   //    int a1_counter = 0;
   for(unsigned int i = 0; i < tdc_data->hits.size(); i++){ // loop over measurements
      if (tdc_data->hits[i].trailing) // skip trailing edge hits
         continue;
      chan = tdc_data->hits[i].channel;
      double t = (tdc_data->hits[i].measurement);//-tdc_trig); //* tdc_bin; // convert to mm
      if (fHTdcRaw[chan]) {
         printf("chan %d, time %f\n", chan, t);
         fHTdcRaw[chan]->Fill(t);
      }
      counts[chan] = counts[chan] + 1;




      if (chan==32 && tdchit==2){
         trf = t;
      }

      if (chan==32 && tdchit==3){
         trf_next = t;
      }
      printf("chan %f\n", chan);
      printf("hit %d\n", hit);
      printf("tdchit %d\n", tdchit);


      datum[chan][hit] = t;

      hit++;
      if (chan==32) {
         tdchit++;
      }

      if(t < earliest_times[chan])
         earliest_times[chan] = t;
   }
   //datum[chan][hit] = t;

   // printf("Hits %d\n", hit);

   Double_t xsum, xdiff, xpos, ysum, ydiff, ypos;

   // Get earliest time for anode (if more than one)
   anode = 999999.0;
   for (int j=0; j<3; j++) {
      if (earliest_times[j*4] < anode)
         anode = earliest_times[j*4];
   }

   at = earliest_times[0];
   am = earliest_times[4];
   ab = earliest_times[8];
   xr = earliest_times[12];
   xl = earliest_times[16];
   yt = earliest_times[20];
   yb = earliest_times[24];
   trig = earliest_times[28];

   //for (int i=0; i<20; i++) {
   //	trf[i] = datum[32][i];
   //printf("trf %f\n", trf[i]);
   //}

   //		trf = datum[32][4];
   if (am<999999) {
      printf("trf %f\n", trf);
      printf("anode %f\n", anode);
   }
   multi_at = counts[0];
   multi_am = counts[4];
   multi_ab = counts[8];
   multi_xr = counts[12];
   multi_xl = counts[16];
   multi_yt = counts[20];
   multi_yb = counts[24];
   multi_trig = counts[28];

   printf("Multi %d\n", multi_xr);

   hmulti_at->Fill(multi_at);
   hmulti_am->Fill(multi_am);
   hmulti_ab->Fill(multi_ab);
   hmulti_xr->Fill(multi_xr);
   hmulti_xl->Fill(multi_xl);
   hmulti_yt->Fill(multi_yt);
   hmulti_yb->Fill(multi_yb);
   hmulti_trig->Fill(multi_trig);

   xsum = xl + xr - 2*anode;
   xdiff = (xl + xloffset) - (xr + xroffset);
   xpos = 80*(xdiff/xsum);

   if( xl<999999 && xr<999999 ){
      x_y_diff[0]->Fill(xdiff*0.0222);
      x_y_sum[0]->Fill(xsum);
      hXPosition->Fill(xpos);
   }

   ysum = yb + yt - 2*anode;
   ydiff = (yb + yboffset) - (yt + ytoffset);
   ypos = 30*(ydiff/ysum);

   if( yt<999999 && yb<999999 ){
      x_y_diff[1]->Fill(ydiff*0.0226);
      x_y_sum[1]->Fill(ysum);
      hYPosition->Fill(ypos);
   }

   if ( xr<999999 && xl<999999 && yb<999999 && yt<999999 ){
      hXYPosition->Fill(xpos,ypos);
   }


   //*******ADC DATA COUNTING***************
   //Make a vector of vectors to have each channel be a dynanmically expanding collection of energies
   //std::vector< std::vector<double> > energy_signals(n_ach, std::vector<double>);
   std::vector<double> energy_signals(32, 0);

   //for each event in the ADC event structure
   for (unsigned int i=0; i < adc_data->hits.size(); i++){

      int chan = adc_data->hits[i].channel;

      if (fHAdcRaw[chan]){
         if (adc_data->hits[i].v) //If we're overflowing our ADC
            fHAdcRaw[chan]->Fill(4096);
         else
            fHAdcRaw[chan]->Fill(adc_data->hits[i].adc_data);
      }

      for (int j=0; j < 6; j++){
         if (ach[j] == chan ) {
            double energy = 1.0*adc_data->hits[i].adc_data;

            hADC_used[j]->Fill(energy);

            energy_signals[chan] =energy;

         }
      }//end chan == ADC_used check

   }//end foreach ADC event

   Sienergy = energy_signals[16];
   ATenergy = energy_signals[0];
   AMenergy = energy_signals[1];
   ABenergy = energy_signals[2];

   sbl_ene = energy_signals[18];
   sbr_ene = energy_signals[20];

   hSienergy->Fill(Sienergy);

   if( xl<999999 && xr<999999 && Sienergy>800 && Sienergy<1100){
      x_y_diff_Gated[0]->Fill(xdiff*0.02);
      hXPosition_Gated->Fill(xpos);
   }
   if( yt<999999 && yb<999999 && Sienergy>800 && Sienergy<1100){
      x_y_diff_Gated[1]->Fill(ydiff*0.02);
      hYPosition_Gated->Fill(ypos);
   }
   if ( xr<999999 && xl<999999 && yb<999999 && yt<999999 && Sienergy>800 && Sienergy<1100 ) {
      hXYPosition_Gated->Fill(xpos,ypos);
   }

   PGACenergy = ATenergy + ABenergy + AMenergy;

   hdE_E->Fill(Sienergy,PGACenergy);

   hRF->Fill(trf);

   hsbl->Fill(sbl_ene);
   hsbr->Fill(sbr_ene);

   t1->Fill();

   //for (int i=0; i<hit; i++) {
   //              trf[i] = 0;
   //	}

} //end UpdateHistograms

void EmmaModule::PlotHistograms(TARunInfo* runinfo)
{
   printf("PlotHistograms!\n");

   {
      TCanvas* c1 = fCanvasTdcRaw;
      c1->Clear();
      c1->Divide(2,2);
      for(int i = 0; i < 4; i++){
         c1->cd(1+i);
         fHTdcRaw[i*4]->Draw();
      }
      c1->Modified();
      c1->Update();
   }

   {
      TCanvas* c1 = fCanvasTdcUsed;
      c1->Clear();
      c1->Divide(3,3);
      for(int i = 0; i < 9; i++){
         c1->cd(1+i);
         fHTdcRaw[i*4]->Draw();
      }
      c1->Modified();
      c1->Update();
   }


   /*     if (fCanvasAdcRaw) {
          fCanvasAdcRaw->Clear();
          fCanvasAdcRaw->Divide(2,2);

          int p=0;
          for (int i=0; i<3; i++) {
          fCanvasAdcRaw->cd(++p);
          fHAdcRaw[i]->Draw();
          }
          fCanvasAdcRaw->cd(4);
          fHAdcRaw[16]->Draw();

          fCanvasAdcRaw->Modified();
          fCanvasAdcRaw->Update();
          printf("update!\n");
          }
   */
   // plot xy diff
   {
      TCanvas* c1 = fCanvasXYDiffs;
      c1->Clear();
      c1->Divide(1,2);
      for(int i = 0; i < 2; i++){
         c1->cd(1+i);
         x_y_diff[i]->Draw();
      }
      c1->Modified();
      c1->Update();
   }

   {
      TCanvas* c1 = fCanvasXYDiffsGated;
      c1->Clear();
      c1->Divide(1,2);
      for(int i = 0; i < 2; i++){
         c1->cd(1+i);
         x_y_diff_Gated[i]->Draw();
      }
      c1->Modified();
      c1->Update();
   }
   // plot xy sums
   {
      TCanvas* c1 = fCanvasXYSums;
      c1->Clear();
      c1->Divide(1,2);
      for(int i = 0; i < 2; i++){
         c1->cd(1+i);
         x_y_sum[i]->Draw();
      }
      c1->Modified();
      c1->Update();
   }
   // Plot X position
   {
      TCanvas* c1 = fCanvasXPosition;
      c1->Clear();
      hXPosition->Draw();
      c1->Modified();
      c1->Update();
   }
   // Plot Y position
   {
      TCanvas* c1 = fCanvasYPosition;
      c1->Clear();
      hYPosition->Draw();
      c1->Modified();
      c1->Update();
   }
   // Plot XY position
   {
      TCanvas* c1 = fCanvasXYPositions;
      c1->Clear();
      hXYPosition->Draw("colz");
      c1->Modified();
      c1->Update();
   }
   // Plot X position Gated
   {
      TCanvas* c1 = fCanvasXPosition_Gated;
      c1->Clear();
      hXPosition_Gated->Draw();
      c1->Modified();
      c1->Update();
   }
   // Plot Y position Gated
   {
      TCanvas* c1 = fCanvasYPosition_Gated;
      c1->Clear();
      hYPosition_Gated->Draw();
      c1->Modified();
      c1->Update();
   }
   // Plot XY position Gated
   {
      TCanvas* c1 = fCanvasXYPositions_Gated;
      c1->Clear();
      hXYPosition_Gated->Draw("colz");
      c1->Modified();
      c1->Update();
   }

   /*     {
          TCanvas* c1 = fCanvasDiffVsSum;
          c1->Clear();
          if(!(c1->GetShowEventStatus()))c1->ToggleEventStatus();
          if(!(c1->GetShowToolBar()))c1->ToggleToolBar();
          c1->Divide(1,2);
          for(int i = 0; i < 2; i++){
          c1->cd(1+i);
          x_y_diff_vs_sum[i]->Draw("colz");
          }
          c1->Modified();
          c1->Update();
          }
   */
   {
      TCanvas* c1 = fCanvasEnergy;
      c1->Clear();
      c1->Divide(2,3);
      for(int i = 0; i < 6; i++){
         c1->cd(1+i);
         hADC_used[i]->Draw();
      }
      c1->Modified();
      c1->Update();
   }

   {
      TCanvas* c1 = fCanvasdE_E;
      c1->Clear();
      hdE_E->Draw("colz");
      c1->Modified();
      c1->Update();
   }

   {
      TCanvas* c1 = fCanvasAnodeMulti;
      c1->Clear();
      c1->Divide(2,2);

      c1->cd(1);
      hmulti_at->Draw();
      c1->cd(2);
      hmulti_am->Draw();
      c1->cd(3);
      hmulti_ab->Draw();
      c1->cd(4);
      hmulti_trig->Draw();

      c1->Modified();
      c1->Update();
   }


   {
      TCanvas* c1 = fCanvasCathodeMulti;
      c1->Clear();
      c1->Divide(2,2);

      c1->cd(1);
      hmulti_xr->Draw();
      c1->cd(2);
      hmulti_xl->Draw();
      c1->cd(3);
      hmulti_yt->Draw();
      c1->cd(4);
      hmulti_yb->Draw();

      c1->Modified();
      c1->Update();
   }

   {
      TCanvas* c1 = fCanvasRF;
      c1->Clear();
      hRF->Draw();
      c1->Modified();
      c1->Update();
   }

   {
      TCanvas* c1 = fCanvasSSB;
      c1->Clear();
      c1->Divide(1,2);
      c1->cd(1);
      hsbr->Draw();
      c1->cd(2);
      hsbl->Draw();
      c1->Modified();
      c1->Update();
   }

}


void EmmaModule::BeginRun(TARunInfo* runinfo)
{
   printf("BeginRun, run %d, file %s\n", runinfo->fRunNo, runinfo->fFileName.c_str());
   time_t run_start_time = runinfo->fOdb->odbReadUint32("/Runinfo/Start time binary", 0, 0);
   printf("ODB Run start time: %d: %s", (int)run_start_time, ctime(&run_start_time));
   fCounter = 0;
   runinfo->fRoot->fOutputFile->cd(); // select correct ROOT directory
   //fATX->BeginRun(runinfo->fRunNo);

   t1 = new TTree("t1","TDC Tree");
   t1->Branch("AnodeTop",&at,"at/D");
   t1->Branch("AnodeMiddle",&am,"am/D");
   t1->Branch("AnodeBottow",&ab,"ab/D");
   t1->Branch("Anode",&anode,"anode/D");
   t1->Branch("CathodeXleft",&xl,"xl/D");
   t1->Branch("CathodeXright",&xr,"xr/D");
   t1->Branch("CathodeYbottom",&yb,"yb/D");
   t1->Branch("CathodeYtop",&yt,"yt/D");
   t1->Branch("TDCtrig",&trig,"trig/D");
   t1->Branch("ATenergy",&ATenergy,"ATenergy/D");
   t1->Branch("AMenergy",&AMenergy,"AMenergy/D");
   t1->Branch("ABenergy",&ABenergy,"ABenergy/D");
   t1->Branch("PGACenergy",&PGACenergy,"PGACenergy/D");
   t1->Branch("Sienergy",&Sienergy,"Sienergy/D");
   t1->Branch("trf",&trf,"trf/D");
   t1->Branch("trf_next",&trf_next,"trf_next/D");
   t1->Branch("sbr_ene",&sbr_ene,"sbr_ene/D");
   t1->Branch("sbl_ene",&sbl_ene,"sbl_ene/D");

} //end BeginRun

void EmmaModule::EndRun(TARunInfo* runinfo)
{
   printf("EndRun, run %d, events %d\n", runinfo->fRunNo, fCounter);
   time_t run_stop_time = runinfo->fOdb->odbReadUint32("/Runinfo/Stop time binary", 0, 0);
   printf("ODB Run stop time: %d: %s", (int)run_stop_time, ctime(&run_stop_time));
   //fATX->EndRun();
   //char fname[1024];
   //sprintf(fname, "output%05d.pdf", runinfo->fRunNo);
   //fATX->fH->fCanvas->SaveAs(fname);

} //end EndRun


TAFlowEvent* EmmaModule::Analyze(TARunInfo* runinfo, TMEvent* event, TAFlags* flags, TAFlowEvent* flow)
{
   //printf("Analyze, run %d, event serno %d, id 0x%04x, data size %d\n", runinfo->fRunNo, event->serial_number, (int)event->event_id, event->data_size);

   if (event->event_id != 1)
      return flow;

   v1190event *xte = NULL;
   mesadc32event *xae = NULL;

   {
      TMBank* b = event->FindBank("EMMT");

      if (b) {
         int bklen = b->data_size;
         const char* bkptr = event->GetBankData(b);

         if (bkptr) {
            printf("EMMA TDC, pointer: %p, len %d\n", bkptr, bklen);

            while (bklen > 0) {
               v1190event *te = UnpackV1190(&bkptr, &bklen, fConfig->fVerboseV1190);
               if (te == NULL)
                  break;
               te->Print();

               int tdc_offset = 0;

               if (runinfo->fRunNo == 73)
                  tdc_offset = 0;

               static int old_ettt = 0;
               int xettt = (te->ettt)<<5;
               int xts = xettt*25 + tdc_offset;

               printf("EMMA TDC timestamp %d\n", xettt);

               printf("EMMA TDC sn %d, delta %5d, ts %d\n", event->serial_number, ((xettt - old_ettt)*25)/800, xts/800);
               old_ettt = xettt;

               if (0) {
                  double ts = te->ettt/1.25;
                  static double prevts = 0;
                  if (prevts == 0) {
                     prevts = ts;
                  } else {
                     double dt = ts - prevts;
                     printf("TDC ts %8d, dt %5d\n", (int)ts, (int)dt);
                     //fHAdcTime->Fill(dt);
                     prevts = ts;
                  }
               }

               fHTdcNhits->Fill(te->hits.size());

               if (!xte) {
                  xte = te;
               } else {
                  printf("ERROR: DUPLICATE TDC EVENT!\n");
                  delete te;
               }

            }
         }
      }
   }

   /*    {
         TMBank* b = event->FindBank("AAAA");

         if (b) {
         int bklen = b->data_size;
         const char* bkptr = event->GetBankData(b);

         if (bkptr) {
         //printf("EMMA ADC, pointer: %p, len %d\n", bkptr, bklen);

         Alpha16Packet *ap = new Alpha16Packet();
         ap->Unpack(bkptr, bklen);
         //ap->Print();

         Alpha16Waveform *aw = new Alpha16Waveform();
         aw->Unpack(bkptr, bklen);

         if (ap->channelId == 0) {
         printf("EMMA ADC, timestamp %d, channel 0\n", ap->eventTimestamp);

         double adc_offset = 0;

         if (runinfo->fRunNo == 73)
         adc_offset = -166696*800;

         if (runinfo->fRunNo == 84)
         adc_offset = -166696*800;

         if (runinfo->fRunNo == 112) { // TDC sn 364629, ADC sn 1459242/364810
         //adc_offset = (-2067919-1056035)*800;
         // TDC 1142, ADC 6614/1653 -> diff

         adc_offset = (1973667-34439772+259169+1973667)*800.0; // TDC 581940, ADC 2327722/581930
         }

         const double adc_clk = 9.9987; //10.0;
         static int old_ts = 0;
         double xts = ap->eventTimestamp*adc_clk + adc_offset;

         printf("EMMA ADC sn %d %d, delta %5d, ts %d\n", event->serial_number, event->serial_number/4, (int)(((ap->eventTimestamp - old_ts)*adc_clk)/800.0), (int)(xts/800.0));
         old_ts = ap->eventTimestamp;

         if (0 && runinfo->fRoot->fgApp) {
         TCanvas *c = new TCanvas();
         c->cd();
         TH1D* hw = new TH1D("adc0", "adc0", ap->nsamples, 0, ap->nsamples);
         for (int i=0; i<ap->nsamples; i++)
         hw->SetBinContent(i+1, (*aw)[i]);
         hw->Draw();
         c->Modified();
         c->Draw();
         }
         }
   */
   /*               if (ap->channelId == 1) {
                    printf("EMMA ADC, timestamp %d, channel 1\n", ap->eventTimestamp);

                    static TH1D* hw = NULL;
                    if (!hw)
                    hw = new TH1D("adc1", "adc1", ap->nsamples, 0, ap->nsamples);
                    for (int i=0; i<ap->nsamples; i++)
                    hw->SetBinContent(i+1, (*aw)[i]);
                    double SiEnergy = hw->GetMaximum();
                    if (SiEnergy < 0 ) SiEnergy = -1;
                    UpdateSiEnergyRawHist(SiEnergy);

                    if (runinfo->fRoot->fgApp) {
                    static TCanvas *c = NULL;
                    if (!c)
                    c = new TCanvas();
                    c->Clear();
                    c->cd();
                    hw->SetMinimum(-12000);
                    hw->SetMaximum(12000);
                    hw->Draw();
                    c->Modified();
                    c->Draw();
                    c->Update();

                    if (runinfo->fRoot->fgApp)
                    runinfo->fRoot->fgApp->Run(kTRUE);
                    }
                    }

                    delete ap;
                    delete aw;
                    }
                    }
                    }
   */
   {
      TMBank* b = event->FindBank("MADC");

      if (b) {
         int bklen = b->data_size;
         const char* bkptr = event->GetBankData(b);

         if (bkptr) {
            printf("EMMA MADC, pointer: %p, len %d\n", bkptr, bklen);

            while (bklen > 0) {
               mesadc32event *ae = UnpackMesadc32(&bkptr, &bklen, fConfig->fVerboseMesadc32);
               if (ae == NULL)
                  break;
               ae->Print();

               fHAdcNhits->Fill(ae->hits.size());

               if (0) {
                  static int prevts = 0;
                  if (prevts == 0) {
                     prevts = ae->time_stamp;
                  } else {
                     int ts = ae->time_stamp;
                     int dt = ts - prevts;
                     printf("ts %8d, dt %5d\n", ts, dt);
                     //fHAdcTime->Fill(dt);
                     prevts = ts;
                  }
               }

               if (!xae) {
                  xae = ae;
               } else {
                  printf("ERROR: DUPLICATE ADC EVENT!\n");
                  delete ae;
               }
            }
         }
      }
   }

   if (xte && xae) {
      UpdateHistograms(runinfo, xte, xae);
   } else {
      printf("ERROR: ADC and TDC event mismatch: %p %p\n", xte, xae);
   }

   if (xte)
      delete xte;
   if (xae)
      delete xae;

   static time_t t = 0;

   time_t now = time(NULL);

   if (now - t > 15) {
      t = now;
      PlotHistograms(runinfo);
   }

   fCounter++;

   return flow;

} // end Analyze

void EmmaModule::AnalyzeSpecialEvent(TARunInfo* runinfo, TMEvent* event)
{
   printf("AnalyzeSpecialEvent, run %d, event serno %d, id 0x%04x, data size %d\n", runinfo->fRunNo, event->serial_number, (int)event->event_id, event->data_size);
}

// ==================== EmmaModuleFacotry Methods ==================== //

void EmmaModuleFactory::Init(const std::vector<std::string> &args)
{
   printf("Init!\n");

   for (unsigned i=0; i<args.size(); i++) {
      if (args[i] == "--verbose-v1190")
         fConfig->fVerboseV1190 = true;
   }

   TARootHelper::fgDir->cd(); // select correct ROOT directory
}

TARunObject* EmmaModuleFactory::NewRunObject(TARunInfo* runinfo)
{
   printf("NewRun, run %d, file %s\n", runinfo->fRunNo, runinfo->fFileName.c_str());
   return new EmmaModule(runinfo, fConfig);
}


// emacs
// Local Variables:
// tab-width: 8
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
