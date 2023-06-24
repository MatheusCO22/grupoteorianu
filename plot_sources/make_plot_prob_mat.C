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

void make_plot_prob_mat()
{
    string dat_dir = "../dat_files/";

    //VACUO
    //Gerar canvas
    int width = 2;
    auto c1 = new TCanvas("c1", "Probabilidades na materia");
    c1->SetGrid();
    c1->SetLogx();
                            // x1   y1   x2   y2
    auto legend = new TLegend(0.7, 0.9, 0.9, 0.8);

    auto gre = new TGraph((dat_dir+"probability_DUNE_e_mat.dat").c_str());
    auto grmu = new TGraph((dat_dir+"probability_DUNE_mu_mat.dat").c_str());
    auto grtau = new TGraph((dat_dir+"probability_DUNE_tau_mat.dat").c_str());

    //muon - azul
    grmu->SetLineColor(kBlue);
    grmu->SetLineStyle(7);
    grmu->SetLineWidth(width);

    //eletron - vermelho
    gre->SetLineColor(kRed);
    gre->SetLineStyle(7);
    gre->SetLineWidth(width);

    //tau - verde
    grtau->SetLineColor(kGreen);
    grtau->SetLineStyle(7);
    grtau->SetLineWidth(width);
    
    //Plot usando MultiGraph
    auto *g = new TMultiGraph();
    
    g->SetTitle("Muon oscillation probabilities (matter);Energy (GeV);Oscillation probability");

    g->GetXaxis()->SetLimits(0.20,10.);
    g->GetXaxis()->SetRangeUser(0.25,10);
    g->GetYaxis()->SetRangeUser(0,1.05);
    
    g->Add(gre);
    g->Add(grmu);
    g->Add(grtau);

    g->Draw("al");

    legend->SetHeader("Oscillation probabilities","C");
    legend->SetTextAlign(13);
    legend->AddEntry(grmu, "Muon -> Muon", "l");
    legend->AddEntry(gre, "Muon -> Eletron", "l");
    legend->AddEntry(grtau, "Muon -> Tau", "l");
    
    legend->Draw();

    c1->Update();
}
