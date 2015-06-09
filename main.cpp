/* 
 * File:   main.cpp
 * Author: Saffie
 *
 * Created on June 3, 2015, 10:16 AM
 */

#include <cstdlib>
#include "lib/minisat/simp/SimpSolver.h"
#include "src/CNF.h"
#include "src/AllSat.h"
#include <fstream>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    //    Minisat::SimpSolver S;
    AllSat s("medium.cnf");
    s.solve();
    return 0;
}

