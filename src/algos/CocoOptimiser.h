/* 
 * File:   CocoOptimiser.h
 * Author: thehedgeify
 *
 * Created on 22 June 2018, 20:54
 */

#ifndef COCOOPTIMISER_H
#define COCOOPTIMISER_H
#include <shark/Algorithms/AbstractOptimizer.h>
#include <string>

#include "../COCOinterface/COCOfunc.h"
#include "../functions/COCOfunc_m.h"
#include "../functions/COCOfunc_s.h"

#include <iostream>

using namespace shark;
class CocoOptimiser: public AbstractOptimizer<RealVector, RealVector, RealVector>{

public:
    void init(ObjectiveFunctionType & function){
        if(this->folder.empty()){
            throw std::invalid_argument("output folder not set");
        }
        if(function.numberOfObjectives()>1){
            COCOfunc_m& fun = dynamic_cast<COCOfunc_m&>(function);
            init(fun);
        }else{
            COCOfunc_s& fun = dynamic_cast<COCOfunc_s&>(function);
            init(fun);
        }
    }
    
    void init(ObjectiveFunctionType & function, ObjectiveFunctionType & function2){
        if(this->folder.empty()){
            throw std::invalid_argument("output folder not set");
        }
        if(function.numberOfObjectives()>1){
            COCOfunc_m& fun = dynamic_cast<COCOfunc_m&>(function);
            COCOfunc_m& fun2 = dynamic_cast<COCOfunc_m&>(function2);
            init(fun, fun2);
        }else{
            COCOfunc_s& fun = dynamic_cast<COCOfunc_s&>(function);
            COCOfunc_s& fun2 = dynamic_cast<COCOfunc_s&>(function2);
            init(fun, fun2);
        }
    }
    
    void step(ObjectiveFunctionType const& function){
        if(function.numberOfObjectives()>1){
            COCOfunc_m const& fun = dynamic_cast<COCOfunc_m const&>(function);
            step(fun);
        }else{
            COCOfunc_s const& fun = dynamic_cast<COCOfunc_s const&>(function);
            step(fun);
        }
    }
       
    virtual void spillBeans()=0;

    virtual void init(COCOfunc_s& function)=0;
    virtual void step(COCOfunc_s const& function) = 0;
    virtual void laststep(COCOfunc_s const& function) = 0;
    virtual bool converged(COCOfunc_s const& function) = 0;
    
    virtual void init(COCOfunc_s& function, COCOfunc_s& function2)=0;

    virtual void init(COCOfunc_m& function)=0;
    virtual void step(COCOfunc_m const& function) = 0;
    virtual void laststep(COCOfunc_m const& function) = 0;   
    virtual bool converged(COCOfunc_m const& function) = 0;
    
    virtual void init(COCOfunc_m& function, COCOfunc_m& function2)=0;

        
    std::string getObserverOptions(char * suite_name, int cbatch, int batches, int timeWindow, Transform trans){
        std::string obs_string = std::string("result_folder: ")+
                this->name +"_on_" + suite_name + "_t" + std::to_string(static_cast<long long>(trans)) +
                "_b" + std::to_string(static_cast<long long>(cbatch)) +"of" +
                std::to_string(static_cast<long long>(batches)) +  "_tw" + std::to_string(static_cast<long long>(timeWindow)) + " " +
               "algorithm_name: "+ this->name + " " +
               "algorithm_info: \"" + this->info +"\"";
        return obs_string;
    }
    std::string getObserverOptionsRW(char * suite_name, int cbatch, int batches, int timeWindow, Transform trans){
        std::string obs_string = std::string("result_folder: ")+
                this->name +"_on_" + suite_name + "_t" + std::to_string(static_cast<long long>(trans)) +
                "_b" + std::to_string(static_cast<long long>(cbatch)) +"of" +
                std::to_string(static_cast<long long>(batches)) +  "_tw" + std::to_string(static_cast<long long>(timeWindow)) + "_rw " +
               "log_only_better: 0 log_variables: all";
        return obs_string;
    }
    
    std::string getName()const{
        return this->name;
    }
    
    void run(COCOfunc_s const& function, int budget){
        int evalsRemaining=budget- (function.getEvaluationConstraints() + function.getEvaluations());
        std::cout << std::endl << "Algorithm starts: budget " << evalsRemaining << std::endl;
        while(evalsRemaining>0){
            evalsRemaining = budget - (function.getEvaluationConstraints() + function.getEvaluations());
            bool conv = converged(function);
            if((function.finalTargetHit() && function.getNumberOfConstraints()==0) || conv){
               std::cout << "Algorithm stops: finalTarget " << (function.finalTargetHit() && function.getNumberOfConstraints()==0)
                        << " converged " << conv << std::endl;
                break;
            }
            step(function);
        }
        laststep(function);
    }
       
    void run(COCOfunc_m const& function, int budget){
        int evalsRemaining=budget- (function.getEvaluationConstraints() + function.getEvaluations());
        std::cout << std::endl << "Algorithm starts: budget " << evalsRemaining << std::endl;
        while(evalsRemaining>0){
            evalsRemaining = budget - (function.getEvaluationConstraints() + function.getEvaluations());
            bool conv = converged(function);
            if((function.finalTargetHit() && function.getNumberOfConstraints()==0) || conv){
               std::cout << "Algorithm stops: finalTarget " << (function.finalTargetHit() && function.getNumberOfConstraints()==0)
                        << " converged " << conv << std::endl;
                break;
            }
            step(function);
        }
        laststep(function);
    }
       
    void setFolder(std::string folder){
        this->folder = folder;
    }
    
    std::string getFolder(){
        return this->folder;
    }
    

protected:
    std::string name;
    std::string info;
    std::string folder;
};

#endif /* COCOOPTIMISER_H */

