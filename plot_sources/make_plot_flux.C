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

void make_plot_flux()
{
    string dat_dir = "../dat_files/";
    
    //FLUXOS
    int width = 2;
    int lineStyle = 9;
    //Gerar canvas
    auto c1 = new TCanvas("c1", "Fluxos", 800, 500);
    c1->SetGrid();
    c1->SetLogx();
                            // x1   y1   x2   y2
    auto legend = new TLegend(0.8, 0.9, 0.9, 0.8);
    auto max_values = new TLegend(0.1, 0.1, 0.3, 0.25);

    auto gre = new TGraph((dat_dir+"spectrum_DUNE_e.dat").c_str());
    auto grmu = new TGraph((dat_dir+"spectrum_DUNE_mu.dat").c_str());
    auto grtau = new TGraph((dat_dir+"spectrum_DUNE_tau.dat").c_str());

    //muon - azul
    grmu->SetLineColor(kBlue);
    grmu->SetLineStyle(lineStyle);
    grmu->SetLineWidth(width);
    grmu->SetMarkerStyle(20);
    grmu->SetMarkerColorAlpha(kBlue, 1);

    //eletron - vermelho
    gre->SetLineColor(kRed);
    gre->SetLineStyle(lineStyle);
    gre->SetLineWidth(width);
    gre->SetMarkerStyle(20);
    gre->SetMarkerColorAlpha(kRed, 1);

    //tau - verde
    grtau->SetLineColor(kGreen);
    grtau->SetLineStyle(lineStyle);
    grtau->SetLineWidth(width);
    grtau->SetMarkerStyle(20);
    grtau->SetMarkerColorAlpha(kGreen, 1);
    
    //Plot usando MultiGraph
    auto *g = new TMultiGraph();
    
    g->SetTitle("Fluxos dos Neutrinos;Energy (GeV);Fluxo (Log)");
    g->GetXaxis()->CenterTitle(true);
    g->GetYaxis()->CenterTitle(true);

    g->GetXaxis()->SetTitleOffset(1.2);
    g->GetYaxis()->SetTitleOffset(1.25);

    g->GetXaxis()->SetLimits(0.225,12.25);
    g->GetXaxis()->SetRangeUser(0.25,10.25);
    g->GetYaxis()->SetRangeUser(-14.5,-10.05);
    
    g->Add(gre);
    g->Add(grmu);
    g->Add(grtau);

    string emax = "Max Eletron: " + to_string(TMath::MaxElement(gre->GetN(),gre->GetY()));
    string mumax = "Max Muon: " + to_string(TMath::MaxElement(grmu->GetN(),grmu->GetY()));
    string taumax = "Max Tau: " + to_string(TMath::MaxElement(grtau->GetN(),grtau->GetY()));

    g->Draw("al");

    legend->SetTextAlign(12);
    legend->AddEntry(grmu, "Muon", "l");
    legend->AddEntry(gre, "Eletron", "l");
    legend->AddEntry(grtau, "Tau", "l");
    legend->SetTextSize(0.025);

    //max_values->SetHeader("Max values","C");
    max_values->SetTextAlign(12);
    max_values->AddEntry(gre, emax.c_str(), "p");
    max_values->AddEntry(grmu, mumax.c_str(), "p");
    max_values->AddEntry(grtau, taumax.c_str(), "p");
    max_values->SetTextSize(0.0225);
    
    max_values->Draw();
    legend->Draw();

    c1->Update();
}
