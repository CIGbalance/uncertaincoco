/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UncertainIndividual.h
 * Author: thehedgeify
 *
 * Created on 22 August 2018, 11:52
 */

#ifndef UNCERTAININDIVIDUAL_H
#define UNCERTAININDIVIDUAL_H

#include <shark/Algorithms/DirectSearch/Individual.h>

using namespace shark;
template< class FitnessTypeT, class Chromosome = RealVector > 
class UncertainIndividual: public Individual<RealVector, FitnessTypeT, Chromosome> {
    public:
        typedef shark::Individual<RealVector, FitnessTypeT, Chromosome> IndividualType;
        static bool IsUncertain(UncertainIndividual const& individual){
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
        static bool previouslyEvaluated(UncertainIndividual const& individual){
            if(individual.uncertainty().empty()){
                return false;
            }else{
                return true;
            }
        }
        
        static bool previouslyRanked(UncertainIndividual const& individual){
            if(individual.rank()==0){
                return false;
            }else{
                return true;
            }
        }
        
        static bool TrueSelected(UncertainIndividual const& individual){
            return individual.trueSelected();
	}
        
      
        
        static std::string toString(UncertainIndividual<double, Chromosome> const& individual){
            std::string ind("Individual ( ");
            for(unsigned int i=0; i<individual.searchPoint().size(); i++){
                ind += std::to_string(static_cast<long double>(individual.searchPoint()[i])) + " ";
            }
            ind+= ") " + std::to_string(static_cast<long double>(individual.trueFitness()));
            ind += " [" + std::to_string(static_cast<long double>(individual.penalizedFitness())) + ","
                    + std::to_string(static_cast<long double>(individual.uncertainty()[0])) + ","
                    + std::to_string(static_cast<long double>(individual.hist()[0])) + "]";
            ind += " - " + std::to_string(static_cast<long long>(individual.rank())) + "," + std::to_string(static_cast<long long>(individual.trueRank()))
                    + " sel" + std::to_string(static_cast<long long>(individual.selected())) + "," + std::to_string(static_cast<long long>(individual.trueSelected()));
            return ind;
        }

        static std::string toString(UncertainIndividual<RealVector, Chromosome> const& individual){
            std::string ind("Individual ( ");
            for(unsigned int i=0; i<individual.searchPoint().size(); i++){
                ind += std::to_string(static_cast<long double>(individual.searchPoint()[i])) + " ";
            }
            ind+= ") ";
            for(unsigned int i=0; i< individual.unpenalizedFitness().size(); i++){
                ind+= std::to_string(static_cast<long double>(individual.trueFitness()[i]));
                ind += " [" + std::to_string(static_cast<long double>(individual.unpenalizedFitness()[i])) + ","
                    + std::to_string(static_cast<long double>(individual.uncertainty()[i])) + ","
                    + std::to_string(static_cast<long double>(individual.hist()[i])) + "] ";
            }
            ind += "- " + std::to_string(static_cast<long long>(individual.rank())) + "," + std::to_string(static_cast<long long>(individual.trueRank()))
                    + " sel" + std::to_string(static_cast<long long>(individual.selected())) + "," + std::to_string(static_cast<long long>(individual.trueSelected()));
            return ind;
        }
        

        UncertainIndividual(): m_trueRank(0), m_trueSelected(false){
            typename Individual<RealVector, FitnessTypeT, Chromosome>::Individual();
        }
        
        UncertainIndividual(RealVector point, FitnessTypeT fitness):m_trueRank(0), m_trueSelected(false){
            typename Individual<RealVector, FitnessTypeT, Chromosome>::Individual();
            Individual<RealVector, FitnessTypeT, Chromosome>::searchPoint() = point;
            trueFitness() = fitness;
        }

        std::vector<double> uncertainty() const {
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
        
        FitnessTypeT trueFitness() const{
            return m_trueFitness;
        }
        FitnessTypeT& trueFitness(){
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
        
        
        void copyTo(IndividualType& ind){
            ind.chromosome() = this->chromosome();
            ind.searchPoint() = this->searchPoint();
            ind.rank() = this->rank();
            ind.age() = this->age();
            ind.selected() = this->selected();
            ind.penalizedFitness() = this->penalizedFitness();
            ind.unpenalizedFitness() = this->unpenalizedFitness();
        }
        
        void createFrom(IndividualType& ind){
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
        FitnessTypeT m_trueFitness;
        unsigned int m_trueRank;
        bool m_trueSelected;
};

#endif /* UNCERTAININDIVIDUAL_H */

