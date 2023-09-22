#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <globes/globes.h>
#include <fstream>

using namespace std;

#define GLB_SIGMA_E 6        /* Index of non-standard parameter sigma_E */

/* If filename given, write to file; for empty filename write to screen */
string dat_dir = "../dat_files/mod/";

/* Square of real number */
inline double square(double x)
{
    return x*x;
}


/***************************************************************************
 *     U S E R - D E F I N E D   P R O B A B I L I T Y   E N G I N E       *
 ***************************************************************************/

double th12;
double th13;
double th23;
double deltacp;
double sdm;
double ldm;
double sigma_E;


/***************************************************************************
 * Store oscillation parameters in internal data structures.               *
 * For more sophisticated probability engines, this would be the right     *
 * place to pre-compute the mixing matrix and parts of the Hamiltonian in  *
 * order to speed up the calls to the actual probability matrix function.  *
 ***************************************************************************/
int my_set_oscillation_parameters(glb_params p, void *user_data)
{
  th12    = glbGetOscParams(p, GLB_THETA_12);
  th13    = glbGetOscParams(p, GLB_THETA_13);
  th23    = glbGetOscParams(p, GLB_THETA_23);
  deltacp = glbGetOscParams(p, GLB_DELTA_CP);
  sdm     = glbGetOscParams(p, GLB_DM_21) * 1.0e-18;   // Convert to GeV^2
  ldm     = glbGetOscParams(p, GLB_DM_31) * 1.0e-18;   // Convert to GeV^2
  sigma_E = glbGetOscParams(p, GLB_SIGMA_E);

	th13 = asin(sqrt(0.02160));
  th12 = asin(sqrt(0.320));
	th23 = asin(sqrt(0.547));
	deltacp = -0.68 * M_PI;
	sdm = 7.55e-5 *1.0e-18;
	ldm = 2.50e-3 *1.0e-18;
  sigma_E = glbGetOscParams(p, GLB_SIGMA_E);
  
  return 0;
}


/***************************************************************************
 * Write oscillation parameters from internal data structures into p.      *
 ***************************************************************************/
int my_get_oscillation_parameters(glb_params p, void *user_data)
{
  /* glbSetOscParams(p, th12, GLB_THETA_12);
  glbSetOscParams(p, th13, GLB_THETA_13);
  glbSetOscParams(p, th23, GLB_THETA_23);
  glbSetOscParams(p, deltacp, GLB_DELTA_CP);
  glbSetOscParams(p, sdm*1.0e18, GLB_DM_21);  // Convert to eV^2
  glbSetOscParams(p, ldm*1.0e18, GLB_DM_31);  // Convert to eV^2
  glbSetOscParams(p, sigma_E, GLB_SIGMA_E); */

  glbSetOscParams(p, th12, GLB_THETA_12);
  glbSetOscParams(p, th13, GLB_THETA_13);
  glbSetOscParams(p, th23, GLB_THETA_23);
  glbSetOscParams(p, deltacp, GLB_DELTA_CP);
  glbSetOscParams(p, sdm*1.0e18, GLB_DM_21);
  glbSetOscParams(p, ldm*1.0e18, GLB_DM_31);
  glbSetOscParams(p, sigma_E, GLB_SIGMA_E);

  return 0;
}

int my_probability_matrix(double P[3][3], int cp_sign, double E, int psteps,
                          const double *length, const double *density,
                          double filter_sigma, void *user_data)
{
  int i, j;
  double L;
  double Delta21, Delta31, Delta32;
  double D21, D31, D32;
  double s13, c13, s12, c12;
  double t;
  
  /* Set all probabilities to zero initially */
  for (i=0; i < 3; i++)
    for (j=0; j < 3; j++)
      P[i][j] = 0.0;

  /* Calculate total baseline */
  L = 0.0;
  for (i=0; i < psteps; i++)
    L += length[i];
  L = GLB_KM_TO_EV(L) * 1.0e9;      /* Convert to GeV^{-1} */

  /* Compute P_ee */
  s12 = sin(th12);
  c12 = cos(th12);
  s13 = sin(th13);
  c13 = cos(th13);
  t = L / (4.0 * E);
  Delta21 = sdm * t;
  Delta31 = ldm * t;
  Delta32 = Delta31 - Delta21;
  t = M_SQRT2 * sigma_E / E;
  D21 = exp(-square( Delta21 * t ));
  D31 = exp(-square( Delta31 * t ));
  D32 = exp(-square( Delta32 * t ));

  P[0][0] = square(square(c13)) * ( 1 - 2.0*square(s12*c12)*(1 - D21*cos(2.0*Delta21)) )
               + 2.0*square(s13*c13) * ( D31*square(c12)*cos(2.0*Delta31)
                                       + D32*square(s12)*cos(2.0*Delta32) )
              + square(square(s13));
  // P[0][0] = 1; // teste

  return 0;
}


