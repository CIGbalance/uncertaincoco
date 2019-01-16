#include "EGO.h"

using namespace shark;

EGO::EGO(int lhsSize, int lhsSeed, int lhsDuplication, int krigSize, int histSize, int infillBudget,
            AbstractMultiObjectiveOptimizer<RealVector > * moea, AbstractSingleObjectiveOptimizer<RealVector > * soea,
            UncertainMode unMode, SampleMode sampMode){
    //Set Parameters here
    this->name = std::string("EGO_lhs") + std::to_string(lhsSize)+ "-" + std::to_string(lhsSeed) + "-" +
            std::to_string(lhsDuplication) + "k" + std::to_string(krigSize) + "h" + std::to_string(histSize)+
            "i" + std::to_string(infillBudget) + "um" + std::to_string(static_cast<long long>(unMode)) + 
            "sm" + std::to_string(static_cast<long long>(sampMode));
    this->info= std::string("EGO");
    this->lhsSize = lhsSize;
    this->lhsSeed = lhsSeed;
    this->lhsDuplication = lhsDuplication;
    this->krigSize = krigSize;
    this->histSize = histSize;
    this->infillBudget = infillBudget;
    this->unMode = unMode;
    this->sampMode = sampMode;
    this->moea = moea;
    this->soea = soea;
}

void EGO::init(COCOfunc_s& function){
    ArchiveData initSample = function.lhs(this->lhsSize*function.numberOfVariables(), this->lhsSeed,this->lhsDuplication);
    func_s.init(&function, this->krigSize*function.numberOfVariables(),
        this->histSize, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
        this->unMode, this->sampMode);
}

void EGO::init(COCOfunc_m& function){
    ArchiveData initSample = function.lhs(this->lhsSize*function.numberOfVariables(), this->lhsSeed,this->lhsDuplication);
    func_m.init(&function, this->krigSize*function.numberOfVariables(),
        this->histSize, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
        this->unMode, this->sampMode);
}

void EGO::step(COCOfunc_s const& function){
    soea->init(func_s);
    int startEvals = func_s.getEvaluations();
    while(func_s.getEvaluations() -startEvals <=  this->infillBudget*((int) function.numberOfVariables())  && !func_s.converged()){
        soea->step( func_s );
    }
    function.eval(soea->solution().point);//Evaluate best solution found   
    func_s.signalArchiveReset();
}

void EGO::step(COCOfunc_m const& function){
    moea->init(func_m);
    
    int startEvals = func_s.getEvaluations();
    std::cout.setstate(std::ios_base::failbit);
    while(func_m.getEvaluations() -startEvals <= this->infillBudget*((int) function.numberOfVariables()) && !func_m.converged()){
        moea->step( func_m );
    }
    std::cout.clear();
    //Evaluate best solution found
    for( std::size_t i = 0; i < moea->solution().size(); i++ ) {
        function.eval(moea->solution()[ i ].point);
    }
    func_m.signalArchiveReset();

}

RealVector const& EGO::solution()const{
    //is never set because not needed
    throw "solution is not implemented yet.";
}

void EGO::spillBeans(){
    //std::string file = this->folder + "/ego.txt";
    //func_s.getArchive().spillBeans(file);
    //func_m.getArchive().spillBeans(file);

}


bool EGO::converged(COCOfunc_s const& function){
    return (function.converged());
}

bool EGO::converged(COCOfunc_m const& function){
    return (function.converged());
}

