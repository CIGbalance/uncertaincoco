/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SAPEOMOCMA.h
 * Author: thehedgeify
 *
 * Created on 27 August 2018, 00:07
 */

#ifndef SAPEOMOCMA_H
#define SAPEOMOCMA_H

#include <shark/Algorithms/DirectSearch/MOCMA.h>
#include <iostream>

#include "../../uncertainty/UncertainCMAIndividual.h"
#include "../../uncertainty/UncertainIndividual.h"
#include "../../uncertainty/SafeComparator.h"
#include "../../uncertainty/UncertainSelect.h"
#include "AbstractSAPEO.h"

using namespace shark;

template<typename Indicator=HypervolumeIndicator>
class SAPEOmocma :  public AbstractMultiObjectiveOptimizer<RealVector >, public AbstractSAPEO{
    public:
        
        SAPEOmocma(DefaultRngType& rng= Rng::globalRng):mpe_rng(&rng) {
            this->m_features |= AbstractMultiObjectiveOptimizer<RealVector >::CAN_SOLVE_CONSTRAINED;
	}
       
        typedef UncertainCMAIndividual<RealVector> IndividualType;
        
        std::size_t mu()const{
            return m_mu;
	}
	std::size_t& mu(){
            return m_mu;
	}
	
	double initialSigma()const{
            return m_initialSigma;
	}
	double& initialSigma(){
            return m_initialSigma;
	}
	
        void init(UncertainFunc_m& function, std::string folder, SelectionStrategy strat, double alpha=-1){
            m_individualSuccessThreshold = 0.44;
            initialSigma() = 1.0;
            mu() = 100;
            this->strat = strat;
            this->alpha = alpha;
            this->folder = folder;
            this->file = folder + "/SAPEOmocma.txt";
            checkFeatures(function);
            if(!function.canProposeStartingPoint())
                throw SHARKEXCEPTION( "Objective function does not propose a starting point");
            m_parents.resize( mu() );
            for(std::size_t i = 0; i != mu(); ++i){
                m_parents[i].chromosome() = CMAChromosome(function.numberOfVariables(), m_individualSuccessThreshold, m_initialSigma);
                m_parents[i].searchPoint() = function.proposeStartingPoint();
                m_parents[i].uncertainty().clear();
            }
            doInit(mu(),initialSigma(),function);
        }

        void step( UncertainFunc_m const& function ) {
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
            m_parents.insert(m_parents.end(),offspring.begin(),offspring.end());          
            UncertainEvaluator<RealVector, CMAChromosome> eval;
            eval( function, m_parents.begin(), m_parents.end(), true);
        }

        

    protected:
        
        void doInit(
		std::size_t mu,
		double initialSigma,
                UncertainFunc_m& function
	){
            checkFeatures(function);
            //if the number of supplied points is smaller than mu, fill everything in

            UncertainEvaluator<RealVector, CMAChromosome> eval;
            eval(function, m_parents.begin(), m_parents.end(), true);
            
            //m_selection( m_parents, function, mu, this->strat, this->alpha, true );

            std::ofstream log;
            log.open(this->file, std::ios::app);
            for(unsigned int i=0; i<mu; i++){
                log << "Init " << i << ": " << UncertainCMAIndividual<RealVector>::toString(m_parents[i]) << std::endl;
            }
            log.close();
        }
            
        
        std::vector<IndividualType> generateOffspring()const{
            std::vector<IndividualType> offspring(mu());
            for(std::size_t i = 0; i != mu(); ++i){
                std::size_t parentId = i;
                offspring[i].chromosome() = m_parents[parentId].chromosome();
                offspring[i].searchPoint() = m_parents[parentId].searchPoint();
                offspring[i].mutate(*mpe_rng);
                offspring[i].parent() = parentId;

            }
            return offspring;
	}
        
        
        void updatePopulation(std::vector<IndividualType>& offspring, UncertainFunc_m const& function){
            m_parents.insert(m_parents.end(),offspring.begin(),offspring.end());          
            m_selection(m_parents, function, mu(), this->strat, this->alpha);

            std::ofstream log;
            log.open(this->file, std::ios::app);
            log  << std::endl;
            for(unsigned int i=0; i<2*mu(); i++){
                log << UncertainCMAIndividual<RealVector>::toString(m_parents[i]) << std::endl;
            }
            log.close();
            
             //determine from the selection which parent-offspring pair has been successful
            for (std::size_t i = 0; i < mu(); i++) {
                CMAChromosome::IndividualSuccess offspringSuccess = CMAChromosome::Unsuccessful;
                //new update: an offspring is successful if it is selected
                if (m_parents[mu()+i].selected()) {
                    m_parents[mu()+i].updateAsOffspring();
                    offspringSuccess = CMAChromosome::Successful;
                }
                if(m_parents[i].selected()) 
                    m_parents[i].updateAsParent(offspringSuccess);
            }            
            //partition the selected individuals to the front and remove the unselected ones
            std::partition(m_parents.begin(), m_parents.end(),IndividualType::IsSelected);
            m_parents.erase(m_parents.begin()+mu(),m_parents.end());
            
	}
        
        std::string name() const {
            return "SAPEO MO-CMA-ES";
	}
	
        
    private:
        std::vector<IndividualType> m_parents; ///< Population of size \f$\mu + 1\f$.
        SelectionStrategy strat;
        double alpha;
        std::string folder;
        std::string file;
        
        std::size_t m_mu; ///< Size of parent population
	
        UncertainSelection<Indicator, RealVector, CMAChromosome> m_selection; ///< Selection operator relying on the (contributing) hypervolume indicator.
	
	double m_individualSuccessThreshold;
	double m_initialSigma;
	DefaultRngType* mpe_rng;

};

#endif /* SAPEOMOCMA_H */

