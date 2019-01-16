/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UncertainCMAIndividual.h
 * Author: thehedgeify
 *
 * Created on 26 August 2018, 00:13
 */

#ifndef UNCERTAINCMAINDIVIDUAL_H
#define UNCERTAINCMAINDIVIDUAL_H

#include <shark/Algorithms/DirectSearch/CMA/Chromosome.h>
#include <shark/Algorithms/DirectSearch/CMA/CMAIndividual.h>
#include <shark/Algorithms/DirectSearch/Individual.h>

#include "UncertainIndividual.h"

using namespace shark;
template<class FitnessTypeT>
class UncertainCMAIndividual : public UncertainIndividual<FitnessTypeT, CMAChromosome>{
    public:
        typedef FitnessTypeT FitnessType;     
        using UncertainIndividual<FitnessTypeT, CMAChromosome>::chromosome;
        using UncertainIndividual<FitnessTypeT, CMAChromosome>::searchPoint;        
        
        UncertainCMAIndividual():m_parent(0){
            typename UncertainIndividual<FitnessType>::UncertainIndividual();
        }

        UncertainCMAIndividual(
                std::size_t searchSpaceDimension,
                double successThreshold = 0.44,
                double initialStepSize = 1.0):m_parent(0){
            UncertainIndividual<FitnessType>::UncertainIndividual();
            chromosome() = CMAChromosome(searchSpaceDimension, successThreshold, initialStepSize);
        }
        void updateAsParent(CMAChromosome::IndividualSuccess offspringSuccess){
		chromosome().updateAsParent(offspringSuccess);
	}
	void updateAsOffspring(){
		chromosome().updateAsOffspring();
	}
	template<class RngType>
	void mutate(RngType& rng){
		chromosome().m_mutationDistribution.generate(
			rng, chromosome().m_lastStep,chromosome().m_lastZ
		);
		noalias(searchPoint()) += chromosome().m_stepSize * chromosome().m_lastStep;
	}
	
	double& noSuccessfulOffspring(){
		return chromosome().m_noSuccessfulOffspring;
	}
	
	double noSuccessfulOffspring()const{
		return chromosome().m_noSuccessfulOffspring;
	}
	
	std::size_t parent()const{
		return m_parent;
	}
	std::size_t& parent(){
		return m_parent;
	}
        
