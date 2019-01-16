#include "Archive.h"
#include "../uncertainty/UncertainSelect.h"

#include <shark/Algorithms/DirectSearch/HypervolumeCalculator.h>
#include <shark/Algorithms/DirectSearch/FitnessExtractor.h>
#include <shark/Algorithms/Trainers/LinearRegression.h>
#include <shark/Algorithms/DirectSearch/Indicators/HypervolumeIndicator.h>

#include <iostream>
#include <vector>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/math/distributions/chi_squared.hpp>

using namespace shark;
using namespace boost::math;


void SubArchive::addPoint(RealVector pointReal, double value, bool replace){
    std::vector<double> values(1);
    values[0] = value;
    this->addPoint(pointReal, values, replace);
}

std::vector<double> SubArchive::getPointData(RealVector pointReal){
    std::vector<double> point(pointReal.size());
    for(unsigned int i=0; i<pointReal.size(); i++){
        point[i] = pointReal(i);
    }
    if(this->archive_data.find(point)==this->archive_data.end()){
        throw std::invalid_argument("Point not found in archive");
    }
    return this->archive_data[point].first;
}

void SubArchive::addPoint(RealVector pointReal, std::vector<double> values, bool replace){
    std::vector<double> point(pointReal.size());
    for(unsigned int i=0; i<pointReal.size(); i++){
        point[i] = pointReal(i);
    }
    if(this->archive_data.find(point)!=this->archive_data.end() && !replace){
        std::vector<double> val(values.size());
        int visits = this->archive_data[point].second;
        for(unsigned int i=0; i<values.size(); i++){
            double prevValue = this->archive_data[point].first[i];
            val[i] = (prevValue*visits + values[i])/(visits+1);
        }
        this->archive_data[point]=std::make_pair(val,visits+1);
    }else{
        this->archive_data[point]=std::make_pair(values,1);
    }
    this->full_archive.push_back(std::make_pair(point, values));
    if(std::count(which_fit.begin(), which_fit.end(), true)>1){
        int counter =0;
        RealVector val(std::count(which_fit.begin(), which_fit.end(), true));
        for(unsigned int i=0; i<values.size(); i++){
            if(this->which_fit[i]){
                val(counter) = values[i];
                if(values[i]>refPoint[counter]){
                    refPoint[counter] = values[i];
                }
                counter ++;
            }
        }
        progress.push_back((-1)*this->hypervolume());
        this->population_mo.push_back(UncertainIndividual<RealVector>(pointReal, val));
        //std::cout << "progress " << progress.back() << std::endl;
    }else{
        for(unsigned int i=0; i<values.size(); i++){
            if(this->which_fit[i]){
                progress.push_back(values[i]);
                this->population_so.push_back(UncertainIndividual<double>(pointReal, values[i]));
                break;
            }
        }
    }    
    this->upToDate=false;
}

int SubArchive::getNoPoints(){
    if(!this->archive_data.empty()){
        return this->archive_data.size();
    }else{
        return 0;
    }
}

void SubArchive::updateData(){
    if(!this->upToDate){       
        //Redo models
        std::vector<RealVector> inputs;
        std::vector<std::vector<double>> labels;
         for(std::map<std::vector<double>,std::pair<std::vector<double>,int>>::iterator it = this->archive_data.begin();
                it != this->archive_data.end(); ++it) {
             std::vector<double> tmp = it->first;
             labels.push_back(tmp);
             RealVector tmp2(tmp.size());
             for(unsigned int i=0; i<tmp.size(); i++){
                 tmp2(i) = tmp[i];
             }
            inputs.push_back(tmp2);
        }
        this->archive = createLabeledDataFromRange(inputs, labels);
        this->upToDate=true;
    }
}

ArchiveData SubArchive::getAll(){
    std::vector<RealVector> sample_x;
    std::vector<RealVector> sample_y;
    for(auto elem : archive_data)
    {
        int n = elem.first.size();
        RealVector tmp4(n);
        for(int j=0; j<n; j++){
            tmp4(j) = elem.first[j];
        }
        sample_x.push_back(tmp4);
        int m = elem.second.first.size();
        RealVector tmp3(m);
        for(int j=0; j<m; j++){
            tmp3(j) = elem.second.first[j];
        }
        sample_y.push_back(tmp3);
    }
    ArchiveData sample = createLabeledDataFromRange(sample_x, sample_y);
    return sample;
}

