/* 
 * File:   CNF.h
 * Author: Saffie
 *
 * Created on June 3, 2015, 10:33 AM
 */

#ifndef CNF_H
#define	CNF_H
#include "Common.h"
#include "CNF_CLAUSE.h"
#include "Parser.h"
#include <set>
#include <vector>
class CNF {
private:
    
    
public:
    std::vector<CNF_CLAUSE> data;
    CNF();
    CNF(const CNF& orig);
    virtual ~CNF();
    void negation(CNF& C, CNF& C_prime);
    void Union(CNF& one, CNF& two, CNF& U);
    void intersection(CNF& one, CNF& two, CNF_CLAUSE& res);
    
private:
    void CNF2SET(CNF& target, std::set<int>& alpha);
    void DNF2CNF(std::vector<CNF_CLAUSE> Disjunction, CNF& Consjunction, int terms);
    int con(CNF_CLAUSE Vec, int& terms, CNF& C_prime);
};

#endif	/* CNF_H */

