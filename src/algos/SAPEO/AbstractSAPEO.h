/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractSAPEO.h
 * Author: thehedgeify
 *
 * Created on 31 August 2018, 13:20
 */

#ifndef ABSTRACTSAPEO_H
#define ABSTRACTSAPEO_H

#include <shark/Algorithms/AbstractOptimizer.h>
#include <shark/Core/ResultSets.h>

#include "../../uncertainty/SafeComparator.h"
#include "../../functions/UncertainFunc_m.h"


using namespace shark;

class AbstractSAPEO {
public:
    virtual void init(UncertainFunc_m& function, std::string folder, SelectionStrategy strat, double alpha=-1) =0;
    virtual void step(UncertainFunc_m const& function) = 0;
    virtual void lastStep(UncertainFunc_m const& function) = 0;
};


#endif /* ABSTRACTSAPEO_H */