ArchiveData SubArchive::getRecentSample(int k){
    std::vector<RealVector> sample_x;
    std::vector<RealVector> sample_y;
    for(unsigned int i=full_archive.size()-1; i>=full_archive.size()-k; i--)
    {
        std::pair<std::vector<double>, std::vector<double>> elem = full_archive[i];
        int n = elem.first.size();
        RealVector tmp4(n);
        for(int j=0; j<n; j++){
            tmp4(j) = elem.first[j];
        }
        sample_x.push_back(tmp4);
        int m = elem.second.size();
        RealVector tmp3(m);
        for(int j=0; j<m; j++){
            tmp3(j) = elem.second[j];
        }
        sample_y.push_back(tmp3);
    }
    ArchiveData sample = createLabeledDataFromRange(sample_x, sample_y);
    return sample;
}

ArchiveData SubArchive::getRandomSample(int k){
    std::vector<int> randomSeq(full_archive.size());
    std::iota (randomSeq.begin(), randomSeq.end(), 0);
    std::random_shuffle(randomSeq.begin(), randomSeq.end());

    std::vector<RealVector> sample_x;
    std::vector<RealVector> sample_y;
    for(int i=0; i<k; i++)
    {
        std::pair<std::vector<double>, std::vector<double>> elem = full_archive[randomSeq[i]];
        int n = elem.first.size();
        RealVector tmp4(n);
        for(int j=0; j<n; j++){
            tmp4(j) = elem.first[j];
        }
        sample_x.push_back(tmp4);
        int m = elem.second.size();
        RealVector tmp3(m);
        for(int j=0; j<m; j++){
            tmp3(j) = elem.second[j];
        }
        sample_y.push_back(tmp3);
    }
    ArchiveData sample = createLabeledDataFromRange(sample_x, sample_y);
    return sample;
}

ArchiveData SubArchive::getFittestSampleSO(int k){
    if(population_so.size()> (unsigned int) k){
        UncertainSelection<HypervolumeIndicator, double> selector;
        selector.trueSelect(this->population_so, k);
        std::partition(population_so.begin(), population_so.end(),UncertainIndividual<double>::TrueSelected);
        population_so.erase(population_so.begin()+k,population_so.end());   
    }
    std::vector<RealVector> sample_x;
    std::vector<RealVector> sample_y;
    for(UncertainIndividual<double> elem : population_so){
        sample_x.push_back(elem.searchPoint());
        RealVector tmp(1);
        tmp(0) = elem.trueFitness();
        sample_y.push_back(tmp);
    }
    ArchiveData sample = createLabeledDataFromRange(sample_x, sample_y);
    return sample;    
}

ArchiveData SubArchive::getFittestSampleMO(int k){
    if(population_mo.size()> (unsigned int) k){
        UncertainSelection<HypervolumeIndicator, RealVector> selector;
        selector.trueSelect(this->population_mo, k);
        std::partition(population_mo.begin(), population_mo.end(),UncertainIndividual<RealVector>::TrueSelected);
        population_mo.erase(population_mo.begin()+k,population_mo.end());   
    }

    std::vector<RealVector> sample_x;
    std::vector<RealVector> sample_y;
    for(UncertainIndividual<RealVector> elem : population_mo){
        sample_x.push_back(elem.searchPoint());
        sample_y.push_back(elem.trueFitness());
    }
    ArchiveData sample = createLabeledDataFromRange(sample_x, sample_y);
    return sample;
}

ArchiveData SubArchive::getFittestSample(int k){
    if(std::count(which_fit.begin(), which_fit.end(), true)>1){
        return getFittestSampleMO(k);
    }
    return getFittestSampleSO(k);
}
        
ArchiveData SubArchive::getNeighbours(RealVector point, int k){
    this->updateData();
    SimpleNearestNeighbors<RealVector, std::vector<double>> nnmodel(this->archive, &(this->kernel));

    RealMatrix mat(1,point.size());
    for(unsigned int i=0; i<point.size(); i++){
        mat.set_element(0, i, point(i));
    }
    std::vector<SimpleNearestNeighbors< RealVector, std::vector<double>>::DistancePair> output = nnmodel.getNeighbors(mat, k);
    std::vector<RealVector> neighbours_x(k);
    std::vector<RealVector> neighbours_y(k);
    for(int i=0; i<k; i++){
        SimpleNearestNeighbors< RealVector, std::vector<double>>::DistancePair tmp = output[i];
        int m = this->archive_data[tmp.value].first.size();
        RealVector tmp4(m);
        for(int j=0; j<m; j++){
            tmp4(j) = this->archive_data[tmp.value].first[j];
        }
        neighbours_y[i] = tmp4;
        std::vector<double> tmp2 = tmp.value;
        RealVector tmp3(tmp2.size());
        for(unsigned int j=0; j<tmp3.size(); j++){
            tmp3(j) = tmp2[j];
        }
        neighbours_x[i] = tmp3;
        
    }
    ArchiveData neighbours = createLabeledDataFromRange(neighbours_x, neighbours_y);
    return neighbours;
    //std::vector<RealVector, double> neighbours = this->NNmodel.getNeighbors(point, k);
}

