/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UncertainTester.cpp
 * Author: thehedgeify
 *
 * Created on 22 August 2018, 15:29
 */

#include <stdlib.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */
#include "../../uncertainty/UncertainIndividual.h"
#include "../../uncertainty/UncertainEvaluator.h"
#include "../../uncertainty/SafeComparator.h"
#include "../../uncertainty/TrueFitnessExtractor.h"
#include "../../uncertainty/UncertainSort.h"
#include "../../uncertainty/UncertainSelect.h"

#include "../../functions/Kriging_pred_s.h"
#include "../../functions/Kriging_pred_m.h"
#include "../../COCOinterface/coco.h"
#include "../../COCOinterface/COCOfunc.h"

#include <shark/Algorithms/DirectSearch/FitnessExtractor.h>
#include <shark/Algorithms/DirectSearch/Indicators/HypervolumeIndicator.h>

using namespace shark;

coco_problem_t* getProblemRef(){
    char suite_name[] = "bbob";
    char observer_name[] = "bbob";
    static coco_problem_t *PROBLEM;
    
    coco_set_log_level("info");
    
    coco_suite_t *suite;
    coco_observer_t *observer;
    
    char *observer_options =
      coco_strdupf("result_folder: RS_on_%s "
                   "algorithm_name: RS "
                   "algorithm_info: \"A simple random search algorithm\"", suite_name);
    
    
    suite = coco_suite(suite_name, "", "");
    observer = coco_observer(observer_name, observer_options);
    coco_free_memory(observer_options);
   
    PROBLEM = coco_suite_get_problem(suite, 0);
    
    coco_observer_free(observer);
    coco_suite_free(suite);
    return PROBLEM;
}


coco_problem_t* getProblemRefMO(){
    char suite_name[] = "bbob-biobj";
    char observer_name[] = "bbob-biobj";
    static coco_problem_t *PROBLEM;
    
    coco_set_log_level("info");
    
    coco_suite_t *suite;
    coco_observer_t *observer;
    
    char *observer_options =
      coco_strdupf("result_folder: RS_on_%s "
                   "algorithm_name: RS "
                   "algorithm_info: \"A simple random search algorithm\"", suite_name);
    
    
    suite = coco_suite(suite_name, "", "");
    observer = coco_observer(observer_name, observer_options);
    coco_free_memory(observer_options);
   
    PROBLEM = coco_suite_get_problem(suite, 0);
    
    coco_observer_free(observer);
    coco_suite_free(suite);
    return PROBLEM;
}

void initPop() {
    int mu=10;
    std::vector<UncertainIndividual<double>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_s f1;
    f1.init(problem, problem2);
    f1.lhs(100, 17, 5);
    Kriging_pred_s f;
    f.init(&f1, 10, 0.5, 60);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
        m_parents[i].penalizedFitness() = 1;
        m_parents[i].unpenalizedFitness()=1;
        m_parents[i].uncertainty()={0.5};
        m_parents[i].trueFitness() = 2;
        m_parents[i].hist() = {1};
    }
    std::cout<< m_parents[0].penalizedFitness() <<std::endl;
    std::cout << "is Uncertain " << UncertainIndividual<double>::IsUncertain(m_parents[0]) << std::endl;
    std::cout << UncertainIndividual<double>::toString(m_parents[0]) << std::endl;
}

void evaluate() {
    int mu=10;
    std::vector<UncertainIndividual<double>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_s f1;
    f1.init(problem, problem2);
    f1.lhs(100, 17, 5);
    Kriging_pred_s f;
    f.init(&f1, 10, 0.5, 60);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
    }
    
    UncertainEvaluator<double> eval;
    eval(f, m_parents.begin(), m_parents.end());
    
    for(int i=0; i<mu; i++){
        std::cout << UncertainIndividual<double>::toString(m_parents[i]) << std::endl;
    }
    std::cout << "Real Evaluations " << f.getRealEvaluations() << std::endl;
    std::cout << "Total Evaluations " << f.getEvaluations() << std::endl;
    f.getArchive().spillBeans("testOutput.txt");
    
    eval(f, m_parents.begin(), m_parents.end(), true);
    
    for(int i=0; i<mu; i++){
        std::cout << UncertainIndividual<double>::toString(m_parents[i]) << std::endl;
    }
    std::cout << "Real Evaluations " << f.getRealEvaluations() << std::endl;
    std::cout << "Total Evaluations " << f.getEvaluations() << std::endl;
}

