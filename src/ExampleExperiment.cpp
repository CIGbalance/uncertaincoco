///@file Experiment Starter
#include <stdlib.h>
#include <iostream>
#include <cstdlib>

#include "COCOinterface/Suite.h"
#include "COCOinterface/coco.h"
#include "algos/CocoOptimiser.h"

#include "algos/SAPEO/AbstractSAPEO.h"
#include "algos/SAPEO/SAPEOCMA.h"
#include "algos/SAPEO/SAPEOMOCMA.h"
#include "algos/SAPEO/SAPEOSMS.h"
#include "algos/SAPEO/SAPEO.h"
#include "algos/SAPEOreader/SAPEOreader.h"

#include "algos/preselect/AbstractPreselecter.h"
#include "algos/preselect/CMA_mod.h"
#include "algos/preselect/MOCMA_mod.h"
#include "algos/preselect/SMS-EMOA_mod.h"
#include "algos/preselect/preselect.h"

#include "algos/EGO/EGO.h"

#include "algos/random/RandomSearch.h"

#include "uncertainty/SafeComparator.h"



using namespace std;

/// quick tester
void quickTester(int cbatch, bool lido=true){
    /*char name[] = "bbob";
    char name2[] = "rw-gan-mario";
    //char name2[] = "rw-top-trumps";
    //char name2[] = "bbob";
    char dim[] = "dimensions: 10";
    Suite suite(name2, 100, 360, cbatch, dim, name,1e5);
    SAPEOmocma<HypervolumeIndicator> mocma;
    //SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 16, NO, lido);//16*100 for mu=1000*/
    
    /*Suite suite2(name, 200, 360, cbatch, dim, name2);
    SAPEOreader algo2(11, 1, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE,
            algo.getRecommendations());
    suite.run(&algo2, 16, NO, lido);  */
    /*char name[] = "bbob";
    char dim[] = "dimensions: 2,3";
    Suite suite(name, 100, 360, cbatch, dim, name);
    MOCMA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            MEAN, CLOSE);
    suite.run(&algo, 16, NO, lido);*/
    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10";
    Suite suite(name2, 100, 588, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, 0, 0, NO_UNCERTAINTY, 0, CERTAIN, CLOSE);
    suite.run(&algo, 16, NO, lido);
    
    /*char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2";
    Suite suite(name, 100, 825, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, 0, 0, NO_UNCERTAINTY, 0, CERTAIN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000*/
}

/// experiment 0
void testFunction0(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 500, 360, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, 0, 0, NO_UNCERTAINTY, 0, CERTAIN, CLOSE);
    suite.run(&algo, 16, NO, lido);
}

void testFunction1(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 500, 360, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 16, NO, lido);
    
    Suite suite2(name, 500, 360, cbatch, dim, name);
    SAPEOreader algo2(11, 1, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE,
            algo.getRecommendations());
    suite.run(&algo2, 16, NO, lido);    
}

void testFunction2(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 500, 360, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 5, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 16, NO, lido);
    
    Suite suite2(name, 500, 360, cbatch, dim, name);
    SAPEOreader algo2(11, 1, std::numeric_limits<double>::infinity(), 5, NO_OVERLAP, 0.05, MEAN, CLOSE,
            algo.getRecommendations());
    suite.run(&algo2, 16, NO, lido);   
}

void testFunction3(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 500, 360, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT);
    suite.run(&algo, 16, NO, lido);
    
    Suite suite2(name, 500, 360, cbatch, dim, name);
    SAPEOreader algo2(11, 1, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT,
            algo.getRecommendations());
    suite.run(&algo2, 16, NO, lido);  
}

void testFunction4(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 500, 360, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 16, NO, lido);
    
    Suite suite2(name, 500, 360, cbatch, dim, name);
    SAPEOreader algo2(11, 1, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            NO_OVERLAP, 0.05, MEAN, CLOSE, algo.getRecommendations());
    suite.run(&algo2, 16, NO, lido);  
}

void testFunction5(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 500, 360, cbatch, dim, name);
    MOCMA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            MEAN, CLOSE);
    suite.run(&algo, 16, NO, lido);
}

void testFunction6(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 500, 360, cbatch, dim, name);
    MOCMA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            LB, CLOSE);
    suite.run(&algo, 16, NO, lido);
}

void testFunction7(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 100, 360, cbatch, dim, name);
    CMA cma;
    MOCMA mocma;
    EGO algo(11, 17, 5,11, 1, 100, &mocma, &cma, PoI, FIT);
    suite.run(&algo, 16, NO, lido);
}

