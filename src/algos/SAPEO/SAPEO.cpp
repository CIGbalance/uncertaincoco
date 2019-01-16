#include "SAPEO.h"
#include <math.h>       /* isinf, sqrt */

SAPEO::SAPEO(int krigSize, int histSize,
        AbstractSAPEO* moea, SAPEOcma* soea, double uncertaintyThreshold,
        double historyThreshold, SelectionStrategy strat, double alpha,
        UncertainMode unMode, SampleMode sampMode, SurrMode surr){
        //Set Parameters here  
    this->name = std::string("SAPEO_k") + std::to_string(static_cast<long long>(krigSize)) + "h" + std::to_string(static_cast<long long>(histSize))+
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
    this->name += str + "um" + std::to_string(static_cast<long long>(unMode)) +  "sm" 
            + std::to_string(static_cast<long long>(sampMode)) + "s" + std::to_string (static_cast<long long>(surr));

    
    this->info= std::string("SAPEO");
    this->krigSize = krigSize;
    this->histSize = histSize;
    this->moea = moea;
    this->soea = soea;
    this->uncertaintyThreshold=uncertaintyThreshold;
    this->historyThreshold = historyThreshold;
    this->alpha = alpha;
    this->strat = strat;
    this->unMode = unMode;
    this->sampMode = sampMode;
    this->surr = surr;
}    

void SAPEO::init(COCOfunc_s& function){
    //function.lhs(this->lhsSize, this->lhsSeed, this->lhsDuplication);
    func_s.init(&function, this->krigSize*function.numberOfVariables(), this->histSize, this->uncertaintyThreshold,
            this->historyThreshold, this->unMode, this->sampMode);
    soea->init(func_s, getFolder(), this->strat, this->alpha);
}

void SAPEO::init(COCOfunc_s& function, COCOfunc_s& function2){
    //function.lhs(this->lhsSize, this->lhsSeed, this->lhsDuplication);
    func_s.init(&function, &function2, this->surr, this->krigSize*function.numberOfVariables(),
            this->histSize, this->uncertaintyThreshold,
            this->historyThreshold, this->unMode, this->sampMode);
    soea->init(func_s, getFolder(), this->strat, this->alpha);
}

void SAPEO::init(COCOfunc_m& function){
    //function.lhs(this->lhsSize, this->lhsSeed, this->lhsDuplication);
    func_m.init(&function, this->krigSize*function.numberOfVariables(), this->histSize, this->uncertaintyThreshold,
            this->historyThreshold, this->unMode, this->sampMode);
    moea->init(func_m, getFolder(), this->strat, this->alpha);
}

void SAPEO::init(COCOfunc_m& function, COCOfunc_m& function2){
    //function.lhs(this->lhsSize, this->lhsSeed, this->lhsDuplication);
    func_m.init(&function, &function2, this->surr, this->krigSize*function.numberOfVariables(),
            this->histSize, this->uncertaintyThreshold,
            this->historyThreshold, this->unMode, this->sampMode);
    moea->init(func_m, getFolder(), this->strat, this->alpha);
}


void SAPEO::step(const COCOfunc_s& function){
    soea->step(func_s);
}

void SAPEO::step(const COCOfunc_m& function){
    moea->step(func_m);
}

void SAPEO::spillBeans(){
    std::string file = getFolder() + "/Krig_pred_log.txt";
    func_m.getArchive().spillBeans(file);
    func_s.getArchive().spillBeans(file);
    if(func_s.getEvaluations()>0){//used func s, get recommendated points
        points[func_s.numberOfVariables()] = func_s.recommendedPoints();
        func_s.pointsClear();
    }
}

RealVector const& SAPEO::solution()const{
    //is never set because not needed
    throw "solution is not implemented yet.";
}


void SAPEO::laststep(COCOfunc_s const& function){
    bool conv = func_s.converged();
    if(!conv){
        soea->lastStep(func_s);    
    }
}

void SAPEO::laststep(COCOfunc_m const& function){
    bool conv = func_m.converged();
    if(!conv){
        moea->lastStep(func_m);    
    }
}

bool SAPEO::converged(COCOfunc_s const& function){
    bool conv = func_s.converged();
    if(conv){
        laststep(function);
    }
    return (func_s.converged());
}

bool SAPEO::converged(COCOfunc_m const& function){
    bool conv = func_m.converged();
    if(conv){
        laststep(function);
    }
    return (func_m.converged());
}

std::map<int, std::vector<RealVector>> SAPEO::getRecommendations(){
    return points;
}
