#include "simulator.h"

int main (int argc, char * const argv[]) {

		TSimParam param;
		
		param.land = new Mat_DP(.5,10,10);
		cout << "initpopulation:\n";
		cin >> param.initpopulation;
		cout << "nsteps:\n";
		cin >> param.nsteps;
		cout << "hrsize:\n";
		cin >> param.hrsize;
		cout << "birthrate:\n";
		cin >> param.birthrate;
		cout << "breedingage:\n";
		cin >> param.breedingage;
		cout << "survival or maximum age:\n";
		cin >> param.survival;
		cout << "distanceweight:\n";
		cin >> param.distanceweight;
		cout << "filename:\n";
		cin >> param.filename;
		
		
		TSimulator simulator(param);
		
		for (int i=0; i<simulator.GetNSteps(); i++)
			simulator.Step();
		
		return 0;	
}
