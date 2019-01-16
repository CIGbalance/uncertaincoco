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
#include "../../algos/SAPEO/AbstractSAPEO.h"
#include "../../algos/SAPEO/SAPEOCMA.h"
#include "../../algos/SAPEO/SAPEOMOCMA.h"
#include "../../algos/SAPEO/SAPEOSMS.h"
#include "../../algos/SAPEO/SAPEO.h"
#include "../../uncertainty/SafeComparator.h"
/*
 * Simple C++ Test Suite
 */

using namespace shark;

void testObserverOptions(){
    char name[] = "bbob";
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(10, 10, &mocma, &cma, 0.05, 100,  NO_OVERLAP, 0.05, MEAN, CLOSE);
    std::cout << algo.getObserverOptions(name, 0, 15, 10, NO) << std::endl;
}

void runAlgo() {
    char name[] = "bbob";
    char dim[] = "dimensions: 2";
    Suite suite(name, 500, 360, 280, dim, name, 5);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    //SAPEO algo(50, 10, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT);
    SAPEO algo(50, 10, &mocma, &cma, 0, 0, NO_UNCERTAINTY, 0.05, CERTAIN, FIT);
    //SAPEO algo(50, 10, &mocma, &cma, std::numeric_limits<double>::infinity(),
    //        std::numeric_limits<double>::infinity(), NO_OVERLAP, 0.05, MEAN, CLOSE);

    suite.run(&algo, 16, NO);
}

void runAlgoMO() {
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2";
    Suite suite(name, 100, 550, 0, dim, name, 5);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(10, 10, &mocma, &cma, 0.05, 100, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 10, NO);
}


int main(int argc, char** argv) {
    testObserverOptions();
    std::cout << "----------------------------------------" <<std::endl;
    runAlgo();
    std::cout << "----------------------------------------" <<std::endl;
    runAlgoMO();
    return (EXIT_SUCCESS);
}

