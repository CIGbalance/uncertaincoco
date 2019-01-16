/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrueFitnessExtractor.h
 * Author: thehedgeify
 *
 * Created on 25 August 2018, 22:21
 */

#ifndef TRUEFITNESSEXTRACTOR_H
#define TRUEFITNESSEXTRACTOR_H

#include <shark/LinAlg/Base.h>
#include "UncertainIndividual.h"
#include "UncertainCMAIndividual.h"
using namespace shark;

struct TrueFitnessExtractor {
    template <class FitnessTypeT, class Chromosome=RealVector>
    FitnessTypeT operator()( UncertainIndividual<FitnessTypeT, Chromosome> const& individual ) const {
        return individual.trueFitness();
    }

};


#endif /* TRUEFITNESSEXTRACTOR_H */

