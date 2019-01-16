/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AbstractPeselecter_Test.cpp
 * Author: thehedgeify
 *
 * Created on 05 August 2018, 11:46
 */

#include <stdlib.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */
#include "../../algos/preselect/AbstractPreselecter.h"
#include "../../algos/preselect/CMA_mod.h"
#include "../../algos/preselect/MOCMA_mod.h"
#include "../../algos/preselect/SMS-EMOA_mod.h"

#include "../../COCOinterface/coco.h"
#include "../../functions/COCOfunc_s.h"
#include "../../functions/Kriging_pred_s.h"
#include "../../functions/COCOfunc_m.h"
#include "../../functions/Kriging_pred_m.h"

#include <shark/ObjectiveFunctions/Benchmarks/Ackley.h>

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

coco_problem_t* getProblemRef2(){
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

void testInit(){
    Ackley optimFunction(5);
    CMA_mod cma;
    optimFunction.init();
    cma.init(optimFunction, optimFunction.proposeStartingPoint());
}

void testRunSO(){
    coco_problem_t* problem = getProblemRef();
    coco_problem_t* problem2 = getProblemRef();
    COCOfunc_s f1;
    f1.init(problem, problem2);
    f1.lhs(100, 17, 5);
    Kriging_pred_s f;
    f.init(&f1, 10, 10);
    
    CMA_mod cma;
    cma.init(f1, f1.proposeStartingPoint());
    std::cout << "test" << std::endl;
    cma.step_presel(f1,f);
}

void testRunMO(){
    coco_problem_t* problem = getProblemRef2();
    coco_problem_t* problem2 = getProblemRef2();
    COCOfunc_m f1;
    f1.init(problem, problem2);
    f1.lhs(100, 17, 5);
    Kriging_pred_m f;
    f.init(&f1, 10, 10);
    
    MOCMA_mod cma;
    cma.init(f1);
    std::cout << "test" << std::endl;
    std::cout.setstate(std::ios_base::failbit);
    cma.step_presel(f1,f);
    std::cout.clear();
}

void testRunMO2(){
    coco_problem_t* problem = getProblemRef2();
    coco_problem_t* problem2 = getProblemRef2();
    COCOfunc_m f1;
    f1.init(problem, problem2);
    f1.lhs(100, 17, 5);
    Kriging_pred_m f;
    f.init(&f1, 10, 10);
    
    SMSEMOA_mod cma;
    cma.init(f1);
    std::cout << "test" << std::endl;
    std::cout.setstate(std::ios_base::failbit);
    cma.step_presel(f1,f);
    std::cout.clear();
}

int main(int argc, char** argv) {
    testInit();
    std::cout << "----------------------------------------" <<std::endl;
    testRunSO();
    std::cout << "----------------------------------------" <<std::endl;
    testRunMO();
    std::cout << "----------------------------------------" <<std::endl;
    testRunMO2();
    return (EXIT_SUCCESS);
}

