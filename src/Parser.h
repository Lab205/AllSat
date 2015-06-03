/* 
 * File:   Parser.h
 * Author: Saffie
 *
 * Created on June 3, 2015, 1:38 PM
 */

#ifndef PARSER_H
#define	PARSER_H
#include "Common.h"
#include "CNF_CLAUSE.h"
template<class B>
void read_clause(B& in, CNF_CLAUSE& term) {
    int parsed_int, var;
    term.clear();
//    neg_term.clear();
    for (;;) {
        parsed_int = Minisat::parseInt(in);
        if (parsed_int == 0) break;
        term.push_back(parsed_int);
//        neg_term.push_back(-parsed_int);
    }
}

#endif	/* PARSER_H */

