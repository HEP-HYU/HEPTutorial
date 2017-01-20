#include "MyAnalysis.h"
#include "Plotter.h"
#include <iostream>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <string>

void example() {
   
   float lumi = 36500.;
   
   TString dir = "/data/users/jhgoh/hep2016/v803_3_dygyun";

   int nevt = -1;

   bool run = true;
   bool proof = true;

   if( run ) {

   const bool isBatch = gROOT->IsBatch();
   gROOT->SetBatch(true);
   if ( proof ) {
      TProof* p = TProof::Open("", "workers=8");
      TString basedir = gSystem->Getenv("PWD");
      gProof->Exec(Form("gSystem->AddDynamicPath(\"%s\");", basedir.Data()));
      gProof->Exec(Form("gSystem->AddIncludePath(\"%s\");", basedir.Data()));
      //gProof->Exec(Form("gSystem->AddIncludePath(%s);", gSystem->Getenv("PWD")));
      gProof->Exec("gSystem->Load(\"libMyAnalysis.so\");");
      gProof->Exec("gROOT->ProcessLine(\"#include \\\"MyAnalysis.h\\\"\");");
   }

   MyAnalysis *A = new MyAnalysis();
   TChain* ch = new TChain("TopTree/events");
   ch->Add(Form("%s/hep_SingleMuon_Run2016B.root",dir.Data()), nevt);
   ch->Add(Form("%s/hep_SingleMuon_Run2016C.root",dir.Data()), nevt);
   ch->Add(Form("%s/hep_SingleMuon_Run2016D.root",dir.Data()), nevt);
   ch->Add(Form("%s/hep_SingleMuon_Run2016E.root",dir.Data()), nevt);
   ch->Add(Form("%s/hep_SingleMuon_Run2016F.root",dir.Data()), nevt);
   ch->Add(Form("%s/hep_SingleMuon_Run2016G.root",dir.Data()), nevt);
   ch->Add(Form("%s/hep_SingleMuon_Run2016H_v2.root",dir.Data()), nevt);
   ch->Add(Form("%s/hep_SingleMuon_Run2016H_v3.root",dir.Data()), nevt);
   if ( proof ) ch->SetProof();
   ch->Process(A,"data");

   MyAnalysis *B = new MyAnalysis(1,1,831.8,lumi);
   TChain* ch2 = new TChain("TopTree/events");
   ch2->Add(Form("%s/hep_TT_powheg.root",dir.Data()), nevt);
   if ( proof ) ch2->SetProof();
   ch2->Process(B,"TT_powheg");

   MyAnalysis *C = new MyAnalysis(1,1,61524,lumi);
   TChain* ch3 = new TChain("TopTree/events");
   ch3->Add(Form("%s/hep_WJets.root",dir.Data()),nevt);
   if ( proof ) ch3->SetProof();
   ch3->Process(C,"WJets");
   
   MyAnalysis *D = new MyAnalysis(1,1,6025.2,lumi);
   TChain* ch4 = new TChain("TopTree/events");
   ch4->Add(Form("%s/hep_DYJets.root",dir.Data()),nevt);
   if ( proof ) ch4->SetProof();
   ch4->Process(D,"DYJets");

   MyAnalysis *E = new MyAnalysis(1,1,18610.0,lumi);
   TChain* ch5 = new TChain("TopTree/events");
   ch5->Add(Form("%s/hep_DYJets_10to50.root",dir.Data()),nevt);
   if ( proof ) ch5->SetProof();
   ch5->Process(E,"DYJets_10to50");
 
   MyAnalysis *F = new MyAnalysis(1,1,44.33,lumi);
   TChain* ch6 = new TChain("TopTree/events");
   ch6->Add(Form("%s/hep_SingleTop_t.root",dir.Data()),nevt);
   if ( proof ) ch6->SetProof();
   ch6->Process(F,"SingleTop_t");

   MyAnalysis *J = new MyAnalysis(1,1,26.38,lumi);
   TChain* ch7 = new TChain("TopTree/events");
   ch7->Add(Form("%s/hep_SingleTbar_t.root",dir.Data()),nevt);
   if ( proof ) ch7->SetProof();
   ch7->Process(J,"SingleTbar_t");

   MyAnalysis *K = new MyAnalysis(1,1,35.6,lumi);
   TChain* ch8 = new TChain("TopTree/events");
   ch8->Add(Form("%s/hep_SingleTop_tW.root",dir.Data()),nevt);
   if ( proof ) ch8->SetProof();
   ch8->Process(K,"SingleTop_tW");

   MyAnalysis *L = new MyAnalysis(1,1,35.6,lumi);
   TChain* ch9 = new TChain("TopTree/events");
   ch9->Add(Form("%s/hep_SingleTbar_tW.root",dir.Data()),nevt);
   if ( proof ) ch9->SetProof();
   ch9->Process(L,"SingleTbar_tW");
 
   MyAnalysis *S1 = new MyAnalysis(1,1,1.5,lumi);
   TChain* ch_s1 = new TChain("TopTree/events");
   ch_s1->Add(Form("%s/hep_TT_TopLeptonicDecay_TH_1L3B_Eta_Hct.root",dir.Data()),nevt);
   if ( proof ) ch_s1->SetProof();
   ch_s1->Process(S1,"TT_TopLeptonicDecay_TH_1L3B_Eta_Hct");

   MyAnalysis *S2 = new MyAnalysis(1,1,1.5,lumi);
   TChain* ch_s2 = new TChain("TopTree/events");
   ch_s2->Add(Form("%s/hep_TT_AntitopLeptonicDecay_TH_1L3B_Eta_Hct.root",dir.Data()),nevt);
   if ( proof ) ch_s2->SetProof();
   ch_s2->Process(S2,"TT_AntitopLeptonicDecay_TH_1L3B_Eta_Hct");

   gROOT->SetBatch(isBatch);
   }

   Plotter P;
   P.SetLumi(36500);
   P.SetData("hist_data.root", std::string("Data"));
   P.AddBg("hist_TT_powheg.root", std::string("t#bar{t}"), 2, lumi) ;//831.8);
   P.AddBg("hist_WJets.root", std::string("WJets" ), 5, lumi); //61524);
   P.AddBg("hist_DYJets.root", std::string("DYJets"), 4, lumi); //6025.2);
   P.AddBg("hist_DYJets_10to50.root", std::string("DYJets"), 4, lumi); //18610.0);
   P.AddBg("hist_SingleTop_t.root", std::string("Single t"), 6, lumi); //44.33);
   P.AddBg("hist_SingleTbar_t.root", std::string("Single t"), 6, lumi); //26.38);
   P.AddBg("hist_SingleTop_tW.root", std::string("Single t"), 6, lumi); //35.6);
   P.AddBg("hist_SingleTbar_tW.root", std::string("Single t"), 6, lumi); //35.6);
   P.AddSig("hist_TT_TopLeptonicDecay_TH_1L3B_Eta_Hct.root", std::string("Signal Hct"), kBlue-4, lumi); //35.6);
   P.AddSig("hist_TT_AntitopLeptonicDecay_TH_1L3B_Eta_Hct.root", std::string("Signal Hct"), kBlue-4, lumi); //35.6);
   P.Plot("results.pdf");

}