/***************************************************************************
 *                            M A I N   P R O G R A M                      *
 ***************************************************************************/

int main(int argc, char *argv[])
{
  //double thetheta13, x, y, res;
  
  /* Define standard oscillation parameters (cf. hep-ph/0405172v5) */
  double true_theta12 = asin(sqrt(0.3));
  double true_theta13 = 0.0;
  double true_theta23 = M_PI/4;
  double true_deltacp = 0.0;
  double true_sdm = 7.9e-5;
  double true_ldm = 2.6e-3;

  double energy;
  double emin= 0.25 ; //GeV
  double emax=10 ; //GeV
  double step= 3000;
  double probe_vac;

  /* Define one non-standard parameter sigma_E (wave packet energy spread
   * responsible for wave packet decoherence) */
  double true_sigma_E = 0.0;

  /* Initialize libglobes */
  glbInit(argv[0]);
 
  /* Register non-standard probability engine. This has to be done
   * before any calls to glbAllocParams() or glbAllocProjections() */
  glbRegisterProbabilityEngine(7,      /* Number of parameters */
                               &my_probability_matrix,
                               &my_set_oscillation_parameters,
                               &my_get_oscillation_parameters,
                               NULL);

  /* Initialize reactor experiment */
  //glbInitExperiment("Reactor1.glb",&glb_experiment_list[0],&glb_num_of_exps);
 
  /* Initialize parameter and projection vector(s) */
  glb_params true_values = glbAllocParams();
  glb_params test_values = glbAllocParams();

  glbDefineParams(true_values,true_theta12,true_theta13,true_theta23, true_deltacp,true_sdm,true_ldm);

  glbSetOscParams(true_values,true_sigma_E, GLB_SIGMA_E);   // Non-standard parameter
  glbSetDensityParams(true_values,1.0,GLB_ALL);

  glbDefineParams(test_values,true_theta12,true_theta13,true_theta23, true_deltacp,true_sdm,true_ldm);
                              
  glbSetOscParams(test_values,true_sigma_E, GLB_SIGMA_E);   // Non-standard parameter 
  glbSetDensityParams(test_values,1.0,GLB_ALL);

  /* The simulated data are computed */
  glbSetOscillationParameters(true_values);
  glbSetOscillationParameters(test_values);
  glbSetRates();

  string dat_dir = "../dat_files/mod/";

  ofstream outstde_teste;
  outstde_teste.open((dat_dir+"probability_e_e_vac.dat").c_str());

  printf("TH12: %f\n", th12);
  printf("TH13: %f\n", th13);
  printf("TH23: %f\n", th23);
  printf("Dcp: %f\n", deltacp);
  printf("sdm: %f\n", sdm);
  printf("ldm: %f\n", ldm);
  printf("sigma_E: %f\n", sigma_E);

  [[maybe_unused]] double L = 1300;// km

    for (energy=emin;energy<=emax;energy+=(emax-emin)/step)
	  {
        glbSetOscillationParameters(test_values);
        probe_vac = glbVacuumProbability(1,1,+1,energy,L);	//eletron para eletron

        outstde_teste <<energy<<"  "<< probe_vac<<endl;
    }
  
  /* Destroy parameter and projection vector(s) */
  glbFreeParams(true_values);
  glbFreeParams(test_values);
    
  exit(0);
}

