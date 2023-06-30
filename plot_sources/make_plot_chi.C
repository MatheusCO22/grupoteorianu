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

void make_plot_chi(){
    string dat_dir = "../dat_files/";
    auto c_chi = new TCanvas("c_chi", "CHI^2");

    auto *grchi = new TGraph2D((dat_dir+"chi2.dat").c_str());
    grchi->SetTitle("Grafico Chi^2;theta13(converter);deltacp; Chi^2");
    grchi->Draw("tri1");
}