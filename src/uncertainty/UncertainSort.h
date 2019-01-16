/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UncertainSort.h
 * Author: thehedgeify
 *
 * Created on 23 August 2018, 23:06
 */

#ifndef UNCERTAINSORT_H
#define UNCERTAINSORT_H

#include <shark/Algorithms/DirectSearch/FitnessExtractor.h>
#include "UncertainIndividual.h"
#include "TrueFitnessExtractor.h"
#include "SafeComparator.h"
using namespace shark;

template<class FitnessType, class Chromosome=RealVector>
struct UncertainSort {

    /**
     * \brief Executes the algorithm.
     *
     * Afterwards every individual is assigned a rank by pop[i].rank() = fronNumber.
     * The front of dominating points has the value 1.
     *
     * \param pop [in,out] Population to subdivide into fronts of non-dominated individuals.
     */
    template <class Individual>
    void trueSort(std::vector<Individual> &pop){//true evaluation

        //dominance relation
        SafeComparator<TrueFitnessExtractor, FitnessType, Chromosome> pdc(PREDICTION);

        //stors for the i-th point which points are dominated by i
        std::vector<std::vector<std::size_t> > s(pop.size());
        //stores for every point how many points are dominating it
        std::vector<std::size_t> numberOfDominatingPoints(pop.size(), 0);
        //stores initially the front of non-dominated points
        std::vector<std::size_t> front;

        for (std::size_t i = 0; i < pop.size(); i++) {
                //check which points j are dominated by i and add them to s[i]
                //also increment n[j] for every i dominating j
                for (std::size_t  j = 0; j < pop.size(); j++) {
                        if (i == j)
                                continue;

                        int domination = pdc(pop[i], pop[j]);
                        if ( domination > 1)//pop[i]> pop[j]
                                s[i].push_back(j);
                        else if (domination < -1)//pop[i]< pop[j]
                                numberOfDominatingPoints[i]++;
                }
                //all non-dominated points form the first front
                if (numberOfDominatingPoints[i] == 0){
                        front.push_back(i);
                        pop[i].trueRank() =1;
                }
        }
        //find subsequent fronts.
        unsigned frontCounter = 2;
        std::vector<std::size_t> nextFront;

        //as long as we can find fronts
        //visit all points of the last front found and remove them from the
        //set. All points which are not dominated anymore form the next front
        while (!front.empty()) {
                //visit all points of the current front and remove them
                // if any point is not dominated, it is part the next front.
                for(std::size_t element = 0; element != front.size(); ++element) {
                        //visit all points dominated by the element
                        std::vector<std::size_t> const& dominatedPoints = s[front[element]];
                        for (std::size_t  j = 0; j != dominatedPoints.size(); ++j){
                                std::size_t point = dominatedPoints[j];
                                numberOfDominatingPoints[point]--;
                                // if no more points are dominating this, add to the next front.
                                if (numberOfDominatingPoints[point] == 0){
                                        nextFront.push_back(point);
                                        pop[point].trueRank() = frontCounter;
                                }
                        }
                }

                //make the new found front the current
                front.swap(nextFront);
                nextFront.clear();
                frontCounter++;
        }        
    }
    
    /*void trueSort(std::vector<UncertainCMAIndividual<FitnessType>> &pop){//true evaluation

        //dominance relation
        SafeComparator<TrueFitnessExtractor, FitnessType> pdc(PREDICTION);

        //stors for the i-th point which points are dominated by i
        std::vector<std::vector<std::size_t> > s(pop.size());
        //stores for every point how many points are dominating it
        std::vector<std::size_t> numberOfDominatingPoints(pop.size(), 0);
        //stores initially the front of non-dominated points
        std::vector<std::size_t> front;

        for (std::size_t i = 0; i < pop.size(); i++) {
                //check which points j are dominated by i and add them to s[i]
                //also increment n[j] for every i dominating j
                for (std::size_t  j = 0; j < pop.size(); j++) {
                        if (i == j)
                                continue;

                        int domination = pdc(pop[i], pop[j]);
                        if ( domination > 1)//pop[i]> pop[j]
                                s[i].push_back(j);
                        else if (domination < -1)//pop[i]< pop[j]
                                numberOfDominatingPoints[i]++;
                }
                //all non-dominated points form the first front
                if (numberOfDominatingPoints[i] == 0){
                        front.push_back(i);
                        pop[i].trueRank() =1;
                }
        }
        //find subsequent fronts.
        unsigned frontCounter = 2;
        std::vector<std::size_t> nextFront;

        //as long as we can find fronts
        //visit all points of the last front found and remove them from the
        //set. All points which are not dominated anymore form the next front
        while (!front.empty()) {
                //visit all points of the current front and remove them
                // if any point is not dominated, it is part the next front.
                for(std::size_t element = 0; element != front.size(); ++element) {
                        //visit all points dominated by the element
                        std::vector<std::size_t> const& dominatedPoints = s[front[element]];
                        for (std::size_t  j = 0; j != dominatedPoints.size(); ++j){
                                std::size_t point = dominatedPoints[j];
                                numberOfDominatingPoints[point]--;
                                // if no more points are dominating this, add to the next front.
                                if (numberOfDominatingPoints[point] == 0){
                                        nextFront.push_back(point);
                                        pop[point].trueRank() = frontCounter;
                                }
                        }
                }

                //make the new found front the current
                front.swap(nextFront);
                nextFront.clear();
                frontCounter++;
        }        
    }*/
    
