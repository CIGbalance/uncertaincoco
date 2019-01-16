#ifndef UNCERTAINFUNC_H
#define UNCERTAINFUNC_H

#include "../utils/Kriging.h"
#include "../COCOinterface/COCOfunc.h"

/// Function types
/**
 * Types of functions that can be computed based on the output of COCOfunc<T>::eval
 * 
 * Formulas for computation of PoI, ExI, LB taken from prescreening Emmerich paper (TODO)
 */
enum UncertainMode{
    MEAN = 0, ///< Kriging prediction
    PoI = 1, ///< Probability of Improvement
    ExI = 2, ///< Expected Improvement
    LB = 3, ///< Lower bound
    CERTAIN = 4, ///< COCOfunc<T>::eval original value
};

/// Types of data inclusion
enum SurrMode{
    LOC = 0, ///< Only location
    SURR = 1, ///< Only surrogate function
    BOTH = 2, ///< location and function
};

using namespace shark;

/// Wrapper for functions computed from COCOfunc<T>
/**
 * This class acts as a template for all functions computed based on the actual
 * function evaluations, such as Kriging predictions. The class includes a features
 * that can automatically decide whether to use e.g. the Kriging prediction or the actual
 * function value instead, based on the allowed uncertainty or cross-validated model fidelity.
 * 
 * The function has an Archive separate from COCOfunc<T> that logs all evaluations
 * in addition to the correct values using COCOfunc<T>::cheater_eval
 */
template <typename T>
class UncertainFunc{
public:
    UncertainFunc(){}
    
    /// Initialises Function properties
    
    /**
     * Saves basic parameters and sets up functionality
     * @param f1 Reference to an object of type COCOfunc<T> to handle the evaluations
     * @param k Number of samples for Kriging model
     * @param h Number of crossvalidation samples to compute fidelity estimate
     * @param uncertaintyThreshold Maximum predicted uncertainty allowed before certain evaluation
     * @param historyThreshold Maximum value for fidelity measure
     * @param samp method to sample for the Kriging model (::UncertainMode)
     */
    void init(const COCOfunc<T> * f1,
            int k, int h, double uncertaintyThreshold, double historyThreshold, SampleMode samp, int minK=-1){
        doInit(f1, k, h, uncertaintyThreshold, historyThreshold, samp, minK);
        this->modelFunc = NULL;
        this->surr = LOC;
    }
    
    void init(const COCOfunc<T> * f1, const COCOfunc<T> * modelFunc, SurrMode surr,
            int k, int h, double uncertaintyThreshold, double historyThreshold, SampleMode samp, int minK=-1){
        doInit(f1, k, h, uncertaintyThreshold, historyThreshold, samp, minK);
        this->modelFunc = modelFunc;
        this->surr = surr;
    }
    
    
    void setUncertaintyThreshold(double uncertaintyThreshold){
        this->uncertaintyThreshold=uncertaintyThreshold;
    }
    
    void setHistoryThreshold(double historyThreshold){
        this->historyThreshold = historyThreshold;
    }
    
    virtual void uncertainEval(double * res, Kriging * krig, const RealVector & p, const RealVector krigPoint) const = 0;
    virtual void certainEval(double * res, const RealVector &p, const RealVector krigPoint) const = 0;
    virtual void cheaterEval(double * res, const RealVector &p) const = 0;
    