void testFunction8(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 100, 360, cbatch, dim, name);
    CMA cma;
    MOCMA mocma;
    EGO algo(11, 17, 5,11, 1, 100, &mocma, &cma, ExI, FIT);
    suite.run(&algo, 16, NO, lido);
}

void testFunction9(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 500, 360, cbatch, dim, name);

    RandomSearch algo(10000);
    suite.run(&algo, 16, NO, lido);
}

void testFunction10(int cbatch, bool lido=true){
    char name[] = "bbob";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 500, 360, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT);
    suite.run(&algo, 16, INVERSE, lido);
    
    Suite suite2(name, 500, 360, cbatch, dim, name);
    SAPEOreader algo2(11, 1, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT,
            algo.getRecommendations());
    suite.run(&algo2, 16, INVERSE, lido);  
}


void testFunction11(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, 0, 0, NO_UNCERTAINTY, 0, CERTAIN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction12(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction13(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 5, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction14(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction15(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction16(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    SMSEMOA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);
}

void testFunction17(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    SMSEMOA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            LB, CLOSE);
    suite.run(&algo, 160, NO, lido);
}

void testFunction18(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 100, 825, cbatch, dim, name);
    CMA cma;
    SMSEMOA_mod mocma;
    EGO algo(11, 17, 5,11, 1, 100, &mocma, &cma, PoI, FIT);
    suite.run(&algo, 160, NO, lido);
}

void testFunction19(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 100, 825, cbatch, dim, name);
    CMA cma;
    SMSEMOA_mod mocma;
    EGO algo(11, 17, 5,11, 1, 100, &mocma, &cma, ExI, FIT);
    suite.run(&algo, 160, NO, lido);
}

void testFunction20(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);

    RandomSearch algo(10000);
    suite.run(&algo, 160, NO, lido);
}

void testFunction21(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT);
    suite.run(&algo, 160, INVERSE, lido);
}

void testFunction22(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    //SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, 0, 0, NO_UNCERTAINTY, 0, CERTAIN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction23(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    //SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction24(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    //SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 5, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction25(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    //SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction26(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    //SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction27(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    MOCMA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);
}

void testFunction28(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    MOCMA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            LB, CLOSE);
    suite.run(&algo, 160, NO, lido);
}

void testFunction29(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 100, 825, cbatch, dim, name);
    CMA cma;
    MOCMA mocma;
    EGO algo(11, 17, 5,11, 1, 100, &mocma, &cma, PoI, FIT);
    suite.run(&algo, 160, NO, lido);
}

void testFunction30(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 100, 825, cbatch, dim, name);
    CMA cma;
    MOCMA mocma;
    EGO algo(11, 17, 5,11, 1, 100, &mocma, &cma, ExI, FIT);
    suite.run(&algo, 160, NO, lido);
}


void testFunction31(int cbatch, bool lido=true){
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 2,3,5,10";
    Suite suite(name, 200, 825, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT);
    suite.run(&algo, 160, INVERSE, lido);
}


void testFunction32(int cbatch, bool lido=true){
    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10, 20, 30, 40";
    Suite suite(name2, 500, 588, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, 0, 0, NO_UNCERTAINTY, 0, CERTAIN, CLOSE);
    suite.run(&algo, 16, NO, lido); 
}

void testFunction33(int cbatch, bool lido=true){
    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10";
    Suite suite(name2, 500, 588, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 16, NO, lido); 
}

void testFunction34(int cbatch, bool lido=true){
    int offset = 0;
    if(cbatch >= 0 && cbatch < 3*7){
        cbatch = (19-1)*7 + cbatch;
        offset = -6;
    }else if(cbatch >= 3*7 && cbatch < 6*7){
        cbatch = (25-1)*7 + cbatch - 3*7;
	offset = -9;
    }else if(cbatch >= 6*7 && cbatch <=18*7){
        cbatch = (31-1)*7 + cbatch - 6*7;
        offset = -12;
    }else{
        return;
    }
    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10";
    Suite suite(name2, 500, 588, cbatch, dim, name, 1e5, offset*7);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE, SURR);
    suite.run(&algo, 16, NO, lido); 
}

void testFunction35(int cbatch, bool lido=true){
    int offset = 0;
    if(cbatch >= 0 && cbatch < 3*7){
        cbatch = (19-1)*7 + cbatch;
        offset = -6;
    }else if(cbatch >= 3*7 && cbatch < 6*7){
        cbatch = (25-1)*7 + cbatch - 3*7;
	offset = -9;
    }else if(cbatch >= 6*7 && cbatch <=18*7){
        cbatch = (31-1)*7 + cbatch - 6*7;
        offset = -12;
    }else{
        return;
    }
    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10";
    Suite suite(name2, 500, 588, cbatch, dim, name, 1e5, offset*7);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE, BOTH);
    suite.run(&algo, 16, NO, lido); 
}

