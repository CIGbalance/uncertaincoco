/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   COCOfunc_m.h
 * Author: thehedgeify
 *
 * Created on 31 July 2018, 15:51
 */

#ifndef COCOFUNC_M_H
#define COCOFUNC_M_H

#include "../COCOinterface/COCOfunc.h"
#include <shark/ObjectiveFunctions/AbstractObjectiveFunction.h>
#include <iostream>

using namespace shark;

class COCOfunc_m : public virtual MultiObjectiveFunction, public virtual COCOfunc<MultiObjectiveFunction>{
public:
    COCOfunc_m(){
        m_features|=CAN_PROPOSE_STARTING_POINT;
        m_features|=HAS_VALUE;
    }

    void init(coco_problem_t *problem, coco_problem_t * cheater_problem, int timeWindow,
    bool m_isNoisy, double penalty, Transform trans){
        COCOfunc<MultiObjectiveFunction>::init(problem, cheater_problem, timeWindow, m_isNoisy, penalty, trans);
        if(this->m_numberOfObjectives==1){
            throw std::invalid_argument("initialising multi-objective function with single-objective one");
        }
    }
    
    RealVector eval( const SearchPointType & p) const{
        double* result = COCOfunc<MultiObjectiveFunction>::eval(p);
        RealVector value(this->numberOfObjectives());
        for(unsigned int i=0; i<this->numberOfObjectives(); i++){
            value(i) = result[i];
        }
        delete[] result;
        return value;
    }
    

};

#endif /* COCOFUNC_M_H */

