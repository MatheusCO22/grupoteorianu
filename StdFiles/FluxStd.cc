#include <iostream>
#include <cmath>
#include <string.h>
#include <float.h>
#include <complex.h>
#include <vector>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <globes/globes.h>
#include <fstream>

using namespace std;

char AEDLFILE[] = "DUNE_GLoBES.glb";

int main(int argc, char * argv[])
{
	glbInit(argv[0]);
	glbInitExperiment(AEDLFILE, &glb_experiment_list[0], &glb_num_of_exps);

	ofstream outstde, outstdmu, outstdtau,
			 outstd_antie, outstd_antimu, outstd_antitau;
	
	string dat_dir = "../dat_files/";

	outstde.open((dat_dir+"spectrum_DUNE_e.dat").c_str());
	outstdmu.open((dat_dir+"spectrum_DUNE_mu.dat").c_str());
	outstdtau.open((dat_dir+"spectrum_DUNE_tau.dat").c_str());

	outstd_antie.open((dat_dir+"spectrum_DUNE_antie.dat").c_str());
	outstd_antimu.open((dat_dir+"spectrum_DUNE_antimu.dat").c_str());
	outstd_antitau.open((dat_dir+"spectrum_DUNE_antitau.dat").c_str());
	
	double dm21 = 7.55e-5;
	double dm31 = 2.50e-3;
	double theta12 = asin(sqrt(0.320));
	double theta23 = asin(sqrt(0.547));
	double theta13 = asin(sqrt(0.02160));
	double deltacp = -0.68 * M_PI;

	/* Define "true" oscillation parameter vector */
	glb_params true_values = glbAllocParams();

    glbDefineParams(true_values,theta12,theta13,theta23,deltacp,dm21,dm31);
	glbSetDensityParams(true_values, 1.0, GLB_ALL);
	glbSetOscillationParameters(true_values);
	glbSetRates();

	double  probmumu, probemu, probtaumu,
			probmue, probee, probtaue,
			probmutau, probetau, probtautau,
			prob_antimumu, prob_antiemu, prob_antitaumu,
			prob_antimue, prob_antiee, prob_antitaue,
			prob_antimutau, prob_antietau, prob_antitautau,

			fluxmu, fluxe, fluxtau,
			flux_antimu, flux_antie, flux_antitau, energy;

	double emin= 0.25 ; //GeV
	double emax=10 ; //GeV
	double step= 3000;
	[[maybe_unused]] double L = 1300;// km

	for (energy=emin;energy<=emax;energy+=(emax-emin)/step)
	{
        fluxe   = glbFlux(0,0,energy,L,1,+1)/4.10414e+12;
        fluxmu  = glbFlux(0,0,energy,L,2,+1)/4.10414e+12;
        fluxtau = glbFlux(0,0,energy,L,3,+1)/4.10414e+12;

        //ELETRON
        probee   = glbProfileProbability(0,1,1,+1,energy);
        probmue  = glbProfileProbability(0,2,1,+1,energy);
        probtaue = glbProfileProbability(0,3,1,+1,energy);

        //MUON
        probemu   = glbProfileProbability(0,1,2,+1,energy);
        probmumu  = glbProfileProbability(0,2,2,+1,energy);
        probtaumu = glbProfileProbability(0,3,2,+1,energy);

        //TAU
        probetau    = glbProfileProbability(0,1,3,+1,energy);
        probmutau   = glbProfileProbability(0,2,3,+1,energy);
        probtautau  = glbProfileProbability(0,3,3,+1,energy);

        outstdmu<<energy<<"  "<<log10(probmumu*fluxmu + probemu*fluxe + probtaumu*fluxtau)<<endl;
        outstde<<energy<<"  "<<log10(probmue*fluxmu + probee*fluxe + probtaue*fluxtau)<<endl;
        outstdtau<<energy<<"  "<<log10(probmutau*fluxmu + probetau*fluxe + probtautau*fluxtau)<<endl;
	}
	//ANTI NEUTRINOS
	for (energy=emin;energy<=emax;energy+=(emax-emin)/step)
	{
        flux_antie   = glbFlux(0,0,energy,L,1,-1)/4.10414e+12;
        flux_antimu  = glbFlux(0,0,energy,L,2,-1)/4.10414e+12;
        flux_antitau = glbFlux(0,0,energy,L,3,-1)/4.10414e+12;

        //ELETRON
        prob_antiee   = glbProfileProbability(0,1,1,-1,energy);
        prob_antimue  = glbProfileProbability(0,2,1,-1,energy);
        prob_antitaue = glbProfileProbability(0,3,1,-1,energy);

        //MUON
        prob_antiemu   = glbProfileProbability(0,1,2,-1,energy);
        prob_antimumu  = glbProfileProbability(0,2,2,-1,energy);
        prob_antitaumu = glbProfileProbability(0,3,2,-1,energy);

        //TAU
        prob_antietau    = glbProfileProbability(0,1,3,-1,energy);
        prob_antimutau   = glbProfileProbability(0,2,3,-1,energy);
        prob_antitautau  = glbProfileProbability(0,3,3,-1,energy);

        outstd_antimu<<energy<<"  "<<log10(prob_antimumu*flux_antimu + prob_antiemu*flux_antie + prob_antitaumu*flux_antitau)<<endl;
        outstd_antie<<energy<<"  "<<log10(prob_antimue*flux_antimu + prob_antiee*flux_antie + prob_antitaue*flux_antitau)<<endl;
        outstd_antitau<<energy<<"  "<<log10(prob_antimutau*flux_antimu + prob_antietau*flux_antie + prob_antitautau*flux_antitau)<<endl;
	}

	outstde.close();
	outstdmu.close();
    outstdtau.close();

	outstd_antie.close();
	outstd_antimu.close();
    outstd_antitau.close();

	glbFreeParams(true_values);
    
 	return 0;
}