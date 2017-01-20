#include "Plotter.h"

Plotter::Plotter() {
	// TODO Auto-generated constructor stub
   
}

Plotter::~Plotter() {
	// TODO Auto-generated destructor stub
}

void Plotter::Plot(std::string filename) {
   
	//gROOT->Reset();
	gROOT->SetStyle("Plain");
   
	TStyle *MyStyle = new TStyle("MyStyle","My Root Styles");
        MyStyle->SetCanvasDefH(600);
        MyStyle->SetCanvasDefW(600);
	MyStyle->SetStatColor(0);
	MyStyle->SetCanvasColor(0);
	MyStyle->SetPadColor(0);
	MyStyle->SetPadBorderMode(0);
	MyStyle->SetCanvasBorderMode(0);
	MyStyle->SetFrameBorderMode(0);
	MyStyle->SetOptStat(0);
	MyStyle->SetStatBorderSize(2);
	MyStyle->SetOptTitle(0);
	MyStyle->SetPadTickX(1);
	MyStyle->SetPadTickY(1);
	MyStyle->SetPadBorderSize(2);
	MyStyle->SetPalette(51, 0);
	MyStyle->SetPadBottomMargin(0.15);
	MyStyle->SetPadTopMargin(0.05);
	MyStyle->SetPadLeftMargin(0.15);
	MyStyle->SetPadRightMargin(0.25);
	MyStyle->SetTitleColor(1);
	MyStyle->SetTitleFillColor(0);
	MyStyle->SetTitleFontSize(0.05);
	MyStyle->SetTitleBorderSize(0);
	MyStyle->SetLineWidth(1);
	MyStyle->SetHistLineWidth(3);
	MyStyle->SetLegendBorderSize(0);
	MyStyle->SetNdivisions(502, "x");
	MyStyle->SetMarkerSize(0.8);
	MyStyle->SetTickLength(0.03);
	MyStyle->SetTitleOffset(1.5, "x");
	MyStyle->SetTitleOffset(1.5, "y");
	MyStyle->SetTitleOffset(1.0, "z");
	MyStyle->SetLabelSize(0.05, "x");
	MyStyle->SetLabelSize(0.05, "y");
	MyStyle->SetLabelSize(0.05, "z");
	MyStyle->SetLabelOffset(0.03, "x");
	MyStyle->SetLabelOffset(0.03, "y");
	MyStyle->SetLabelOffset(0.03, "z");
	MyStyle->SetTitleSize(0.05, "x");
	MyStyle->SetTitleSize(0.05, "y");
	MyStyle->SetTitleSize(0.05, "z");
	gROOT->SetStyle("MyStyle");
   
	gROOT->ForceStyle();
   
	bool DrawLog = true;

	for (int i = 0; i < N_histos; ++i) {
      
          THStack *hs;
          TLegend *l;
          TH1D *h_signal; 

          int Nset = data.size() + bg.size() + signal.size();
          if (Nset > 20)
            Nset = 20.;
          l = new TLegend(0.76, 0.90 - 0.8 * Nset / 20., 0.95, 0.95);
          l->SetTextSize(0.04);
          l->SetFillColor(0);
          l->SetLineColor(0);

          if (bg.size() > 0) {
	    hs = new THStack("hs", bg.at(0).at(i)->GetName());

            for (int j = 0 ; j < bg.size() ; j++) {

              bg.at(j).at(i)->SetFillColor(bg_color.at(j));

              float scale = luminosity/bg_X.at(j);
              bg.at(j).at(i)->Scale( scale );

              hs->Add(bg.at(j).at(i));
              
              //Add Entry in the legend only for the last one when the names are the same. 
              //check with next background if the name is the same.  
              if( (j < bg.size()-1) && bg_names.at(j) == bg_names.at(j+1) ) { 
                bg.at(j).at(i)->SetLineColor(bg_color.at(j));
                continue;
              }
              l->AddEntry(bg.at(j).at(i), bg_names.at(j).c_str(), "f");
            }
          }

          if (signal.size() > 0 ){
            for (int j = 0 ; j < signal.size(); j++){
              signal.at(j).at(i)->SetLineColor(signal_color.at(j)); 
              signal.at(j).at(i)->SetLineStyle(2); 
              float scale = luminosity/signal_X.at(j);
              signal.at(j).at(i)->Scale( scale );

              if( j == 0 ) h_signal = signal.at(j).at(i);
              else h_signal->Add(signal.at(j).at(i));

              if( (j < signal.size()-1) && signal_names.at(j) == signal_names.at(j+1) ) {
                continue;
              }
              l->AddEntry(signal.at(j).at(i), signal_names.at(j).c_str(), "L");

            } 
          }

          TCanvas *c = new TCanvas(Form("c_%i",i), "c", 800, 600);
          c->SetLogy(DrawLog);
          std::string plotname;
  
          if (data.size() > 0) {
            plotname = std::string(data.at(0).at(i)->GetName());
            data.at(0).at(i)->SetMaximum(5 * data.at(0).at(i)->GetMaximum());
            data.at(0).at(i)->GetXaxis()->SetTitleOffset(1.3);
            data.at(0).at(i)->GetYaxis()->SetTitleOffset(1.3);
            data.at(0).at(i)->GetYaxis()->SetTitle("Events");
            data.at(0).at(i)->GetXaxis()->SetNdivisions(505);
            data.at(0).at(i)->Draw("");
            l->AddEntry(data.at(0).at(i), data_names.at(0).c_str(), "p");
            
            if (bg.size() > 0)
              hs->Draw("histsame");
            if (signal.size() > 0) 
              h_signal->Draw("histsame");
  
            data.at(0).at(i)->SetMarkerStyle(20);
            data.at(0).at(i)->Draw("psame");
            l->Draw("same");
         }
	
         if (data.size() == 0 && bg.size() > 0) {
           plotname = std::string(bg.at(0).at(i)->GetName());
           hs->Draw("hist");
           hs->GetXaxis()->SetTitleOffset(1.3);
           hs->GetXaxis()->SetNdivisions(505);
           hs->GetYaxis()->SetTitleOffset(1.3);
           
           hs->GetXaxis()->SetTitle(bg.at(0).at(i)->GetXaxis()->GetTitle());
           hs->GetYaxis()->SetTitle("Events");
           if (signal.size() > 0)
              h_signal->Draw("histsame");         
           l->Draw("same");
         }

         
         TPaveText *label = new TPaveText();
         label->SetX1NDC(gStyle->GetPadLeftMargin());
         label->SetY1NDC(1.0-gStyle->GetPadTopMargin());
         label->SetX2NDC(1.0-gStyle->GetPadRightMargin());
         label->SetY2NDC(1.0);
         label->SetTextFont(42);
         label->AddText(Form("CMS, %3.1f fb^{-1} at #sqrt{s} = 13 TeV",luminosity/1000));
         label->SetFillStyle(0);
         label->SetBorderSize(0);
         label->SetTextSize(0.04);
         label->SetTextAlign(32);
         label->Draw("same");

         c->Print((plotname+std::string(".pdf")).c_str());
         if (i == 0 && N_histos > 1) {
           c->Print((filename+std::string("(")).c_str());
         } else if (i > 0 && i == N_histos - 1)
	   c->Print((filename+std::string(")")).c_str());
         else
           c->Print(filename.c_str());
	 }
   
}
