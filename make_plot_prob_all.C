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

void make_plot_prob_all()
{
    //VACUO
    //Gerar canvas
    int width = 1;
    auto c1 = new TCanvas("c1", "Probabilidades na materia");
    c1->SetGrid();
    c1->SetLogx();
                            // x1   y1   x2   y2
    //auto legend = new TLegend(0.7, 0.9, 0.9, 0.75);
    auto legend = new TLegend(0.7,0.9,0.9,0.8);

    auto grmu_mat = new TGraph("./dat_files/probability_DUNE_mu_mat.dat");
    auto gre_mat = new TGraph("./dat_files/probability_DUNE_e_mat.dat");
    auto grtau_mat = new TGraph("./dat_files/probability_DUNE_tau_mat.dat");

    auto grmu_vac = new TGraph("./dat_files/probability_DUNE_mu_vac.dat");
    auto gre_vac = new TGraph("./dat_files/probability_DUNE_e_vac.dat");
    auto grtau_vac = new TGraph("./dat_files/probability_DUNE_tau_vac.dat");

    //muon - azul
    grmu_vac->SetLineColor(kBlue);
    grmu_vac->SetLineStyle(7);
    grmu_vac->SetLineWidth(width);

    grmu_mat->SetLineColor(kBlue);
    grmu_mat->SetLineStyle(1);
    grmu_mat->SetLineWidth(width);

    //eletron - vermelho
    gre_vac->SetLineColor(kRed);
    gre_vac->SetLineStyle(7);
    gre_vac->SetLineWidth(width);

    gre_mat->SetLineColor(kRed);
    gre_mat->SetLineStyle(1);
    gre_mat->SetLineWidth(width);

    //tau - verde
    grtau_vac->SetLineColor(kGreen);
    grtau_vac->SetLineStyle(7);
    grtau_vac->SetLineWidth(width);
    
    grtau_mat->SetLineColor(kGreen);
    grtau_mat->SetLineStyle(1);
    grtau_mat->SetLineWidth(width);
    
    //Plot usando MultiGraph
    auto *g = new TMultiGraph();
    
    g->SetTitle("Muon oscillation probabilities (matter);Energy (GeV);Oscillation probability");

    g->GetXaxis()->SetLimits(0.20,10.);
    g->GetXaxis()->SetRangeUser(0.25,10);
    g->GetYaxis()->SetRangeUser(0,1.05);
    
    g->Add(gre_mat);
    g->Add(grmu_mat);
    g->Add(grtau_mat);

    g->Add(gre_vac);
    g->Add(grmu_vac);
    g->Add(grtau_vac);

    g->Draw("al");

    legend->SetHeader("Oscillation probabilities","C");
    legend->SetTextAlign(13);
    legend->AddEntry(grmu_mat, "Muon -> Muon", "l");
    legend->AddEntry(gre_mat, "Muon -> Eletron", "l");
    legend->AddEntry(grtau_mat, "Muon -> Tau", "l");
    
    legend->Draw();

    c1->Update();

    // grmu->Draw();
    // gre->Draw("SAME");
    // grtau->Draw("SAME");
}
