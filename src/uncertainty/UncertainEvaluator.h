/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UncertainEvaluator.h
 * Author: thehedgeify
 *
 * Created on 22 August 2018, 12:09
 */

#ifndef UNCERTAINEVALUATOR_H
#define UNCERTAINEVALUATOR_H

#include "UncertainIndividual.h"
#include "../functions/UncertainFunc_m.h"
#include "../functions/UncertainFunc_s.h"
#include "../functions/COCOfunc_s.h"
#include "../functions/COCOfunc_m.h"
#include "UncertainCMAIndividual.h"
#include <iostream>

using namespace shark;

template <class FitnessType, class Chromosome = RealVector>
class UncertainEvaluator{
public:
    typedef UncertainIndividual<FitnessType, Chromosome> IndividualType;
    template <class Function, class Individual>
    void operator()(Function const& f, Individual& individual, bool forceCertain=false) const{
        if(!f.isFeasible( individual.searchPoint() ) ) {
            throw std::invalid_argument("Uncertain evaluator only deals with feasible points");
        }
        if(!IndividualType::IsUncertain(individual)){
            //Individual was safely evaluated before, no need to do anything
            return;
        }
        if(forceCertain){
            FitnessType tmp = f.certainEval(individual.searchPoint());
            individual.unpenalizedFitness() = tmp;
            individual.penalizedFitness() = tmp;
            individual.uncertainty() = std::vector<double>(f.numberOfObjectives(), 0);
            individual.trueFitness() = tmp;
            individual.hist() = std::vector<double>(f.numberOfObjectives(), 0);  
        }else{
            FitnessType tmp = f.eval(individual.searchPoint());
            individual.unpenalizedFitness() = tmp;
            individual.penalizedFitness() = tmp;
            individual.uncertainty() = f.uncertainty(individual.searchPoint());
            individual.trueFitness() = f.cheaterEval(individual.searchPoint());
            individual.hist() = f.hist(individual.searchPoint());   
        }
    }
    
    
    template<class ObjectiveFunctionType, typename Iterator>
    void operator()(ObjectiveFunctionType const& f, Iterator begin, Iterator end, bool forceCertain=false) const{
        for(Iterator pos = begin; pos != end; ++pos){
            (*this)(f,*pos, forceCertain);
        }
    }
    
};

#endif /* UNCERTAINEVALUATOR_H */

