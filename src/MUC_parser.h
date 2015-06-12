/* 
 * File:   MUC_parser.h
 * Author: saffie
 *
 * Created on May 21, 2015, 4:30 PM
 */

#ifndef MUC_PARSER_H
#define	MUC_PARSER_H

#include <stdio.h>
#include <utility>
#include <vector>
#include "../lib/minisat/utils/ParseUtils.h"
#include "../lib/minisat/core/SolverTypes.h"
#include "../lib/minisat/utils/System.h"
#include "../lib/minisat/utils/Options.h"
#include "../lib/minisat/core/Dimacs.h"
#include "../lib/minisat/simp/SimpSolver.h"
typedef std::pair<int, int> item_type;
typedef Minisat::vec<Minisat::Lit> data_type;

template<class B, class Solver>
void read_clause(B& in, )





template<class B, class Solver>
    void read_Clause(B& in, Solver& S, Minisat::vec<Minisat::Lit>& lits) {
        int parsed_lit, var;
        lits.clear();
        for (;;) {
            parsed_lit = Minisat::parseInt(in);
            if (parsed_lit == 0) break;
            var = abs(parsed_lit) - 1;
            //            printf("%d..",var);
            while (var >= S.nVars()) S.newVar();
            lits.push((parsed_lit > 0) ? Minisat::mkLit(var) : ~Minisat::mkLit(var));
            //            printf("%d\n",lits[0].x);
        }
    }

    template<class B, class Solver>
    void parse_main(B& in, Solver& S, bool strictp = false) {
        //    item lits;
        int vars = 0;
        int clauses = 0;
        int cnt = 0;

        int i = 1;
        for (;;) {
            Minisat::skipWhitespace(in);
            if (*in == EOF) break;
            else if (*in == 'p') {
                if (Minisat::eagerMatch(in, "p cnf")) {
                    vars = Minisat::parseInt(in);
                    clauses = Minisat::parseInt(in);
                    // SATRACE'06 hack
                    // if (clauses > 4000000)
                    //     S.eliminate(true);
                } else {
                    printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
                }
            } else if (*in == 'c' || *in == 'p')
                Minisat::skipLine(in);
            else {
                cnt++;
                data_type lits;
                read_Clause(in, S, lits);
                data_type *copy = new data_type();
                lits.copyTo(*copy);
                S.addClause_(lits);
                data.push_back(copy);
            }
        }
        if (strictp && cnt != clauses)
            printf("PARSE ERROR! DIMACS header mismatch: wrong number of clauses\n");

//        printf("%d:: \n", data.size());
//        for (int i = 0; i < data.size(); i++) {
//            data_type *lits = data[i];
//            printf("%d %d %d: ", i , lits, lits->size());
//            for (int j = 0; j < lits->size(); j++) {
//                //printf("test");
//                Minisat::Lit lit = (*lits)[j];
//                printf("%d ", lit.x);
//            }
//            printf("\n");
//        }


//        for (int i = 0; i < data.size(); i++) {
//            delete data[i];
//        }


    }

    // Inserts problem into solver.
    //

    template<class Solver>
    void parse(gzFile input_stream, Solver& S, bool strictp = false) {
        //std::vector<item> data;
        Minisat::StreamBuffer in(input_stream);
        parse_main(in, S, strictp);
        //return data; 
    }


#endif	/* MUC_PARSER_H */

