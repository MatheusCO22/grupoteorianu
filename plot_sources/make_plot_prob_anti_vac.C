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

void make_plot_prob_anti_vac()
{
    string dat_dir = "../dat_files/";
    
    //VACUO - ANTI NEUTRINO
    //Gerar canvas
    int width = 2;
    auto c1 = new TCanvas("c3", "Probabilidades no vacuo");
    c1->SetGrid();
    c1->SetLogx();
                            // x1   y1   x2   y2
    auto legend = new TLegend(0.7, 0.9, 0.9, 0.8);

    auto gr_antie = new TGraph((dat_dir+"probability_DUNE_antie_vac.dat").c_str());
    auto gr_antimu = new TGraph((dat_dir+"probability_DUNE_antimu_vac.dat").c_str());
    auto gr_antitau = new TGraph((dat_dir+"probability_DUNE_antitau_vac.dat").c_str());

    //muon - azul
    gr_antimu->SetLineColor(kBlue);
    gr_antimu->SetLineStyle(7);
    gr_antimu->SetLineWidth(width);

    //eletron - vermelho
    gr_antie->SetLineColor(kRed);
    gr_antie->SetLineStyle(7);
    gr_antie->SetLineWidth(width);

    //tau - verde
    gr_antitau->SetLineColor(kGreen);
    gr_antitau->SetLineStyle(7);
    gr_antitau->SetLineWidth(width);
    
    //Plot usando MultiGraph
    auto *g = new TMultiGraph();

    g->SetTitle("Anti Muon oscillation probabilities (vacuum);Energy (GeV);Oscillation probability");

    g->GetXaxis()->SetLimits(0.20,10.);
    g->GetXaxis()->SetRangeUser(0.25,10);
    g->GetYaxis()->SetRangeUser(0,1.05);
    
    g->Add(gr_antie);
    g->Add(gr_antimu);
    g->Add(gr_antitau);

    g->Draw("al");

    legend->SetHeader("Oscillation probabilities","C");
    legend->SetTextAlign(13);
    legend->AddEntry(gr_antimu, "Anti Muon -> Anti Muon", "l");
    legend->AddEntry(gr_antie, "Anti Muon -> Anti Eletron", "l");
    legend->AddEntry(gr_antitau, "Anti Muon -> Anti Tau", "l");
    
    legend->Draw();

    c1->Update();
}
