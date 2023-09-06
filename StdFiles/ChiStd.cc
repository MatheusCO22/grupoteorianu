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

int main(int argc, char *argv[])
{
    glbInit(argv[0]);
    glbInitExperiment(AEDLFILE, &glb_experiment_list[0], &glb_num_of_exps);

    ofstream outstdchi;
    string dat_dir = "../dat_files/";

   	outstdchi.open((dat_dir+"chi2.dat").c_str());

    double dm21 = 7.55e-5;
	double dm31 = 2.50e-3;
	double theta12 = asin(sqrt(0.320));
	double theta23 = asin(sqrt(0.547));
	double theta13 = asin(sqrt(0.02160));       //VARIAR
	double deltacp = -0.68 * M_PI;              //VARIAR

    double steps_theta13 = 50;
    double xmin_theta13 = -5;                  //theta13 = asin(sqrt(10^-x_theta13)) -5<x<-1 (?)
    double xmax_theta13 = -1;
    double x_theta13;

    double steps_deltacp = 50;
    double ymin_deltacp = -M_PI;
    double ymax_deltacp = +M_PI;
    double y_deltacp;
    
    double chi;

    glb_params true_values = glbAllocParams();

    glbDefineParams(true_values,theta12,theta13,theta23,deltacp,dm21,dm31);
	glbSetDensityParams(true_values, 1.0, GLB_ALL);
	glbSetOscillationParameters(true_values);
	glbSetRates();

    for(x_theta13 = xmin_theta13; x_theta13 <= xmax_theta13; x_theta13 += (xmax_theta13 - xmin_theta13)/steps_theta13){
        for(y_deltacp = ymin_deltacp; y_deltacp<= ymax_deltacp; y_deltacp+= (ymax_deltacp - ymin_deltacp)/steps_deltacp){
                            //const       const    variavel           variavel
            glbDefineParams(true_values, theta12, asin(sqrt(pow(10, x_theta13))), theta23, y_deltacp, dm21, dm31);
            chi = glbChiSys(true_values, -1, -1);

            outstdchi << pow(10, x_theta13) << "  " << y_deltacp << "  " << chi << endl;
        }
    }

    // for(x_theta13 = -4.0; x_theta13 < -1.0; x_theta13 += 3.0/50)
    // {
    //     for(y_deltacp = -M_PI; y_deltacp < M_PI; y_deltacp += 2 * M_PI/50)
    //     {
    //         /* Set parameters in vector of test values */
    //         glbSetOscParams(true_values, asin(sqrt(pow(10,x_theta13)))/2, GLB_THETA_13);
    //         glbSetOscParams(true_values, y_deltacp, GLB_DELTA_CP);
    //
    //         /* Compute Chi2 for all loaded experiments and all rules */
    //         chi = glbChiSys(true_values,GLB_ALL,GLB_ALL);
    //
    //         outstdchi << x_theta13 << "  " << y_deltacp << "  " << chi << endl;
    //         //AddToOutput(x,y,res);
    //     }
    // }

    outstdchi.close();

    glbFreeParams(true_values);
 	return 0;
}