    void eval(double * res, const RealVector & p, bool forceCertain=false) const{
        RealVector krigPoint;
        if(this->surr==LOC || this->surr==BOTH){
            for(unsigned int i=0; i<f1->numberOfVariables(); i++){
                krigPoint.push_back(p(i));   
            }
        }
        if(this->surr==SURR || this->surr==BOTH){
            RealVector surrEv = surrEval(p);
            for(unsigned int i=0; i<f1->numberOfObjectives(); i++){
                krigPoint.push_back(surrEv(i));
            }
            std::vector<double> pvec;
            pvec.insert(pvec.end(), p.begin(), p.end());
            krigPoints[pvec] = krigPoint;
        }
        
        this->evalCounter++;
        Archive arch = f1->getArchive();
        bool uncertain = false;
        bool fCertain = forceCertain;
        bool notEvaluated = false;
        std::vector<double> data;
        if(arch.getNoPointsCurrent()< this->minK){
            fCertain=true;
            data.resize(3*this->f1->numberOfObjectives(), 0);
            certainEval(res, p, krigPoint);
            data.push_back(2);
        }else{
            ArchiveData arch_data = arch.getSample(std::min(this->k, arch.getNoPointsCurrent()), this->sampMode, krigPoint);
            Kriging krig(arch_data, f1->numberOfVariables(), f1->numberOfObjectives(), -1);
            std::vector<std::pair<double,double>> result = krig.predict(krigPoint);
            std::vector<double> hist = Kriging::getHistory(krigPoint, this->h, std::min(this->k,
                    arch.getNoPointsCurrent()), arch, f1->numberOfObjectives(), this->sampMode);
            for(unsigned int i=0; i< f1->numberOfObjectives(); i++){
                data.push_back(result[i].first);
                data.push_back(result[i].second);
                data.push_back(hist[i]);
                if(result[i].second>this->uncertaintyThreshold ||
                         hist[i]>this->historyThreshold){
                    uncertain=true;
                }
            }
            if(uncertain){
                certainEval(res, p, krigPoint);
                data.push_back(1);
            }else if(forceCertain){
                certainEval(res, p, krigPoint);
                data.push_back(2);
            }else{
                uncertainEval(res, &krig, p, krigPoint);
                data.push_back(0);
            }
        }
        notEvaluated = (data.back()==0); //This point was not evaluated
        for(unsigned int i=0; i< f1->numberOfObjectives(); i++){
            data.push_back(res[i]);
        }
        if(!uncertain && !fCertain){
            double res2[f1->numberOfObjectives()];
            cheaterEval(res2, p);
            for(unsigned int i=0; i< f1->numberOfObjectives(); i++){
                data.push_back(res2[i]);
            }
        }else{
            for(unsigned int i=0; i< f1->numberOfObjectives(); i++){
                data.push_back(res[i]);
            }   
        }
        if(f1->numberOfObjectives()==1){//Recommendation only currently implemented for SO
            double newProgress = data.back();
            if(!prevEvaluated){//previous individual was not evaluated
                if(newProgress < progress){//Individual is better, replace prev
                   this->progress=newProgress;
                   this->points.back() = p;
                }
                //If not improved: do nothing
            }else{//just add new individual
                this->points.push_back(p);//TODO check if this actually a reference
                this->progress = newProgress;

            }
            prevEvaluated = !notEvaluated;//last individual not evaluated
        }
        /*std::cout << "data ";
        for(unsigned int i=0; i<data.size(); ++i)
            std::cout << data[i] << ' ';
        std::cout << std::endl;*/
        this->unArchive.addPoint(krigPoint, data, false);
        return;
    }
    
    
    int getEvaluations() const{
        return this->evalCounter;
    }

    int getRealEvaluations() const{
        return f1->getEvaluations();
    }
    
    Archive getArchive(){
        return unArchive;
    }
    
    bool converged()const{
        return this->unArchive.converged();
    }
    
    std::vector<double> uncertainty(RealVector point) const{
        if(point.size()!=this->f1->numberOfVariables()){
            throw std::invalid_argument("wrong input size");
        }
        std::vector<double> un(this->f1->numberOfObjectives(), 0.0);
        if(this->uncertaintyThreshold < std::numeric_limits<double>::infinity()){
            RealVector krigPoint = point;
            if(this->surr==SURR || this->surr==BOTH){
                std::vector<double> pvec;
                pvec.insert(pvec.end(), point.begin(), point.end());
                krigPoint= krigPoints[pvec];
            }  
            std::vector<double> data = this->unArchive.getPointData(krigPoint);
            if(data[3*f1->numberOfObjectives()]==0){
                for(unsigned int i=0; i< f1->numberOfObjectives(); i++){
                    un[i] = data[i*3+1]; //relates to how points are added when evaluating
                }    
            }  
        }
        return un;
    }
    
