## DESCRIPTION
This project implements a minimal Binary Decision Diagram (BDD) package in C++. This package shall implement the fundamental manipulation methods for Reduced Ordered BDDs (ROBDDs) as they were introduced in the lecture Verification of Digital System by Prof. Kunz. The package will be implemented using the Test Driven Development pardigm presented by Dr. Wedler.

Part I: Develop a BDD Package following a TDD (test driven development) approach

- Write the tests for the BDD package
- Implement the package in a C++ class
- Run the code against the predefined tests

---

## SETUP
GoogleTest is required for compile and has been added as a submodule. After cloning repo, run the following commands:
git submodule init
git submodule update

---

## BUILD
cmake CMakeLists.txt
make

This builds googletest into a library used by this repo. It also builds all relevant code for the BDD package.