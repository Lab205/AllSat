/* 
 * File:   MUCParser.h
 * Author: saffie
 *
 * Created on May 22, 2015, 2:02 PM
 */

#ifndef MUCPARSER_H
#define	MUCPARSER_H

#include "MUCCommon.h"

template<class B>
void read_clause(B& in, data_type* lits) {
    int parsed_lit, var;
    lits->clear();
    for (;;) {
        parsed_lit = Minisat::parseInt(in);
        if (parsed_lit == 0) break;
        var = abs(parsed_lit) - 1;
        lits->push((parsed_lit > 0) ? Minisat::mkLit(var) : ~Minisat::mkLit(var));
    }
}

#endif	/* MUCPARSER_H */

