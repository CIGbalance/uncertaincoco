/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UncertainSelect.h
 * Author: thehedgeify
 *
 * Created on 24 August 2018, 01:19
 */

#ifndef UNCERTAINSELECT_H
#define UNCERTAINSELECT_H

#include "UncertainSort.h"
#include <shark/Algorithms/DirectSearch/FitnessExtractor.h>
#include "TrueFitnessExtractor.h"
#include "UncertainEvaluator.h"
#include <map>
#include <iostream>
using namespace shark;

template<typename Indicator, class FitnessType, class Chromosome=RealVector>
class UncertainSelection{
    public:
        typedef std::vector<UncertainIndividual<FitnessType, Chromosome>*> Pop;
        typedef AbstractObjectiveFunction<RealVector, FitnessType> Function;

        
    void operator()(std::vector<UncertainIndividual<double>>& population, UncertainFunc_s const& f,
            std::size_t mu, SelectionStrategy strat, double alpha=-1){
        if(population.empty()) return;
        UncertainEvaluator<FitnessType> eval;
        UncertainSort<FitnessType> unSort;
        
        //Phase 1: uncertain Eval + Sort
        eval(f, population.begin(), population.end(), false); //Do uncertain evaluation for all population
        bool certain = false;
        unsigned int popSize = 0; //Initial popsize: everyone selected
        while(popSize != mu && !certain){ //While selection unclear
            unSort(population, strat, alpha); //compute uncertain Ranks
            /*for(unsigned int i=0; i<population.size(); i++){
                std::cout << UncertainIndividual<double>::toString(population[i]) << " " << population[i].rank() << std::endl;
            }*/
            
            std::pair<unsigned int,std::size_t> selected = selectionStep(population, mu); //Select best individuals according to rank
            popSize = selected.second;
            
                        
            if(popSize !=mu){ //only evaluate more if ranking unclear
                certain = true;
                for(unsigned int i=0; i<population.size(); i++){
                    if(population[i].rank()==selected.first
                            && UncertainIndividual<double, Chromosome>::IsUncertain(population[i])){//Individuals in critical rank
                        certain = false;
                        eval(f, population[i], true);  //force to evaluate true fitness
                    }
                }
                
            }
            
        }
        if(popSize==mu){ //We have selected the right number, no need for phase 2
            this->trueSelect(population, mu);
            return;
        }
        
        std::sort(population.begin(), population.end(), std_comp);
        for(unsigned int i=0; i<mu; i++){
            population[i].selected() = true;
        }
        for(unsigned int i=mu; i<population.size(); i++){
            population[i].selected() = false;
        }
    }
    
    template <class Individual>
    void operator()(std::vector<Individual>& population, UncertainFunc_m const& f,
            std::size_t mu, SelectionStrategy strat, double alpha=-1){
        if(population.empty()) return;
        UncertainEvaluator<FitnessType, Chromosome> eval;
        UncertainSort<FitnessType, Chromosome> unSort;
        //Phase 1: uncertain Eval + Sort
        eval(f, population.begin(), population.end(), false); //Do uncertain evaluation for all population
        unsigned int popSize = 0; //Initial popsize: everyone selected
        unsigned int critRank = 0; //critical rank
        bool certain = false; //Switches to true if all critical individuals are certain, go to indicator phase
        while(popSize != mu && !certain){ //While selection unclear
            unSort(population, strat, alpha); //compute uncertain Ranks
            /*for(unsigned int i=0; i<population.size(); i++){
                std::cout << UncertainIndividual<double>::toString(population[i]) << " " << population[i].rank() << std::endl;
            }*/
            
            std::pair<unsigned int,std::size_t> selected = selectionStep(population, mu); //Select best individuals according to rank
            popSize = selected.second;
            critRank = selected.first;
            
            if(popSize !=mu){ //only evaluate more if ranking unclear
                certain = true;
                for(unsigned int i=0; i<population.size(); i++){
                    if(population[i].rank()==critRank && UncertainIndividual<RealVector, Chromosome>::IsUncertain(population[i])){//Individuals in critical rank
                        certain = false;
                        eval(f, population[i], true);  //force to evaluate true fitness
                    }
                }   
            }
            //std::cout<< "select result: popsize" << popSize << " critRank " << critRank << " certain " << certain << std::endl;
        }
                
        if(popSize==mu){ //We have selected the right number, no need for phase 2
            this->trueSelect(population, mu);
            return;
        }
        
        //Phase 2: indicator sort
        //DO range check popsize > mu
        std::vector<UncertainIndividual<RealVector, Chromosome>> front;
        std::vector<int> individualIds;
        for(unsigned int i=0; i<population.size(); i++){
            if(population[i].rank()==critRank){//Individuals in critical rank
                front.push_back(population[i]);
                individualIds.push_back(i);
            }
        }
        std::cout.setstate(std::ios_base::failbit);
        m_indicator.updateInternals(FitnessExtractor(),population);
        while(popSize !=mu){
            std::size_t lc = m_indicator.leastContributor(FitnessExtractor(),front);
            population[individualIds[lc]].selected()=false;
            front.erase(front.begin() +lc);
            individualIds.erase(individualIds.begin()+lc);
            popSize--;
        }
        std::cout.clear();
        
        this->trueSelect(population, mu);
    }
    
