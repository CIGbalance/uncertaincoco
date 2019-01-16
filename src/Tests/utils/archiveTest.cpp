/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   archiveSOtest.cpp
 * Author: volz
 *
 * Created on 19. Juni 2018, 14:10
 */

#include <stdlib.h>
#include <iostream>
#include "../../utils/Archive.h"
#include <shark/Data/Dataset.h>
/*
 * Simple C++ Test Suite
 */
using namespace shark;

void testInit() {
    std::vector<RealVector> inputs;
    std::vector<RealVector> labels;
    std::vector<double> labels2;
    Archive archive;
    archive.init(1, 0);
    for(int i=0; i<5; i++){
        RealVector point(2);
        point(0) = rand();
        point(1) = rand();
        inputs.push_back(point);
        RealVector p(1); p(0) = rand();
        labels.push_back(p);
        labels2.push_back(p(0));
        archive.addPoint(point, p(0));
    }
    ArchiveData data = createLabeledDataFromRange(inputs, labels);
    std::cout << "5 Elements: " <<data.numberOfElements()<< std::endl;
}

void testAdd() {
    Archive archive;
    archive.init(2, 0);
    for(int i=0; i<5; i++){
        RealVector point(2);
        point(0) = rand();
        point(1) = rand();
        std::vector<double> val(2);
        val[0]=rand(); val[1]=rand();
        archive.addPoint(point, val);
    }
    std::cout << "5 Elements: " <<archive.getNoPointsCurrent()<< std::endl;
}


/*void testAdd(){
    std::vector<RealVector> inputs;
    std::vector<RealVector> labels;
    std::vector<double> labels2;
    for(int i=0; i<5; i++){
        RealVector point(2);
        point(0) = rand();
        point(1) = rand();
        inputs.push_back(point);
        RealVector p(1); p(0) = rand();
        labels.push_back(p);
        labels2.push_back(p(0));
    }
    ArchiveData data = createLabeledDataFromRange(inputs, labels);
    Archive archive(inputs, labels2);
    std::cout << "5 Elements: " <<archive.getNoPointsCurrent()<< std::endl;
    RealVector point(2);
    point(0) = rand();
    point(1) = rand();
    archive.addPoint(point, rand());
    std::cout << "6 Elements: " << archive.getNoPointsCurrent()<< std::endl;
}*/

void testGetNeighbors(){
    std::vector<RealVector> inputs;
    std::vector<RealVector> labels;
    std::vector<double> labels2;
    Archive archive;
    archive.init(1, 0);
    for(int i=0; i<5; i++){
        RealVector point(2);
        point(0) = rand()%10;
        point(1) = rand()%10;
        inputs.push_back(point);
        RealVector p(1); p(0) = rand();
        labels.push_back(p);
        labels2.push_back(p(0));
        std::cout << "inserted (" << point(0) << ", " << point(1) << ") -> " << p(0) << std::endl;
        archive.addPoint(point, p(0));
    }
    ArchiveData data = createLabeledDataFromRange(inputs, labels);
    std::cout << "5 Elements: " <<data.numberOfElements()<< std::endl;
    RealVector point(2);
    point(0) = rand()%10;
    point(1) = rand()%10;
    std::cout << "nearest neighbours  of (" << point(0) << ", " << point(1) << ")"<< std::endl;
    ArchiveData test = archive.getSample(4, CLOSE, point, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << std::endl;
    }
    
    std::cout << "all samples"<< std::endl;
    test = archive.getSample(4, ALL, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << std::endl;
    }
    
    std::cout << "recent samples"<< std::endl;
    test = archive.getSample(4, RECENT, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << std::endl;
    }
    
    std::cout << "random samples"<< std::endl;
    test = archive.getSample(4, RANDOM, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << std::endl;
    }
    
        
    std::cout << "fittest samples"<< std::endl;
    test = archive.getSample(4, FIT, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << std::endl;
    }
}

void testGetNeighborsMO(){
    Archive archive;
    archive.init(2, 0);
    for(int i=0; i<5; i++){
        RealVector point(2);
        point(0) = rand()%10;
        point(1) = rand()%10;
        std::vector<double> p(2); p[0] = rand(); p[1] = rand();
        std::cout << "inserted (" << point(0) << ", " << point(1) << ") -> " << p[0]<< " "  << p[1] << std::endl;
        archive.addPoint(point, p);
    }
    RealVector point(2);
    point(0) = rand()%10;
    point(1) = rand()%10;
    std::cout << "nearest neighbours  of (" << point(0) << ", " << point(1) << ")"<< std::endl;
    ArchiveData test = archive.getSample(4, CLOSE, point, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << " " << tmp.label(1) << std::endl;
    }
    
    std::cout << "all samples"<< std::endl;
    test = archive.getSample(4, ALL, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << " " << tmp.label(1) << std::endl;
    }
    
    std::cout << "recent samples"<< std::endl;
    test = archive.getSample(4, RECENT, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << " " << tmp.label(1) << std::endl;
    }
    
    std::cout << "random samples"<< std::endl;
    test = archive.getSample(4, RANDOM, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << " " << tmp.label(1) << std::endl;
    }
    
        
    std::cout << "fittest samples"<< std::endl;
    test = archive.getSample(4, FIT, true);
    for(auto element: test.elements()) {
        DataPair<RealVector, RealVector> tmp = element;
        std::cout << tmp.input(0) << " " << tmp.input(1) << ":" << tmp.label(0) << " " << tmp.label(1) << std::endl;
    }
}


void spillBeansTest(){
    std::vector<RealVector> inputs;
    std::vector<RealVector> labels;
    std::vector<double> labels2;
    Archive archive;
    archive.init(1, 0);
    for(int i=0; i<5; i++){
        RealVector point(2);
        point(0) = rand()%10;
        point(1) = rand()%10;
        inputs.push_back(point);
        RealVector p(1); p(0) = rand();
        labels.push_back(p);
        labels2.push_back(p(0));
        std::cout << "inserted (" << point(0) << ", " << point(1) << ") -> " << p(0) << std::endl;
        archive.addPoint(point, p(0));
    }
    archive.signalReset();
    archive.spillBeans("testOutput.txt");
    std::cout << "Test output to testOutput.txt, should contain 5 elements + empty row" << std::endl;
}

void testConverged(){
    Archive archive;
    archive.init(2, 5);
    for(int i=0; i<5; i++){
        RealVector point(2);
        point(0) = rand();
        point(1) = rand();
        std::vector<double> val(2);
        val[0]=rand(); val[1]=rand();
        archive.addPoint(point, val);
    }
    std::cout << "5 Elements: " <<archive.getNoPointsCurrent()<< std::endl;
    std::cout << archive.converged() << std::endl;
}


int main(int argc, char** argv) {
    testInit();
    std::cout << "----------------------------------------" <<std::endl;
    testAdd();
    std::cout << "----------------------------------------" <<std::endl;
    testGetNeighbors();
    std::cout << "----------------------------------------" <<std::endl;
    testGetNeighborsMO();
    std::cout << "----------------------------------------" <<std::endl;
    spillBeansTest();
    std::cout << "----------------------------------------" <<std::endl;
    testConverged();
    return (EXIT_SUCCESS);
}

