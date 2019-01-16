/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SafeComparator.h
 * Author: thehedgeify
 *
 * Created on 22 August 2018, 11:11
 */

#ifndef SAFECOMPARATOR_H
#define SAFECOMPARATOR_H

#include "UncertainIndividual.h"
#include "UncertainCMAIndividual.h"

#include <shark/Algorithms/DirectSearch/ParetoDominanceComparator.h>
#include <boost/math/distributions/normal.hpp>
#include <iostream>
using boost::math::normal;
using namespace shark;

enum SelectionStrategy{
    NONE = -1,
    NO_UNCERTAINTY = 1,
    NO_OVERLAP = 2,
    PREDICTION = 3,
    MO=4,
};

template<typename Extractor, class FitnessType, class Chromosome = RealVector>
class SafeComparator{
    public:

        typedef UncertainIndividual<FitnessType, Chromosome> IndividualType;
        SafeComparator(SelectionStrategy strat, double alpha=-1){
            this->strat = strat;
            this->alpha=alpha;
            if(strat==NO_OVERLAP && alpha<0){
                throw std::invalid_argument("overlap strategy needs threshold");
            }
        }
        
        
        //For overlap, potentially domineering one needs to be sent first
        template <class Individual>
        bool compSafe(const Individual & A, const Individual& B,
                SelectionStrategy strat=NONE) {
            if(strat==NONE){
                strat=this->strat;
            }
            switch(strat){
                case NO_UNCERTAINTY:
                    if(Individual::IsUncertain(A) || Individual::IsUncertain(B)){
                        return false;
                    }else{
                        return true;
                    }
                case PREDICTION:
                    return true;
                case NO_OVERLAP:
                    return !overlapChecker(A,B);
                case MO:
                    throw std::invalid_argument("MO comparator not implemented");
                case NONE:
                    throw std::invalid_argument("No comparison strategy given (none)");
            }
            throw std::invalid_argument("Comparison strategy unknown");
        }
        
        
        template <class Individual>
        int operator()( const Individual& A, const Individual& B ) {
            int val = pdc(A,B);
            return checkUncertainty(A, B, val);
            //return val;
        }
        
        int operator()( const UncertainIndividual<double>& A, const UncertainIndividual<double>& B){
            Extractor e;
            int val = pdc.TRADE_OFF;
            if(e(A) < e(B)){
                val = pdc.A_STRICTLY_DOMINATES_B;
            }else if(e(A) == e(B)){
                val = pdc.A_EQUALS_B;
            }else if(e(A) > e(B)){
                val = pdc.B_STRICTLY_DOMINATES_A;
            }
            return checkUncertainty(A, B, val);
        }
        
        
    protected:
        template <class Individual>
        int checkUncertainty(const Individual& A, const Individual& B,
                int priorRanking){
            bool safe = false;
            if(priorRanking==pdc.A_STRICTLY_DOMINATES_B || priorRanking==pdc.A_WEAKLY_DOMINATES_B){
                safe = compSafe(A, B);
            }else if(priorRanking==pdc.A_EQUALS_B){
                if(this->strat!=PREDICTION){
                    safe = compSafe(A,B,NO_UNCERTAINTY);
                }
            }else if(priorRanking==pdc.B_STRICTLY_DOMINATES_A || priorRanking== pdc.B_WEAKLY_DOMINATES_A){
                safe = compSafe(B, A);
            }
            if(safe){
                return priorRanking;
            }else{
                return pdc.TRADE_OFF;
            }
        }
        
        bool overlapChecker(const UncertainIndividual<double> & A, const UncertainIndividual<double> & B){
            Extractor e;
            double qA;
            if(A.uncertainty()[0]>0){
                normal a(e(A), A.uncertainty()[0]);
                qA = quantile(a, 1-(alpha/2));
            }else{
                qA = e(A);
            }
            double qB;
            if(B.uncertainty()[0]>0){
                normal b(e(B), B.uncertainty()[0]);
                qB = quantile(b, alpha/2);
            }else{
                qB = e(B);
            }
            //std::cout << " (quantiles " << qA << " vs. " << qB << ")";
            if(qA >=  qB){
                return true;
            }else{
                return false;
            }
                       
        }
        
        
        template<class Individual>
        bool overlapChecker(const Individual & A, const Individual& B){
            Extractor e;
            //std::cout << A.uncertainty().size() << std::endl;
            for(unsigned int i=0; i<A.uncertainty().size(); i++){
                double qA;
                if(A.uncertainty()[i]>0){
                    normal a(e(A)[i], A.uncertainty()[i]);
                    qA = quantile(a, 1-(alpha/2));
                }else{
                    qA = e(A)[i];
                }
                double qB;
                if(B.uncertainty()[i]>0){
                    normal b(e(B)[i], B.uncertainty()[i]);
                    qB = quantile(b, alpha/2);
                }else{
                    qB = e(B)[i];
                }
                //std::cout << " (quantiles " << qA << " vs. " << qB << ")";
                if(qA >=  qB){
                    return true;
                }
            }
            return false;
        }
        
    
    private:
        SelectionStrategy strat;
        double alpha;
        ParetoDominanceComparator<Extractor> pdc;
    
};


#endif /* SAFECOMPARATOR_H */

