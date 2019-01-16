/* 
 * File:   RandomSearch.h
 * Author: vv
 *
 * Created on June 28, 2018, 5:24 PM
 */

#ifndef RANDOMSEARCH_H
#define RANDOMSEARCH_H

#include "../CocoOptimiser.h"


class RandomSearch : public CocoOptimiser{
public:
    RandomSearch(int granularity);
    void init(COCOfunc_s& function);
    void step(COCOfunc_s const& function);
    void init(COCOfunc_m& function);
    void step(COCOfunc_m const& function);
    SolutionType const& solution()const;
    void spillBeans(){}
    bool converged(COCOfunc_s const& function);
    bool converged(COCOfunc_m const& function);
    void laststep(COCOfunc_s const& function){}
    void laststep(COCOfunc_m const& function){}
    
    void init(COCOfunc_s& function, COCOfunc_s& function2){}   
    void init(COCOfunc_m& function, COCOfunc_m& function2){}
private:
    int granularity;
};


#endif /* RANDOMSEARCH_H */