    void trueSelect(std::vector<UncertainIndividual<double>>& population, std::size_t mu){
        UncertainSort<FitnessType> unSort;

        unSort.trueSort(population); //compute certain Ranks           
        trueSelectionStep(population, mu); //Select best individuals according to trueRank    
    }
        
    template <class Individual>   
    void trueSelect(std::vector<Individual>& population, std::size_t mu){
        UncertainSort<FitnessType, Chromosome> unSort;
        
        unSort.trueSort(population); //compute certain Ranks
        std::pair<unsigned int,std::size_t> selected = trueSelectionStep(population, mu); //Select best individuals according to trueRank    
        
        unsigned int popSize = selected.second;
        unsigned int critRank = selected.first;
        
        if(popSize==mu){ //We have selected the right number, no need for phase 2
            return;
        }
        
        //Phase 2: indicator sort
        //DO range check popsize > mu
        std::vector<Individual> front;
        std::vector<int> individualIds;
        for(unsigned int i=0; i<population.size(); i++){
            if(population[i].trueRank()==critRank){//Individuals in critical rank
                front.push_back(population[i]);
                individualIds.push_back(i);
            }
        }
        std::cout.setstate(std::ios_base::failbit);
        m_indicator.updateInternals(TrueFitnessExtractor(),population);
        while(popSize !=mu){
            std::size_t lc = m_indicator.leastContributor(TrueFitnessExtractor(),front);
            population[individualIds[lc]].trueSelected()=false;
            front.erase(front.begin() +lc);
            individualIds.erase(individualIds.begin()+lc);
            popSize--;
        }
        std::cout.clear();
    }
    
    
    protected:
        template <class Individual>
        std::pair<unsigned int,std::size_t> selectionStep(std::vector<Individual>& population, std::size_t mu){
            std::map<int, Pop> fronts;

            unsigned int maxRank=0;
            for(unsigned int i=0; i< population.size(); i++){
                if(fronts.find(population[i].rank())==fronts.end()){
                    fronts[population[i].rank()] = Pop();
                    if(population[i].rank()>maxRank){
                        maxRank = population[i].rank();
                    }
                }
                fronts[population[i].rank()].push_back(&population[i]);
                population[i].selected() = true;
            }

            //std::cout << "max Rank " << maxRank << std::endl;
            //deselect the highest rank fronts until we would end up with less or equal mu elements
            unsigned int rank = maxRank;
            std::size_t popSize = population.size();
            while(popSize-fronts[rank].size() >= mu){
                //deselect all elements in this front
                Pop front = fronts[rank];
                for(std::size_t i = 0; i != front.size(); ++i){
                        front[i]->selected() = false;
                        //std::cout << UncertainIndividual<FitnessType>::toString(*front[i]) << " " << front[i]->rank() << std::endl;
                }
                popSize -= front.size();
                --rank;
            }
            //delete &fronts;
            //delete &population;
            return std::make_pair(rank, popSize);
        }
    
    template <class Individual>
    std::pair<unsigned int,std::size_t> trueSelectionStep(std::vector<Individual>& population, std::size_t mu){
        std::map<int, Pop> fronts;

        unsigned int maxRank=0;
        for(unsigned int i=0; i< population.size(); i++){
            if(fronts.find(population[i].trueRank())==fronts.end()){
                fronts[population[i].trueRank()] = Pop();
                if(population[i].trueRank()>maxRank){
                    maxRank = population[i].trueRank();
                }
            }
            fronts[population[i].trueRank()].push_back(&population[i]);
            population[i].trueSelected() = true;
        }

        //std::cout << "max Rank " << maxRank << std::endl;
        //deselect the highest rank fronts until we would end up with less or equal mu elements
        unsigned int rank = maxRank;
        std::size_t popSize = population.size();
        while(popSize-fronts[rank].size() >= mu){
            //deselect all elements in this front
            Pop front = fronts[rank];
            for(std::size_t i = 0; i != front.size(); ++i){
                    front[i]->trueSelected() = false;
            }
            popSize -= front.size();
            --rank;
        }
        //delete &fronts;
        //delete &population;
        return std::make_pair(rank, popSize);
    }
    
    static bool std_comp(const UncertainIndividual<double> & A, const UncertainIndividual<double> & B){
        if(A.rank() == B.rank()){
            return A.penalizedFitness() < B.penalizedFitness();
        }else{
            return A.rank() < B.rank();    
        }    
    }
        
        Indicator m_indicator;
};


#endif /* UNCERTAINSELECT_H */