void compare1() {
    int mu=10;
    std::vector<UncertainIndividual<double>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_s f1;
    f1.init(problem, problem2);
    f1.lhs(100, 17, 5);
    Kriging_pred_s f;
    f.init(&f1, 10, 0.5, 60);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
    }
    
    UncertainEvaluator<double> eval;
    eval(f, m_parents.begin(), m_parents.end());
    
    
    SafeComparator<FitnessExtractor, double> comp(NO_UNCERTAINTY);
    
    for(int i=0; i<mu-1; i++){
        for(int j=i+1; j<mu; j++){
            std::cout << UncertainIndividual<double>::toString(m_parents[i]) <<  " vs ";
            std::cout << UncertainIndividual<double>::toString(m_parents[j]) <<  " : ";
            std::cout <<  comp(m_parents[i],m_parents[j]) << std::endl;       
        }
    }
    
    std::cout << "true" << std::endl;
    
    SafeComparator<TrueFitnessExtractor, double> comp2(PREDICTION);
    
    for(int i=0; i<mu-1; i++){
        for(int j=i+1; j<mu; j++){
            std::cout << UncertainIndividual<double>::toString(m_parents[i]) <<  " vs ";
            std::cout << UncertainIndividual<double>::toString(m_parents[j]) << " : ";
            std::cout <<  comp2(m_parents[i],m_parents[j]) << std::endl;       
        }
    }
}

void compare2() {
    int mu=15;
    std::vector<UncertainIndividual<double>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_s f1;
    f1.init(problem, problem2);
    f1.lhs(50, 17, 5);
    Kriging_pred_s f;
    f.init(&f1, 10, 0.5, 100);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
    }
    
    UncertainEvaluator<double> eval;
    eval(f, m_parents.begin(), m_parents.end());
    
    SafeComparator<FitnessExtractor, double> comp(NO_OVERLAP, 0.05);
    
    for(int i=0; i<mu-1; i++){
        for(int j=i+1; j<mu; j++){
            std::cout << "Individual A " << m_parents[i].penalizedFitness() << " "
                    << m_parents[i].uncertainty()[0] <<  " vs ";
            std::cout << "Individual B " << m_parents[j].penalizedFitness() << " "
                    << m_parents[j].uncertainty()[0] << " : ";
            std::cout <<  comp(m_parents[i],m_parents[j]) << std::endl;       
        }
    }
    
    f.getArchive().spillBeans("testOutput.txt");
    std::cout << "total Evaluations " << f.getRealEvaluations() << std::endl;
    
}

void compare3() {
    int mu=5;
    std::vector<UncertainIndividual<double>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_s f1;
    f1.init(problem, problem2);
    f1.lhs(50, 17, 5);
    Kriging_pred_s f;
    f.init(&f1, 10, 0.5, 100);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
    }
    
    UncertainEvaluator<double> eval;
    eval(f, m_parents.begin(), m_parents.end());
    
    SafeComparator<FitnessExtractor, double> comp(PREDICTION);
    
    for(int i=0; i<mu-1; i++){
        for(int j=i+1; j<mu; j++){
            std::cout << "Individual A " << m_parents[i].penalizedFitness() << " "
                    << m_parents[i].uncertainty()[0] <<  " vs ";
            std::cout << "Individual B " << m_parents[j].penalizedFitness() << " "
                    << m_parents[j].uncertainty()[0] << " : ";
            std::cout <<  comp(m_parents[i],m_parents[j]) << std::endl;       
        }
    }
    
    f.getArchive().spillBeans("testOutput.txt");
    std::cout << "total Evaluations " << f.getRealEvaluations() << std::endl;
    
}

void sort(){
    int mu=15;
    std::vector<UncertainIndividual<double>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_s f1;
    f1.init(problem, problem2);
    f1.lhs(50, 17, 5);
    Kriging_pred_s f;
    f.init(&f1, 10, 0.5, 100);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
    }
    
    UncertainEvaluator<double> eval;
    eval(f, m_parents.begin(), m_parents.end());
    
        
    //f.getArchive().spillBeans("testOutput.txt");
    std::cout << "total Evaluations " << f.getRealEvaluations() << std::endl;
    UncertainSort<double> unSort;
    unSort(m_parents, NO_OVERLAP, 0.05);
    unSort.trueSort(m_parents);
    for(int i=0; i< mu; i++){
        std::cout << UncertainIndividual<double>::toString(m_parents[i]) << std::endl;
    } 
    
}

