/*
 * Plotter.h
 *
 *  Created on: 25.06.2012
 *      Author: csander
 */

#ifndef PLOTTER_H_
#define PLOTTER_H_

#include <vector>
#include <string>
#include <iostream>

#include <TH1D.h>
#include <TFile.h>
#include <TStyle.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TKey.h>
#include <TClass.h>
#include <TLatex.h>
#include <TPaveText.h>

class Plotter {
public:
	Plotter();
	virtual ~Plotter();
        void SetLumi(float l){
          luminosity = l;
        }
	void SetData(std::vector<TH1D*> v, std::string n){
		data.push_back(v);
		data_names.push_back(n);
		N_histos = v.size();
	}
        void SetData(TString name, std::string n){
               TFile * f = new TFile(Form("%s",name.Data()));
               std::vector<TH1D*> v;
               TIter next(f->GetListOfKeys()); 
               TKey *key;
               while (key = (TKey*)next() ){
                 TClass *cl = gROOT->GetClass(key->GetClassName());
                 if ( cl->InheritsFrom("TH1D")) {
                   TH1D * h = (TH1D*) key->ReadObj(); 
                   v.push_back(h);
                 }
               }
               data.push_back(v);
               data_names.push_back(n);
               N_histos = v.size();
        }

	void ClearData(){
		data.clear();
		data_names.clear();
	}
	void AddBg(std::vector<TH1D*> v, std::string n, Color_t color, float x = 1.0){
		bg.push_back(v);
		bg_names.push_back(n);
		N_histos = v.size();
                bg_X.push_back(x);
                bg_color.push_back(color);
	}
        void AddBg(TString name, std::string n, Color_t color, float x = 1.0){
                TFile * f = new TFile(Form("%s",name.Data()));
                std::vector<TH1D*> v;
                TIter next(f->GetListOfKeys());             
                TKey *key;
                while (key = (TKey*)next() ){
                  TClass *cl = gROOT->GetClass(key->GetClassName());
                  if ( cl->InheritsFrom("TH1D")) {
                    TH1D * h = (TH1D*) key->ReadObj();
                    v.push_back(h);
                  }
                }
                bg.push_back(v);
                bg_names.push_back(n);
                N_histos = v.size();
                bg_X.push_back(x);
                bg_color.push_back(color);
        }

	void ClearBg(){
		bg.clear();
		bg_names.clear();
	}
	void AddSig(std::vector<TH1D*> v, std::string n, Color_t color, float x = 1.0){
		signal.push_back(v);
		signal_names.push_back(n);
		N_histos = v.size();
                signal_X.push_back(x);
                signal_color.push_back(color);
	}
        void AddSig(TString name, std::string n, Color_t color, float x = 1.0){
                TFile * f = new TFile(Form("%s",name.Data()));
                std::vector<TH1D*> v;
                TIter next(f->GetListOfKeys());             
                TKey *key;
                while (key = (TKey*)next() ){
                  TClass *cl = gROOT->GetClass(key->GetClassName());
                  if ( cl->InheritsFrom("TH1D")) {
                    TH1D * h = (TH1D*) key->ReadObj();
                    v.push_back(h);
                  }
                }
                signal.push_back(v);
                signal_names.push_back(n);
                N_histos = v.size();
                signal_X.push_back(x);
                signal_color.push_back(color);
        }

	void ClearSig(){
		signal.clear();
		signal_names.clear();
	}
	void Plot(std::string filename = "result.pdf");

private:
	std::vector < std::vector<TH1D*> > data;
	std::vector < std::vector<TH1D*> > bg;
	std::vector < std::vector<TH1D*> > signal;

        std::vector < float > bg_X;
        std::vector < float > signal_X;

        std::vector < Color_t > bg_color;
        std::vector < Color_t > signal_color;

	std::vector < std::string > data_names;
	std::vector < std::string > bg_names;
	std::vector < std::string > signal_names;

	int N_histos;
        float luminosity;

};

#endif /* PLOTTER_H_ */
