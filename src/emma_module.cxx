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
   fCanvasSums = new TCanvas("XY Sums");
   fCanvas1DPosition = new TCanvas("1D Position");
   fCanvas2DPosition = new TCanvas("2D Position");
   fCanvas1DPositionSiliconGated = new TCanvas("1D Position Silicon Gated");
   fCanvas2DPositionSiliconGated = new TCanvas("2D Position Silicon Gated");
   fCanvas1DPositionAnodeGated = new TCanvas("1D Position Anode Gated");
   fCanvas2DPositionAnodeGated = new TCanvas("2D Position Anode Gated");
   fCanvas1DPositionRFTOFGated = new TCanvas("1D Position RFTOF Gated");
   fCanvas2DPositionRFTOFGated = new TCanvas("2D Position RFTOF Gated");
   fCanvas1DPositionPID2Gated = new TCanvas("1D Position Anode-RFTOF Gated (PID2)");
   fCanvas2DPositionPID2Gated = new TCanvas("2D Position Anode-RFTOF Gated (PID2)");
   fCanvasEnergySpectra = new TCanvas("Energy Spectra");
   fCanvasPID1 = new TCanvas("Silicon vs Anode Energy");
   fCanvasPID2 = new TCanvas("RFTOF vs Anode Energy");
   fCanvasMulti = new TCanvas("Multiplicity");

   // initialize histograms

   ct = 0;
   char name[100];

   // Initialize TDC histograms
   {
   
   for (int i=0; i<64; i++) {
      char title[256];
      sprintf(title, "TDC_%d", i);
      fHTdcRaw[i] = new TH1D(title, title, 4000, 0, 40000);
   }

   }
   // Initialize ADC histograms
   {
   for (int i=0; i<32; i++) {
      char title[256];
      sprintf(title, "ADC_%d", i);
      fHAdcRaw[i] = new TH1D(title, title, 4096, 0, 4096);
   }
   }
	
   {
   const char* title[] = { "Anode Top", "Anode Middle", "Anode Bottom","Focal Plane Silicon","Left SSB","Right SSB" };
   for (int i=0; i<6; i++) {
      sprintf(name,"hADCused_%i",i);
      fHAdcUsed[i] = new TH1D(name, title[i], 1920, 1, 1921);
   }
   }
   // initialize x_y_sum histogram
   {     
      const char* title[] = {"XR + XL", "YT + YB"};
      for(int i = 0; i < 2; i++){
         sprintf(name,"hsum_%i",i);
         if(i == 0)
            hSum[i] = new TH1D(name,title[i],800,0,8000);
         if(i == 1)
            hSum[i] = new TH1D(name,title[i],800,0,8000);

         hSum[i]->SetXTitle("Time Sums (chan)");
      }
   }
   // initialize 1D position histogram
   {      
            h1DPosition[0] = new TH1D("h1DPosition_0","X Position",166,-83,83);
	    h1DPosition[0]->SetXTitle("X Position (mm)");
	
            h1DPosition[1] = new TH1D("h1DPosition_1","Y Position",66,-33,33);
	    h1DPosition[1]->SetXTitle("Y Position (mm)");
   }

   {
      // initialize 2D position histogram
      h2DPosition = new TH2D("h2DPosition","2D Position",166,-83,83,66,-33,33);
      h2DPosition->SetXTitle("X Position (mm)");
      h2DPosition->SetYTitle("Y Position (mm)");
   }

   {
      // initialize 1D position histograms (Silicon Gated)
      
            h1DPositionSiliconGated[0] = new TH1D("h1DPositionSiliconGated_0","X Position Silicon Gated",166,-83,83);
	    h1DPositionSiliconGated[0]->SetXTitle("X Position (mm)");
	
            h1DPositionSiliconGated[1] = new TH1D("h1DPositionSiliconGated_1","Y Position Silicon Gated",66,-33,33);
	    h1DPositionSiliconGated[1]->SetXTitle("Y Position (mm)");
   }

   {
      // initialize 2D position histogram (Silicon Gated)
      h2DPositionSiliconGated = new TH2D("h2DPositionSiliconGated","2D Position Silicon Gated",166,-83,83,66,-33,33);
      h2DPositionSiliconGated->SetXTitle("X Position (mm)");
      h2DPositionSiliconGated->SetYTitle("Y Position (mm)");
   }

   {
      // initialize 1D position histograms (Anode Gated)
      
            h1DPositionAnodeGated[0] = new TH1D("h1DPositionAnodeGated_0","X Position Anode Gated",166,-83,83);
	    h1DPositionAnodeGated[0]->SetXTitle("X Position (mm)");

            h1DPositionAnodeGated[1] = new TH1D("h1DPositionAnodeGated_1","Y Position Anode Gated",66,-33,33);
	    h1DPositionAnodeGated[1]->SetXTitle("Y Position (mm)");

   }

   {
      // initialize 2D position histogram (Anode Gated)
      h2DPositionAnodeGated = new TH2D("h2DPositionAnodeGated","2D Position Anode Gated",166,-83,83,66,-33,33);
      h2DPositionAnodeGated->SetXTitle("X Position (mm)");
      h2DPositionAnodeGated->SetYTitle("Y Position (mm)");
   }

   {
      // initialize 1D position histograms (RFTOF Gated)
            h1DPositionRFTOFGated[0] = new TH1D("h1DPositionRFTOFGated_0","X Position RF-TOF Gated",166,-83,83);
	    h1DPositionRFTOFGated[0]->SetXTitle("X Position (mm)");
	
            h1DPositionRFTOFGated[1] = new TH1D("h1DPositionRFTOFGated_1","Y Position RF-TOF Gated",66,-33,33);
	    h1DPositionRFTOFGated[1]->SetXTitle("Y Position (mm)");
   }

   {
      // initialize 2D position histogram (RFTOF Gated)
      h2DPositionRFTOFGated = new TH2D("h2DPositionRFTOFGated","2D Position RF-TOF Gated",166,-83,83,66,-33,33);
      h2DPositionRFTOFGated->SetXTitle("X Position (mm)");
      h2DPositionRFTOFGated->SetYTitle("Y Position (mm)");
   }

   {
      // initialize 1D position histograms (PID2 Gated)
            h1DPositionPID2Gated[0] = new TH1D("h1DPositionPID2Gated_0","X Position PID2 Gated",166,-83,83);
	    h1DPositionPID2Gated[0]->SetXTitle("X Position (mm)");
	
            h1DPositionPID2Gated[1] = new TH1D("h1DPositionPID2Gated_1","Y Position PID2 Gated",66,-33,33);
	    h1DPositionPID2Gated[1]->SetXTitle("Y Position (mm)");
   }

   {
      // initialize 2D position histogram (PID2 Gated)
      h2DPositionPID2Gated = new TH2D("h2DPositionPID2Gated","2D Position PID2 Gated",166,-83,83,66,-33,33);
      h2DPositionPID2Gated->SetXTitle("X Position (mm)");
      h2DPositionPID2Gated->SetYTitle("Y Position (mm)");
   }

   {
      // initialize rftof histogram
      hrft = new TH1D("hrft","Corrected RF-TOF",200,1200,2400);
   }

   {
      // initialize anode total energy histogram
      hAnodeEnergy = new TH1D("hAnodeEnergy","Total Anode Energy",511,1,2048);
   }
   {
      // initialize first PID histogram
      hSiliconAnodeEnergy = new TH2D("hSiliconAnodeEnergy","Silicon vs AnodeEnergy",512,-1,2047,512,-1,2047);
   }

   {
      // initialize second PID histogram
      hAnodeEnergyRFTOF = new TH2D("hAnodeEnergyRFTOF","AnodeEnergy vs RF-TOF",300,1200,2400,512,-1,2047);
   }

   {
      // initialize multiplicity histograms
      const char* title[] = {"Anode Top", "Anode Middle", "Anode Bottom", "RF" , "Cathode Left", "Cathode Right", "Cathode Top", "Cathode Botttom" };
      for(int i = 0; i < 8; i++){
	 sprintf(name,"hMulti_%i",i);
	 hMulti[i] = new TH1D(name,title[i],20,0,20);
      }
   }
} //end EmmaModule Constructor

