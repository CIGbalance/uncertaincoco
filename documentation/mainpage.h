/**
 * \mainpage Thesis experiments documentation
 * 
 * \section Introduction
 * TODO stuff on what this does
 * 
 * \section install Installation
 * 
 * \subsection rec Requirements
 * + Install gbea Benchmark https://github.com/TheHedgeify/coco/tree/rw-new-suite
 * + make and cmake
 * + boost
 * + clone repository (git clone git@bitbucket.org:TheHedgeify/final.git)
 *   
 * \subsection inst_shark Install shark
 * \code{.sh}
 * cd {root_dir}/final/src/
 * unzip Shark-3.1.0.zip (choose option [N] when asked about replacing files)
 * cd Shark-3.1.0
 * mkdir build
 * cmake ../
 * make shark
 * \endcode
 * 
 * \subsection inst_eigen Setup Eigen
 * \code{.sh}
 * cd {root_dir}/final/src/
 * unzip eigen-eigen-5a0156e40feb.zip 
 * cd eigen-eigen-5a0156e40feb
 * mkdir build (generates FindEigen files for gp library)
 * cmake ../
 * export EIGEN3_INCLUDE_DIR={root_dir}/final/src/eigen-eigen-5a0156e40feb
 * export Eigen3_DIR={root_dir}/eigen-eigen-5a0156e40feb/build
 * \endcode
 * 
 * \subsection inst_gp Install libgp
 * \code{.sh}
 * cd {root_dir}/final/src/libgp-master
 * mkdir build
 * cmake ../
 * make gp
 * \endcode
 * 
 * \subsection inst_exp Install experiments
 * \code{.sh}
 * cd {root_dir}/final/src/shark_version
 * mkdir build
 * cmake ../src
 * make ExampleExperiment
 * \endcode
 * 
 * \subsection lido LiDO setup
 * 
 * LiDO Software modules (loaded in ~/.bashrc)
 * + python/2.7.11
 * + binutils/2.25
 * + gcc/5.4.0
 * + boost/1.59.0
 * + cmake/3.5.2
 * 
 * LiDO compiler always automatically chooses old gcc. Specifically set compiler:
 * \code{.sh}
 * export CC=/sysdata/shared/sfw/gcc/5.4.0-static-gmp-mpfr-mpc-isl/bin/gcc
 * export CXX=/sysdata/shared/sfw/gcc/5.4.0-static-gmp-mpfr-mpc-isl/bin/gcc
 * \endcode
 *
 * \code{.sh}
 * for host in $(qstat -1natu volz | grep -v " Q " | grep volz |awk '{print $NF}' | tr '+' '\n' | sort -u); do echo -n "$host: "; ssh $host uptime; done
 * for host in $(qstat -1natu volz | sed -n -e '/\bvolz\b.* R / { s/^.* \([^ ]*$\)/\1/; p; }' | tr '+' '\n' | sort -u); do echo "$host: "; ssh $host ps -u volz -fL; echo; done
 * \endcode
 *
 * \section Usage
 * TODO
 * 
 * \section Tests
 * TODO
 * 
 */
