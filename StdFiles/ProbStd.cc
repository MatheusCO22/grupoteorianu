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

	ofstream outstdmu_vac, outstde_vac, outstdtau_vac,
			 outstdmu_mat, outstde_mat, outstdtau_mat,
			 outstd_antimu_vac, outstd_antie_vac, outstd_antitau_vac,
			 outstd_antimu_mat, outstd_antie_mat, outstd_antitau_mat;

	string dat_dir = "../dat_files/";

	//					diretorio/nome arquivo.dat						
	outstde_vac.open((dat_dir+"probability_DUNE_e_vac.dat").c_str());
	outstdmu_vac.open((dat_dir+"probability_DUNE_mu_vac.dat").c_str());
	outstdtau_vac.open((dat_dir+"probability_DUNE_tau_vac.dat").c_str());

	outstde_mat.open((dat_dir+"probability_DUNE_e_mat.dat").c_str());
	outstdmu_mat.open((dat_dir+"probability_DUNE_mu_mat.dat").c_str());
	outstdtau_mat.open((dat_dir+"probability_DUNE_tau_mat.dat").c_str());

	//ANTI NEUTRINOS
	outstd_antie_vac.open((dat_dir+"probability_DUNE_antie_vac.dat").c_str());
	outstd_antimu_vac.open((dat_dir+"probability_DUNE_antimu_vac.dat").c_str());
	outstd_antitau_vac.open((dat_dir+"probability_DUNE_antitau_vac.dat").c_str());

	outstd_antie_mat.open((dat_dir+"probability_DUNE_antie_mat.dat").c_str());
	outstd_antimu_mat.open((dat_dir+"probability_DUNE_antimu_mat.dat").c_str());
	outstd_antitau_mat.open((dat_dir+"probability_DUNE_antitau_mat.dat").c_str());
	
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

	double energy, flux, probmu_vac, probe_vac, probtau_vac, 
				   		 probmu_mat, probe_mat, probtau_mat,
						 prob_antimu_vac, prob_antie_vac, prob_antitau_vac, 
				   		 prob_antimu_mat, prob_antie_mat, prob_antitau_mat;
	double emin= 0.25 ; //GeV
	double emax=10 ; //GeV
	double step= 3000;
	[[maybe_unused]] double L = 1300;// km

	for (energy=emin;energy<=emax;energy+=(emax-emin)/step)
	{
		probe_vac = glbVacuumProbability(2,1,+1,energy,L);		//muon para eletron
		probmu_vac = glbVacuumProbability(2,2,+1,energy,L);		//muon para muon
		probtau_vac = glbVacuumProbability(2,3,+1,energy,L);	//muon para tau
		
		probe_mat = glbProfileProbability(0,2,1,+1,energy);		//muon para eletron
		probmu_mat = glbProfileProbability(0,2,2,+1,energy);	//muon para muon
		probtau_mat = glbProfileProbability(0,2,3,+1,energy);	//muon para tau

		glbSetOscillationParameters(true_values);
		flux=glbFlux(0,0,energy,L,2,+1);

		outstde_vac<<energy<<"  "<<(probe_vac)<<endl;
		outstdmu_vac<<energy<<"  "<<(probmu_vac)<<endl;
		outstdtau_vac<<energy<<"  "<<(probtau_vac)<<endl;

		outstde_mat<<energy<<"  "<<(probe_mat)<<endl;
		outstdmu_mat<<energy<<"  "<<(probmu_mat)<<endl;
		outstdtau_mat<<energy<<"  "<<(probtau_mat)<<endl;
	}

	//ANTI NEUTRINOS
	for (energy=emin;energy<=emax;energy+=(emax-emin)/step)
	{
		prob_antie_vac = glbVacuumProbability(2,1,-1,energy,L);		//anti muon para anti eletron
		prob_antimu_vac = glbVacuumProbability(2,2,-1,energy,L);	//anti muon para anti muon
		prob_antitau_vac = glbVacuumProbability(2,3,-1,energy,L);	//anti muon para anti tau
		
		prob_antie_mat = glbProfileProbability(0,2,1,-1,energy);	//anti muon para eletron
		prob_antimu_mat = glbProfileProbability(0,2,2,-1,energy);	//anti muon para anti muon
		prob_antitau_mat = glbProfileProbability(0,2,3,-1,energy);	//anti muon para anti tau

		glbSetOscillationParameters(true_values);
		flux=glbFlux(0,0,energy,L,2,-1);

		outstd_antie_vac<<energy<<"  "<<(prob_antie_vac)<<endl;
		outstd_antimu_vac<<energy<<"  "<<(prob_antimu_vac)<<endl;
		outstd_antitau_vac<<energy<<"  "<<(prob_antitau_vac)<<endl;

		outstd_antie_mat<<energy<<"  "<<(prob_antie_mat)<<endl;
		outstd_antimu_mat<<energy<<"  "<<(prob_antimu_mat)<<endl;
		outstd_antitau_mat<<energy<<"  "<<(prob_antitau_mat)<<endl;
	}

	outstde_vac.close();
	outstdmu_vac.close();
	outstdtau_vac.close();

	outstde_mat.close();
	outstdmu_mat.close();
	outstdtau_mat.close();

	outstd_antie_vac.close();
	outstd_antimu_vac.close();
	outstd_antitau_vac.close();

	outstd_antie_mat.close();
	outstd_antimu_mat.close();
	outstd_antitau_mat.close();

	glbFreeParams(true_values);
 	return 0;
}