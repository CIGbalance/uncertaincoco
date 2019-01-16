#include "RandomSearch.h"
#include <random>

using namespace shark;

RandomSearch::RandomSearch(int granularity){
    //Set Parameters here
    this->name = std::string("RS");
    this->info= std::string("RandomSearch");
    this->granularity = granularity;
}

void RandomSearch::init(COCOfunc_s& function){
}

void RandomSearch::init(COCOfunc_m& function){
}

void RandomSearch::step(COCOfunc_s const& function){
    RealVector point(function.numberOfVariables());
    const double * max = function.getUpperBounds();
    const double * min = function.getLowerBounds();
    for(unsigned int i=0; i<function.numberOfVariables(); i++){
        point(i) = (max[i]-min[i])*(rand()%granularity)/granularity+min[i];
    }
    function.eval(point);
}

void RandomSearch::step(COCOfunc_m const& function){
    RealVector point(function.numberOfVariables());
    const double * max = function.getUpperBounds();
    const double * min = function.getLowerBounds();
    for(unsigned int i=0; i<function.numberOfVariables(); i++){
        point(i) = (max[i]-min[i])*(rand()%granularity)/granularity+min[i];
    }
    RealVector result = function.eval(point);
}

RealVector const& RandomSearch::solution()const{
    //is never set because not needed
    throw "solution is not implemented yet.";
}


bool RandomSearch::converged(COCOfunc_s const& function){
    return false;
}

bool RandomSearch::converged(COCOfunc_m const& function){
    return false;
}