/* 
 * File:   SuiteTest.cpp
 * Author: vv
 *
 * Created on June 28, 2018, 1:25 PM
 */

#include <stdlib.h>
#include <iostream>
#include "../../COCOinterface/Suite.h"
#include "../../COCOinterface/coco.h"
#include "../../algos/random/RandomSearch.h"
#include "../../algos/CocoOptimiser.h"

/*
 * Simple C++ Test Suite
 */
using namespace shark;


void setup() {
    char name[] = "bbob";
    char dim[] = "dimensions: 2,10";
    Suite suite(name, 10, 360, 0, dim, name, 5);
    std::cout << "setup suite" << std::endl;
}

void run() {
    char name[] = "bbob";
    char dim[] = "dimensions: 2,10";
    Suite suite(name, 10, 360, 0, dim, name, 5);
    RandomSearch algo;
    std::cout << algo.getObserverOptions(name, 0, 15) << std::endl;
    suite.run(&algo);
}

int main(int argc, char** argv) {
    setup();
    std::cout << "----------------------------------------" <<std::endl;
    run();
    return (EXIT_SUCCESS);
}

