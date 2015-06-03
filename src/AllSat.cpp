/* 
 * File:   AllSat.cpp
 * Author: Saffie
 * 
 * Created on June 3, 2015, 1:47 PM
 */

#include "AllSat.h"
#include "Parser.h"
#include <fstream>

AllSat::AllSat() {
}

AllSat::AllSat(const AllSat& orig) {
}

AllSat::AllSat(const char* path) {
    flag = true;
    this->read_from_file(path);
    flag = false;
}

AllSat::~AllSat() {
}

void AllSat::read_from_file(const char* path) {
    gzFile in = gzopen(path, "rb");
    if (in == NULL)
        printf("ERROR! Could not open file: %s\n", path), exit(1);

    Minisat::StreamBuffer buffer(in);
    CNF_CLAUSE term;
    //    CNF_CLAUSE neg_term;
    int cnt = 0;
    int i = 1;
    for (;;) {
        Minisat::skipWhitespace(buffer);
        if (*buffer == EOF) break;
        else if (*buffer == 'p') {
            if (Minisat::eagerMatch(buffer, "p cnf")) {
                terms = Minisat::parseInt(buffer);
                clauses = Minisat::parseInt(buffer);
            } else {
                printf("PARSE ERROR! Unexpected char: %c\n", *buffer), exit(3);
            }
        } else if (*buffer == 'c' || *buffer == 'p')
            Minisat::skipLine(buffer);
        else {
            cnt++;
            term.clear();
            read_clause(buffer, term);
            if (flag == true) {
                C.data.push_back(term);
            } else {
                if (term.size() == 1) {
                    PUC.data.push_back(term);
                } else {

                }
            }
        }
    }
    if (cnt != clauses)
        printf("PARSE ERROR! DIMACS header mismatch: wrong number of clauses\n");
}

void AllSat::solve() {
    C.negation(C, C_prime);

    CNF P, P1, CUAP;
    C.Union(C, A_prime, CUAP);
    while (get_assign(CUAP, P)) {
        U.Union(P, C_prime, U);
        if (!get_assign(U, P1)) {
            print_file(U);
            system("./muser2 -w input.cnf");
            this->read_from_file("muser2-output.cnf");
            CNF_CLAUSE alpha, alpha_prime;
            P.intersection(P, PUC, alpha);
            P.data.clear();
            PUC.data.clear();
           
            A.data.push_back(alpha);
            for (int i = 0; i < alpha.size(); i++) {
                alpha_prime.push_back(-alpha[i]);
            }
            alpha.clear();
            A_prime.data.push_back(alpha_prime);
            alpha_prime.clear();
        }
        C.Union(C, A_prime, CUAP);
    }
    print_result();
}

bool AllSat::get_assign(CNF& C, CNF& P) {
    Minisat::SimpSolver S;
    int var;
    CNF_CLAUSE temp;
    Minisat::Lit lit;
    for (int i = 0; i < C.data.size(); i++) {
        Minisat::vec<Minisat::Lit> lits;
        lits.clear();
        for (int j = 0; j < C.data[i].size(); j++) {
            var = abs(C.data[i][j]) - 1;
            while (var >= S.nVars()) S.newVar();
            lits.push((C.data[i][j] > 0) ? Minisat::mkLit(var) : ~Minisat::mkLit(var));
        }
        S.addClause_(lits);
    }
    Minisat::BoolOption solve("MAIN", "solve", "Completely turn on/off solving after preprocessing.", true);
    S.eliminate(true); 
    if (!S.okay()) {
        return false;
        //        exit(20);
    }
    Minisat::lbool ret = Minisat::l_Undef;
    if (solve) {
        Minisat::vec<Minisat::Lit> dummy;
        ret = S.solveLimited(dummy);
    }
    if (ret == Minisat::l_True) {
        //        printf("SAT\n");
        //        printf("model:%d\n", S.model.size());
        for (int i = 0; i < S.nVars(); i++)
            if (S.model[i] != Minisat::l_Undef) {
                temp.push_back((S.model[i] == Minisat::l_True) ? (i + 1) : -(i + 1));
                P.data.push_back(temp);
                temp.clear();
            }

        return true;
    } else if (ret == Minisat::l_False) {
        return false;
    } else {
        return false;
    }
}

void AllSat::print_file(CNF& U) {
    int terms = 0;
    int clauses = U.data.size();
    for (int i = 0; i < U.data.size(); i++) {
        for (int j = 0; j < U.data[i].size(); j++) {
            terms = ((terms < abs(U.data[i][j])) ? U.data[i][j] : terms);
        }
    }
    std::ofstream out;
    out.open("input.cnf");
    out << "p cnf " << terms << " " << clauses << std::endl;
    for (int i = 0; i < U.data.size(); i++) {
        for (int j = 0; j < U.data[i].size(); j++) {
            out << U.data[i][j] << " ";
        }
        out << "0" << std::endl;
    }
}

void AllSat::print_result(){
    std::ofstream res;
    res.open("result");
    for (int i = 0; i < A.data.size(); i++) {
        for(int j = 0; j < A.data[i].size(); j++){
            res<<A.data[i][j]<<" ";
        }
        res<<"\n";
    }
}