void select(){
    int mu=15;
    std::vector<UncertainIndividual<double>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_s f1;
    f1.init(problem, problem2);
    f1.lhs(20, 17, 5);
    Kriging_pred_s f;
    f.init(&f1, 10, 1, 100);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
    }
    
    UncertainSelection<HypervolumeIndicator, double> selector;
    selector(m_parents, f, 5, NO_OVERLAP, 0.05);
    
    for(unsigned int i=0; i<m_parents.size(); i++){
        std::cout << UncertainIndividual<double>::toString(m_parents[i]) << std::endl;
    }
    
    std::cout << "total Evaluations " << f.getRealEvaluations() << std::endl;
}


void initPopMO() {
    int mu=10;
    std::vector<UncertainIndividual<RealVector>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRefMO();
    coco_problem_t* problem2 = getProblemRefMO();
    COCOfunc_m f1;
    f1.init(problem, problem2);
    f1.lhs(100, 17, 5);
    Kriging_pred_m f;
    f.init(&f1, 10, 0.5, 60);
    
    RealVector tmp(2);
    tmp(0) = 0.5; tmp(1) = 0.6;
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
        m_parents[i].penalizedFitness() = tmp;
        m_parents[i].unpenalizedFitness()= tmp;
        m_parents[i].uncertainty()={0.5, 0.5};
        m_parents[i].trueFitness() = tmp;
        m_parents[i].hist() = {0.5, 0.5};
    }
    std::cout<< m_parents[0].penalizedFitness()(0) <<" " << m_parents[0].penalizedFitness()(1) <<std::endl;
    std::cout << "is Uncertain " << UncertainIndividual<RealVector>::IsUncertain(m_parents[0]) << std::endl;
}

void evaluateMO() {
    int mu=10;
    std::vector<UncertainIndividual<RealVector>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRefMO();
    coco_problem_t* problem2 = getProblemRefMO();
    COCOfunc_m f1;
    f1.init(problem, problem2);
    f1.lhs(100, 17, 5);
    Kriging_pred_m f;
    f.init(&f1, 10, 0.9, 10);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
    }
    
    UncertainEvaluator<RealVector> eval;
    eval(f, m_parents.begin(), m_parents.end());
    
    for(int i=0; i<mu; i++){
        std::cout << UncertainIndividual<RealVector>::toString(m_parents[i]) << std::endl;
    }
    std::cout << "Real Evaluations " << f.getRealEvaluations() << std::endl;
    f.getArchive().spillBeans("testOutput.txt");
    
    eval(f, m_parents.begin(), m_parents.end(), true);
    
    for(int i=0; i<mu; i++){
        std::cout << UncertainIndividual<RealVector>::toString(m_parents[i]) << std::endl;
    }
    std::cout << "Real Evaluations " << f.getRealEvaluations() << std::endl;
}

void compare1MO() {
    int mu=3;
    std::vector<UncertainIndividual<RealVector>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRefMO();
    coco_problem_t* problem2 = getProblemRefMO();
    COCOfunc_m f1;
    f1.init(problem, problem2);
    f1.lhs(10, 17, 5);
    Kriging_pred_m f;
    f.init(&f1, 10, 0.5, 60);
    
    RealVector tmp(2);
    tmp(0) = 1; tmp(1) = 2;
    m_parents[0].searchPoint() = f.proposeStartingPoint();
    m_parents[0].penalizedFitness() = tmp;
    m_parents[0].unpenalizedFitness()= tmp;
    m_parents[0].uncertainty()={0.05, 0.05};
    
    
    tmp(0) = 1.2; tmp(1) = 1.5;
    m_parents[1].searchPoint() = f.proposeStartingPoint();
    m_parents[1].penalizedFitness() = tmp;
    m_parents[1].unpenalizedFitness()= tmp;
    m_parents[1].uncertainty()={0.5, 0.5};

    tmp(0) = 0.9; tmp(1) = 2.3;
    m_parents[2].searchPoint() = f.proposeStartingPoint();
    m_parents[2].penalizedFitness() = tmp;
    m_parents[2].unpenalizedFitness()= tmp;
    m_parents[2].uncertainty()={0.05, 0.05};
    
    
    SafeComparator<FitnessExtractor, RealVector> comp(NO_UNCERTAINTY);
    
    for(int i=0; i<mu-1; i++){
        for(int j=i+1; j<mu; j++){
            std::cout << "Individual A " << m_parents[i].penalizedFitness()(0)
                << " " << m_parents[i].penalizedFitness()(1) << " : "
                    << m_parents[i].uncertainty()[0] << " " << m_parents[i].uncertainty()[1] <<  " vs ";
            std::cout << "Individual B " << m_parents[j].penalizedFitness()(0)
                << " " << m_parents[j].penalizedFitness()(1) << " : "
                    << m_parents[j].uncertainty()[0] << " " << m_parents[j].uncertainty()[1] << " : ";
            std::cout <<  comp(m_parents[i],m_parents[j]) << std::endl;       
        }
    }
}

