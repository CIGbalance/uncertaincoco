#include "Suite.h"
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */

using namespace shark;

Suite::Suite(char* suite_name, int budget_multiplier,
        int batches, int cbatch, const char* dimension_string,
        char* observer_name, int independent_restarts, int batchOffset){
    //Set up coco
    coco_set_log_level("info");
    
    this->suite = coco_suite(suite_name, "", dimension_string);//http://numbbo.github.io/coco-doc/C/#suite-parameters
    this->cbatch = cbatch;
    this->batches = batches;
    this->observer_name = observer_name;
    this->suite_name = suite_name;
    this->budget_multiplier = budget_multiplier;
    this->independent_restarts = independent_restarts;
    this->batchOffset = batchOffset;
}


void Suite::run(CocoOptimiser *algo, int timeWindow, Transform trans, bool lido=true){
    std::string obs_string = algo->getObserverOptions(this->suite_name, this->cbatch, this->batches,
            timeWindow, trans);
    std::string obs_stringRW = algo->getObserverOptionsRW(this->suite_name, this->cbatch, this->batches,
            timeWindow, trans);
    char * observer_options = const_cast <char *>(obs_string.c_str());
    char * observer_optionsRW = const_cast <char *>(obs_stringRW.c_str());
    observer_options = coco_strdupf(observer_options);
    observer_optionsRW = coco_strdupf(observer_optionsRW);
    coco_observer_t * observer = coco_observer(this->observer_name, observer_options);
    coco_observer_t * rw_observer = coco_observer("rw", observer_optionsRW);
  
    coco_problem_t * problem;
    coco_problem_t * cheater_problem;
    coco_problem_t * problem2;
    coco_problem_t * cheater_problem2;
    bool isNoisy = false;
    double penalty=0;
    std::string sn(this->suite_name);
    if(sn=="rw-top-trumps" || sn=="rw-top-trumps-biobj" || sn=="rw-gan-mario" || sn=="rw-gan-mario-biobj"){
        isNoisy = true;
    }
    int total_problems = coco_suite_get_number_of_problems(this->suite);
    int errors;
    for(int problem_index=0; problem_index<total_problems; problem_index++){
    //while((problem=coco_suite_get_next_problem(this->suite, observer))!=NULL){
        //int problem_index = coco_problem_get_suite_dep_index(problem);
        if((problem_index) % this->batches !=this->cbatch){
            continue;
        }
        problem = coco_suite_get_problem(this->suite, problem_index);
        if(!problem){
            continue;
        }
        problem = coco_problem_add_observer(problem, observer);
        problem = coco_problem_add_observer(problem, rw_observer);

        cheater_problem = coco_suite_get_problem(this->suite, problem_index);
        COCOfunc_m fun_m;
        COCOfunc_s fun_s;
        if(coco_problem_get_number_of_objectives(problem)>1){
            fun_m.init(problem, cheater_problem, timeWindow, isNoisy, penalty, trans);
        }else{
            fun_s.init(problem, cheater_problem, timeWindow, isNoisy, penalty, trans);
        }
        
        COCOfunc_m fun_m2;
        COCOfunc_s fun_s2;
        if(this->batchOffset!=0){
            problem2 = coco_suite_get_problem(this->suite, problem_index+this->batchOffset);
            cheater_problem2 = coco_suite_get_problem(this->suite, problem_index+this->batchOffset);
            if(coco_problem_get_number_of_objectives(problem2)>1){
                fun_m2.init(problem2, cheater_problem2, timeWindow, isNoisy, penalty, trans);
            }else{
                fun_s2.init(problem2, cheater_problem2, timeWindow, isNoisy, penalty, trans);
            }   
        }
                           
        int dimension = coco_problem_get_dimension(problem);
        int budget = dimension * this->budget_multiplier;
        int evals_done = coco_problem_get_evaluations(problem) + coco_problem_get_evaluations_constraints(problem);
        int evaluations_remaining = budget - evals_done;
        
        std::string folder = std::string(coco_observer_get_result_folder(observer)) + "/fid_" + coco_problem_get_id(problem);        
        errors = system(("test -d " +folder + " || mkdir -p " + folder).c_str());
        std::cout << "Folder created " << errors << std::endl; 
        
        algo->setFolder(folder);
        
        
        for(int r=0; r<this->independent_restarts+1; r++){
            if(coco_problem_get_number_of_objectives(problem)>1){
                if(this->batchOffset!=0){
                    algo->init(fun_m, fun_m2);  
                }else{
                    algo->init(fun_m);                    
                }
            }else{
                if(this->batchOffset!=0){
                    algo->init(fun_s, fun_s2);  
                }else{
                    algo->init(fun_s);                    
                }
            }
            evals_done = coco_problem_get_evaluations(problem) + coco_problem_get_evaluations_constraints(problem);
            evaluations_remaining = budget - evals_done;
            if(evaluations_remaining<=0 ||
                    (coco_problem_final_target_hit(problem) && coco_problem_get_number_of_constraints(problem)==0)){
                break;
            }
            if(coco_problem_get_number_of_objectives(problem)>1){
                algo->run(fun_m, budget);
                fun_m.signalArchiveReset();
            }else{
                algo->run(fun_s, budget);
                fun_s.signalArchiveReset();
            }
            
            evals_done = coco_problem_get_evaluations(problem) + coco_problem_get_evaluations_constraints(problem);
            evaluations_remaining = budget - evals_done;
            if(evaluations_remaining<=0 ||
                    (coco_problem_final_target_hit(problem) && coco_problem_get_number_of_constraints(problem)==0)){
                break;
            }
            
        }
        algo->spillBeans();
        std::string file = folder + "/fevals.txt";
        
        if(coco_problem_get_number_of_objectives(problem)>1){
            fun_m.getArchive().spillBeans(file);
        }else{
            fun_s.getArchive().spillBeans(file);
        }
        problem = coco_problem_remove_observer(problem, rw_observer);
        problem = coco_problem_remove_observer(problem, observer);
        coco_problem_free(problem);
        coco_problem_free(cheater_problem);
    }
    
    
    if(lido){
        std::string folder = std::string(coco_observer_get_result_folder(observer));

        std::cout << ("zip -r "+folder+".zip "+folder+"/*").c_str() << std::endl;
        errors = system(("zip -r "+folder+".zip "+folder+"/*").c_str());
        errors = system(("mv "+folder+".zip /data/autoScratch/weekly/eex642/").c_str());
        errors = system(("rm -rf "+folder+"/*").c_str());
        std::cout << "Zipping and moving " << errors << std::endl; 

    }
      
    //coco_problem_free(problem);
    //coco_problem_free(cheater_problem);
    coco_observer_free(observer);
    coco_suite_free(this->suite);
    coco_free_memory(observer_options);
}