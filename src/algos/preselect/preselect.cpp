#include "preselect.h"
#include <iostream>

using namespace shark;

preselect::preselect(int krigSize, int histSize,
            AbstractPreselecter<RealVector> * moea, AbstractPreselecter<double> * soea,
            double uncertaintyThreshold, double historyThreshold,
            UncertainMode unMode, SampleMode sampMode, SurrMode surr){
    //Set Parameters here
    this->name = std::string("presel_k") + std::to_string(static_cast<long long>(krigSize)) + "h" + std::to_string(static_cast<long long>(histSize))+
        "ut";
    std::string str = std::to_string (static_cast<long double>(uncertaintyThreshold));
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
    this->name += str + "ht";
    str = std::to_string (static_cast<long double>(historyThreshold));
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
    this->name += str + "um" + std::to_string(static_cast<long long>(unMode)) +  "sm" +
            std::to_string(static_cast<long long>(sampMode)) + "s" + std::to_string(static_cast<long long>(surr));
    
    this->info= std::string("preselects");

    this->krigSize = krigSize;
    this->histSize = histSize;
    this->moea = moea;
    this->soea = soea;
    this->uncertaintyThreshold=uncertaintyThreshold;
    this->historyThreshold = historyThreshold;
    this->unMode = unMode;
    this->sampMode = sampMode;
    this->surr = surr;
}



void preselect::init(COCOfunc_s& function){  
    //ArchiveData initSample = function.lhs(this->lhsSize, this->lhsSeed,this->lhsDuplication);
    func_s.init(&function, this->krigSize*function.numberOfVariables(),
            this->histSize, this->uncertaintyThreshold, this->historyThreshold, this->unMode, this->sampMode, 1);
    soea->init(func_s);
}

void preselect::init(COCOfunc_s& function, COCOfunc_s& function2){  
    //ArchiveData initSample = function.lhs(this->lhsSize, this->lhsSeed,this->lhsDuplication);
    func_s.init(&function, &function2, this->surr, this->krigSize*function.numberOfVariables(),
            this->histSize, this->uncertaintyThreshold, this->historyThreshold, this->unMode, this->sampMode, 1);
    soea->init(func_s);
}

void preselect::init(COCOfunc_m& function){
    //ArchiveData initSample = function.lhs(this->lhsSize, this->lhsSeed,this->lhsDuplication);
    func_m.init(&function, this->krigSize*function.numberOfVariables(),
            this->histSize, this->uncertaintyThreshold, this->historyThreshold, this->unMode, this->sampMode,1);
    moea->init(func_m); 
}

void preselect::init(COCOfunc_m& function, COCOfunc_m& function2){
    //ArchiveData initSample = function.lhs(this->lhsSize, this->lhsSeed,this->lhsDuplication);
    func_m.init(&function, &function2, this->surr, this->krigSize*function.numberOfVariables(),
            this->histSize, this->uncertaintyThreshold, this->historyThreshold, this->unMode, this->sampMode,1);
    moea->init(func_m); 
}

void preselect::step(COCOfunc_s const& function){
    soea->step_presel(function, func_s); 
}

void preselect::step(COCOfunc_m const& function){
    moea->step_presel(function, func_m);
}

RealVector const& preselect::solution()const{
    //is never set because not needed
    throw "solution is not implemented yet.";
}

void preselect::spillBeans(){
    std::string file = getFolder() + "/presel.txt";
    func_m.getArchive().spillBeans(file);
    func_s.getArchive().spillBeans(file);
}

bool preselect::converged(COCOfunc_s const& function){
    return (function.converged());
}

bool preselect::converged(COCOfunc_m const& function){
    return (function.converged());
}
