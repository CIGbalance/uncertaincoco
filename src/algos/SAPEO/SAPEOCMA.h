/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SAPEOCMA.h
 * Author: thehedgeify
 *
 * Created on 26 August 2018, 00:28
 */

#ifndef SAPEOCMA_H
#define SAPEOCMA_H
#include <shark/Algorithms/DirectSearch/CMA.h>
#include <shark/Algorithms/DirectSearch/Indicators/HypervolumeIndicator.h>

#include <fstream>
#include <iostream>

#include "../../uncertainty/UncertainSelect.h"
#include "../../uncertainty/UncertainEvaluator.h"

using namespace shark;
class SAPEOcma : public CMA{
public:
    void init(UncertainFunc_s & function, std::string folder, SelectionStrategy strat, double alpha=-1){
        CMA::init(function, function.proposeStartingPoint());
        this->strat = strat;
        this->alpha = alpha;
        this->folder = folder;
        this->file = folder + "/SAPEOcma.txt";
    }
    
    void step(UncertainFunc_s const& function){
        std::vector< CMA::IndividualType > cmaOffspring = CMA::generateOffspring();
        std::vector<IndividualType> offspring(cmaOffspring.size());
        for(unsigned int i=0; i<offspring.size(); i++){
            offspring[i].createFrom(cmaOffspring[i]);
        }
        updatePopulation(offspring, function);
    }
    
    std::string name() const {
        return "SAPEO CMA-ES";
    }
    
    void lastStep(UncertainFunc_s const& function){
        std::vector< CMA::IndividualType > cmaOffspring = CMA::generateOffspring();
        std::vector<IndividualType> offspring(cmaOffspring.size());
        for(unsigned int i=0; i<offspring.size(); i++){
            offspring[i].createFrom(cmaOffspring[i]);
        }
        UncertainEvaluator<double, RealVector> eval;
	eval( function, offspring.begin(), offspring.end(), true);
    }
        
       
    
protected:
    typedef UncertainIndividual<double> IndividualType;
    void updatePopulation(std::vector<IndividualType>& offspring, UncertainFunc_s const& function){
        std::vector<CMA::IndividualType> cmaOffspring(CMA::mu());
        UncertainSelection<HypervolumeIndicator, double> selector;
        selector(offspring, function, CMA::mu(), this->strat, this->alpha);
        std::ofstream log;
        log.open(this->file, std::ios::app);
        
        int counter = 0;
        for(unsigned int i=0; i<offspring.size(); i++){
            log << IndividualType::toString(offspring[i]) << std::endl;
            if(offspring[i].selected()){
                offspring[i].copyTo(cmaOffspring[counter]);
                counter++;
            }
        }
        log << std::endl;
        log.close();
        
        std::sort(cmaOffspring.begin(), cmaOffspring.end(), std_comp);
        //From here just regular CMA stuff
        
        CMA::updatePopulationActual(cmaOffspring);
    }
private:
    static bool std_comp(const CMA::IndividualType & A, const CMA::IndividualType & B){
        if(A.rank() == B.rank()){
            return A.penalizedFitness() < B.penalizedFitness();
        }else{
            return A.rank() < B.rank();    
        }    
    }
    
    SelectionStrategy strat;
    double alpha;
    std::string folder;
    std::string file;
    
};

#endif /* SAPEOCMA_H */

