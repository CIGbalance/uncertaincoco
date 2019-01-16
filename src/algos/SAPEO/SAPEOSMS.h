/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SAPEOSMS.h
 * Author: thehedgeify
 *
 * Created on 29 August 2018, 15:37
 */

#ifndef SAPEOSMS_H
#define SAPEOSMS_H

#include "../../uncertainty/UncertainIndividual.h"
#include "../../uncertainty/SafeComparator.h"
#include "../../uncertainty/UncertainSelect.h"
#include "AbstractSAPEO.h"

#include <shark/Algorithms/DirectSearch/Indicators/HypervolumeIndicator.h>
#include <shark/Algorithms/DirectSearch/Operators/Selection/TournamentSelection.h>
#include <shark/Algorithms/DirectSearch/Operators/Recombination/SimulatedBinaryCrossover.h>
#include <shark/Algorithms/DirectSearch/Operators/Mutation/PolynomialMutation.h>
#include <shark/Algorithms/AbstractMultiObjectiveOptimizer.h>


using namespace shark;
class SAPEOSMS: public AbstractMultiObjectiveOptimizer<RealVector >, public AbstractSAPEO{
public:
    SAPEOSMS(DefaultRngType& rng = Rng::globalRng):mpe_rng(&rng) {
        this->m_features |= AbstractMultiObjectiveOptimizer<RealVector >::CAN_SOLVE_CONSTRAINED;
    }
    
    double crossoverProbability()const{
            return m_crossoverProbability;
    }

    double nm()const{
            return m_mutator.m_nm;
    }

    double nc()const{
            return m_crossover.m_nc;
    }

    unsigned int mu()const{
            return m_mu;
    }

    unsigned int& mu(){
            return m_mu;
    }
    
    typedef UncertainIndividual<RealVector> IndividualType;

    std::string name() const {
        return "SAPEO SMS-EMOA";
    }
    
    void init(UncertainFunc_m& function, std::string folder, SelectionStrategy strat, double alpha=-1){
        m_mu = 100;
        m_mutator.m_nm = 20.0;
        m_crossover.m_nc = 20.0;
        m_crossoverProbability = 0.9;
        this->strat = strat;
        this->alpha = alpha;
        this->folder = folder;
        this->file = folder + "/SAPEOsms.txt";
        checkFeatures(function);
        if(!function.canProposeStartingPoint())
            throw SHARKEXCEPTION( "Objective function does not propose a starting point");
        m_parents.resize( mu() );
        for(std::size_t i = 0; i != mu(); ++i){
            m_parents[i].searchPoint() = function.proposeStartingPoint();
            m_parents[i].uncertainty().clear();

        }
        init(function);
    }
    
    virtual void step( UncertainFunc_m const& function ) {
        std::vector<IndividualType> offspring = generateOffspring();
        updatePopulation(offspring, function);
    }
    
    void step(ObjectiveFunctionType const & function){
        throw SHARKEXCEPTION( "only usable for UncertainFunc_m");
    }
    
    using AbstractMultiObjectiveOptimizer<RealVector >::init;
    void init( 
		ObjectiveFunctionType& function, 
		std::vector<SearchPointType> const& initialSearchPoints
	){
        throw SHARKEXCEPTION( "only usable for UncertainFunc_m");
    }
    void lastStep(UncertainFunc_m const& function){
        std::vector<IndividualType> offspring = generateOffspring();
        m_parents.push_back(offspring[0]);
        UncertainEvaluator<RealVector> eval;
        eval(function, m_parents.begin(), m_parents.end(), true);
        
    }

protected:
    virtual void init(UncertainFunc_m& function){
        checkFeatures(function);
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
                typedef BoxConstraintHandler<RealVector> ConstraintHandler;
                ConstraintHandler  const& handler = static_cast<ConstraintHandler const&>(function.getConstraintHandler());

                lowerBounds = handler.lower();
                upperBounds = handler.upper();
            } else{
                throw SHARKEXCEPTION("[SMS-EMOA::init] Algorithm does only allow box constraints");
            }   
        }
        doInit(lowerBounds, upperBounds,mu(),nm(),nc(),crossoverProbability(), function);
    }

    void doInit(RealVector const& lowerBounds,
		RealVector const& upperBounds,
		std::size_t mu,
		double nm,
		double nc,
		double crossover_prob,
                UncertainFunc_m& function){
        //if the number of supplied points is smaller than mu, fill everything in

        UncertainEvaluator<RealVector> eval;
        eval(function, m_parents.begin(), m_parents.end(), true);
        m_selection( m_parents, function, mu, this->strat, this->alpha );
        
        /*std::ofstream log;
        log.open(this->file, std::ios::app);
        for(unsigned int i=0; i<mu; i++){
            log << "Init " << i << ": " << UncertainIndividual<RealVector>::toString(m_parents[i]) << std::endl;
        }
        log.close();*/

        m_crossover.init(lowerBounds,upperBounds);
        m_mutator.init(lowerBounds,upperBounds);
    }
    
    void updatePopulation(  std::vector<IndividualType> const& offspring, UncertainFunc_m const& function) {
        m_parents.push_back(offspring[0]);
        //std::ofstream log;
        //log.open(this->file, std::ios::app);
        m_selection( m_parents, function, mu(), this->strat, this->alpha );
        
        //log << "Child " << UncertainIndividual<RealVector>::toString(m_parents.back()) << std::endl;
        
        
        //if the individual got selected, insert it into the parent population
        if(m_parents.back().selected()){
            for(std::size_t i = 0; i != mu(); ++i){
                if(!m_parents[i].selected()){
                    //log << "Replaced " << UncertainIndividual<RealVector>::toString(m_parents[i]) << std::endl;
                    m_parents[i] = m_parents.back();
                    break;
                }
            }
        }
        
        m_parents.pop_back();
        //log.close();
    }
    
    std::vector<IndividualType> generateOffspring()const{
        std::vector<IndividualType> offspring(1);
        offspring[0].searchPoint() = createOffspring(m_parents.begin(),m_parents.begin()+mu());
        return offspring;
    }
    
    
    RealVector createOffspring(
            std::vector<IndividualType>::const_iterator begin,
            std::vector<IndividualType>::const_iterator end
    )const{
        TournamentSelection< IndividualType::RankOrdering > selection;

        IndividualType mate1( *selection(*mpe_rng, begin, end ) );
        IndividualType mate2( *selection(*mpe_rng, begin, end) );

        if( coinToss(*mpe_rng, m_crossoverProbability ) ) {
            m_crossover(*mpe_rng, mate1, mate2 );
        }

        if( coinToss(*mpe_rng,0.5) ) {
            m_mutator(*mpe_rng, mate1 );
            return mate1.searchPoint();
        } else {
            m_mutator(*mpe_rng, mate2 );
            return mate2.searchPoint();
        }
    }
    
    std::vector<IndividualType> m_parents; ///< Population of size \f$\mu + 1\f$.
private:
    SelectionStrategy strat;
    double alpha;
    std::string folder;
    std::string file;
    
    unsigned int m_mu; ///< Size of parent generation

    UncertainSelection<HypervolumeIndicator, RealVector> m_selection; ///< Selection operator relying on the (contributing) hypervolume indicator.
    SimulatedBinaryCrossover< RealVector > m_crossover; ///< Crossover operator.
    PolynomialMutator m_mutator; ///< Mutation operator.

    double m_crossoverProbability; ///< Crossover probability.
    DefaultRngType* mpe_rng;
    
};

#endif /* SAPEOSMS_H */