    private:
	std::size_t m_parent;
        
        
               /* static bool IsUncertain(UncertainCMAIndividual const& individual){
            if(individual.uncertainty().empty()){
                return true;
            }
            for(unsigned int i=0; i< individual.uncertainty().size(); i++){
                if(individual.uncertainty()[i]>0){
                    return true;
                }
            }
            return false;
        }
        static bool previouslyEvaluated(UncertainCMAIndividual const& individual){
            if(individual.uncertainty().empty()){
                return false;
            }else{
                return true;
            }
        }
        
        static bool previouslyRanked(UncertainCMAIndividual const& individual){
            if(individual.rank()==0){
                return false;
            }else{
                return true;
            }
        }
        
        static std::string toString(UncertainCMAIndividual<double> const& individual){
            std::string ind("Individual ( ");
            for(unsigned int i=0; i<individual.searchPoint().size(); i++){
                ind += std::to_string(individual.searchPoint()[i]) + " ";
            }
            ind+= ") " + std::to_string(individual.trueFitness());
            ind += " [" + std::to_string(individual.penalizedFitness()) + ","
                    + std::to_string(individual.uncertainty()[0]) + ","
                    + std::to_string(individual.hist()[0]) + "]";
            ind += " - " + std::to_string(individual.rank()) + "," + std::to_string(individual.trueRank())
                    + " sel" + std::to_string(individual.selected()) + "," + std::to_string(individual.trueSelected());
            return ind;
        }

        static std::string toString(UncertainCMAIndividual<RealVector> const& individual){
            std::string ind("Individual ( ");
            for(unsigned int i=0; i<individual.searchPoint().size(); i++){
                ind += std::to_string(individual.searchPoint()[i]) + " ";
            }
            ind+= ") ";
            for(unsigned int i=0; i< individual.unpenalizedFitness().size(); i++){
                ind+= std::to_string(individual.trueFitness()[i]);
                ind += " [" + std::to_string(individual.unpenalizedFitness()[i]) + ","
                    + std::to_string(individual.uncertainty()[i]) + ","
                    + std::to_string(individual.hist()[i]) + "] ";
            }
            ind += "- " + std::to_string(individual.rank()) + "," + std::to_string(individual.trueRank())
                    + " sel" + std::to_string(individual.selected()) + "," + std::to_string(individual.trueSelected());
            return ind;
        }*/
        
        
        /*std::vector<double> uncertainty() const {
            return m_uncertainty;
        }
        std::vector<double>& uncertainty() {
            return m_uncertainty;
        }
        
        std::vector<double> hist() const {
            return m_hist;
        }
        std::vector<double>& hist() {
            return m_hist;
        }
        
        FitnessType trueFitness() const{
            return m_trueFitness;
        }
        FitnessType& trueFitness(){
            return m_trueFitness;
        }
        
	unsigned int trueRank() const {
		return m_trueRank;
	}

	unsigned int& trueRank() {
		return m_trueRank;
	}
        
        bool trueSelected() const {
            return m_trueSelected;
	}

	bool& trueSelected() {
            return m_trueSelected;
	}
        
    
        void copyTo(UncertainIndividual<FitnessType>& ind){
            ind.uncertainty() = this->uncertainty();
            ind.hist() = this->hist();
            ind.trueFitness() = this->trueFitness();
            ind.trueRank() = this->trueRank();
            ind.trueSelected() = this->trueSelected();
            //ind.chromosome() = this->chromosome();
            ind.searchPoint() = this->searchPoint();
            ind.rank() = this->rank();
            ind.age() = this->age();
            ind.selected() = this->selected();
            ind.penalizedFitness() = this->penalizedFitness();
            ind.unpenalizedFitness() = this->unpenalizedFitness();
        }
        
        void copyTo(CMAIndividual<FitnessType>& ind){
            ind.chromosome() = this->chromosome();
            ind.searchPoint() = this->searchPoint();
            ind.rank() = this->rank();
            ind.age() = this->age();
            ind.selected() = this->selected();
            ind.penalizedFitness() = this->penalizedFitness();
            ind.unpenalizedFitness() = this->unpenalizedFitness();
        }
        
        void createFrom(UncertainIndividual<FitnessType>& ind){
            this->uncertainty() = ind.uncertainty();
            this->hist() = ind.hist();
            this->trueFitness() = ind.trueFitness();
            this->trueRank() = ind.trueRank();
            this->trueSelected() = ind.trueSelected();
            //this->chromosome() = ind.chromosome();
            this->searchPoint() = ind.searchPoint();
            this->rank() = ind.rank();
            this->age() = ind.age();
            this->selected() = ind.selected();
            this->penalizedFitness() = ind.penalizedFitness();
            this->unpenalizedFitness() = ind.unpenalizedFitness();
        }
        
        void createFrom(CMAIndividual<FitnessType>& ind){
            this->chromosome() = ind.chromosome();
            this->searchPoint() = ind.searchPoint();
            this->rank() = ind.rank();
            this->age() = ind.age();
            this->selected() = ind.selected();
            this->penalizedFitness() = ind.penalizedFitness();
            this->unpenalizedFitness() = ind.unpenalizedFitness();
        }
        
    
    protected:
        std::vector<double> m_uncertainty;
        std::vector<double> m_hist;
        FitnessType m_trueFitness;
        unsigned int m_trueRank;
        bool m_trueSelected;*/
    
    
};


#endif /* UNCERTAINCMAINDIVIDUAL_H */

