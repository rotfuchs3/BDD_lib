## DESCRIPTION
This project implements a minimal Binary Decision Diagram (BDD) package in C++. This package shall implement the fundamental manipulation methods for Reduced Ordered BDDs (ROBDDs) as they were introduced in the lecture Verification of Digital System by Prof. Kunz. The package will be implemented using the Test Driven Development pardigm presented by Dr. Wedler.

###### Part I: Develop a BDD Package following a TDD (test driven development) approach

- Write the tests for the BDD package
- Implement the package in a C++ class
- Run the code against the predefined tests

###### Part II: 
 - Part A: Implementation of the performance improvements The goal is to improve your implementation regarding runtime and memory efficiency by implementing the appropriate computed table and hashing functions.
 - Part B: Reachable states The existing implementation is extended by a practical application of BDD. With BDD it is possible to symbolically represent a state-space. This representation allows to check quickly, whether a specific state is within the reachable state space or not. 

---

## SETUP
GoogleTest is required for compile and has been added as a submodule.  

###### To obtain GoogleTest:
```
sudo apt-get install libgtest-dev
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
# copy or symlink libgtest.a and libgtest_main.a to your /usr/lib folder
sudo cp *.a /usr/lib
```
###### OR:
Run `git submodule init` and `git submodule update` which will fetch GoogleTest source code that will later be compiled in the build step.

---

## BUILD
This also may build GoogleTest source into a library if GoogleTest packages are not found.
```
cd build/
sudo cmake CMakeLists.txt
sudo make
```
---

## Execute
Executables are located in build/bin/ directory.
```
./bin/EXECUTABLE_NAME.bin
```
---