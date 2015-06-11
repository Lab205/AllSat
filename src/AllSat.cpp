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
    gzclose(in);
}

void AllSat::solve() {
    Minisat::SimpSolver Sat;
    std::ofstream out;
    out.open("res.txt");
    C.negation(C, C_prime);
    int count = 1;
    CNF P, P1, CUAP;
    CNF_CLAUSE alpha, alpha_prime;
    alpha.clear();
    alpha_prime.clear();
    int var;
    if (!sat_check(C_prime, P1)) {
        printf("All Sat!\n");
    } else {
        for (int i = 0; i < C.data.size(); i++) {
            Minisat::vec<Minisat::Lit> lits;
            lits.clear();
            for (int j = 0; j < C.data[i].size(); j++) {
                var = abs(C.data[i][j]) - 1;
                while (var >= Sat.nVars()) Sat.newVar();
                lits.push((C.data[i][j] > 0) ? Minisat::mkLit(var) : ~Minisat::mkLit(var));
            }
            Sat.addClause_(lits);
        }
        //        C.Union(C, A_prime, CUAP);
        while (get_assign(alpha_prime, P, Sat)) {

            alpha.clear();

            alpha_prime.clear();
            U.Union(C_prime, P, U);
            print_file(U);
            system("./muser2 -w input.cnf > tmp");

            this->read_from_file("muser2-output.cnf");
            P.intersection(P, PUC, alpha);
            P.data.clear();
            PUC.data.clear();
            for (int j = 0; j < alpha.size(); j++) {
                printf("%d ", alpha[j]);
            }
            printf("\n-----%d------size: %d------\n", count++, alpha.size());
            for (int j = 0; j < alpha.size(); j++) {
                out << alpha[j] << " ";
            }
            out << "0" << std::endl;
            A.data.push_back(alpha);
            for (int i = 0; i < alpha.size(); i++) {
                alpha_prime.push_back(-alpha[i]);
            }
            A_prime.data.push_back(alpha_prime);

            //            C.Union(C, A_prime, CUAP);
        }
        print_result();
        out.close();
    }
}

bool AllSat::sat_check(CNF& Add, CNF& P) {

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
    }
    Minisat::lbool ret = Minisat::l_Undef;
    if (solve) {
        Minisat::vec<Minisat::Lit> dummy;
        ret = S.solveLimited(dummy);
    }
    if (ret == Minisat::l_True) {
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

bool AllSat::get_assign(CNF_CLAUSE& alpha_neg, CNF& P, Minisat::SimpSolver& Sat) {
    int var;
    CNF_CLAUSE temp;
    Minisat::Lit lit;
    Minisat::vec<Minisat::Lit> lits;
    lits.clear();
    for (int j = 0; j < alpha_neg.size(); j++) {
        printf("%d ", alpha_neg[j]);
    }
    if (!alpha_neg.empty()) {
        for (int j = 0; j < alpha_neg.size(); j++) {
            var = abs(alpha_neg[j]) - 1;
            while (var >= Sat.nVars()) Sat.newVar();
            lits.push((alpha_neg[j] > 0) ? Minisat::mkLit(var) : ~Minisat::mkLit(var));
        }
        Sat.addClause_(lits);
    }

    printf("Sat_size: %d \n", Sat.nVars());
    Minisat::BoolOption solve("MAIN", "solve", "Completely turn on/off solving after preprocessing.", true);
    Sat.eliminate(true);
    if (!Sat.okay()) {
        printf("false\n");
        return false;
    }
    Minisat::lbool ret = Minisat::l_Undef;
    if (solve) {
        Minisat::vec<Minisat::Lit> dummy;
        ret = Sat.solveLimited(dummy);
    }
    if (ret == Minisat::l_True) {
        for (int i = 0; i < Sat.nVars(); i++)
            if (Sat.model[i] != Minisat::l_Undef) {
                temp.push_back((Sat.model[i] == Minisat::l_True) ? (i + 1) : -(i + 1));
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
    out.close();
}

void AllSat::print_result() {
    std::ofstream res;
    res.open("result");
    for (int i = 0; i < A.data.size(); i++) {
        for (int j = 0; j < A.data[i].size(); j++) {
            res << A.data[i][j] << " ";
        }
        res << "\n";
    }
}