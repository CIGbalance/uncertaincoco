/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   UncertainFunc_m.h
 * Author: thehedgeify
 *
 * Created on 04 September 2018, 02:14
 */

#ifndef UNCERTAINFUNC_M_H
#define UNCERTAINFUNC_M_H

#include "../COCOinterface/COCOfunc.h"
#include "COCOfunc_m.h"
#include "UncertainFunc.h"
#include "../utils/Kriging.h"
#include "../utils/Archive.h"

#include <shark/ObjectiveFunctions/AbstractObjectiveFunction.h>

using namespace shark;

class UncertainFunc_m : public virtual MultiObjectiveFunction, public virtual COCOfunc<MultiObjectiveFunction>,
        public virtual UncertainFunc<MultiObjectiveFunction>{
public:
    UncertainFunc_m(){
        m_features|=CAN_PROPOSE_STARTING_POINT;
        /*m_features|=IS_CONSTRAINED_FEATURE;
        m_features|=HAS_CONSTRAINT_HANDLER;*/
    }
    
    
    void init(const COCOfunc_m * f1, int k, int h, double uncertaintyThreshold,
    double historyThreshold, UncertainMode mode, SampleMode sampMode, int minK=-1){
        doInit(f1, k, h, uncertaintyThreshold, historyThreshold, mode, sampMode, minK);
        UncertainFunc<MultiObjectiveFunction>::init(f1, k, h, uncertaintyThreshold, historyThreshold, sampMode, minK);
    }
    
    void init(const COCOfunc_m * f1, const COCOfunc_m * modelFunc, SurrMode surr,
            int k, int h, double uncertaintyThreshold, double historyThreshold,
            UncertainMode mode, SampleMode sampMode, int minK=-1){
        doInit(f1, k, h, uncertaintyThreshold, historyThreshold, mode, sampMode, minK);
        UncertainFunc<MultiObjectiveFunction>::init(f1, modelFunc, surr, k, h, uncertaintyThreshold,
                historyThreshold, sampMode, minK);
    }  
    
    using UncertainFunc<MultiObjectiveFunction>::getEvaluations;
    using UncertainFunc<MultiObjectiveFunction>::getRealEvaluations;
    using UncertainFunc<MultiObjectiveFunction>::getArchive;
    using UncertainFunc<MultiObjectiveFunction>::signalArchiveReset;
    using UncertainFunc<MultiObjectiveFunction>::converged;
    using UncertainFunc<MultiObjectiveFunction>::cheaterEval;        
    
    virtual void uncertainEval(double * res, Kriging * krig, const RealVector & p, const RealVector krigPoint) const{
        if(mode==MEAN){
            std::vector<std::pair<double,double>> result = krig->predict(krigPoint);
            for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
                res[i]=result[i].first;
            }
        }else if(mode==PoI){
            std::vector<double> result = krig->probImprovement(krigPoint);
            for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
                res[i]=(-1) *result[i];
            }            
        }else if(mode==ExI){
            std::vector<double> result = krig->expectedImprovement(krigPoint);
            for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
                res[i]=result[i];
            }
        }else if(mode==LB){
            std::vector<double> result = krig->lowerBound(krigPoint);
            for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
                res[i]=(-1) * result[i];
            }
            
        }else if(mode == CERTAIN){
            double * result = COCOfunc<MultiObjectiveFunction>::eval(p, krigPoint);
            for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
                res[i]=result[i];
            }
            delete[] result;
        }
        
    }
    virtual void certainEval(double * res, const RealVector &p, const RealVector krigPoint) const{
        RealVector result = UncertainFunc<MultiObjectiveFunction>::certainEval(p, krigPoint);
        for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
            res[i]=result(i);
        }
    }
    virtual void cheaterEval(double * res, const RealVector &p) const{
        double * result = COCOfunc<MultiObjectiveFunction>::cheater_eval(p);
        for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
            res[i]=result[i];
        }
        delete[] result;
    }
    
  
    RealVector eval( const SearchPointType & p) const{
        double result[this->m_numberOfObjectives];
        UncertainFunc<MultiObjectiveFunction>::eval(result, p, false);
        RealVector value(this->m_numberOfObjectives);
        for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
            value(i)=result[i];
            //std::cout << "res " << result[i] << std::endl;
        }
        return value;
    }
    
    RealVector certainEval( const SearchPointType & p) const{
        double result[this->m_numberOfObjectives];
        UncertainFunc<MultiObjectiveFunction>::eval(result, p, true);
        RealVector value(this->m_numberOfObjectives);
        for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
            value(i)=result[i];
        }
        return value;
    }
    

    std::string name() const{
        if(mode==MEAN){
            return "Mean prediction of "+COCOfunc<MultiObjectiveFunction>::name();
        }else if(mode==PoI){
            return "PoI of "+COCOfunc<MultiObjectiveFunction>::name();
        }else if(mode==ExI){
            return "ExI of "+COCOfunc<MultiObjectiveFunction>::name();
        }else if(mode==LB){
            return "Lower bounds of "+COCOfunc<MultiObjectiveFunction>::name();
        }else if(mode==CERTAIN){
            return COCOfunc<MultiObjectiveFunction>::name();
        }
        return "unspecified";
    }

private:
    UncertainMode mode;
    void doInit(const COCOfunc_m * f1, int k, int h, double uncertaintyThreshold,
    double historyThreshold, UncertainMode mode, SampleMode sampMode, int minK=-1){
        if( (mode==PoI || mode==ExI) &&
               (uncertaintyThreshold < std::numeric_limits<double>::infinity() ||
                    historyThreshold < std::numeric_limits<double>::infinity()) ){
            throw std::invalid_argument("thresholds not implemented for improvement stuff");
        }else if( mode == CERTAIN && (uncertaintyThreshold>0 || historyThreshold>0)){
            throw std::invalid_argument("certain evaluation must have all thresholds as 0");
        }
        COCOfunc<MultiObjectiveFunction>::init(f1->getProblem(), f1->getCheaterProblem(),
                f1->getTimeWindow(), f1->isNoisy(), 0, f1->getTransform());
        if(this->m_numberOfObjectives==1){
            throw std::invalid_argument("initialising multi-objective function with single-objective one");
        }
        this->mode = mode;
    }
};

#endif /* UNCERTAINFUNC_M_H */

