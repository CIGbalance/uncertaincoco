cmake_minimum_required(VERSION 2.8)

project(ExampleProject)

cmake_policy(SET CMP0015 OLD)

set ( Shark_DIR "../Shark-3.1.0/build" )
set ( libgp_DIR "../libgp-master" )
set ( eigen_DIR "../eigen-eigen-5a0156e40feb" )
set ( gbea "../../gbea" )


# Find the Shark libraries and includes
# set Shark_DIR to the proper location of Shark
find_package(Shark REQUIRED)
include(${SHARK_USE_FILE})

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

#add_library(libgp STATIC IMPORTED)
#find_library(${libgp_DIR}/build libgp )
#set_target_properties(libgp PROPERTIES IMPORTED_LOCATION "${libgp_DIR}/build")

#find_package(libgp REQUIRED)

LINK_DIRECTORIES(${libgp_DIR}/build "../lib" ${gbea})##gplib
INCLUDE_DIRECTORIES(${libgp_DIR}/include ${eigen_DIR} ${gbea})


set( CMAKE_CXX_FLAGS "-O3 -Wall -fopenmp -lstdc++")

# Executable
add_executable(ExampleExperiment ExampleExperiment.cpp COCOinterface/coco.c COCOinterface/Suite.cpp utils/Archive.cpp utils/ihs.cpp utils/Kriging.cpp algos/preselect/preselect.cpp algos/SAPEO/SAPEO.cpp algos/EGO/EGO.cpp algos/random/RandomSearch.cpp algos/SAPEOreader/SAPEOreader.cpp)
set_property(TARGET ExampleExperiment PROPERTY CXX_STANDARD 11)
target_link_libraries(ExampleExperiment ${SHARK_LIBRARIES} gp rw_top_trumps)

#Test Executables
#add_executable(COCOfunc_Test COCOinterface/Test/COCOfuncTest.cpp COCOinterface/coco.c utils/ihs.cpp utils/Archive.cpp)
#set_property(TARGET COCOfunc_Test PROPERTY CXX_STANDARD 11)
#target_link_libraries(COCOfunc_Test ${SHARK_LIBRARIES})

add_executable(COCOfunc_s_Test Tests/functions/COCOfunc_sTest.cpp COCOinterface/coco.c utils/ihs.cpp utils/Archive.cpp)
set_property(TARGET COCOfunc_s_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(COCOfunc_s_Test ${SHARK_LIBRARIES})

add_executable(COCOfunc_m_Test Tests/functions/COCOfunc_mTest.cpp COCOinterface/coco.c utils/ihs.cpp utils/Archive.cpp)
set_property(TARGET COCOfunc_m_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(COCOfunc_m_Test ${SHARK_LIBRARIES})

add_executable(Kriging_pred_s_Test Tests/functions/Kriging_pred_Test.cpp COCOinterface/coco.c utils/ihs.cpp utils/Archive.cpp utils/Kriging.cpp)
set_property(TARGET Kriging_pred_s_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(Kriging_pred_s_Test ${SHARK_LIBRARIES} gp)

add_executable(Kriging_pred_m_Test Tests/functions/Kriging_pred_m_Test.cpp COCOinterface/coco.c utils/ihs.cpp utils/Archive.cpp utils/Kriging.cpp)
set_property(TARGET Kriging_pred_m_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(Kriging_pred_m_Test ${SHARK_LIBRARIES} gp)

add_executable(EI_s_Test Tests/functions/EI_s_Test.cpp COCOinterface/coco.c utils/ihs.cpp utils/Archive.cpp utils/Kriging.cpp)
set_property(TARGET EI_s_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(EI_s_Test ${SHARK_LIBRARIES} gp)

add_executable(EI_m_Test Tests/functions/EI_m_Test.cpp COCOinterface/coco.c utils/ihs.cpp utils/Archive.cpp utils/Kriging.cpp)
set_property(TARGET EI_m_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(EI_m_Test ${SHARK_LIBRARIES} gp)

add_executable(UF_s_Test Tests/uncertainty/UF_s_Test.cpp COCOinterface/coco.c utils/ihs.cpp utils/Archive.cpp utils/Kriging.cpp)
set_property(TARGET UF_s_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(UF_s_Test ${SHARK_LIBRARIES} gp)

add_executable(archive_Test Tests/utils/archiveTest.cpp utils/Archive.cpp)
set_property(TARGET archive_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(archive_Test ${SHARK_LIBRARIES})

add_executable(Kriging_Test Tests/utils/KrigingTest.cpp utils/Kriging.cpp utils/Archive.cpp)
set_property(TARGET Kriging_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(Kriging_Test ${SHARK_LIBRARIES} gp)

#add_executable(tester HelloWorld.cpp COCOinterface/coco.c)
#set_property(TARGET tester PROPERTY CXX_STANDARD 11)
#target_link_libraries(tester ${SHARK_LIBRARIES})

add_executable(Suite_Test Tests/COCOinterface/SuiteTest.cpp COCOinterface/coco.c COCOinterface/Suite.cpp algos/random/RandomSearch.cpp utils/Archive.cpp)
set_property(TARGET Suite_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(Suite_Test ${SHARK_LIBRARIES})

add_executable(RS_Test Tests/algos/RS_Test.cpp COCOinterface/coco.c COCOinterface/Suite.cpp algos/random/RandomSearch.cpp utils/Archive.cpp)
set_property(TARGET RS_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(RS_Test ${SHARK_LIBRARIES})

add_executable(AbstractPreselecter_Test Tests/algos/AbstractPreselecter_Test.cpp COCOinterface/coco.c utils/Archive.cpp utils/ihs.cpp utils/Kriging.cpp)
set_property(TARGET AbstractPreselecter_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(AbstractPreselecter_Test ${SHARK_LIBRARIES} gp)

add_executable(presel_Test Tests/algos/presel_Test.cpp COCOinterface/coco.c COCOinterface/Suite.cpp algos/preselect/preselect.cpp utils/Archive.cpp utils/ihs.cpp utils/Kriging.cpp)
set_property(TARGET presel_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(presel_Test ${SHARK_LIBRARIES} gp)

add_executable(EGO_Test Tests/algos/EGO_Test.cpp COCOinterface/coco.c COCOinterface/Suite.cpp algos/EGO/EGO.cpp utils/Archive.cpp utils/ihs.cpp utils/Kriging.cpp)
set_property(TARGET EGO_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(EGO_Test ${SHARK_LIBRARIES} gp)

add_executable(SAPEO_Test Tests/algos/SAPEOTest.cpp COCOinterface/coco.c COCOinterface/Suite.cpp algos/SAPEO/SAPEO.cpp utils/Archive.cpp utils/ihs.cpp utils/Kriging.cpp)
set_property(TARGET SAPEO_Test PROPERTY CXX_STANDARD 11)
target_link_libraries(SAPEO_Test ${SHARK_LIBRARIES} gp)

add_executable(UncertainTester Tests/uncertainty/UncertainTester.cpp COCOinterface/coco.c utils/Archive.cpp utils/ihs.cpp utils/Kriging.cpp)
set_property(TARGET UncertainTester PROPERTY CXX_STANDARD 11)
target_link_libraries(UncertainTester ${SHARK_LIBRARIES} gp)

add_executable(ihs_Test Tests/utils/ihsTest.cpp utils/ihs.cpp)
set_property(TARGET ihs_Test PROPERTY CXX_STANDARD 11)
