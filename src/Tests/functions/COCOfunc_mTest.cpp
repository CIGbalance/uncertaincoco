/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   COCOfunc_mTest.cpp
 * Author: thehedgeify
 *
 * Created on 31 July 2018, 16:10
 */

#include <stdlib.h>
#include <iostream>
#include "../../COCOinterface/COCOfunc.h"
#include "../../COCOinterface/coco.h"
#include "../../functions/COCOfunc_m.h"
#include "../../algos/preselect/MOCMA_mod.h"
#include "../../algos/preselect/SMS-EMOA_mod.h"

using namespace shark;

/*
 * Simple C++ Test Suite
 */

coco_problem_t* getProblemRef(){
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

void evaluateFunction() {
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_m f;
    f.init(problem, problem2);

    std::cout << "test"<< std::endl;    
    RealVector point(2);
    point(0) = 1; point(1)=2;
    std::cout<<"evaluating "<<f.name()<<" ";
    RealVector result = f.eval(point);
    for(unsigned int i=0; i<f.numberOfObjectives(); i++){
        std::cout << " " << result(i) << " ";
    }
    std::cout << std::endl;   
    std::cout << "number of evaluations, should be 1: " << f.getEvaluations() << std::endl;
    std::cout<<"evaluating "<<f.name()<<" ";
    result = f.eval(point);
    for(unsigned int i=0; i<f.numberOfObjectives(); i++){
        std::cout << " " << result(i) << " ";
    }
    std::cout << std::endl;   
    std::cout << "number of evaluations, should be 2: " << f.getEvaluations() << std::endl;
    //optimFunction.init(problem, 1, 2, 5);
    //std::cout << "BBOBSOtest test 1" << std::endl;
    //BBOBSO optimFunction();
}

void testTrans() {
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_m f;
    f.init(problem, problem2, 10, false, 0, INVERSE);

    std::cout << "test"<< std::endl;    
    RealVector point(2);
    point(0) = 1; point(1)=2;
    std::cout<<"evaluating "<<f.name()<<" ";
    RealVector result = f.eval(point);
    for(unsigned int i=0; i<f.numberOfObjectives(); i++){
        std::cout << " " << result(i) << " ";
    }
    std::cout << std::endl;   
    std::cout << "number of evaluations, should be 1: " << f.getEvaluations() << std::endl;
    std::cout<<"evaluating "<<f.name()<<" ";
    result = f.eval(point);
    for(unsigned int i=0; i<f.numberOfObjectives(); i++){
        std::cout << " " << result(i) << " ";
    }
    std::cout << std::endl;   
    std::cout << "number of evaluations, should be 2: " << f.getEvaluations() << std::endl;
    //optimFunction.init(problem, 1, 2, 5);
    //std::cout << "BBOBSOtest test 1" << std::endl;
    //BBOBSO optimFunction();
}


void testGetters() {
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_m f;
    f.init(problem, problem2);
    std::cout << "number of variables " << f.numberOfVariables() << std::endl;
    std::cout << "number of objectives " << f.numberOfObjectives() << std::endl;
    //std::cout << "scalability " << f.hasScalableDimensionality() << std::endl;
    
}

void testFeasible(){
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_m f;
    f.init(problem, problem2);
    RealVector point(2);
    point(0) = 1; point(1)=2;
    std::cout << f.isFeasible(point) << std::endl;
    point(1)=101;
    std::cout << f.isFeasible(point) << std::endl;
}

void testLhs(){
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_m f;
    f.init(problem, problem2);
    ArchiveData data = f.lhs(10, 5);
    for(auto element: data.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        for(unsigned int i=0; i<f.numberOfVariables(); i++){
            std::cout << tmp.input(i) << " ";
        }
        std::cout << " : ";
        if(f.numberOfObjectives()==2){
            std::cout<< tmp.label(0) << " " << tmp.label(1) << std::endl;
        }else{
            std::cout << tmp.label(0) << std::endl;   
        }
    }
}

void testStartPoint(){
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_m f;
    f.init(problem, problem2);
    std::cout << "can propose starting point " << f.canProposeStartingPoint() << std::endl;
    RealVector start = f.proposeStartingPoint();
    std::cout <<" proposed starting point: ";
    for(unsigned int i=0; i<start.size(); i++){
        std::cout << start(i) << " ";
    }
    std::cout << std::endl;
    start = f.proposeStartingPoint();
    std::cout <<" proposed starting point: ";
    for(unsigned int i=0; i<start.size(); i++){
        std::cout << start(i) << " ";
    }
    std::cout << std::endl;
}

void testCMA(){
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_m optimFunction;
    optimFunction.init(problem, problem2);

    MOCMA_mod mocma;
    //cma.setInitialSigma(0.1);// Explicitely set initial global step size.   
    //RealVector p2 = p;  
    mocma.init(optimFunction);

    std::cout.setstate(std::ios_base::failbit);
    for(int i = 0; i <= 10; ++i){
        mocma.step( optimFunction );
        // Report information on the optimizer state and the current solution to the console.
    }
    std::cout.clear();
    // Print the optimal pareto front
    /*for( std::size_t i = 0; i < mocma.solution().size(); i++ ) {
            for( std::size_t j = 0; j < optimFunction.numberOfObjectives(); j++ ) {
                    std::cout<< mocma.solution()[ i ].value[j]<<" ";
            }
            std::cout << std::endl;
    }*/
}

void testSMSEMOA(){
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_m optimFunction;
    optimFunction.init(problem, problem2);

    SMSEMOA_mod smsemoa;
    //cma.setInitialSigma(0.1);// Explicitely set initial global step size.   
    //RealVector p2 = p;  
    smsemoa.init(optimFunction);
    
    std::cout.setstate(std::ios_base::failbit);
    for(int i = 0; i <= 10; ++i){
        smsemoa.step( optimFunction );
        // Report information on the optimizer state and the current solution to the console.
    }
    std::cout.clear();
    // Print the optimal pareto front
    /*for( std::size_t i = 0; i < smsemoa.solution().size(); i++ ) {
            for( std::size_t j = 0; j < optimFunction.numberOfObjectives(); j++ ) {
                    std::cout<< smsemoa.solution()[ i ].value[j]<<" ";
            }
            std::cout << std::endl;
    }*/
}

void testArchiving(){
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_m f;
    f.init(problem, problem2);
    std::cout << "Archive data size should be 0: " << f.getArchive().getNoPointsCurrent() << std::endl;
    RealVector point(2);
    point(0) = 1; point(1)=2;
    std::cout<<"evaluating "<<f.name()<<" ";
    RealVector result = f.eval(point);
    for(unsigned int i=0; i<f.numberOfObjectives(); i++){
        std::cout << " " << result(i) << " ";
    }
    std::cout << std::endl;  
    std::cout << "Archive data size should be 1: " << f.getArchive().getNoPointsCurrent() << std::endl;
}


int main(int argc, char** argv) {
    evaluateFunction();
    std::cout << "----------------------------------------" <<std::endl;
    testTrans();
    std::cout << "----------------------------------------" <<std::endl;
    testGetters();
    std::cout << "----------------------------------------" <<std::endl;
    testFeasible();
    std::cout << "----------------------------------------" <<std::endl;
    testLhs();
    std::cout << "----------------------------------------" <<std::endl;
    testStartPoint();
    std::cout << "----------------------------------------" <<std::endl;
    testCMA();
    std::cout << "----------------------------------------" <<std::endl;
    testSMSEMOA();
    std::cout << "----------------------------------------" <<std::endl;
    testArchiving(); 
    return (EXIT_SUCCESS);
}


