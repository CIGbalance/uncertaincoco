/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EGO_Test.cpp
 * Author: thehedgeify
 *
 * Created on 29 July 2018, 15:23
 */

#include <stdlib.h>
#include <iostream>

#include "../../algos/EGO/EGO.h"
#include "../../COCOinterface/Suite.h"
#include "../../COCOinterface/coco.h"
#include "../../algos/CocoOptimiser.h"

#include <shark/Algorithms/DirectSearch/CMA.h>
#include <shark/Algorithms/DirectSearch/MOCMA.h>

/*
 * Simple C++ Test Suite
 */
using namespace shark;


void testObserverOptions(){
    char name[] = "bbob";
    CMA cma;
    MOCMA mocma;
    EGO algo(10, 17, 5,10, 10, 10, &mocma, &cma, ExI, FIT);
    std::cout << algo.getObserverOptions(name, 0, 15, 16, NO) << std::endl;
}

void runAlgo() {
    char name[] = "bbob";
    char dim[] = "dimensions: 2";
    Suite suite(name, 500, 360, 0, dim, name);
    CMA cma;
    MOCMA mocma;
    EGO algo(11, 17, 5,11, 1, 1000, &mocma, &cma, ExI, FIT);
    suite.run(&algo, 16, NO, false);
}

void runAlgoMO() {
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,10";
    Suite suite(name, 10, 550, 0, dim, name, 5);
    CMA cma;
    MOCMA mocma;
    EGO algo(10, 17, 5,10, 10, 10, &mocma, &cma, ExI, FIT);
    suite.run(&algo, 16, NO, false);
}

int main(int argc, char** argv) {
    testObserverOptions();
    std::cout << "----------------------------------------" <<std::endl;
    runAlgo();
    std::cout << "----------------------------------------" <<std::endl;
    runAlgoMO();
    return (EXIT_SUCCESS);
}
