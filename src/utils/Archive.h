#ifndef ARCHIVESO_H
#define ARCHIVESO_H

#include "../uncertainty/UncertainIndividual.h"

#include <shark/Algorithms/NearestNeighbors/SimpleNearestNeighbors.h>
#include <shark/Models/Kernels/LinearKernel.h>
#include <map>
#include <shark/Data/Dataset.h>
#include <string>
#include <fstream>
//#include <iostream>

using namespace shark;
typedef LabeledData<RealVector, RealVector> ArchiveData;
typedef LabeledData<RealVector, std::vector<double>> NNmodelData;

enum SampleMode{
    FIT = 0,
    RECENT = 1,
    CLOSE = 2,
    RANDOM = 3,
    ALL = 4,
};


class SubArchive{
public:
    SubArchive(){}
        
    void init(int dim, std::vector<bool> which_fit){
        this->refPoint = std::vector<double>(dim, std::numeric_limits<double>::min());
        this->which_fit = which_fit;
        this->prevConverged = false;
        this->upToDate = false;
        //this->bestf = std::numeric_limits<double>::max();
    }
    
    std::vector<double> getPointData(RealVector point); 
    void addPoint(RealVector point, double value, bool replace=false);
    void addPoint(RealVector point, std::vector<double> values, bool replace=false);
    int getNoPoints();

    ArchiveData getNeighbours(RealVector point, int k);
    ArchiveData getRandomSample(int k);
    ArchiveData getRecentSample(int k);
    ArchiveData getAll();
    ArchiveData getFittestSample(int k);
    
    void spillBeans(std::ofstream& myfile);
    bool converged(int timeWindow, double varLimit);    
    
private:
    void updateData();
    double hypervolume();
    std::vector<double> fitness(std::vector<double> point);
    ArchiveData getFittestSampleSO(int k);
    ArchiveData getFittestSampleMO(int k);
    

    std::map<std::vector<double>, std::pair<std::vector<double>, int>> archive_data;
    std::vector<std::pair<std::vector<double>, std::vector<double>>> full_archive;
    
    NNmodelData archive;
    LinearKernel<RealVector> kernel;

    bool upToDate;
    std::vector<double> refPoint;
    bool prevConverged;
    //double bestf;
    std::vector<double> progress;    //int progress_counter;
    std::vector<bool> which_fit;
    std::vector<UncertainIndividual<double>> population_so;
    std::vector<UncertainIndividual<RealVector>> population_mo;
};


class Archive{
public:
    Archive(){}
    
    void init(int dim, int timeWindow, double varLimit=1e-8){
        this->init(dim, std::vector<bool>(dim, true), timeWindow);
    }
    
    void init(int dim, std::vector<bool> which_fit, int timeWindow, double varLimit=1e-8){
        if(dim!=count(which_fit.begin(), which_fit.end(), true)){
            throw std::invalid_argument("wrong dimension of which_fit");
        }
        this->dim = dim;
        this->which_fit = which_fit;
        archives.clear();
        archives.push_back(SubArchive());
        archives[this->archives.size()-1].init(dim, this->which_fit);
        shared_archive.init(dim, this->which_fit);
        this->timeWindow = timeWindow;
        this->varLimit = varLimit;
        //std::cout << this->archives.size() << std::endl;
    }
    
    
    void signalReset(){
        archives.push_back(SubArchive());
        archives[this->archives.size()-1].init(dim, this->which_fit);
    }
    
    std::vector<double> getPointData(RealVector point){
        return archives[this->archives.size()-1].getPointData(point);
    }
    
    void addPoint(RealVector point, double value, bool replace=false){
        archives[this->archives.size()-1].addPoint(point, value, replace);
        shared_archive.addPoint(point, value, replace);
    }
    void addPoint(RealVector point, std::vector<double> values, bool replace=false){
        archives[this->archives.size()-1].addPoint(point, values, replace);
        shared_archive.addPoint(point, values, replace);
    }
    int getNoPointsCurrent(){
        if(archives.size()>=1){
            return archives[this->archives.size()-1].getNoPoints();
        }
        return -1;
    }
    
    int getNoPointsTotal(){
        if(archives.size()>=1){
            return shared_archive.getNoPoints();
        }
        return -1;
    }
    

    
    ArchiveData getSample(int k, SampleMode mode){
        if(mode==ALL || k>= shared_archive.getNoPoints()){
            return shared_archive.getAll();
        }else if(mode==FIT){
            return shared_archive.getFittestSample(k);
        }else if(mode==RECENT){
            return shared_archive.getRecentSample(k);
        }else if(mode==CLOSE){
            throw std::invalid_argument("missing point");
        }else if(mode == RANDOM){
            return shared_archive.getRandomSample(k);
        }
        return shared_archive.getRandomSample(k);
    
    }
    
    ArchiveData getSample(int k, SampleMode mode, RealVector point){
        if(mode==ALL || k>= shared_archive.getNoPoints()){
            return shared_archive.getAll();
        }else if(mode==FIT){
            return shared_archive.getFittestSample(k);
        }else if(mode==RECENT){
            return shared_archive.getRecentSample(k);
        }else if(mode==CLOSE){
            return shared_archive.getNeighbours(point, k);
        }else if(mode == RANDOM){
            return shared_archive.getRandomSample(k);
        }
        return shared_archive.getRandomSample(k);
    
    }
    

    void spillBeans(std::string file){
        std::ofstream myfile;
        myfile.open (file,std::ios::app);
        for(unsigned int i=0; i<archives.size(); i++){
            archives[i].spillBeans(myfile);
            myfile << "-----\n";
        }
        myfile.close();
    }
    bool converged(){
        bool conv = false;
        if(archives[this->archives.size()-1].getNoPoints()>=timeWindow){
            conv = archives[this->archives.size()-1].converged(timeWindow, varLimit); 
        }
        return conv;
    }    
    
    
private:
    std::vector<SubArchive> archives;
    std::vector<bool> which_fit;
    int dim;
    SubArchive shared_archive;
    int timeWindow;
    double varLimit;
};


#endif /* ARCHIVE_H */

