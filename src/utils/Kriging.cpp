#include "Kriging.h"
#include "gp.h"
#include "gp_utils.h"

#include <Eigen/Dense>
#include <boost/math/distributions/normal.hpp>
#include <utility>      // std::pair
#include <iostream>
#include <math.h>

using namespace libgp;
using namespace shark;
using boost::math::normal;

#include <iomanip>
  using std::setw; using std::setprecision;
#include <limits>
  using std::numeric_limits;

Kriging::Kriging(){
      
}
  
Kriging::Kriging(ArchiveData data, int n, int m, int holdOut){
      this->init(data, n, m, holdOut);
}
  
void Kriging::init(ArchiveData data, int n, int m, int holdOut){
    this->data = data;
    this->m_numberOfVariables = n;
    this->m_numberOfObjectives = m;
    this->fmin = std::vector<double>(this->m_numberOfObjectives);
    this->gp = std::vector<GaussianProcess>(this->m_numberOfObjectives);
    for(int i=0; i<this->m_numberOfObjectives; i++){
        this->gp[i].setup(this->m_numberOfVariables, "CovSum ( CovSEiso, CovNoise)");
        // initialize hyper parameter vector
        Eigen::VectorXd params(this->gp[i].covf().get_param_dim());
        params << 0.0, 0.0, -2.0;
        // set parameters of covariance function
        this->gp[i].covf().set_loghyper(params);
        fmin[i] = std::numeric_limits<double>::max();
    }
    // add training patterns
    int counter=0;
    for(auto element: this->data.elements()) {
        if(counter!=holdOut){
           DataPair<RealVector, RealVector> tmp = element;
           double x[this->m_numberOfVariables];
           for(int i=0; i<this->m_numberOfVariables; i++){
               x[i] = tmp.input(i);
           }
           double y =0;
           for(int i=0; i<this->m_numberOfObjectives; i++){
               y = tmp.label(i);
               gp[i].add_pattern(x, y);
               if(fmin[i]>y){
                   fmin[i] = y;
               }
           }   
        }
        counter++;
    }
}

std::vector<std::pair<double,double>> Kriging::predict(const RealVector point){
    double x[point.size()];
    for(unsigned int i=0; i<point.size(); i++){
        x[i] = point(i);
    }
    std::vector<std::pair<double, double>> result(this->m_numberOfObjectives);
    for(int i=0; i<this->m_numberOfObjectives; i++){
        double pred = gp[i].f(x);
        double var = gp[i].var(x);
        result[i] =  std::make_pair(pred, var);
    }
    return result;
}

std::vector<double> Kriging::getHistory(RealVector point, int h, int k, Archive arch, int nObj, SampleMode mode){
    ArchiveData dat = arch.getSample(h, CLOSE, point);
    std::vector<double> output(nObj, 0.0);
    int counter = 0;
    for(auto element: dat.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        RealVector neighbour = tmp.input;
        Kriging krig(arch.getSample(k, mode, neighbour), point.size(), nObj, counter);
        std::vector<std::pair<double,double>> result = krig.predict(neighbour);
        for(int i=0; i<nObj; i++){
//            std::cout << neighbour(0) << " " << neighbour(1) << " " << result[i].first <<" " <<
 //                   result[i].second << " " << tmp.label << fabs(tmp.label(i)-result[i].first) << " " << fabs(tmp.label(i)-result[i].first)/sqrt(result[i].second ) << std::endl;
            output[i]+= fabs(tmp.label(i)-result[i].first)/sqrt(result[i].second );
        }
        counter++;
    }
    for(int i=0; i< nObj; i++){
        output[i]/=h;
    }
    return output;
}

std::vector<double> Kriging::expectedImprovement(RealVector point){
    std::vector<std::pair<double,double>> result = this->predict(point);
    std::vector<double> output(this->m_numberOfObjectives);
    normal nd;
    for(int i=0; i<this->m_numberOfObjectives; i++){
        output[i] = (this->fmin[i]-result[i].first)*cdf(nd, (this->fmin[i]-result[i].first)/result[i].second)+
                result[i].second*pdf(nd, (this->fmin[i]-result[i].first)/result[i].second);
    }
    return output;
}

std::vector<double> Kriging::probImprovement(RealVector point){
    std::vector<std::pair<double,double>> result = this->predict(point);
    std::vector<double> output(this->m_numberOfObjectives);
    normal nd;
    for(int i=0; i<this->m_numberOfObjectives; i++){
        output[i] = cdf(nd, (this->fmin[i]-result[i].first)/result[i].second);
    }
    return output;
}

std::vector<double> Kriging::lowerBound(RealVector point){
    std::vector<std::pair<double,double>> result = this->predict(point);
    std::vector<double> output(this->m_numberOfObjectives);
    for(int i=0; i<this->m_numberOfObjectives; i++){
        output[i] = result[i].first - 2*result[i].second;
    }
    return output;
}