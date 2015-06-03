/* 
 * File:   AllSat.h
 * Author: Saffie
 *
 * Created on June 3, 2015, 1:47 PM
 */

#ifndef ALLSAT_H
#define	ALLSAT_H
#include "CNF_CLAUSE.h"
#include "Common.h"
#include "CNF.h"
#include <set>
#include <vector>

class AllSat {
private:
    CNF C_prime;
    CNF A_prime;
    CNF C;
    CNF C_prime_DNF;
    CNF U;
    CNF PUC;
    bool flag;
    int terms;
    int clauses;
public:
    
    CNF A;
    AllSat();
    AllSat(const AllSat& orig);
    AllSat(const char* path);
    virtual ~AllSat();
    void solve();
    
private:
    void read_from_file(const char* path);
    void print_file(CNF& U);
    bool get_assign(CNF& C, CNF& P);
    void print_result();
};

#endif	/* ALLSAT_H */

