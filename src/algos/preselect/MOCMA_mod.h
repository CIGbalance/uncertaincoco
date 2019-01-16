#ifndef SHARK_ALGORITHMS_DIRECT_SEARCH_MOCMA_mod
#define SHARK_ALGORITHMS_DIRECT_SEARCH_MOCMA_mod

// MOO specific stuff

#include <shark/Algorithms/DirectSearch/MOCMA.h>
#include <shark/Algorithms/AbstractMultiObjectiveOptimizer.h>
#include <shark/Algorithms/DirectSearch/CMA/CMAIndividual.h>
#include "../preselect/AbstractPreselecter.h"

using namespace shark;

template<typename Indicator=HypervolumeIndicator>
class IndicatorBasedMOCMA_mod: public IndicatorBasedMOCMA<Indicator>, public AbstractPreselecter<RealVector>{
public:
    typedef AbstractMultiObjectiveOptimizer<RealVector>::SearchPointType SearchPointType;
    typedef CMAIndividual<RealVector> IndividualType;
    typedef AbstractMultiObjectiveOptimizer<RealVector>::ObjectiveFunctionType ObjectiveFunctionType;
    
    IndicatorBasedMOCMA_mod(DefaultRngType& rng= Rng::globalRng):IndicatorBasedMOCMA<Indicator>::IndicatorBasedMOCMA(rng), pe_rng(&rng){
        mu_ps() = 1.5*IndicatorBasedMOCMA<Indicator>::mu();
    }
    
    std::string name() const {
        return "MOCMA_mod";
    }
    
    std::size_t mu_ps()const{
            return m_mu_ps;
    }
    std::size_t& mu_ps(){
            return m_mu_ps;
    }
    
    using IndicatorBasedMOCMA<Indicator>::init;
    
    virtual void init(ObjectiveFunctionType& function){
        IndicatorBasedMOCMA<Indicator>::init(function);
    }
    
    void init( 
            ObjectiveFunctionType& function, 
            std::vector<SearchPointType> const& initialSearchPoints
    ){
        IndicatorBasedMOCMA<Indicator>::init(function, initialSearchPoints);
        m_mu_ps = mu_ps();
    }
    
    void step_presel(ObjectiveFunctionType const& function, ObjectiveFunctionType const& function_pred ) {
        std::vector<IndividualType> offspring = generateOffspring(mu_ps());
        PenalizingEvaluator penalizingEvaluator;

        penalizingEvaluator( function_pred, offspring.begin(), offspring.end() );                     
        pe_selection( offspring, IndicatorBasedMOCMA<Indicator>::mu());
        //partition the selected individuals to the front and remove the unselected ones
        std::partition(offspring.begin(), offspring.end(),IndividualType::IsSelected);
        offspring.erase(offspring.begin()+IndicatorBasedMOCMA<Indicator>::mu(),offspring.end());

        penalizingEvaluator( function, offspring.begin(), offspring.end() );              
        IndicatorBasedMOCMA<Indicator>::updatePopulation(offspring);
    }
    
    
private:
    std::size_t m_mu_ps;

    std::vector<IndividualType> generateOffspring(std::size_t number)const{
        std::cout << number << std::endl;
        std::vector<IndividualType> offspring(number);
        double frac = (double)mu_ps()/IndicatorBasedMOCMA<Indicator>::mu();
        for(std::size_t i = 0; i != number; ++i){
                std::size_t parentId = i/frac;
                offspring[i] = IndicatorBasedMOCMA<Indicator>::m_parents[parentId];
                offspring[i].mutate(*pe_rng);
                offspring[i].parent() = parentId;
        }
        return offspring;
    }
    DefaultRngType* pe_rng;
    IndicatorBasedSelection<Indicator> pe_selection;
};

namespace shark{
    typedef IndicatorBasedMOCMA_mod< HypervolumeIndicator > MOCMA_mod;
    typedef IndicatorBasedMOCMA_mod< AdditiveEpsilonIndicator > EpsilonMOCMA_mod;
    typedef IndicatorBasedMOCMA_mod< LeastContributorApproximator< FastRng, HypervolumeCalculator > > ApproximatedVolumeMOCMA_mod;

}

#endif
