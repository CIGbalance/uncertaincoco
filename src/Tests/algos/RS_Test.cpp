/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RS_Test.cpp
 * Author: volz
 *
 * Created on 23. Juli 2018, 11:18
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

void testObserverOptions(){
    char name[] = "bbob";
    RandomSearch algo;
    std::cout << algo.getObserverOptions(name, 0, 15) << std::endl;
}

void runAlgo() {
    char name[] = "bbob";
    char dim[] = "dimensions: 2,10";
    Suite suite(name, 10, 360, 0, dim, name, 5);
    RandomSearch algo;
    suite.run(&algo);
}


int main(int argc, char** argv) {
    testObserverOptions();
    std::cout << "----------------------------------------" <<std::endl;
    runAlgo();
    return (EXIT_SUCCESS);
}