    template <class Individual>
    void operator()(std::vector<Individual> &pop, SelectionStrategy strategy, double alpha=-1) {

        //dominance relation
        SafeComparator<FitnessExtractor, FitnessType, Chromosome> pdc(strategy, alpha);

        //stors for the i-th point which points are dominated by i
        std::vector<std::vector<std::size_t> > s(pop.size());
        //stores for every point how many points are dominating it
        std::vector<std::size_t> numberOfDominatingPoints(pop.size(), 0);
        //stores initially the front of non-dominated points
        std::vector<std::size_t> front;

        for (std::size_t i = 0; i < pop.size(); i++) {
                //check which points j are dominated by i and add them to s[i]
                //also increment n[j] for every i dominating j
                for (std::size_t  j = 0; j < pop.size(); j++) {
                        if (i == j)
                                continue;

                        int domination = pdc(pop[i], pop[j]);
                        if ( domination > 1)//pop[i]> pop[j]
                                s[i].push_back(j);
                        else if (domination < -1)//pop[i]< pop[j]
                                numberOfDominatingPoints[i]++;
                }
                //all non-dominated points form the first front
                if (numberOfDominatingPoints[i] == 0){
                        front.push_back(i);
                        pop[i].rank() = 1;
                }
        }
        //find subsequent fronts.
        unsigned frontCounter = 2;
        std::vector<std::size_t> nextFront;

        //as long as we can find fronts
        //visit all points of the last front found and remove them from the
        //set. All points which are not dominated anymore form the next front
        while (!front.empty()) {
                //visit all points of the current front and remove them
                // if any point is not dominated, it is part the next front.
                for(std::size_t element = 0; element != front.size(); ++element) {
                        //visit all points dominated by the element
                        std::vector<std::size_t> const& dominatedPoints = s[front[element]];
                        for (std::size_t  j = 0; j != dominatedPoints.size(); ++j){
                                std::size_t point = dominatedPoints[j];
                                numberOfDominatingPoints[point]--;
                                // if no more points are dominating this, add to the next front.
                                if (numberOfDominatingPoints[point] == 0){
                                        nextFront.push_back(point);
                                        pop[point].rank() = frontCounter;
                                }
                        }
                }

                //make the new found front the current
                front.swap(nextFront);
                nextFront.clear();
                frontCounter++;
        }
    }
    
    /*void operator()(std::vector<UncertainCMAIndividual<FitnessType>> &pop, SelectionStrategy strategy, double alpha=-1) {

        //dominance relation
        SafeComparator<FitnessExtractor, FitnessType> pdc(strategy, alpha);

        //stors for the i-th point which points are dominated by i
        std::vector<std::vector<std::size_t> > s(pop.size());
        //stores for every point how many points are dominating it
        std::vector<std::size_t> numberOfDominatingPoints(pop.size(), 0);
        //stores initially the front of non-dominated points
        std::vector<std::size_t> front;

        for (std::size_t i = 0; i < pop.size(); i++) {
                //check which points j are dominated by i and add them to s[i]
                //also increment n[j] for every i dominating j
                for (std::size_t  j = 0; j < pop.size(); j++) {
                        if (i == j)
                                continue;

                        int domination = pdc(pop[i], pop[j]);
                        if ( domination > 1)//pop[i]> pop[j]
                                s[i].push_back(j);
                        else if (domination < -1)//pop[i]< pop[j]
                                numberOfDominatingPoints[i]++;
                }
                //all non-dominated points form the first front
                if (numberOfDominatingPoints[i] == 0){
                        front.push_back(i);
                        pop[i].rank() = 1;
                }
        }
        //find subsequent fronts.
        unsigned frontCounter = 2;
        std::vector<std::size_t> nextFront;

        //as long as we can find fronts
        //visit all points of the last front found and remove them from the
        //set. All points which are not dominated anymore form the next front
        while (!front.empty()) {
                //visit all points of the current front and remove them
                // if any point is not dominated, it is part the next front.
                for(std::size_t element = 0; element != front.size(); ++element) {
                        //visit all points dominated by the element
                        std::vector<std::size_t> const& dominatedPoints = s[front[element]];
                        for (std::size_t  j = 0; j != dominatedPoints.size(); ++j){
                                std::size_t point = dominatedPoints[j];
                                numberOfDominatingPoints[point]--;
                                // if no more points are dominating this, add to the next front.
                                if (numberOfDominatingPoints[point] == 0){
                                        nextFront.push_back(point);
                                        pop[point].rank() = frontCounter;
                                }
                        }
                }

                //make the new found front the current
                front.swap(nextFront);
                nextFront.clear();
                frontCounter++;
        }
    }*/
};



#endif /* UNCERTAINSORT_H */

