/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EGOSO.h
 * Author: vv
 *
 * Created on June 15, 2018, 5:04 PM
 */

#ifndef EGO_H
#define EGO_H


#include "../CocoOptimiser.h"
#include "../../functions/UncertainFunc_s.h"
#include "../../functions/UncertainFunc_m.h"
#include <shark/Algorithms/AbstractMultiObjectiveOptimizer.h>
#include <shark/Algorithms/AbstractSingleObjectiveOptimizer.h>


class EGO : public CocoOptimiser{
public:
    EGO(int lhsSize, int lhsSeed, int lhsDuplication, int krigSize, int histSize, int infillBudget,
            AbstractMultiObjectiveOptimizer<RealVector > * moea, AbstractSingleObjectiveOptimizer<RealVector > * soea,
            UncertainMode unMode, SampleMode sampMode);
    void init(COCOfunc_s& function);
    void step(COCOfunc_s const& function);
    void init(COCOfunc_m& function);
    void step(COCOfunc_m const& function);
    SolutionType const& solution()const;
    void spillBeans();
    bool converged(COCOfunc_s const& function);
    bool converged(COCOfunc_m const& function);
    
    void laststep(COCOfunc_s const& function){}
    void laststep(COCOfunc_m const& function){}
    
    void init(COCOfunc_s& function, COCOfunc_s& function2){
        throw std::invalid_argument("no double functions for EGO");
    }
    void init(COCOfunc_m& function, COCOfunc_m& function2){
        throw std::invalid_argument("no double functions for EGO");
    }
private:
    int lhsSize;
    int lhsSeed;
    int lhsDuplication;
    int krigSize;
    int histSize;
    int infillBudget;
    UncertainMode unMode;
    SampleMode sampMode;
    AbstractMultiObjectiveOptimizer<RealVector > * moea;
    AbstractSingleObjectiveOptimizer<RealVector > * soea;
    
    UncertainFunc_m func_m;
    UncertainFunc_s func_s;
};


#endif /* EGO_H */

