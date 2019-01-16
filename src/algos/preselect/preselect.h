/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   preCMA.h
 * Author: vv
 *
 * Created on June 17, 2018, 7:10 PM
 */

#ifndef PRESEL_H
#define PRESEL_H

#include "../../utils/Archive.h"
#include "../CocoOptimiser.h"
#include "../../COCOinterface/COCOfunc.h"
#include "../../functions/COCOfunc_s.h"
#include "../../functions/COCOfunc_m.h"
#include "../../functions/UncertainFunc_s.h"
#include "../../functions/UncertainFunc_m.h"
#include "AbstractPreselecter.h"

#include <shark/Algorithms/AbstractMultiObjectiveOptimizer.h>
#include <shark/Algorithms/AbstractSingleObjectiveOptimizer.h>


class preselect : public CocoOptimiser{
public:
    preselect(int krigSize, int histSize,
            AbstractPreselecter<RealVector> * moea, AbstractPreselecter<double> * soea,
            double uncertaintyThreshold, double historyThreshold,
            UncertainMode unMode, SampleMode sampMode, SurrMode surr=LOC);
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
    
    void init(COCOfunc_s& function, COCOfunc_s& function2);   
    void init(COCOfunc_m& function, COCOfunc_m& function2);
private:
    int seed;
    UncertainFunc_s func_s;
    UncertainFunc_m func_m;
    int krigSize;
    int histSize;
    AbstractPreselecter<RealVector> * moea;
    AbstractPreselecter<double> * soea;
    double uncertaintyThreshold;
    double historyThreshold;
    UncertainMode unMode;
    SampleMode sampMode;
    SurrMode surr;
};

#endif /* PRESEL_H */

