/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef ABSPRE_H
#define ABSPRE_H
#include <shark/Algorithms/AbstractOptimizer.h>
#include <shark/Core/ResultSets.h>

using namespace shark;

template<class ResultType>
class AbstractPreselecter {
public:
    typedef AbstractOptimizer<RealVector,ResultType,ResultSet<RealVector, ResultType> > base_type;
    typedef typename base_type::ObjectiveFunctionType ObjectiveFunctionType;
    typedef typename base_type::SearchPointType SearchPointType;
    virtual void step_presel(ObjectiveFunctionType const& function,
            ObjectiveFunctionType const& function_pred) =0;
    virtual void init(ObjectiveFunctionType& function) =0;
};

#endif