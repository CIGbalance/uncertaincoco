/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SAPEO.h
 * Author: thehedgeify
 *
 * Created on 22 August 2018, 10:50
 */

#ifndef SAPEO_H
#define SAPEO_H

#include "../CocoOptimiser.h"
#include "../../functions/UncertainFunc_s.h"
#include "../../functions/UncertainFunc_m.h"
#include "SAPEOCMA.h"
#include "../../uncertainty/SafeComparator.h"
#include "AbstractSAPEO.h"

#include <shark/Algorithms/AbstractMultiObjectiveOptimizer.h>
#include <shark/Algorithms/AbstractSingleObjectiveOptimizer.h>
#include <map>


class SAPEO : public CocoOptimiser{
public:       
    SAPEO(int krigSize, int histSize,
        AbstractSAPEO * moea, SAPEOcma * soea,
        double uncertaintyThreshold, double historyThreshold,
        SelectionStrategy strat, double alpha,
        UncertainMode unMode, SampleMode sampMode, SurrMode surr=LOC);
    void init(COCOfunc_s& function);
    void step(COCOfunc_s const& function);
    void init(COCOfunc_m& function);
    void step(COCOfunc_m const& function);
    SolutionType const& solution()const;
    void spillBeans();
    bool converged(COCOfunc_s const& function);
    bool converged(COCOfunc_m const& function);
    std::map<int, std::vector<RealVector>> getRecommendations();
    
    void laststep(COCOfunc_s const& function);
    void laststep(COCOfunc_m const& function);
    
    void init(COCOfunc_s& function, COCOfunc_s& function2);   
    void init(COCOfunc_m& function, COCOfunc_m& function2);
private:

    int histSize;
    int krigSize;
    std::string folder;
    AbstractSAPEO * moea;
    SAPEOcma * soea;
    
    UncertainFunc_m func_m;
    UncertainFunc_s func_s;
    double uncertaintyThreshold;
    double historyThreshold;
    
    SelectionStrategy strat;
    UncertainMode unMode;
    SampleMode sampMode;
    double alpha;
    SurrMode surr;
    
    std::map<int, std::vector<RealVector>> points;
};


#endif /* SAPEO_H */