EmmaModule::~EmmaModule()
{
   printf("EmmaModule::dtor!\n");

   DELETE(fCanvasSums);
   DELETE(fCanvas1DPosition);
   DELETE(fCanvas2DPosition);
   DELETE(fCanvas1DPositionSiliconGated);
   DELETE(fCanvas2DPositionSiliconGated);
   DELETE(fCanvas1DPositionAnodeGated);
   DELETE(fCanvas2DPositionAnodeGated);
   DELETE(fCanvas1DPositionRFTOFGated);
   DELETE(fCanvas2DPositionRFTOFGated);
   DELETE(fCanvas1DPositionPID2Gated);
   DELETE(fCanvas2DPositionPID2Gated);
   DELETE(fCanvasEnergySpectra);
   DELETE(fCanvasPID1);
   DELETE(fCanvasPID2);
   DELETE(fCanvasMulti);

} //end ~EmmaModule Destructor

void EmmaModule::ResetHistograms()
{
   for (int i=0; i<64; i++) {
      fHTdcRaw[i]->Reset();
   }

   for (int i=0; i<32; i++) {
      fHAdcRaw[i]->Reset();
   }

    for (int i=0; i<6; i++) {
      fHAdcUsed[i]->Reset();
   }
   
  for (int i=0; i<2; i++) {
      hSum[i]->Reset();
   }   

  for (int i=0; i<2; i++) {
      h1DPosition[i]->Reset();
   }

   h2DPosition->Reset();

  for (int i=0; i<2; i++) {
      h1DPositionSiliconGated[i]->Reset();
   }

   h2DPositionSiliconGated->Reset();

  for (int i=0; i<2; i++) {
      h1DPositionAnodeGated[i]->Reset();
   }

   h2DPositionAnodeGated->Reset();

  for (int i=0; i<2; i++) {
      h1DPositionRFTOFGated[i]->Reset();
   }

   h2DPositionRFTOFGated->Reset();

  for (int i=0; i<2; i++) {
      h1DPositionPID2Gated[i]->Reset();
   }

   h2DPositionPID2Gated->Reset();

   hAnodeEnergy->Reset();

   hSiliconAnodeEnergy->Reset();

   hAnodeEnergyRFTOF->Reset();

   for (int i=0; i<8; i++) {
      hMulti[i]->Reset();
   }

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

   std::vector<double> earliest_times(64,999999);
   std::vector<double> counts(64,0);

   Int_t tdchit = 0; // for TDC

   //double tdc_bin = 0.01; // 100ps V1190
   int tdc_trig_chan = 7;

   Double_t xloffset = 40.0; // 4 ns cable delay for XL
   Double_t xroffset = 20.0; // 2 ns cable delay for XR
   Double_t yboffset = 20.0; // 2 ns cable delay for YB
   Double_t ytoffset = 10.0; // 1 ns cable delay for YT
   

   if (runinfo->fRunNo >= 202) {
      //tdc_bin = 0.025; // 25ps V1290
      tdc_trig_chan = 28;
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

   //fHTdcTrig->Fill(tdc_trig);

   int chan = -1;
   
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

      if (chan==32) {
         tdchit++;
      }

      if(t < earliest_times[chan])
         earliest_times[chan] = t;
   }

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
/*
   multi_at = counts[0];
   multi_am = counts[4];
   multi_ab = counts[8];
   multi_xr = counts[12];
   multi_xl = counts[16];
   multi_yt = counts[20];
   multi_yb = counts[24];
   multi_rf = counts[32];
*/
  for (int i=0; i<8; i++) {

   hMulti[i]->Fill(counts[i*4]);
  
  }

   xsum = xl + xr - 2*anode;
   xdiff = (xl + xloffset) - (xr + xroffset);
   xpos = 80*(xdiff/xsum);

   if( xl<999999 && xr<999999 ){
      hSum[0]->Fill(xsum);
      h1DPosition[0]->Fill(xpos);
   }

   ysum = yb + yt - 2*anode;
   ydiff = (yb + yboffset) - (yt + ytoffset);
   ypos = 30*(ydiff/ysum);

   if( yt<999999 && yb<999999 ){
      hSum[1]->Fill(ysum);
      h1DPosition[1]->Fill(ypos);
   }

   if ( xr<999999 && xl<999999 && yb<999999 && yt<999999 ){
      h2DPosition->Fill(xpos,ypos);
   }

   rftof = trf - anode;
   rft = rftof - 867.*(rftof > 2250.);

   hrft->Fill(rft);


   //*******ADC DATA COUNTING***************
 
   std::vector<double> energy(32, 0);
   std::vector<int> adchit(32,0);

   //for each event in the ADC event structure
   for (unsigned int i=0; i < adc_data->hits.size(); i++){

      int chan = adc_data->hits[i].channel;

      if (fHAdcRaw[chan]){
         if (adc_data->hits[i].v) //If we're overflowing our ADC
            fHAdcRaw[chan]->Fill(4096);
         else
            fHAdcRaw[chan]->Fill(adc_data->hits[i].adc_data);
      }

      adchit[chan] = adchit[chan] + 1;

      energy[chan] = adc_data->hits[i].adc_data;

   }//end foreach ADC event

   ATenergy = energy[0];
   AMenergy = energy[1];
   ABenergy = energy[2];
   AnodeEnergy = ATenergy + ABenergy + AMenergy;

   Sienergy = energy[16];
   sbl_ene = energy[18];
   sbr_ene = energy[20];

   hAnodeEnergy->Fill(AnodeEnergy);
   hSiliconAnodeEnergy->Fill(Sienergy,AnodeEnergy);
   hAnodeEnergyRFTOF->Fill(rft,AnodeEnergy);

   fHAdcUsed[0]->Fill(ATenergy);
   fHAdcUsed[1]->Fill(AMenergy);
   fHAdcUsed[2]->Fill(ABenergy);
   fHAdcUsed[3]->Fill(Sienergy);
   fHAdcUsed[4]->Fill(sbl_ene);
   fHAdcUsed[5]->Fill(sbr_ene);


   if( xl<999999 && xr<999999 && Sienergy>800 && Sienergy<1100){
      h1DPositionSiliconGated[0]->Fill(xpos);
   }

   if( yt<999999 && yb<999999 && Sienergy>800 && Sienergy<1100){
      h1DPositionSiliconGated[1]->Fill(ypos);
   }
   if ( xr<999999 && xl<999999 && yb<999999 && yt<999999 && Sienergy>800 && Sienergy<1100 ) {
     h2DPositionSiliconGated->Fill(xpos,ypos);
   }

   if( xl<999999 && xr<999999 && AnodeEnergy>1100 && AnodeEnergy<1300){
      h1DPositionAnodeGated[0]->Fill(xpos);
   }

   if( yt<999999 && yb<999999 && AnodeEnergy>1100 && AnodeEnergy<1300){
      h1DPositionAnodeGated[1]->Fill(ypos);
   }
   if ( xr<999999 && xl<999999 && yb<999999 && yt<999999 && AnodeEnergy>1100 && AnodeEnergy<1300 ) {
     h2DPositionAnodeGated->Fill(xpos,ypos);
   }

   if( xl<999999 && xr<999999 && rft>1900 && rft<2200){
      h1DPositionRFTOFGated[0]->Fill(xpos);
   }

   if( yt<999999 && yb<999999 && rft>1900 && rft<2200){
      h1DPositionRFTOFGated[1]->Fill(ypos);
   }
   if ( xr<999999 && xl<999999 && yb<999999 && yt<999999 && rft>1900 && rft<2200 ) {
     h2DPositionRFTOFGated->Fill(xpos,ypos);
   }

   if( xl<999999 && xr<999999 && rft>1900 && rft<2200 && AnodeEnergy>1100 && AnodeEnergy<1300 ){
      h1DPositionPID2Gated[0]->Fill(xpos);
   }

   if( yt<999999 && yb<999999 && rft>1900 && rft<2200 && AnodeEnergy>1100 && AnodeEnergy<1300 ){
      h1DPositionPID2Gated[1]->Fill(ypos);
   }
   if ( xr<999999 && xl<999999 && yb<999999 && yt<999999 && rft>1900 && rft<2200 && AnodeEnergy>1100 && AnodeEnergy<1300  ) {
     h2DPositionPID2Gated->Fill(xpos,ypos);
   }

   t1->Fill();

} //end UpdateHistograms