void compare2MO() {
    int mu=3;
    std::vector<UncertainIndividual<RealVector>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRefMO();
    coco_problem_t* problem2 = getProblemRefMO();
    COCOfunc_m f1;
    f1.init(problem, problem2);
    f1.lhs(10, 17, 5);
    Kriging_pred_m f;
    f.init(&f1, 10, 0.5, 60);
    
    RealVector tmp(2);
    tmp(0) = 1; tmp(1) = 2;
    m_parents[0].searchPoint() = f.proposeStartingPoint();
    m_parents[0].penalizedFitness() = tmp;
    m_parents[0].unpenalizedFitness()= tmp;
    m_parents[0].uncertainty()={0.05, 0.05};
    m_parents[0].trueFitness() = tmp;
    m_parents[0].hist() = {0.5, 0.5};
    
    tmp(0) = 1.2; tmp(1) = 1.5;
    m_parents[1].searchPoint() = f.proposeStartingPoint();
    m_parents[1].penalizedFitness() = tmp;
    m_parents[1].unpenalizedFitness()= tmp;
    m_parents[1].uncertainty()={0.5, 0.5};
    m_parents[1].trueFitness() = tmp;
    m_parents[1].hist() = {0.5, 0.5};
    
    tmp(0) = 0.9; tmp(1) = 1.9;
    m_parents[2].searchPoint() = f.proposeStartingPoint();
    m_parents[2].penalizedFitness() = tmp;
    m_parents[2].unpenalizedFitness()= tmp;
    m_parents[2].uncertainty()={0.05, 0.05};
    m_parents[2].trueFitness() = tmp;
    m_parents[2].hist() = {0.5, 0.5};    
    
    SafeComparator<FitnessExtractor, RealVector> comp(NO_OVERLAP, 0.05);
    
    for(int i=0; i<mu-1; i++){
        for(int j=i+1; j<mu; j++){
            std::cout << "Individual A " << m_parents[i].penalizedFitness()(0)
                << " " << m_parents[i].penalizedFitness()(1) << " : "
                    << m_parents[i].uncertainty()[0] << " " << m_parents[i].uncertainty()[1] <<  " vs ";
            std::cout << "Individual B " << m_parents[j].penalizedFitness()(0)
                << " " << m_parents[j].penalizedFitness()(1) << " : "
                    << m_parents[j].uncertainty()[0] << " " << m_parents[j].uncertainty()[1] << " : ";
            std::cout <<  comp(m_parents[i],m_parents[j]) << std::endl;       
        }
    }
    
    f.getArchive().spillBeans("testOutput.txt");
    std::cout << "total Evaluations " << f.getRealEvaluations() << std::endl;
    
    UncertainSort<RealVector> unSort;
    unSort(m_parents, NO_OVERLAP, 0.05);
    for(int i=0; i< mu; i++){
        std::cout << UncertainIndividual<RealVector>::toString(m_parents[i]);
        std::cout << " " << m_parents[i].rank() << std::endl;
    }
}

