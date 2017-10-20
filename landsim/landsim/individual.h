#ifndef _INDIVIDUAL_H_
#define _INDIVIDUAL_H_

#include "landscape.h"

class TSimulator;

class TIndividual
{
 public:
         TIndividual(TSimulator*, TCell&);
         THomeRange& GetHomeRange() {return homerange;}
         const THomeRange& GetHomeRange() const {return homerange;}
         unsigned int GetAge() {return age;}
         bool ApplyMortality();
         void ApplyBreeding(TPopulation& popjuv);
         bool HasEmptyHomeRange() {return homerange.empty();}
         void OutputHomeRange(ofstream&);
         void SettleHomeRange();
 private:
         unsigned int age;
         int offspring;
         THomeRange homerange;
         TCell hrcenter;
         TCell hrcentermother;
         TSimulator* simulator;
         void CalculateOffspring();
};

int iround(double x);

#endif