bool SubArchive::converged(int timeWindow, double varLimit){
    /*std::cout << "progres counter " << progress_counter << " " << progress << std::endl;
    return (progress_counter>timeWindow);*/
    if(progress.size()> (unsigned int) timeWindow){
        progress.erase(progress.begin(), progress.begin()+(progress.size()-timeWindow-1));
    }
    
    boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::variance> > acc;
    /*for(int i=0; i<timeWindow; i++){
        acc(progress[i]);
    }*/
    for_each(progress.begin(), progress.end(), boost::bind<void>(boost::ref(acc), _1));
    
   
    chi_squared dist(timeWindow-1);  
    double chi = (boost::accumulators::variance(acc)*(timeWindow-1))/varLimit;
    
    double p = cdf(dist,chi);
    //std::cout << "conv " << chi << " " << p << " " << this->prevConverged << std::endl;
    if(p<=0.05){
        if(this->prevConverged){
            return true;
        }else{
            this->prevConverged = true;
            return false;
        }
    }else{
        this->prevConverged = false;
        return false;
    }
}

/*bool SubArchive::converged(int timeWindow)const{
    LinearModel<> model;
    LinearRegression trainer;
    Data<RealVector> inputs(timeWindow, RealVector(1));
    Data<RealVector> outputs(timeWindow, RealVector(1));
//    RealVector counter(timeWindow);
    for(int i=0; i<timeWindow; i++){
//        counter(i) = i;
        inputs.element(i).set_element(0,i);
        outputs.element(i).set_element(0,progress[progress.size()-timeWindow+i]);
        //std::cout << outputs.element(i) << std::endl;
    }
    //RealVector labels = subrange(progress, progress.size()-timeWindow, progress.size());
    //Data<double> input = createDataFromRange(counter);
    //Data<double> output = createDataFromRange(labels);
    RegressionDataset data(inputs, outputs);
    trainer.train(model, data);
    //std::cout << "intercept: " << model.offset() << std::endl;
    //std::cout << "matrix: " << model.matrix() << std::endl;
    std::cout << "regression " << model.matrix()(0,0) << std::endl;
    if(model.matrix()(0,0)>=-0.005){
        return true;
    }else{
        return false;
    }
}*/

double SubArchive::hypervolume(){
    HypervolumeCalculator hyp;
    IdentityFitnessExtractor ife;
    std::vector<RealVector> points;
    for(auto const& element : this->archive_data){
        std::vector<double> f = fitness(element.first);
        RealVector point(f.size());
        for(unsigned int i=0; i<f.size(); i++){
            point(i) = f[i];
        }
        points.push_back(point);
    }
    RealVector ref(refPoint.size());
    
    for(unsigned int i=0; i<refPoint.size(); i++){
        ref(i) = refPoint[i];
    }
    
    /*auto fitness=[&](std::vector<double> p){
        std::vector<double> result(std::count(which_fit.begin(), which_fit.end(), true));
        std::vector<double> values = this->archive_data[p].first;
        int counter=0;
        for(int i=0; i<values.size(); i++){
            if(this->which_fit[i]){
                result[counter] = values[i];
                counter ++;
            }
        }
        return result; 
    };*/
    //std::cout<< "hyp " << hyp(ife, points, ref) << std::endl;
    return hyp(ife, points, ref);
}

std::vector<double> SubArchive::fitness(std::vector<double> point){
    std::vector<double> result(count(which_fit.begin(), which_fit.end(), true));
    std::vector<double> values = this->archive_data[point].first;
    int counter=0;
    for(unsigned int i=0; i<values.size(); i++){
        if(this->which_fit[i]){
            result[counter] = values[i];
            counter ++;
        }
    }
    
    return result;
}


void SubArchive::spillBeans(std::ofstream& myfile){
    for(unsigned int i=0; i<full_archive.size(); i++){
        std::pair<std::vector<double>, std::vector<double>> val = this->full_archive[i];
        for(unsigned int j=0; j<val.first.size(); j++){
            myfile << val.first[j] << " ";
        }
        myfile << ": ";
        for(unsigned int j=0; j<val.second.size(); j++){
            myfile << val.second[j] << " ";
        }
        myfile <<"\n";
    }
}