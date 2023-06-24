#include "TH1.h"
#include "TH2.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

void make_plot_flux_anti()
{
    string dat_dir = "../dat_files/";

    //FLUXOS ANTI NEUTRINOS
    int width = 2;
    int lineStyle = 9;
    //Gerar canvas
    auto canvas = new TCanvas("c_anti", "Fluxos Anti", 800, 500);
    canvas->SetGrid();
    canvas->SetLogx();
                            // x1   y1   x2   y2
    auto legend = new TLegend(0.8, 0.9, 0.9, 0.8);
    auto max_values = new TLegend(0.65, 0.1, 0.9, 0.25);
    
    auto gr_antie = new TGraph((dat_dir+"spectrum_DUNE_antie.dat").c_str());
    auto gr_antimu = new TGraph((dat_dir+"spectrum_DUNE_antimu.dat").c_str());
    auto gr_antitau = new TGraph((dat_dir+"spectrum_DUNE_antitau.dat").c_str());

    //muon - azul
    gr_antimu->SetLineColor(kBlue);
    gr_antimu->SetLineStyle(lineStyle);
    gr_antimu->SetLineWidth(width);
    gr_antimu->SetMarkerStyle(20);
    gr_antimu->SetMarkerColorAlpha(kBlue, 1);

    //eletron - vermelho
    gr_antie->SetLineColor(kRed);
    gr_antie->SetLineStyle(lineStyle);
    gr_antie->SetLineWidth(width);
    gr_antie->SetMarkerStyle(20);
    gr_antie->SetMarkerColorAlpha(kRed, 1);

    //tau - verde
    gr_antitau->SetLineColor(kGreen);
    gr_antitau->SetLineStyle(lineStyle);
    gr_antitau->SetLineWidth(width);
    gr_antitau->SetMarkerStyle(20);
    gr_antitau->SetMarkerColorAlpha(kGreen, 1);
    
    //Plot usando MultiGraph
    auto *g = new TMultiGraph();
    
    g->SetTitle("Fluxos Anti Neutrinos;Energy (GeV);Fluxo (Log)");
    g->GetXaxis()->CenterTitle(true);
    g->GetYaxis()->CenterTitle(true);

    g->GetXaxis()->SetTitleOffset(1.2);
    g->GetYaxis()->SetTitleOffset(1.25);

    g->GetXaxis()->SetLimits(0.225,12.25);
    g->GetXaxis()->SetRangeUser(0.25,10.25);
    g->GetYaxis()->SetRangeUser(-15.25,-11.);
    
    g->Add(gr_antie);
    g->Add(gr_antimu);
    g->Add(gr_antitau);

    string emax = "Max Anti-Eletron: " + to_string(TMath::MaxElement(gr_antie->GetN(),gr_antie->GetY()));
    string mumax = "Max Anti-Muon: " + to_string(TMath::MaxElement(gr_antimu->GetN(),gr_antimu->GetY()));
    string taumax = "Max Anti-Tau: " + to_string(TMath::MaxElement(gr_antitau->GetN(),gr_antitau->GetY()));

    g->Draw("al");

    legend->SetTextAlign(12);
    legend->AddEntry(gr_antimu, "Anti-Muon", "l");
    legend->AddEntry(gr_antie, "Anti-Eletron", "l");
    legend->AddEntry(gr_antitau, "Anti-Tau", "l");
    legend->SetTextSize(0.025);

    //max_values->SetHeader("Max values","C");
    max_values->SetTextAlign(12);
    max_values->AddEntry(gr_antie, emax.c_str(), "p");
    max_values->AddEntry(gr_antimu, mumax.c_str(), "p");
    max_values->AddEntry(gr_antitau, taumax.c_str(), "p");
    max_values->SetTextSize(0.0225);
    
    max_values->Draw();
    legend->Draw();

    canvas->Update();
}
