/****************************************************************************
*                        Project: LandSim.exe                               *
*                        Version: 1.1                                       *
*                        File: mathmain.cpp                                 *
*                        Henrique Pereira, 2002                             *                       *
****************************************************************************/

#include "wstp.h"
#pragma hdrstop
#include "simulator.h"

/* 
 Our model takes as inputs the model parameters and a two dimensional matrix with the landscape.
   It produces a file with a matrix containing the home ranges of each individual at each time step 
 and a matrix with the population age structure at each time step. 
   To apply the model to other species and landscape configurations, use the values of the
 parameters specific to the species or population of interest, and a matrix with the desired
 landscape configuration.
*/



// CSimulPop is the function called from Mathematica, with the respective parameters

//---------------------------------------------------------------------------
void CSimulPop(
               int nsteps,
               int initpopulation,
               int hrsize,
               double birthrate,
               int breedingage,
               double survival,
               double distanceweight,
               double dispersaldistance,
               int dispersalmode,
               double sinkavoidance,
               double neighavoidance,
               double sinkmortality,
               const char* filename)
{

 /* 
    nsteps: Number of time steps
    initpopulation: Initial population size
    hrsize: Home range size
    birthrate: Fecundity (bi)
    breedingage: Age at first breeding
    survival: Survival probability (s)
    distanceweight: parameter of the weight function
        the weight function gives a higher contribution to cells closer to the home range centroid
        not used in the simulations for assessing population persistence in landscapes fragmented by roads, since hrsize (Home range size) was 1 cell in all simulations
    dispersaldistance: Dispersal distance (d)
    dispersalmode:
        case 0: Global dispersal
        case 1: Local dispersal, habitat search in a local kernel
        case 2: Local dispersal, (biased) random walk
        only case 2 was used in the simulations for assessing population persistence in landscapes fragmented by roads
        see landscape.cpp
    sinkavoidance: Road avoidance (pRA)
    neighavoidance: probability of avoiding dispersing through a cell that is already occupied
        set to 1 in all simulations performed for assessing persistence in landscapes fragmented by roads
    sinkmortality: Road mortality (mRM) 
  */

 // Any time the list of these parameters is changed, one needs to update landsim.tm and run wsprep
    

 // The landscape is a two dimensional array and has to be obtained manually from the Mathematica call with WSGetRealArray
 double* land;
 long* dims;
 char** heads;
 long d;
 WSGetRealArray(stdlink, &land, &dims, &heads, &d);
    
    
 // Stores the simulation parameters in the object param
 TSimParam param;
 param.land = new Mat_DP(land,dims[0],dims[1]);
        // stores the landscape passed from Mathematica with dims[0] rows and dims[1] columns
 param.initpopulation = initpopulation;
 param.nsteps = nsteps;
 param.hrsize = hrsize;
 param.birthrate = birthrate;
 param.breedingage = breedingage;
 param.survival = survival;
 param.distanceweight = distanceweight;
 param.dispersaldistance = dispersaldistance;
 param.dispersalmode = dispersalmode;
 param.sinkavoidance = sinkavoidance;
 param.neighavoidance = neighavoidance;
 param.sinkmortality = sinkmortality;
 param.filename = filename;

 TSimulator simulator(param);  // creates and starts simulation
 valarray<int> popsizehist(nsteps + 1); // vector which stores population sizes at each time step

 popsizehist[0]=simulator.GetPopulationSize(); // stores first value of population size
 for (int i=1; i<=nsteps; i++)
   {
   simulator.Step();  // executes a step of the simulation
   popsizehist[i]=simulator.GetPopulationSize(); // stores the value of population size
   }
 WSPutIntegerList(stdlink, &(popsizehist[0]), nsteps + 1);  // returns a vector of population sizes
}

int main(int argc, char* argv[])
{
    return WSMain(argc, argv);
}

//*********************** End of main.cpp ***********************************


