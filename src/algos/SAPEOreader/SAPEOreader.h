/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SAPEOreader.h
 * Author: thehedgeify
 *
 * Created on 13 September 2018, 04:30
 */

#ifndef SAPEOREADER_H
#define SAPEOREADER_H

#include "../CocoOptimiser.h"
#include "../../uncertainty/SafeComparator.h"
#include "../../functions/UncertainFunc.h"
#include <map>
using namespace shark;

class SAPEOreader : public CocoOptimiser{
    public:
        SAPEOreader(int krigSize, int histSize,
            double uncertaintyThreshold, double historyThreshold,
            SelectionStrategy strat, double alpha,
            UncertainMode unMode, SampleMode sampMode,
            std::map<int, std::vector<RealVector>> points,
            SurrMode surr = LOC);
        void init(COCOfunc_s& function);
        void step(COCOfunc_s const& function);
        void init(COCOfunc_m& function);
        void step(COCOfunc_m const& function);
        SolutionType const& solution()const;
        void spillBeans();
        bool converged(COCOfunc_s const& function);
        bool converged(COCOfunc_m const& function);
        
        void laststep(COCOfunc_s const& function){}
        void laststep(COCOfunc_m const& function){}
        void init(COCOfunc_s& function, COCOfunc_s& function2){
            throw std::invalid_argument("no double functions for SAPEOreader");
        }   
        void init(COCOfunc_m& function, COCOfunc_m& function2){
            throw std::invalid_argument("no double functions for SAPEOreader");
        }
        
    private:       
        std::vector<RealVector> searchPoints;
        std::map<int, std::vector<RealVector>> points;
};

#endif /* SAPEOREADER_H */

