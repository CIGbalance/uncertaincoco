/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   COCOfunc_s.h
 * Author: volz
 *
 * Created on 23. Juli 2018, 13:38
 */

#ifndef COCOFUNC_S_H
#define COCOFUNC_S_H

#include "../COCOinterface/COCOfunc.h"
#include "../utils/Archive.h"

#include <shark/ObjectiveFunctions/AbstractObjectiveFunction.h>

using namespace shark;
class COCOfunc_s : public virtual SingleObjectiveFunction, public virtual COCOfunc<SingleObjectiveFunction>{
public:
    COCOfunc_s(){
        m_features|=CAN_PROPOSE_STARTING_POINT;
        /*m_features|=IS_CONSTRAINED_FEATURE;
        m_features|=HAS_CONSTRAINT_HANDLER;*/
    }
    
    void init(coco_problem_t *problem, coco_problem_t * cheater_problem,
    int timeWindow, bool m_isNoisy, double penalty,  Transform trans){
        COCOfunc<SingleObjectiveFunction>::init(problem, cheater_problem, timeWindow, m_isNoisy, penalty, trans);
        if(this->m_numberOfObjectives!=1){
            throw std::invalid_argument("initialising single-objective function with multi-objective one");
        }
    }
    
    double eval( const SearchPointType & p) const{
        double* result = COCOfunc<SingleObjectiveFunction>::eval(p);
        double res = result[0];
        delete[] result;
        return res;
    }
    
    double cheater_eval( const SearchPointType & p) const{
        double* result = COCOfunc<SingleObjectiveFunction>::cheater_eval(p);
        double res = result[0];
        delete[] result;
        return res;
    }

};


#endif /* COCOFUNC_S_H */

