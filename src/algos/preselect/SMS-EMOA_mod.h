#ifndef SHARK_ALGORITHMS_DIRECT_SEARCH_SMS_EMOA_MOD_H
#define SHARK_ALGORITHMS_DIRECT_SEARCH_SMS_EMOA_MOD_H

// MOO specific stuff
#include <shark/Algorithms/DirectSearch/SMS-EMOA.h>
#include "../preselect/AbstractPreselecter.h"
#include <iostream>
#include "../../COCOinterface/COCOfunc.h"

using namespace shark;


class SMSEMOA_mod : public SMSEMOA, public AbstractPreselecter<RealVector>{
public:
    SMSEMOA_mod(DefaultRngType& rng = Rng::globalRng):SMSEMOA::SMSEMOA(rng){
        pe_rng =&rng;
        m_mu_ps = 5;
    }
    
    unsigned int mu_ps()const{
        return m_mu_ps;
    }

    unsigned int& mu_ps(){
        return m_mu_ps;
    }
    

    virtual void init( 
        SMSEMOA::ObjectiveFunctionType& function
    ){
        SMSEMOA::init(function);
    }
     
    virtual void init( 
        SMSEMOA::ObjectiveFunctionType& function, 
        std::vector<SMSEMOA::SearchPointType> const& initialSearchPoints
    ){
        checkFeatures(function);
        std::vector<RealVector> values(initialSearchPoints.size());
        for(std::size_t i = 0; i != initialSearchPoints.size(); ++i){
                if(!function.isFeasible(initialSearchPoints[i]))
                        throw SHARKEXCEPTION("[SMS-EMOA::init] starting point(s) not feasible");
                values[i] = function.eval(initialSearchPoints[i]);
        }

        std::size_t dim = function.numberOfVariables();
        RealVector lowerBounds(dim, -1E20);
        RealVector upperBounds(dim, 1E20);
        COCOfunc<MultiObjectiveFunction> * test = dynamic_cast<COCOfunc<MultiObjectiveFunction> *>(&function);
        if(test!=NULL){
            const double * lb = test->getLowerBounds();
            const double * ub = test->getUpperBounds();
            for(unsigned int i=0; i<dim; i++){
                lowerBounds(i) = lb[i];
                upperBounds(i) = ub[i];
            }
        }else{
            if (function.hasConstraintHandler() && function.getConstraintHandler().isBoxConstrained()) {
                typedef BoxConstraintHandler<SMSEMOA::SearchPointType> ConstraintHandler;
                ConstraintHandler  const& handler = static_cast<ConstraintHandler const&>(function.getConstraintHandler());

                lowerBounds = handler.lower();
                upperBounds = handler.upper();
            } else{
                throw SHARKEXCEPTION("[SMS-EMOA::init] Algorithm does only allow box constraints");
            }   
        }
        SMSEMOA::doInit(initialSearchPoints,values,lowerBounds, upperBounds,mu(),nm(),nc(),crossoverProbability());
        m_mu_ps = mu_ps();
    }
    

    
    
    void step_presel(SMSEMOA::ObjectiveFunctionType const& function, SMSEMOA::ObjectiveFunctionType const& function_pred ) {
        std::vector<IndividualType> offspring(mu_ps());
        std::vector<IndividualType> final_offspring(1);
        PenalizingEvaluator penalizingEvaluator;
        
        for(unsigned int i=0; i<mu_ps(); i++){
            offspring[i] = generateOffspring()[0];
        }
	penalizingEvaluator( function_pred, offspring.begin(), offspring.end() );
        pe_selection( offspring, 1);
        for(unsigned int i=0; i<mu_ps(); i++){
            if(offspring[i].selected()){
                final_offspring[0] = offspring[i];
                break;
            }
        }
        penalizingEvaluator(function, final_offspring.begin(), final_offspring.end());
        updatePopulation(final_offspring);
    }    

private:
    unsigned int m_mu_ps;    
    DefaultRngType* pe_rng;
    IndicatorBasedSelection<HypervolumeIndicator> pe_selection; ///< Selection operator relying on the (contributing) hypervolume indicator.
};


#endif
