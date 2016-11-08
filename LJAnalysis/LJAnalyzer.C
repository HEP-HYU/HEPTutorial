#define LJAnalyzer_cxx
// The class definition in LJAnalyzer.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("LJAnalyzer.C")
// root> T->Process("LJAnalyzer.C","some options")
// root> T->Process("LJAnalyzer.C+")
//

#include "LJAnalyzer.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TObjString.h>

#include <vector>
#include <iostream>

void LJAnalyzer::book(const char* name, const char* title,
                             const int nbinx, const double xmin, const double xmax,
                             const int nbiny=0, const double ymin=0, const double ymax=0)
{
  if ( nbiny == 0 ) h_[name] = new TH1F(name, title, nbinx, xmin, xmax);
  else new TH2F(name, title, nbinx, xmin, xmax, nbiny, ymin, ymax);
}

void LJAnalyzer::Begin(TTree * /*tree*/)
{
}

void LJAnalyzer::SlaveBegin(TTree * /*tree*/)
{
  auto options = fOption.Tokenize(",");
  if ( options->GetEntries() == 0 ) sampleName_ = "NONE";
  else sampleName_ = ((TObjString*)options->At(0))->String();
  for ( int i=1; i<options->GetEntries(); ++i ) {
    const auto str = ((TObjString*)options->At(i))->String();
    if ( str == "SIGNAL" ) mode_ = SIGNAL;
    else if ( str == "OTHERS" ) mode_ = OTHERS;
    else mode_ = NONE;
  }

  book("allmuons_n", "All muon;Muon multiplicity", 10, 0, 10);
  book("allmuons_pt", "All muon;p_{T} (GeV)", 100, 0, 100);
  book("muons_n", "Selected muon;Muon multiplicity", 10, 0, 10);
  book("muons_pt", "Selected muon;p_{T} (GeV)", 100, 0, 100);
  book("vetomuons_n", "Veto muon;Muon multiplicity", 10, 0, 10);
  book("allelectrons_n", "All electrons;Muon multiplicity", 10, 0, 10);
  book("allelectrons_pt", "All electrons;p_{T} (GeV)", 100, 0, 100);
  book("vetoelectrons_n", "Veto electrons;Muon multiplicity", 10, 0, 10);

  book("alljets_n", "All jets;Jet multiplicity", 10, 0, 10);

  book("jets_n", "jets;Jet multiplicity", 10, 0, 10);
  book("jets_pt", "jets;p_{T} (GeV)", 100, 0, 100);
  book("jets_eta", "jets;#eta", 100, -2.5, 2.5);
  book("jets_m", "jets;Mass (GeV)", 100, 0, 100);
  book("jets_btag", "jets;b tag", 100, 0, 1);

  book("jets_allm12", "any dijets;M(jet1, jet2);Candidates per 10GeV", 50, 0, 500);
  book("jets_allm123", "any trijets;M(jet1, jet2, jet3);Candidates per 10GeV", 50, 0, 500);

  book("jets_leadm12", "leading dijets;M(jet1, jet2);Events per 10GeV", 50, 0, 500);
  book("jets_leadm123", "leading trijets;M(jet1, jet2, jet3);Events per 10GeV", 50, 0, 500);

  book("jets_btagm12", "leading dijets with 3rd jet b tag;M(jet1, jet2);Events per 20GeV", 25, 0, 500);
  book("jets_btagm123", "leading trijets with 3rd jet b tag;M(jet1, jet2, jet3);Events per 20GeV", 25, 0, 500);

  book("jets_btagmwm12", "leading dijets with 3rd jet b tag and MW cut;M(jet1, jet2);Events per 20GeV", 25, 0, 500);
  book("jets_btagmwm123", "leading trijets with 3rd jet b tag and MW cut;M(jet1, jet2, jet3);Events per 20GeV", 25, 0, 500);
}

