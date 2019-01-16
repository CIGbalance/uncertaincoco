/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   KrigingTest.cpp
 * Author: thehedgeify
 *
 * Created on 25 June 2018, 23:14
 */

#include <stdlib.h>
#include <iostream>
#include "../../utils/Kriging.h"
#include "gp_utils.h"

using namespace shark;

/*
 * Simple C++ Test Suite
 */

void testInit() {
    std::vector<RealVector> inputs;
    std::vector<RealVector> labels;
    for(int i=0; i<5; i++){
        RealVector point(2);
        point(0) = rand();
        point(1) = rand();
        inputs.push_back(point);
        RealVector p(1); p(0) = rand();
        labels.push_back(p);
    }
    ArchiveData data = createLabeledDataFromRange(inputs, labels);
    Kriging kriging(data, 2,1);
}

void testPred(){
    srand(17);
    std::vector<RealVector> inputs;
    std::vector<RealVector> labels;
    int n=2;
    int m=1;
    for(int i=0; i<100; i++){
        RealVector point(n);
        for(int j=0; j<n; j++){
            point(j) = drand48()*4-2;
        }
        inputs.push_back(point);
        RealVector y(m);
        for(int j=0; j<m; j++){
            y(j) = Utils::hill(point(0), point(1)) + Utils::randn() * 0.1;
        }
        labels.push_back(y);
    }
    ArchiveData data = createLabeledDataFromRange(inputs, labels);
    Kriging kriging(data, n,m);
    RealVector point(n);
    for(int j=0; j<n; j++){
        point(j) = drand48()*4-2;
    }
    std::vector<std::pair<double,double>> result = kriging.predict(point);
    for(int i=0; i<m; i++){
        std::cout << result[i].first <<" " << result[i].second << std::endl;
    }
}

void testEI(){
    srand(17);
    std::vector<RealVector> inputs;
    std::vector<RealVector> labels;
    int n=3;
    int m=2;
    std::vector<double> fbest(2);
    fbest[0]=100.0; fbest[1]=100.0;
    for(int i=0; i<100; i++){
        RealVector point(n);
        for(int j=0; j<n; j++){
            point(j) = drand48()*4-2;
        }
        inputs.push_back(point);
        RealVector y(m);
        for(int j=0; j<m; j++){
            y(j) = Utils::hill(point(i), point(i+1)) + Utils::randn() * 0.1;
            if(y(j)<fbest[j]){
                fbest[j]=y(j);
            }
        }
        labels.push_back(y);
    }
    ArchiveData data = createLabeledDataFromRange(inputs, labels);
    Kriging kriging(data, n,m);
    RealVector point(n);
    for(int j=0; j<n; j++){
        point(j) = -2;
    }
    std::vector<std::pair<double,double>> result1 = kriging.predict(point);
    for(int i=0; i<m; i++){
        std::cout << "Prediction " << result1[i].first <<" " << result1[i].second << std::endl;
    }
    std::cout << "fbest:";
    for(int i=0; i<m; i++){
        std::cout << " " << fbest[i];
    }
    std::cout << std::endl;
    std::vector<double> result = kriging.expectedImprovement(point);
    for(int i=0; i<m; i++){
        std::cout << result[i]<< std::endl;
    }
    //fbest=-1.86566; m=0.235615; sig=0.628587; s=(fbest-m)/sig; sig*(s*pnorm(s)+dnorm(s))
    //fbest=-1.91964; m=0.512526; sig=0.628587; s=(fbest-m)/sig; sig*(s*pnorm(s)+dnorm(s))
    
    std::cout << "Expected output from R: 6.792626e-05, 7.955382e-06" << std::endl;
}

/*void testHistory(){
    srand(17);
    std::vector<RealVector> inputs;
    std::vector<RealVector> labels;
    int n=3;
    int m=2;
    for(int i=0; i<5; i++){
        std::cout << "inserted ";
        RealVector point(n);
        for(int j=0; j<n; j++){
            point(j) = drand48()*4-2;
            std::cout << point(j) << " ";
        }
        inputs.push_back(point);
        std::cout << ": ";
        RealVector y(m);
        for(int j=0; j<m; j++){
            y(j) = Utils::hill(point(i), point(i+1)) + Utils::randn() * 0.1;
            std::cout << y(j) << " ";
        }
        labels.push_back(y);
        std::cout << std::endl;
    }
    ArchiveData data = createLabeledDataFromRange(inputs, labels);
    Kriging kriging(data, n,m);
    RealVector point(n);
    for(int j=0; j<n; j++){
        point(j) = -2;
    }
    std::vector<double> result = kriging.getHistory(point);
    for(int i=0; i<m; i++){
        std::cout << result[i]<< std::endl;
    }
    
    fs = c(1.52379, 1.88007, 1.88708, 1.11595, 1.0217)
mus = c(0.188342, 0.258492, 0.231172, 0.278123, 1.21409)
sigs = c(0.950895, 0.881747, 0.941076, 0.977956, 0.814225)
mean(abs(fs-mus)/sigs)

fs = c(1.57719, 1.68427, 2.05342, 1.02454, 1.0814)
mus = c(0.198247, 0.269498, 0.240563, 0.300425, 1.19269)
sigs = c(0.950895, 0.881747, 0.941076, 0.977956, 0.814225)
mean(abs(fs-mus)/sigs)
    std::cout << "Expected output from R: 1.21921, 1.17163" << std::endl;
 }*/

int main(int argc, char** argv) {
    testInit();
    std::cout << "----------------------------------------" <<std::endl;
    testPred();
    std::cout << "----------------------------------------" <<std::endl;
    testEI();
    std::cout << "----------------------------------------" <<std::endl;
    //testHistory();
    return (EXIT_SUCCESS);
}