void testFunction36(int cbatch, bool lido=true){
    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10";
    Suite suite(name2, 500, 588, cbatch, dim, name);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT);
    suite.run(&algo, 16, NO, lido); 
}

void testFunction37(int cbatch, bool lido=true){
    int offset = 0;
    if(cbatch >= 0 && cbatch < 3*7){
        cbatch = (19-1)*7 + cbatch;
        offset = -6;
    }else if(cbatch >= 3*7 && cbatch < 6*7){
        cbatch = (25-1)*7 + cbatch - 3*7;
	offset = -9;
    }else if(cbatch >= 6*7 && cbatch <=18*7){
        cbatch = (31-1)*7 + cbatch - 6*7;
        offset = -12;
    }else{
        return;
    }
    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10";
    Suite suite(name2, 500, 588, cbatch, dim, name, 1e5, offset*7);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT, SURR);
    suite.run(&algo, 16, NO, lido); 
}

void testFunction38(int cbatch, bool lido=true){
    int offset = 0;
    if(cbatch >= 0 && cbatch < 3*7){
        cbatch = (19-1)*7 + cbatch;
        offset = -6;
    }else if(cbatch >= 3*7 && cbatch < 6*7){
        cbatch = (25-1)*7 + cbatch - 3*7;
	offset = -9;
    }else if(cbatch >= 6*7 && cbatch <=18*7){
        cbatch = (31-1)*7 + cbatch - 6*7;
        offset = -12;
    }else{
        return;
    }

    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10";
    Suite suite(name2, 500, 588, cbatch, dim, name, 1e5, offset*7);
    SAPEOmocma<HypervolumeIndicator> mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT, BOTH);
    suite.run(&algo, 16, NO, lido); 
}

void testFunction39(int cbatch, bool lido=true){
    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10";
    Suite suite(name2, 500, 588, cbatch, dim, name);
    MOCMA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            MEAN, CLOSE);
    suite.run(&algo, 16, NO, lido); 
}

void testFunction40(int cbatch, bool lido=true){
    char name[] = "bbob";
    char name2[] = "rw-gan-mario";

    char dim[] = "dimensions: 10";
    Suite suite(name2, 500, 588, cbatch, dim, name);
    RandomSearch algo(10000);
    suite.run(&algo, 16, NO, lido); 
}

void testFunction41(int cbatch, bool lido=true){
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, 0, 0, NO_UNCERTAINTY, 0, CERTAIN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction42(int cbatch, bool lido=true){
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction43(int cbatch, bool lido=true){
    int offset = 0;
    if(cbatch >=(1-1)*15 && cbatch < (2-1)*15){
        offset = -1;
        cbatch = cbatch +15;
    }else if(cbatch >= (2-1)*15 && cbatch < (3-1)*15){
        offset = -2;
        cbatch = cbatch+15;
    }else{
        return;
    }
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name, 1e5, offset*15);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE, SURR);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction44(int cbatch, bool lido=true){
    int offset = 0;
    if(cbatch >=(1-1)*15 && cbatch < (2-1)*15){
        offset = -1;
        cbatch = cbatch +15;
    }else if(cbatch >= (2-1)*15 && cbatch < (3-1)*15){
        offset = -2;
        cbatch = cbatch+15;
    }else{
        return;
    }
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name, 1e5, offset*15);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, CLOSE, BOTH);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}


void testFunction45(int cbatch, bool lido=true){
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction46(int cbatch, bool lido=true){
    int offset = 0;
    if(cbatch >=(1-1)*15 && cbatch < (2-1)*15){
        offset = -1;
        cbatch = cbatch +15;
    }else if(cbatch >= (2-1)*15 && cbatch < (3-1)*15){
        offset = -2;
        cbatch = cbatch+15;
    }else{
        return;
    }
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name, 1e5, offset*15);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT, SURR);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction47(int cbatch, bool lido=true){
    int offset = 0;
    if(cbatch >=(1-1)*15 && cbatch < (2-1)*15){
        offset = -1;
        cbatch = cbatch +15;
    }else if(cbatch >= (2-1)*15 && cbatch < (3-1)*15){
        offset = -2;
        cbatch = cbatch+15;
    }else{
        return;
    }
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name, 1e5, offset*15);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), 3, NO_OVERLAP, 0.05, MEAN, FIT, BOTH);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}

void testFunction48(int cbatch, bool lido=true){
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name);
    SMSEMOA_mod mocma;
    CMA_mod cma;
    preselect algo(11, 1, &mocma, &cma, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
            MEAN, CLOSE);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}


