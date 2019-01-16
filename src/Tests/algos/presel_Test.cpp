/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   presel_Test.cpp
 * Author: volz
 *
 * Created on 23. Juli 2018, 11:18
 */

#include <stdlib.h>
#include <iostream>
#include "../../COCOinterface/Suite.h"
#include "../../COCOinterface/coco.h"
#include "../../algos/CocoOptimiser.h"
#include "../../algos/preselect/preselect.h"
#include "../../algos/preselect/AbstractPreselecter.h"
#include "../../algos/preselect/CMA_mod.h"
#include "../../algos/preselect/MOCMA_mod.h"
#include "../../algos/preselect/SMS-EMOA_mod.h"
/*
 * Simple C++ Test Suite
 */

using namespace shark;

void testObserverOptions(){
    char name[] = "bbob";
    MOCMA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            MEAN, CLOSE);
    std::cout << algo.getObserverOptions(name, 0, 15, 16, NO) << std::endl;
}

void runAlgo() {
    char name[] = "bbob";
    char dim[] = "dimensions: 2, 10";
    Suite suite(name, 500, 360, 1, dim, name);
    MOCMA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            MEAN, CLOSE);
    suite.run(&algo, 16, NO, false);
}

void runAlgoMO() {
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,10";
    Suite suite(name, 10, 360, 0, dim, name, 5);
    MOCMA_mod mocma;
    CMA_mod cma;
    
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            MEAN, CLOSE);
    std::cout.setstate(std::ios_base::failbit);
    suite.run(&algo, 16, NO, false);
    std::cout.clear();

}


int main(int argc, char** argv) {
    testObserverOptions();
    std::cout << "----------------------------------------" <<std::endl;
    runAlgo();
    std::cout << "----------------------------------------" <<std::endl;
    runAlgoMO();
    return (EXIT_SUCCESS);
}