    std::vector<double> hist(RealVector point) const{
        if(point.size()!=this->f1->numberOfVariables()){
            throw std::invalid_argument("wrong input size");
        }
        std::vector<double> hist(this->f1->numberOfObjectives(), 0.0);
        if(this->historyThreshold < std::numeric_limits<double>::infinity()){
            RealVector krigPoint = point;
            if(this->surr==SURR || this->surr==BOTH){
                std::vector<double> pvec;
                pvec.insert(pvec.end(), point.begin(), point.end());
                krigPoint= krigPoints[pvec];
            }      
            std::vector<double> data = this->unArchive.getPointData(krigPoint);
            if(data[3*f1->numberOfObjectives()]==0){
                for(unsigned int i=0; i< f1->numberOfObjectives(); i++){
                    hist[i] = data[i*3+2]; //relates to how points are added when evaluating
                    //std::cout << hist[i] << std::endl;
                }    
            }  
        }
        return hist;
    }
    
    RealVector cheaterEval(RealVector point) const{
        if(point.size()!=this->f1->numberOfVariables()){
            throw std::invalid_argument("wrong input size");
        }
        RealVector eval(this->f1->numberOfObjectives());
        RealVector krigPoint = point;
        if(this->surr==SURR || this->surr==BOTH){
            std::vector<double> pvec;
            pvec.insert(pvec.end(), point.begin(), point.end());
            krigPoint= krigPoints[pvec];
        }    
        std::vector<double> data = this->unArchive.getPointData(krigPoint);
        for(unsigned int i=0; i< f1->numberOfObjectives(); i++){
            eval(i) = data[4*f1->numberOfObjectives()+1+i]; //relates to how points are added when evaluating
        } 
        return eval;
    }
    
    RealVector certainEval(RealVector point, RealVector krigPoint) const{
        double* result = f1->eval(point, krigPoint);
        RealVector value(this->f1->numberOfObjectives());
        for(unsigned int i=0; i<this->f1->numberOfObjectives(); i++){
            value(i) = result[i];
        }
        delete[] result;
        return value;
    }
    
    RealVector surrEval(RealVector point) const{
        double* result = modelFunc->eval(point);
        RealVector value(this->modelFunc->numberOfObjectives());
        for(unsigned int i=0; i<this->modelFunc->numberOfObjectives(); i++){
            value(i) = result[i];
        }
        delete[] result;
        return value;
    }
    
    /// Signals algorithm restart to Archive
    void signalArchiveReset(){
        this->unArchive.signalReset();
    }
    
    
    std::vector<RealVector> recommendedPoints(){
        return points;
    }
    
    void pointsClear(){
        points.clear();
    }
        
    
private:
    int k;
    int minK;
    int h;
    mutable int evalCounter;
    const COCOfunc<T> * f1;
    mutable COCOfunc<T> * cheater;
    double uncertaintyThreshold;
    double historyThreshold;
    mutable Archive unArchive;
    SampleMode sampMode;
    mutable double progress;
    mutable bool prevEvaluated;
    mutable std::vector<RealVector> points;
    mutable std::map<std::vector<double>, RealVector> krigPoints;
    
    const COCOfunc<T> * modelFunc;
    SurrMode surr;
        
    void doInit(const COCOfunc<T> * f1,
            int k, int h, double uncertaintyThreshold, double historyThreshold, SampleMode samp, int minK=-1){
                this->k = k;
        if(minK==-1){
            this->minK = k;
        }else{
            this->minK = minK;
        }
        this->h = h;
        this->evalCounter=0;
        this->f1 = f1;
        this->uncertaintyThreshold=uncertaintyThreshold;
        this->historyThreshold = historyThreshold;
        this->sampMode = samp;
        std::vector<bool> which_fit(5*f1->numberOfObjectives()+1);
        std::fill(which_fit.begin(), which_fit.begin()+3*f1->numberOfObjectives()+1, false);
        std::fill(which_fit.begin()+3*f1->numberOfObjectives()+1, which_fit.begin()+4*f1->numberOfObjectives()+1, true);
        std::fill(which_fit.begin()+4*f1->numberOfObjectives()+1, which_fit.end(), false);
        this->unArchive.init(f1->numberOfObjectives(), which_fit, f1->getTimeWindow());
        this->progress = std::numeric_limits<double>::infinity();
        this->prevEvaluated=true;
    }
    
};

#endif /* UNCERTAINFUNC_H */

