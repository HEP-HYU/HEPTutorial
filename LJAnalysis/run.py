#!/usr/bin/env python

import sys, os
from ROOT import *

sampleNames = [
    "ttbar", "ttbarOthers", "WW", "WZ", "ZZ", "SingleTop", "WJets", "ZJets", "QCD",
]
samples = {
    "Data":{"title":"Data", "files":["data.root"], "color":kWhite},
    "ZJets":{"title":"Z#rightarrowl^{+}l^{-}+jets", "files":["dy.root"], "color":kAzure+2},
    "QCD":{"title":"QCD", "files":["qcd.root"], "color":kYellow},
    "SingleTop":{"title":"Single top", "files":["single_top.root"], "color":kCyan+1},
    "WJets":{"title":"W+jets", "files":["wjets.root"], "color":kGreen+1},
    "WW":{"title":"Dibosons", "files":["ww.root"], "color":kMagenta},
    "WZ":{"title":"Dibosons", "files":["wz.root"], "color":kMagenta},
    "ZZ":{"title":"Dibosons", "files":["zz.root"], "color":kMagenta},
    "ttbar":{"title":"t#bar{t}#rightarrowl^{#pm}+jets", "files":["ttbar.root"], "color":kRed+1},
    "ttbarOthers":{"title":"t#bar{t} others", "files":["ttbar.root"], "color":kRed+3},
}

if not os.path.exists("hists"):
    os.mkdir("hists")
    for sample in samples:
        chain = TChain("events")
        for f in samples[sample]["files"]: chain.Add("files/"+f)

        opt = sample
        if sample == 'ttbarOthers': opt += ',OTHERS'
        elif sample == 'ttbar': opt += ',SIGNAL'
        chain.Process('LJAnalyzer.C+', opt)

for sample in samples:
    samples[sample]["hfile"] = TFile("hists/"+sample+".root")
hists = [x.GetName() for x in samples["Data"]["hfile"].GetListOfKeys()]

gStyle.SetOptTitle(0)
gStyle.SetOptStat(0)
objs = []
for hName in hists:
    #if "m12" not in hName: continue

    c = TCanvas("c"+hName, hName, 500, 500)
    c.SetGridx()
    c.SetGridy()

    leg = TLegend(0.60, 0.60, 1-c.GetTopMargin()-0.02, 1.-c.GetRightMargin()-0.02)
    hs = THStack()
    hRD = samples["Data"]["hfile"].Get(hName)
    hMC = None
    for sample in sampleNames:
        h = samples[sample]["hfile"].Get(hName)

        if hMC == None:
            hMC = h.Clone()
            hMC.SetName("hSumMC"+hName)
            hMC.Reset()

        h.SetFillColor(samples[sample]["color"])
        h.SetLineColor(samples[sample]["color"])
        h.SetOption("hist")
        hs.Add(h)
        hMC.Add(h)

    titles = []
    for sample in reversed(sampleNames):
        title = samples[sample]['title']
        if title in titles: continue
        titles.append(title)
        leg.AddEntry(samples[sample]["hfile"].Get(hName), title, 'f')
    leg.AddEntry(hRD, "Data", "lp")
    leg.SetFillStyle(0)
    leg.SetBorderSize(0)

    hMC.SetLineColor(kBlack)
    hMC.SetFillStyle(0)

    hRD.SetMaximum(1.2*max(hMC.GetMaximum(), hRD.GetMaximum()))
    hRD.SetMarkerSize(1)
    hRD.SetMarkerStyle(kFullCircle)

    hMC.Draw("hist")
    hs.Draw("samehist")
    hMC.Draw("samehist")
    hRD.Draw("sameE")
    leg.Draw()

    label1 = TLatex().DrawLatexNDC(c.GetLeftMargin()+0.01, 1-c.GetTopMargin()+0.01, "CMS Open Data 2010")
    label2 = TLatex().DrawLatexNDC(1-c.GetRightMargin()-0.01, 1-c.GetTopMargin()+0.01, "#sqrt{s}=7TeV L=50pb^{-1}")
    label1.SetTextAlign(11)
    label2.SetTextAlign(31)
    label1.SetTextSize(0.05)
    label2.SetTextSize(0.04)
    label2.SetTextFont(62)

    hRD.Draw("sameaxis")

    objs.extend([c, hs, hRD, hMC, leg, label1, label2])
