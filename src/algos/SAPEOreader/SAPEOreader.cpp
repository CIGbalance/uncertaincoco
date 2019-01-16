/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "SAPEOreader.h"


SAPEOreader::SAPEOreader(int krigSize, int histSize,
            double uncertaintyThreshold, double historyThreshold,
            SelectionStrategy strat, double alpha,
            UncertainMode unMode, SampleMode sampMode, 
            std::map<int, std::vector<RealVector>> points,
            SurrMode surr){
    this->name = std::string("SAPEOreader_k") + std::to_string(static_cast<long long>(krigSize)) + "h" + std::to_string(static_cast<long long>(histSize))+
        "ut";
    std::string str = std::to_string (static_cast<long double>(uncertaintyThreshold));
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
    this->name += str + "ht";
    str = std::to_string (static_cast<long double>(historyThreshold));
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
    this->name += str + "sel" + std::to_string(static_cast<long long>(strat)) + "a";
    str = std::to_string (static_cast<long double>(alpha));
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
    this->name += str + "um" + std::to_string(static_cast<long long>(unMode)) +
            "sm" + std::to_string(static_cast<long long>(sampMode)) + "s" + std::to_string (static_cast<long double>(surr));
    this->points = points;
}

void SAPEOreader::init(COCOfunc_s& function){
    searchPoints = points[function.numberOfVariables()];
}
void SAPEOreader::init(COCOfunc_m& function){
    searchPoints = points[function.numberOfVariables()];
}

void SAPEOreader::step(const COCOfunc_m& function){
    for(unsigned int i=0; i<searchPoints.size(); i++){
        function.eval(searchPoints[i]);
    }
}

void SAPEOreader::step(const COCOfunc_s& function){
    for(unsigned int i=0; i<searchPoints.size(); i++){
        function.eval(searchPoints[i]);
    }
}

void SAPEOreader::spillBeans(){
}

RealVector const& SAPEOreader::solution()const{
    //is never set because not needed
    throw "solution is not implemented yet.";
}

bool SAPEOreader::converged(COCOfunc_s const& function){
    return function.getEvaluations()>0;
}

bool SAPEOreader::converged(COCOfunc_m const& function){
    return function.getEvaluations()>0;
}