Bool_t LJAnalyzer::Process(Long64_t entry)
{
  GetEntry(entry);

  if ( mode_ != NONE ) {
    const bool isSignal = [&](){
      if ( std::abs(MCleptonPDGid) != 13 ) return false;

      if ( MCleptonicBottom_px == 0 and MCleptonicBottom_py == 0 and
           MCleptonicBottom_pz == 0 ) return false;
      if ( MChadronicWDecayQuark_px == 0 and MChadronicWDecayQuark_py == 0 and
           MChadronicWDecayQuark_pz == 0 ) return false;
      if ( MChadronicWDecayQuarkBar_px == 0 and MChadronicWDecayQuarkBar_py == 0 and
           MChadronicWDecayQuarkBar_pz == 0 ) return false;
      return true;
    }();

    if ( mode_ == SIGNAL and !isSignal ) return false;
    if ( mode_ == OTHERS and isSignal ) return false;
  }

  std::vector<TLorentzVector> muons, vetomuons;
  for ( int i=0; i<NMuon; ++i ) {
    TLorentzVector mu(Muon_Px[i], Muon_Py[i], Muon_Pz[i], Muon_E[i]);
    const double pt = mu.Pt(), eta = mu.Eta();

    h_["allmuons_pt"]->Fill(pt, EventWeight);

    if ( Muon_Iso[i]/pt > 0.2 ) continue;
    if ( std::abs(eta) > 2.4 ) continue;

    if ( pt > 10 ) vetomuons.push_back(mu);
    if ( std::abs(eta) < 2.1 and pt > 26 ) muons.push_back(mu);
  }
  h_["allmuons_n"]->Fill(NMuon, EventWeight);
  h_["muons_n"]->Fill(muons.size(), EventWeight);

  std::vector<TLorentzVector> vetoelectrons;
  for ( int i=0; i<NElectron; ++i ) {
    TLorentzVector el(Electron_Px[i], Electron_Py[i], Electron_Pz[i], Electron_E[i]);
    const double pt = el.Pt(), eta = el.Eta();

    h_["allelectrons_pt"]->Fill(pt, EventWeight);

    if ( Electron_Iso[i]/pt > 0.2 ) continue;
    if ( std::abs(eta) > 2.4 ) continue;
    
    if ( pt > 10 ) vetoelectrons.push_back(el);
  }
  h_["allelectrons_n"]->Fill(NElectron, EventWeight);
  h_["vetoelectrons_n"]->Fill(vetoelectrons.size(), EventWeight);

  if ( muons.size() != 1 ) return false;
  if ( !vetoelectrons.empty() or vetomuons.size() > 1 ) return false;

  const auto muon0 = muons[0];

  std::vector<TLorentzVector> jets;
  std::vector<double> jetsBtag;
  for ( int i=0; i<NJet; ++i ) {
    if ( !Jet_ID[i] ) continue;
    TLorentzVector jet(Jet_Px[i], Jet_Py[i], Jet_Pz[i], Jet_E[i]);
    const double pt = jet.Pt(), eta = jet.Eta();
    if ( pt < 30 or std::abs(eta) > 2.4 ) continue;
    if ( muon0.DeltaR(jet) < 0.3 ) continue;

    jets.push_back(jet);
    jetsBtag.push_back(Jet_btag[i]);

    h_["jets_pt"]->Fill(pt, EventWeight);
    h_["jets_eta"]->Fill(eta, EventWeight);
    h_["jets_m"]->Fill(jet.M(), EventWeight);
    h_["jets_btag"]->Fill(Jet_btag[i], EventWeight);
  }
  h_["alljets_n"]->Fill(NJet, EventWeight);
  h_["jets_n"]->Fill(jets.size(), EventWeight);
  if ( jets.size() < 4 ) return false;

  std::vector<int> jetIdxsByPt;
  for ( int i=0, n=jets.size(); i<n; ++i ) jetIdxsByPt.push_back(i);

  std::sort(jetIdxsByPt.begin(), jetIdxsByPt.end(),
            [&](const int i, const int j){return jets[i].Pt() > jets[j].Pt();});

  // Find a jet nearest to the lepton
  TLorentzVector jet0;
  for ( int i=0, n=jets.size(); i<n; ++i ) {
    const auto& jet = jets[i];
    
    const double dRLep = muon0.DeltaR(jet);
    if ( dRLep > TMath::Pi()/2 ) continue;
    if ( jet0.Pt() == 0 or dRLep < muon0.DeltaR(jet0) ) jet0 = jet;
  }

  // Continue to the other jets to form top
  TLorentzVector wCand, tCand;
  TLorentzVector wCandWithBtag, tCandWithBtag;
  TLorentzVector wCandWithBtagMW, tCandWithBtagMW;
  for ( int i=0, n=jetIdxsByPt.size(); i<n; ++i ) {
    const auto& jet1 = jets[jetIdxsByPt[i]];
    if ( jet1 == jet0 ) continue;
    if ( muon0.DeltaR(jet1) < TMath::Pi()/2 ) continue;
    for ( int j=i+1; j<n; ++j ) {
      const auto& jet2 = jets[jetIdxsByPt[j]];
      if ( jet2 == jet0 ) continue;
      if ( muon0.DeltaR(jet2) < TMath::Pi()/2 ) continue;
      h_["jets_allm12"]->Fill((jet1+jet2).M(), EventWeight);
      for ( int k=j+1; k<n; ++k ) {
        const auto& jet3 = jets[jetIdxsByPt[k]];
        if ( jet3 == jet0 ) continue;
        if ( muon0.DeltaR(jet3) < TMath::Pi()/2 ) continue;

        h_["jets_allm123"]->Fill((jet1+jet2+jet3).M(), EventWeight);

        if ( wCand.Pt() == 0 ) {
          wCand = jet1+jet2;
          tCand = wCand+jet3;
        }
        if ( jetsBtag[jetIdxsByPt[k]] > 0.1 and wCandWithBtag.Pt() == 0 ) {
          wCandWithBtag = jet1+jet2;
          tCandWithBtag = wCandWithBtag+jet3;
          if ( wCandWithBtag.M() < 150 ) {
            wCandWithBtagMW = wCandWithBtag;
            tCandWithBtagMW = tCandWithBtag;
          }
        }
      }
    }
  }
  if ( wCand.M() > 0 ) {
    h_["jets_leadm12"]->Fill(wCand.M(), EventWeight);
    h_["jets_leadm123"]->Fill(tCand.M(), EventWeight);
  }
  if ( wCandWithBtag.M() > 0 ) {
    h_["jets_btagm12"]->Fill(wCandWithBtag.M(), EventWeight);
    h_["jets_btagm123"]->Fill(tCandWithBtag.M(), EventWeight);
  }
  if ( wCandWithBtagMW.M() > 0 ) {
    h_["jets_btagmwm12"]->Fill(wCandWithBtagMW.M(), EventWeight);
    h_["jets_btagmwm123"]->Fill(tCandWithBtagMW.M(), EventWeight);
  }

  return kTRUE;
}

void LJAnalyzer::SlaveTerminate()
{
  for ( auto x = h_.begin(); x != h_.end(); ++x ) {
    fOutput->Add(x->second);
  }
}

void LJAnalyzer::Terminate()
{
  TFile* f = TFile::Open(("hists/"+sampleName_+".root").c_str(), "recreate");

  for ( int i=0; i<fOutput->GetEntries(); ++i ) {
    TH1* h = dynamic_cast<TH2F*>(fOutput->At(i));
    if ( !h ) h = dynamic_cast<TH1F*>(fOutput->At(i));
    if ( !h ) continue;

    h->Write();
  }    
  f->Close();
}
