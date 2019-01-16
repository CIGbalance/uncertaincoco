/**
 * \file contains abstract wrapper for COCO functions
*/
#ifndef COCOfunc_H
#define COCOfunc_H
#include <string>
#include <stdexcept>
#include <cmath>

#include <shark/ObjectiveFunctions/AbstractObjectiveFunction.h>
#include <shark/ObjectiveFunctions/BoxConstraintHandler.h>

#include "../utils/Archive.h"
#include "../utils/ihs.h"
#include "coco.h"
#include <math.h>      //transforms

using namespace shark;

/// Function transformation options
/** Can be used to transform the function values of the referenced function.
 *  
 * Passed to class in COCOfunc<T>::init. Used in COCOfunc<T>::eval and COCOfunc<T>::cheater_eval
 */
enum Transform{
    NO = 0, ///< No transformation
    LOG = 1, ///< ln(y). Will interrupt run for negative y
    INVERSE = 2, ///< -1 /y
};

/// Wrapper for functions in COCO

/** This class provides a convenient interface to a function in the COCO framework.
 *  It allows access to various of the original functionalities and adds features such as
 *  logging and cheater evaluations.
 * 
 *  Cheater evaluations are evaluations of the COCO function that are not counted through the
 *  COCO framework. They are never accessible to the optimisation algorithms and are
 *  solely logged along with predicted values in order to be able to compute errors.
 *  \relates Transform
 */
template <typename T>
class COCOfunc: virtual T{
public:
   
    /// Initialises Function properties
    
