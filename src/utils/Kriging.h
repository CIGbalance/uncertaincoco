/* 
 * File:   Kriging.h
 * Author: vv
 *
 * Created on June 17, 2018, 7:30 PM
 */

#ifndef KRIGING_H
#define KRIGING_H

#include "Archive.h"
#include "gp.h"
//#include "../../build/gp.h"
//#include <libgp/gp.h>
//#include <shark/Algorithms/Trainers/RegularizationNetworkTrainer.h> //Trains Gaussian Process Model
//#include <shark/Models/Kernels/KernelExpansion.h> //Model Type
//#include <shark/Models/Kernels/LinearKernel.h>

using namespace libgp;
using namespace shark;

class Kriging{
public:
    Kriging();
    Kriging(ArchiveData data, int n, int m, int holdOut=-1);
    void init(ArchiveData data, int n, int m, int holdOut);
    std::vector<std::pair<double,double>> predict(const RealVector point);
    static std::vector<double> getHistory(RealVector point, int h, int k, Archive arch, int nObj, SampleMode mode);
    std::vector<double> expectedImprovement(RealVector point);
    std::vector<double> probImprovement(RealVector point);
    std::vector<double> lowerBound(RealVector point);
private:
    ArchiveData data;
    std::vector<GaussianProcess> gp;
    int m_numberOfVariables;
    int m_numberOfObjectives;
    std::vector<double> fmin;


};


#endif /* KRIGING_H */