void EmmaModule::PlotHistograms(TARunInfo* runinfo)
{
   gStyle->SetOptStat("eMRi");
   printf("PlotHistograms!\n");

   // plot xy sums
   {
      TCanvas* c1 = fCanvasSums;
      c1->Clear();
      c1->Divide(1,2);
      for(int i = 0; i < 2; i++){
         c1->cd(1+i);
         hSum[i]->Draw();
      }
      c1->Modified();
      c1->Update();
   }
   // Plot 1D Position
   {
	TCanvas* c1 = fCanvas1DPosition;
      	c1->Clear();
//	c1->Divide(1,2);

	c1->cd(1);
        h1DPosition[0]->Draw();
//	c1->cd(2);
//        h1DPosition[1]->Draw();

        c1->Modified();
        c1->Update();
   }
   // Plot 2D Position
   {
      TCanvas* c1 = fCanvas2DPosition;
      c1->Clear();
      h2DPosition->Draw("colz");
      c1->Modified();
      c1->Update();
   }
   // Plot 1D Position Gated
   {
      TCanvas* c1 = fCanvas1DPositionSiliconGated;
      c1->Clear();
      //c1->Divide(1,2);
      //for(int i = 0; i < 2; i++){
      //c1->cd(1);
      h1DPositionSiliconGated[0]->Draw();
      //}
      c1->Modified();
      c1->Update();
   }
   // Plot 2D Position Gated
   {
      TCanvas* c1 = fCanvas2DPositionSiliconGated;
      c1->Clear();
      h2DPositionSiliconGated->Draw("colz");
      c1->Modified();
      c1->Update();
   }
   // Plot 1D Position Gated
   {
      TCanvas* c1 = fCanvas1DPositionAnodeGated;
      c1->Clear();
      //c1->Divide(1,2);
      //for(int i = 0; i < 2; i++){
      //   c1->cd(1+i);
      h1DPositionAnodeGated[0]->Draw();
      //}
      c1->Modified();
      c1->Update();
   }
   // Plot 2D Position Gated
   {
      TCanvas* c1 = fCanvas2DPositionAnodeGated;
      c1->Clear();
      h2DPositionAnodeGated->Draw("colz");
      c1->Modified();
      c1->Update();
   }
   // Plot 1D Position Gated
   {
      TCanvas* c1 = fCanvas1DPositionRFTOFGated;
      c1->Clear();
      //c1->Divide(1,2);
      //for(int i = 0; i < 2; i++){
      //c1->cd(1+i);
      h1DPositionRFTOFGated[0]->Draw();
      //}
      c1->Modified();
      c1->Update();
   }
   // Plot 2D Position Gated
   {
      TCanvas* c1 = fCanvas2DPositionRFTOFGated;
      c1->Clear();
      h2DPositionRFTOFGated->Draw("colz");
      c1->Modified();
      c1->Update();
   }
   // Plot 1D Position Gated
   {
      TCanvas* c1 = fCanvas1DPositionPID2Gated;
      c1->Clear();
      //c1->Divide(1,2);
      //for(int i = 0; i < 2; i++){
      //   c1->cd(1+i);
      h1DPositionPID2Gated[0]->Draw();
      //}
      c1->Modified();
      c1->Update();
   }
   // Plot 2D Position Gated
   {
      TCanvas* c1 = fCanvas2DPositionPID2Gated;
      c1->Clear();
      h2DPositionPID2Gated->Draw("colz");
      c1->Modified();
      c1->Update();
   }
   // Plot all Energy Spectra
   {
      TCanvas* c1 = fCanvasEnergySpectra;

      c1->Clear();
      c1->Divide(2,3);

      c1->cd(1);
      fHAdcUsed[0]->Draw();
      c1->cd(2);
      fHAdcUsed[1]->Draw();
      c1->cd(3);
      fHAdcUsed[2]->Draw();
      c1->cd(4);
      fHAdcUsed[3]->Draw();
      c1->cd(5);
      fHAdcUsed[4]->Draw();
      c1->cd(6);
      fHAdcUsed[5]->Draw();
      
      c1->Modified();
      c1->Update();
   }
   // Plot Silicon Energy vs Anode Energy (PID1)
   {
      TCanvas* c1 = fCanvasPID1;
      c1->Clear();
      hSiliconAnodeEnergy->Draw("colz");
      c1->Modified();
      c1->Update();
   }
   // Plot Anode Energy vs RF-TOF (PID2)
   {
      TCanvas* c1 = fCanvasPID2;
      c1->Clear();
      hAnodeEnergyRFTOF->Draw("colz");
      c1->Modified();
      c1->Update();
   }
   // Plot Multiplicity
   {
      TCanvas* c1 = fCanvasMulti;
      c1->Clear();
      c1->Divide(2,4);

      for(int i = 0; i < 8; i++){
         c1->cd(1+i);
          hMulti[i]->Draw();
      }
      
      c1->Modified();
      c1->Update();
   }

} // End Plot Histigrams


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
   t1->Branch("PGACenergy",&AnodeEnergy,"AnodeEnergy/D");
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

   // Read TDC Midas Bank

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

   // Read MADC Midas Bank
  
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