    /**
     * Saves basic parameters and sets up functionality
     * @param problem Reference to the original COCO function
     * @param cheater_problem Reference to separate instance of same COCO function
     * @param timeWindow Time window for convergence detection (in number of function evaluations)
     * @param m_isNoisy Function noisy?
     * @param penalty Penalty added to function values if outside region of interest
     * @param trans Transformation option (::Transform) for function values
     */
    void init(coco_problem_t *problem, coco_problem_t * cheater_problem,
            int timeWindow, bool m_isNoisy, double penalty,
            Transform trans){
        this->problem = problem;
        this->m_id = coco_problem_get_id(this->problem);
        this->m_numberOfVariables = coco_problem_get_dimension(this->problem);
        this->m_numberOfObjectives = coco_problem_get_number_of_objectives(this->problem);
        this->m_max = coco_problem_get_largest_values_of_interest(this->problem);
        this->m_min = coco_problem_get_smallest_values_of_interest(this->problem);
        std::string test(coco_problem_get_name(this->problem));
        this->m_name = test;
        this->m_isNoisy=m_isNoisy;
        this->m_numberOfConstraints = coco_problem_get_number_of_constraints(this->problem);
        this->m_penalty= penalty;
        this->m_timeWindow=timeWindow;
        this->archive.init(this->m_numberOfObjectives, timeWindow);
        this->cheater_problem = cheater_problem;
        this->trans = trans;
        this->dep_ind =  coco_problem_get_suite_dep_index(this->problem);
    }
    
        
    /// Evaluates Function (Passed from COCO)
    /**
     * Evaluates the COCO function. Transformation (::Transform) added as initialised in COCOfunc<T>::init
     * 
     * @param point in search space
     * @return function value
     */
    double * eval(RealVector point) const{
        if(point.size()!=this->m_numberOfVariables){
            throw std::invalid_argument("wrong input size");
        }
        double* x = point.storage();
        double* y = new double[this->m_numberOfObjectives];
        coco_evaluate_function(problem, x, &y[0]);
        if(!this->inRegionOfInterest(point)){
            for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
                y[i] +=this->m_penalty;
            }
        }
        if(trans!=NO){
            for(unsigned int i=0; i< this->m_numberOfObjectives; i++){
                if(trans==LOG){
                    if(y[i]<=0){
                        throw std::invalid_argument("log transformation for negative values!");
                    }
                    y[i] = log(y[i]);
                }else if(trans==INVERSE){
                    y[i] = -(1)/y[i];
                }
            }
        }
        std::vector<double> y_vec(y, y+this->m_numberOfObjectives);
        this->archive.addPoint(point, y_vec);
        return y;
    }
    
       double * eval(RealVector point, RealVector krigPoint) const{
        if(point.size()!=this->m_numberOfVariables){
            throw std::invalid_argument("wrong input size");
        }
        double* x = point.storage();
        double* y = new double[this->m_numberOfObjectives];
        coco_evaluate_function(problem, x, &y[0]);
        if(!this->inRegionOfInterest(point)){
            for(unsigned int i=0; i<this->m_numberOfObjectives; i++){
                y[i] +=this->m_penalty;
            }
        }

        if(trans!=NO){
            for(unsigned int i=0; i< this->m_numberOfObjectives; i++){
                if(trans==LOG){
                    if(y[i]<=0){
                        throw std::invalid_argument("log transformation for negative values!");
                    }
                    y[i] = log(y[i]);
                }else if(trans==INVERSE){
                    y[i] = -(1)/y[i];
                }
            }
        }
        std::vector<double> y_vec(y, y+this->m_numberOfObjectives);
        this->archive.addPoint(krigPoint, y_vec);
        return y;
    }
    
    
    /// Evaluates Cheater Function (Passed from COCO)
    /**
     * Evaluates the COCO function duplicate. Transformation (::Transform) added as initialised in COCOfunc<T>::init
     * 
     * @param point in search space
     * @return function value
     */
    double * cheater_eval(RealVector point) const{
        if(point.size()!=this->m_numberOfVariables){
            throw std::invalid_argument("wrong input size");
        }
        double* x = point.storage();
        double* y = new double[this->m_numberOfObjectives];
        coco_evaluate_function(this->cheater_problem, x, &y[0]);
        if(trans!=NO){
            for(unsigned int i=0; i< this->m_numberOfObjectives; i++){
                if(trans==LOG){
                    if(y[i]<=0){
                        throw std::invalid_argument("log transformation for negative values!");
                    }
                    y[i] = log(y[i]);
                }else if(trans==INVERSE){
                    y[i] = -(1)/y[i];
                }
            }
        }
        
        return y;
    }
    
        
    /// Suggests a search point within the region of interest
    RealVector proposeStartingPoint()const{
        RealVector point(this->m_numberOfVariables);
        for(unsigned int i=0; i<this->m_numberOfVariables; i++){
            point(i) = (this->m_max[i]-this->m_min[i])*(rand()%100)/100+this->m_min[i];
        }
        return point;
    }
    
    
    /// LHC sample with evaluation
    /**
     * Uses Improved Distributed Hypercube Sampling
     * https://people.sc.fsu.edu/~jburkardt/cpp_src/ihs/ihs.hpp
     * @param point_num Number of points to generated
     * @param seed Random seed
     * @param duplication Restarts of lhs algorithm
     * @return ArchiveData sample result
     */
    
    ArchiveData lhs(int point_num, int seed, int duplication=5)const{
        int *x;// An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
        //m = dim_num, n = point_num
        x = ihs ( m_numberOfVariables, point_num, duplication, seed);
        //i4mat_transpose_print ( m_numberOfVariables, point_num, x, "  X:" );
        std::vector<RealVector> inputs(point_num);
        std::vector<RealVector> labels(point_num);
        for(int j=0; j<point_num; j++){
            RealVector point(m_numberOfVariables);
            for(unsigned int i=0; i<m_numberOfVariables; i++){
                point(i) = x[i+j*m_numberOfVariables]-1;
                point(i) = fabs(m_max[i]-m_min[i])*point(i)/(double)(point_num-1) + m_min[i]; //Rescaling
            }
            inputs[j] = point;
            double * y = COCOfunc::eval(point);
            RealVector values(m_numberOfObjectives);
            for(unsigned int i=0; i<m_numberOfObjectives; i++){
                values(i) = y[i];
            }
            labels[j] = values;
            delete[] y;
        }
        delete[] x;
        ArchiveData sample = createLabeledDataFromRange(inputs, labels);
        return sample;
    }
    
    
    /// LHC sample without evaluation
    /**
     * Uses Improved Distributed Hypercube Sampling
     * https://people.sc.fsu.edu/~jburkardt/cpp_src/ihs/ihs.hpp
     * @param point_num Number of points to generated
     * @param seed Random seed
     * @param duplication Restarts of lhs algorithm
     * @return sample points in search space
     */
    std::vector<RealVector> lhsSample(int point_num, int seed, int duplication=5) const{
        int *x;// An I4MAT is an MxN array of I4's, stored by (I,J) -> [I+J*M].
        //m = dim_num, n = point_num
        x = ihs ( m_numberOfVariables, point_num, duplication, seed);
        //i4mat_transpose_print ( m_numberOfVariables, point_num, x, "  X:" );
        std::vector<RealVector> inputs(point_num);
        for(int j=0; j<point_num; j++){
            RealVector point(m_numberOfVariables);
            for(unsigned int i=0; i<m_numberOfVariables; i++){
                point(i) = x[i+j*m_numberOfVariables]-1;
                point(i) = fabs(m_max[i]-m_min[i])*point(i)/(double)(point_num-1) + m_min[i]; //Rescaling
            }
            inputs[j] = point;
        }
        return inputs;
    }

    /// Returns reference to COCO problem
    coco_problem_t * getProblem()const{
        return problem;
    }
    
    /// Returns reference to cheater COCO problem
    coco_problem_t * getCheaterProblem() const{
        return cheater_problem;
    }
    
    /// Name of the function (Passed from COCO)
    std::string name() const{
        return this->m_name;
    }
    
    /// Dimension of search space (Passed from COCO)
    virtual std::size_t numberOfVariables()const{
        return this->m_numberOfVariables;
    }
    
    /// Dimension of objective spaces (Passed from COCO)
    std::size_t numberOfObjectives()const{
        return this->m_numberOfObjectives;
    }

    /// Number of evaluations (Passed from COCO))
    virtual int getEvaluations()const{
        return coco_problem_get_evaluations(this->problem);
    }

    /// Final target hit? (Passed from COCO)
    int finalTargetHit()const{
        return coco_problem_final_target_hit(this->problem);
    }
       
    /// Check feasibility (Passed from COCO)
    int getEvaluationConstraints()const{
        return coco_problem_get_evaluations_constraints(this->problem);
    }


    /// Number of constraints (Passed from COCO)
    int getNumberOfConstraints()const{
        return this->m_numberOfConstraints;
    }

    
    /// Check if point is in region of interest
    /**
     * In COCO, all functions have a specified region of interest where the optimum is located
     * @param input Point in search space
     * @return true if in region of interest, else false
     */
    bool inRegionOfInterest(RealVector const& input) const{
        for(unsigned int i=0; i<m_numberOfVariables; i++){
            if(input(i) < this->m_min[i] || input(i) > this->m_max[i]){
                return false;
            }
        }
        return true;
    }
    
    /// Lower bound for region of interest (Passed from COCO)
    const double* getLowerBounds()const{
        return this->m_min;
    }

    /// Upper bound for region of interest (Passed from COCO)
    const double * getUpperBounds()const{
        return this->m_max;
    }
    

    /// \deprecated Check if point is feasible (Untested, currently no constraints in COCO
    /**
     * Check if constraints defined in COCO are fulfilled
     * @param input Point in search space
     * @return true if feasible, else false
     */
    bool isFeasible(RealVector const& input) const{
        if(this->m_numberOfConstraints>0){
            double * y = evalConstraint(input);   
            for(unsigned int i=0; i<this->m_numberOfConstraints; i++){
                if(y[i]>0){
                    return false;
                }
            }
        }
        return true;
    }
    
    /// Returns Archive that stores all evaluations
    Archive getArchive()const{
        return this->archive;
    }
    
    /// Returns true if archive converged
    bool converged()const{
        return this->archive.converged();
    }
    
    /// Signals algorithm restart to Archive
    void signalArchiveReset(){
        this->archive.signalReset();
    }
    
    /// Time window for convergence detection
    int getTimeWindow()const{
        return m_timeWindow;
    }


    /// Function noisy?
    bool isNoisy()const{
        return m_isNoisy;
    }
    
    /// Penalty set for points outside region of interest
    double getPenalty()const{
        return m_penalty;
    }
    
    
    /// Returns transformation option used
    Transform getTransform() const{
        return trans;
    }
    
    int getInd()const{
        return dep_ind;
    }
    

protected:
    std::size_t m_numberOfVariables; ///< Search space dimension
    std::size_t m_numberOfObjectives; ///< Objective space dimension

private:  
    
    const char* m_id;
    int dep_ind;
    std::string m_name;
    
    std::size_t m_numberOfConstraints;

    const double * m_min;
    const double * m_max;
    
    coco_problem_t *problem;
    coco_problem_t *cheater_problem;
    mutable Archive archive;
    bool m_isNoisy;
    double m_penalty;
    int m_timeWindow;
    Transform trans;
    
    /// Evaluates constraints at point (Passed from COCO)
    double * evalConstraint(RealVector point) const{
        if(point.size()!=this->m_numberOfVariables){
            throw std::invalid_argument("wrong input size");
        }
        double* x = point.storage();
        double* y = new double[this->m_numberOfConstraints];
        coco_evaluate_constraint(problem, x, &y[0]);
        return y;
    }
    
};


#endif /* COCOfunc_H */