void testFunction49(int cbatch, bool lido=true){
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name);
    RandomSearch algo(10000);
    suite.run(&algo, 160, NO, lido);//16*100 for mu=1000
}


void testFunction50(int cbatch, bool lido=true){
    char name2[] = "rw-top-trumps-biobj";
    char name[] = "bbob-biobj";
    char dim[] = "dimensions: 128";
    Suite suite(name2, 100, 90, cbatch, dim, name);
    //SAPEOsms<HypervolumeIndicator> mocma;
    SAPEOSMS mocma;
    SAPEOcma cma;
    
    SAPEO algo(11, 1, &mocma, &cma, 0, 0, NO_UNCERTAINTY, 0, CERTAIN, CLOSE);
    suite.run(&algo, 160, INVERSE, lido);//16*100 for mu=1000
}



int main(int argc, char** argv) {
    int mode = atoi(argv[1]);
    int cbatch = atoi(argv[2]);

    bool lido = true;
    if(argc>3){
        lido=false;
    }
    if(mode==0){
        testFunction0(cbatch, lido);
    }else if(mode==1){
        testFunction1(cbatch, lido);
    }else if(mode==2){
        testFunction2(cbatch, lido);
    }else if(mode==3){
        testFunction3(cbatch, lido);
    }else if(mode==4){
        testFunction4(cbatch, lido);
    }else if(mode==5){
        testFunction5(cbatch, lido);
    }else if(mode==6){
        testFunction6(cbatch, lido);
    }else if(mode==7){
        testFunction7(cbatch, lido);
    }else if(mode==8){
        testFunction8(cbatch, lido);
    }else if(mode==9){
        testFunction9(cbatch, lido);
    }else if(mode==10){
        testFunction10(cbatch, lido);
    }else if(mode==11){
        testFunction11(cbatch, lido);
    }else if(mode==12){
        testFunction12(cbatch, lido);
    }else if(mode==13){
        testFunction13(cbatch, lido);
    }else if(mode==14){
        testFunction14(cbatch, lido);
    }else if(mode==15){
        testFunction15(cbatch, lido);
    }else if(mode==16){
        testFunction16(cbatch, lido);
    }else if(mode==17){
        testFunction17(cbatch, lido);
    }else if(mode==18){
        testFunction18(cbatch, lido);
    }else if(mode==19){
        testFunction19(cbatch, lido);
    }else if(mode==20){
        testFunction20(cbatch, lido);
    }else if(mode==21){
        testFunction21(cbatch, lido);
    }else if(mode==22){
        testFunction22(cbatch, lido);
    }else if(mode==23){
        testFunction23(cbatch, lido);
    }else if(mode==24){
        testFunction24(cbatch, lido);
    }else if(mode==25){
        testFunction25(cbatch, lido);
    }else if(mode==26){
        testFunction26(cbatch, lido);
    }else if(mode==27){
        testFunction27(cbatch, lido);
    }else if(mode==28){
        testFunction28(cbatch, lido);
    }else if(mode==29){
        testFunction29(cbatch, lido);
    }else if(mode==30){
        testFunction30(cbatch, lido);
    }else if(mode==31){
        testFunction31(cbatch, lido);
    }else if(mode==32){
        testFunction32(cbatch, lido);
    }else if(mode==33){
        testFunction33(cbatch, lido);
    }else if(mode==34){
        testFunction34(cbatch, lido);
    }else if(mode==35){
        testFunction35(cbatch, lido);
    }else if(mode==36){
        testFunction36(cbatch, lido);
    }else if(mode==37){
        testFunction37(cbatch, lido);
    }else if(mode==38){
        testFunction38(cbatch, lido);
    }else if(mode==39){
        testFunction39(cbatch, lido);
    }else if(mode==40){
        testFunction40(cbatch, lido);
    }else if(mode==41){
        testFunction41(cbatch, lido);
    }else if(mode==42){
        testFunction42(cbatch, lido);
    }else if(mode==43){
        testFunction43(cbatch, lido);
    }else if(mode==44){
        testFunction44(cbatch, lido);
    }else if(mode==45){
        testFunction45(cbatch, lido);
    }else if(mode==46){
        testFunction46(cbatch, lido);
    }else if(mode==47){
        testFunction47(cbatch, lido);
    }else if(mode==48){
        testFunction48(cbatch, lido);
    }else if(mode==49){
        testFunction49(cbatch, lido);
    }else if(mode==50){
        testFunction50(cbatch, lido);
    }else if(mode==1000){
        quickTester(cbatch, lido);
    }
    
}