void compare3MO() {
        int mu=3;
    std::vector<UncertainIndividual<RealVector>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRefMO();
    coco_problem_t* problem2 = getProblemRefMO();
    COCOfunc_m f1;
    f1.init(problem, problem2);
    f1.lhs(10, 17, 5);
    Kriging_pred_m f;
    f.init(&f1, 10, 0.5, 60);
    
    RealVector tmp(2);
    tmp(0) = 1; tmp(1) = 2;
    m_parents[0].searchPoint() = f.proposeStartingPoint();
    m_parents[0].penalizedFitness() = tmp;
    m_parents[0].unpenalizedFitness()= tmp;
    m_parents[0].uncertainty()={0.05, 0.05};
    m_parents[0].trueFitness() = tmp;
    m_parents[0].hist() = {0.5, 0.5};
    
    tmp(0) = 1.2; tmp(1) = 1.5;
    m_parents[1].searchPoint() = f.proposeStartingPoint();
    m_parents[1].penalizedFitness() = tmp;
    m_parents[1].unpenalizedFitness()= tmp;
    m_parents[1].uncertainty()={0.5, 0.5};
    m_parents[1].trueFitness() = tmp;
    m_parents[1].hist() = {0.5, 0.5};
    
    tmp(0) = 0.9; tmp(1) = 1.9;
    m_parents[2].searchPoint() = f.proposeStartingPoint();
    m_parents[2].penalizedFitness() = tmp;
    m_parents[2].unpenalizedFitness()= tmp;
    m_parents[2].uncertainty()={0.05, 0.05};
    m_parents[2].trueFitness() = tmp;
    m_parents[2].hist() = {0.5, 0.5};    
    
    
    SafeComparator<FitnessExtractor, RealVector> comp(PREDICTION);
    
    for(int i=0; i<mu-1; i++){
        for(int j=i+1; j<mu; j++){
            std::cout << "Individual A " << m_parents[i].penalizedFitness()(0)
                << " " << m_parents[i].penalizedFitness()(1) << " : "
                    << m_parents[i].uncertainty()[0] << " " << m_parents[i].uncertainty()[1] <<  " vs ";
            std::cout << "Individual B " << m_parents[j].penalizedFitness()(0)
                << " " << m_parents[j].penalizedFitness()(1) << " : "
                    << m_parents[j].uncertainty()[0] << " " << m_parents[j].uncertainty()[1] << " : ";
            std::cout <<  comp(m_parents[i],m_parents[j]) << std::endl;       
        }
    }
    
    UncertainSort<RealVector> unSort;
    unSort(m_parents, PREDICTION);
    for(int i=0; i< mu; i++){
        std::cout << UncertainIndividual<RealVector>::toString(m_parents[i]);
        std::cout << " " << m_parents[i].rank() << std::endl;
    }
    
}

void selectMO(){
    int mu=15;
    std::vector<UncertainIndividual<RealVector>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRefMO();
    coco_problem_t* problem2 = getProblemRefMO();
    COCOfunc_m f1;
    f1.init(problem, problem2);
    f1.lhs(20, 17, 5);
    Kriging_pred_m f;
    f.init(&f1, 10, 1, 100);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
    }
    
    UncertainSelection<HypervolumeIndicator, RealVector> selector;
    selector(m_parents, f, 2, NO_OVERLAP, 0.05);
    
    for(unsigned int i=0; i<m_parents.size(); i++){
        std::cout << UncertainIndividual<RealVector>::toString(m_parents[i]) <<  std::endl;
    }
    
    std::cout << "total Evaluations " << f.getRealEvaluations() << std::endl;
}

void selectMO2(){
    int mu=15;
    std::vector<UncertainIndividual<RealVector>> m_parents(mu);
    
    coco_problem_t* problem = getProblemRefMO();
    coco_problem_t* problem2 = getProblemRefMO();
    COCOfunc_m f1;
    f1.init(problem, problem2);
    f1.lhs(1000, 17, 5);
    Kriging_pred_m f;
    f.init(&f1, 100, 10, 10000);
    
    for(int i=0; i<mu; i++){
        m_parents[i].searchPoint() = f.proposeStartingPoint();
    }
    
    UncertainSelection<HypervolumeIndicator, RealVector> selector;
    selector(m_parents, f, 8, NO_OVERLAP, 0.05);
    
    for(unsigned int i=0; i<m_parents.size(); i++){
        std::cout << UncertainIndividual<RealVector>::toString(m_parents[i]) <<  std::endl;
    }
    
    std::cout << "total Evaluations " << f.getRealEvaluations() << std::endl;
    f.getArchive().spillBeans("testOutput.txt");
}


int main(int argc, char** argv) {
    initPop();
    std::cout << "----------------------------------------" <<std::endl;
    evaluate();
    std::cout << "----------------------------------------" <<std::endl;
    compare1();
    std::cout << "----------------------------------------" <<std::endl;
    compare2();
    std::cout << "----------------------------------------" <<std::endl;
    compare3();
    std::cout << "----------------------------------------" <<std::endl;
    sort();
    std::cout << "----------------------------------------" <<std::endl;
    select();
    std::cout << "----------------------------------------" <<std::endl;
    initPopMO();
    std::cout << "----------------------------------------" <<std::endl;
    evaluateMO();
    std::cout << "----------------------------------------" <<std::endl;
    compare1MO();
    std::cout << "----------------------------------------" <<std::endl;
    compare2MO();
    std::cout << "----------------------------------------" <<std::endl;
    compare3MO();
    std::cout << "----------------------------------------" <<std::endl;
    selectMO();
    std::cout << "----------------------------------------" <<std::endl;
    selectMO2();
    return (EXIT_SUCCESS);
}

