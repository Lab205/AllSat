/* 
 * File:   CNF.cpp
 * Author: Saffie
 * 
 * Created on June 3, 2015, 10:33 AM
 */

#include "CNF.h"
#include <fstream>
#include <iostream>

CNF::CNF() {
}

CNF::CNF(const CNF& orig) {
}

CNF::~CNF() {
}

void CNF::negation(CNF& C, CNF& C_prime) {
    int terms = 0;
    int clauses = C.data.size();
    CNF_CLAUSE temp;
    std::vector<CNF_CLAUSE> DNF;
    for (int i = 0; i < C.data.size(); i++) {
        for (int j = 0; j < C.data[i].size(); j++) {
            terms = ((terms < abs(C.data[i][j])) ? C.data[i][j] : terms);
            temp.push_back(-C.data[i][j]);
        }
        DNF.push_back(temp);
        temp.clear();
    }
    DNF2CNF(DNF, C_prime, terms);

}

void CNF::DNF2CNF(std::vector<CNF_CLAUSE>& Disjunction, CNF& Conjunction, int& terms) {
    int left = 0;
    int right = 0;
    int i = 0;
    CNF_CLAUSE temp;
    left = con(Disjunction[i++], terms, Conjunction);
    if (Disjunction.size() > 1) {
        right = con(Disjunction[i++], terms, Conjunction);
        while (i < Disjunction.size()) {
            temp.push_back(-(++terms));
            temp.push_back(left);
            temp.push_back(right);
            Conjunction.data.push_back(temp);
            temp.clear();
            left = terms;
            right = con(Disjunction[i++], terms, Conjunction);
        }
        temp.push_back(-(++terms));
        temp.push_back(left);
        temp.push_back(right);
        Conjunction.data.push_back(temp);
        temp.clear();
        temp.push_back(terms);
        Conjunction.data.push_back(temp);
        temp.clear();
    } else {
        temp.push_back(left);
        Conjunction.data.push_back(temp);
        temp.clear();
    }
}

int CNF::con(std::vector<int> Vec, int& terms, CNF& C_prime) {
    int j = 0;
    std::vector<int> temp;

    if (Vec.size() == 1) {
        return Vec[j];
    } else {
        terms++;
        temp.push_back(-(terms));
        temp.push_back(Vec[j++]);
        C_prime.data.push_back(temp);
        temp.clear();
        temp.push_back(-(terms));
        temp.push_back(Vec[j++]);
        C_prime.data.push_back(temp);
        temp.clear();
        while (j < Vec.size()) {
//            terms++;
//            temp.push_back(-(terms));
//            temp.push_back(terms - 1);
//            C_prime.data.push_back(temp);
//            temp.clear();
            temp.push_back(-(terms));
            temp.push_back(Vec[j++]);
            C_prime.data.push_back(temp);
            temp.clear();
        }
    }
    return terms;

}

void CNF::Union(CNF& one, CNF& two, CNF& U) {
    U.data.clear();
    for (int i = 0; i < one.data.size(); i++) {
        U.data.push_back(one.data[i]);
    }
    for (int j = 0; j < two.data.size(); j++) {
        U.data.push_back(two.data[j]);
    }
}

void CNF::intersection(CNF& one, CNF& two, CNF_CLAUSE& res) {
    std::set<int> set_one;
    std::set<int> set_two;
    CNF2SET(one, set_one);
    CNF2SET(two, set_two);
    CNF_CLAUSE temp;
    //    printf("set_one:%d\n", set_two.size());
    //    std::set_intersection(set_one.begin(), set_one.end(), set_two.begin(), set_two.end(), inserter(set_res, set_res.begin()));
    for (std::set<int>::iterator i = set_one.begin(); i != set_one.end(); i++) {
        if (set_two.find(*i) != set_two.end()) {
            //            set_res.insert(*i);
            //            printf("insert:%d\n", *i);
            res.push_back(*i);
        }
    }
}

void CNF::CNF2SET(CNF& target, std::set<int>& target_set) {
    for (int i = 0; i < target.data.size(); i++) {
        if (target.data[i].size() == 1) {
            target_set.insert(target.data[i][0]);
        }
        //        printf("insert:%d\n", target.data[i][0]);
    }
}