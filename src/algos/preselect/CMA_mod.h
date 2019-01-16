#ifndef SHARK_ALGORITHMS_DIRECT_SEARCH_CMA_mod_H
#define SHARK_ALGORITHMS_DIRECT_SEARCH_CMA_mod_H

#include <shark/Algorithms/DirectSearch/CMA.h>
#include "../preselect/AbstractPreselecter.h"
#include <shark/Algorithms/DirectSearch/Operators/Evaluation/PenalizingEvaluator.h>
#include <shark/Algorithms/DirectSearch/FitnessExtractor.h>
#include <shark/Algorithms/DirectSearch/Operators/Selection/ElitistSelection.h>

using namespace shark;

class CMA_mod : public CMA, public AbstractPreselecter<double>{
    public:             
        SHARK_EXPORT_SYMBOL CMA_mod(DefaultRngType& rng = Rng::globalRng):
        CMA::CMA(rng),pe_rng(&rng){
            m_userSetPsLambda = false;
        }
	/// \brief From INameable: return the class name.
	std::string name() const
	{ return "CMA-ES modified"; }
        
        virtual SHARK_EXPORT_SYMBOL void init(CMA::ObjectiveFunctionType& function){
            CMA_mod::init(function, function.proposeStartingPoint());
        }
        
        virtual SHARK_EXPORT_SYMBOL void init(CMA::ObjectiveFunctionType& function, CMA::SearchPointType const& p) {
            CMA::init(function, p);
            std::size_t lambda_ps = m_userSetPsLambda? m_ps_lambda:CMA_mod::suggestPsLambda(this->lambda());
            RANGE_CHECK(this->lambda() < lambda_ps);
            m_ps_lambda = lambda_ps;
        }
        
        virtual SHARK_EXPORT_SYMBOL void init( 
            CMA::ObjectiveFunctionType& function, 
            CMA::SearchPointType const& initialSearchPoint,
            std::size_t lambda,
            std::size_t mu,
            std::size_t lambda_ps,
            double initialSigma,				       
            const boost::optional< RealMatrix > & initialCovarianceMatrix = boost::optional< RealMatrix >()
	){
            CMA::init(function, initialSearchPoint, lambda, mu, initialSigma, initialCovarianceMatrix);
            RANGE_CHECK(this->lambda() < lambda_ps);
            setLambdaPs(lambda_ps);
            m_ps_lambda = lambda_ps;
        }
        
        virtual SHARK_EXPORT_SYMBOL void init( 
            CMA::ObjectiveFunctionType& function, 
            CMA::SearchPointType const& initialSearchPoint,
            std::size_t lambda,
            std::size_t mu,
            double initialSigma,				       
            const boost::optional< RealMatrix > & initialCovarianceMatrix = boost::optional< RealMatrix >()
	){
            throw std::invalid_argument("use initialisation with preselect lambda");
        }
        
        void step_presel(CMA::ObjectiveFunctionType const& function, CMA::ObjectiveFunctionType const& function_pred){
            std::vector<IndividualType> offspring = generateOffspring(m_ps_lambda); //generate more offspring
            ElitistSelection<FitnessExtractor> selection;
            PenalizingEvaluator penalizingEvaluator;

            //Evaluate and select lambda using pred function
            penalizingEvaluator( function_pred, offspring.begin(), offspring.end() );
            std::vector< IndividualType > selectedOffspring_pre( lambda() );
            /*for(unsigned int i=0; i<offspring.size(); i++){
                std::cout << "pre " << offspring[i].penalizedFitness()<<std::endl;
            }*/
            selection(offspring.begin(),offspring.end(),selectedOffspring_pre.begin(), selectedOffspring_pre.end());
            /*for(unsigned int i=0; i<selectedOffspring_pre.size(); i++){
                std::cout << "step1 " << selectedOffspring_pre[i].penalizedFitness()<<std::endl;
            }*/
            
            //Select from preselected offspring
            penalizingEvaluator( function, selectedOffspring_pre.begin(), selectedOffspring_pre.end() );
            std::vector< IndividualType > selectedOffspring( mu() );
            selection(selectedOffspring_pre.begin(), selectedOffspring_pre.end(), selectedOffspring.begin(), selectedOffspring.end());
            /*for(unsigned int i=0; i<selectedOffspring.size(); i++){
                std::cout << "step2 " << selectedOffspring[i].penalizedFitness()<<std::endl;
            }
            std::cout << std::endl;*/
            
            //Update final population
            updatePopulation(selectedOffspring);
        }
        
        void setLambdaPs(std::size_t lambdaPs){
            m_ps_lambda = lambdaPs;
            m_userSetPsLambda = true;
        }
        std::size_t lambdaPs()const{
            return m_ps_lambda;
        }
        static std::size_t suggestPsLambda( std::size_t lambda){
            return lambda*2;
        }
    protected:
        SHARK_EXPORT_SYMBOL std::vector<IndividualType> generateOffspring(std::size_t lambda) const{
            std::vector< IndividualType > offspring( lambda );
            mutationDistribution.covarianceMatrix() = CMA::covarianceMatrix();
            mutationDistribution.update();
            for( std::size_t i = 0; i < offspring.size(); i++ ) {
                    MultiVariateNormalDistribution::result_type sample = mutationDistribution(*pe_rng);
                    offspring[i].chromosome() = sample.second;
                    offspring[i].searchPoint() = CMA::mean() + CMA::sigma() * sample.first;
            }
            return offspring;
        }
        
    private:
        std::size_t m_ps_lambda;
        bool m_userSetPsLambda;
        mutable MultiVariateNormalDistribution mutationDistribution;
        DefaultRngType* pe_rng;
};

#endif