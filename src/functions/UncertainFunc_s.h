/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UncertainFunc_s.h
 * Author: thehedgeify
 *
 * Created on 03 September 2018, 23:42
 */

#ifndef UNCERTAINFUNC_S_H
#define UNCERTAINFUNC_S_H


#include "../COCOinterface/COCOfunc.h"
#include "COCOfunc_s.h"
#include "UncertainFunc.h"
#include "../utils/Kriging.h"
#include "../utils/Archive.h"

#include <shark/ObjectiveFunctions/AbstractObjectiveFunction.h>

using namespace shark;

class UncertainFunc_s : public virtual SingleObjectiveFunction, public virtual COCOfunc<SingleObjectiveFunction>,
        public virtual UncertainFunc<SingleObjectiveFunction>{
public:
    UncertainFunc_s(){
        m_features|=CAN_PROPOSE_STARTING_POINT;
        /*m_features|=IS_CONSTRAINED_FEATURE;
        m_features|=HAS_CONSTRAINT_HANDLER;*/
    }
    
    
    void init(const COCOfunc_s * f1, int k, int h, double uncertaintyThreshold, 
                double historyThreshold, UncertainMode mode, SampleMode sampMode, int minK=-1){
        doInit(f1, k, h, uncertaintyThreshold, historyThreshold, mode, sampMode, minK);
        UncertainFunc<SingleObjectiveFunction>::init(f1, k, h, uncertaintyThreshold, historyThreshold, sampMode, minK);
    }
    
    void init(const COCOfunc_s * f1, const COCOfunc_s * modelFunc, SurrMode surr,
            int k, int h, double uncertaintyThreshold, double historyThreshold, UncertainMode mode,
            SampleMode sampMode, int minK=-1){
        doInit(f1, k, h, uncertaintyThreshold, historyThreshold, mode, sampMode, minK);
        UncertainFunc<SingleObjectiveFunction>::init(f1, modelFunc, surr, k, h, uncertaintyThreshold, historyThreshold, sampMode, minK);
    }  
    
    
    using UncertainFunc<SingleObjectiveFunction>::getEvaluations;
    using UncertainFunc<SingleObjectiveFunction>::getRealEvaluations;
    using UncertainFunc<SingleObjectiveFunction>::getArchive;
    using UncertainFunc<SingleObjectiveFunction>::signalArchiveReset;
    using UncertainFunc<SingleObjectiveFunction>::converged;
    using UncertainFunc<SingleObjectiveFunction>::cheaterEval;
    
    virtual void uncertainEval(double * res, Kriging * krig, const RealVector & p, const RealVector krigPoint) const{
        if(mode==MEAN){
            std::vector<std::pair<double,double>> result = krig->predict(krigPoint);
            res[0] = result[0].first;   
        }else if(mode==PoI){
            std::vector<double> result = krig->probImprovement(krigPoint);
            res[0] = (-1) * result[0];
        }else if(mode==ExI){
            //std::cout << "computing exi..";
            std::vector<double> result = krig->expectedImprovement(krigPoint);
            //std::cout << ".. done" << std::endl;
            res[0] = (-1) * result[0];
        }else if(mode==LB){
            std::vector<double> result = krig->lowerBound(krigPoint);
            res[0] = result[0];
        }else if(mode == CERTAIN){
            double* result = COCOfunc<SingleObjectiveFunction>::eval(p, krigPoint);
            res[0]= result[0];
            delete[] result;
        }
    }

    virtual void certainEval(double * res, const RealVector &p, const RealVector krigPoint) const{
        RealVector result = UncertainFunc<SingleObjectiveFunction>::certainEval(p, krigPoint);
        res[0] = result(0);
    }
    
    virtual void cheaterEval(double * res, const RealVector &p) const{
        double* result = COCOfunc<SingleObjectiveFunction>::cheater_eval(p);
        res[0] = result[0];
        delete[] result;
    }
    
    double eval( const SearchPointType & p) const{
        double result[this->m_numberOfObjectives];
        UncertainFunc<SingleObjectiveFunction>::eval(result, p, false);
        return result[0];
    }
    
    double certainEval( const SearchPointType & p) const{
        double result[this->m_numberOfObjectives];
        UncertainFunc<SingleObjectiveFunction>::eval(result, p, true);
        return result[0];
    }
    
    double cheaterEval(RealVector point) const{
        return UncertainFunc<SingleObjectiveFunction>::cheaterEval(point)(0);
    }
    
    std::string name() const{
        if(mode==MEAN){
            return "Mean prediction of "+COCOfunc<SingleObjectiveFunction>::name();
        }else if(mode==PoI){
            return "PoI of "+COCOfunc<SingleObjectiveFunction>::name();
        }else if(mode==ExI){
            return "ExI of "+COCOfunc<SingleObjectiveFunction>::name();
        }else if(mode==LB){
            return "Lower bounds of "+COCOfunc<SingleObjectiveFunction>::name();
        }else if(mode==CERTAIN){
            return COCOfunc<SingleObjectiveFunction>::name();
        }
        return "unspecified";
    }
private:
    UncertainMode mode;
    void doInit(const COCOfunc_s * f1, int k, int h, double uncertaintyThreshold, 
                double historyThreshold, UncertainMode mode, SampleMode sampMode, int minK=-1){
        if( (mode==PoI || mode==ExI) &&
               (uncertaintyThreshold < std::numeric_limits<double>::infinity() ||
                    historyThreshold < std::numeric_limits<double>::infinity()) ){
            throw std::invalid_argument("thresholds not implemented for improvement stuff");
        }else if( mode == CERTAIN && (uncertaintyThreshold>0 || historyThreshold>0)){
            throw std::invalid_argument("certain evaluation must have all thresholds as 0");
        }
        
        COCOfunc<SingleObjectiveFunction>::init(f1->getProblem(), f1->getCheaterProblem(), f1->getTimeWindow(), f1->isNoisy(), 0, f1->getTransform());
        if(this->m_numberOfObjectives!=1){
            throw std::invalid_argument("initialising single-objective function with multi-objective one");
        }
        this->mode = mode;
    }
    
};


#endif /* UNCERTAINFUNC_S_H */

