/* 
 * File:   MUC.cpp
 * Author: saffie
 * 
 * Created on May 11, 2015, 4:33 PM
 */

#include "MUC.h"

#include <vector>
#include <stdio.h>
#include <time.h>
#include "CNF.h"
#include "MUCParser.h"
#include "Utility.h"

MUC::MUC() {

}

MUC::MUC(const CNF& assign, const CNF& neg_C) {
    this->clear();
    int var;
    for (int i = 0; i < assign.data.size(); i++) {

        data_type *lits = new data_type();
        for (int j = 0; j < assign.data[i].size(); j++) {
            var = abs(assign.data[i][j]) - 1;
            lits->push((assign.data[i][j] > 0) ? Minisat::mkLit(var) : ~Minisat::mkLit(var));
        }
        data.push_back(lits);
    }

    for (int i = 0; i < neg_C.data.size(); i++) {

        data_type *lits = new data_type();
        for (int j = 0; j < neg_C.data[i].size(); j++) {
            var = abs(neg_C.data[i][j]) - 1;
            lits->push((neg_C.data[i][j] > 0) ? Minisat::mkLit(var) : ~Minisat::mkLit(var));
        }
        C_prime.push_back(lits);
    }
    C_prime_num = std::make_pair(0, C_prime.size() - 1);

//    printf("-------data-------\n");
//    for (int i = 0; i < data.size(); ++i) {
//        data_type* lits = data[i];
//        for (int j = 0; j < lits->size(); ++j) {
//            char sign = Minisat::sign((*lits)[j]) ? '-' : ' ';
//            var = Minisat::var((*lits)[j]) + 1;
//            printf("%c%d ", sign, var);
//        }
//        printf("\n");
//    }
//
//    printf("-------C_prime-------\n");
//    for (int i = 0; i < C_prime.size(); ++i) {
//        data_type* lits = C_prime[i];
//        for (int j = 0; j < lits->size(); ++j) {
//            char sign = Minisat::sign((*lits)[j]) ? '-' : ' ';
//            var = Minisat::var((*lits)[j]) + 1;
//            printf("%c%d ", sign, var);
//        }
//        printf("\n");
//    }
}

MUC::MUC(const MUC& orig) {
}

MUC::MUC(const char* path) {
    // init

    this->read_from_file(path);
}

MUC::~MUC() {
    this->clear();
}

/**
 * read from file
 * @param path
 */
void MUC::read_from_file(const char* path) {
    // TODO: clear
    this->clear();

    gzFile in = gzopen(path, "rb");
    if (in == NULL)
        printf("ERROR! Could not open file: %s\n", path), exit(1);

    Minisat::StreamBuffer buffer(in);
    int vars = 0;
    int clauses = 0;
    int cnt = 0;
    int i = 1;
    for (;;) {
        Minisat::skipWhitespace(buffer);
        if (*buffer == EOF) break;
        else if (*buffer == 'p') {
            if (Minisat::eagerMatch(buffer, "p cnf")) {
                vars = Minisat::parseInt(buffer);
                clauses = Minisat::parseInt(buffer);
            } else {
                printf("PARSE ERROR! Unexpected char: %c\n", *buffer), exit(3);
            }
        } else if (*buffer == 'c' || *buffer == 'p')
            Minisat::skipLine(buffer);
        else {
            cnt++;
            data_type *lits = new data_type();
            read_clause(buffer, lits);
            data.push_back(lits);
        }
    }

    if (cnt != clauses)
        printf("PARSE ERROR! DIMACS header mismatch: wrong number of clauses\n");
}

void MUC::muc_solve() {
    item_type C = std::make_pair(0, data.size() - 1);
//    printf("___________C:\n");
//    printf("first: %d, second: %d\n", C.first, C.second);
    L.push_back(C);
    item_type P, P1, P2;
    //        long int timestamp = getCurrentTime();
    //        long int bck = timestamp;
    int count = 0;
    while (!L.empty()) {
        count++;
        //            long int time = getCurrentTime();
        //            printf("-------------------------\n loop:%d  timestamp:%ld spend:%d\n", count, (time - timestamp), time - bck);
        //            bck = time;
        //            printf("-------------------------\ntimestamp: %ld\n", getCurrentTime() % 10000);
        P = L.back();
        L.pop_back();
        this->split(P, P1, P2);
        //            printf("(%d, %d)\n", P.first, P.second);
        //            printf("split timestamp: %ld\n", getCurrentTime() % 10000);
        if (minisolve(P1)) {
            continue;
        }
        //            printf("___________P2:\n");
        //            printf("solve P1 timestamp: %ld\n", getCurrentTime() % 10000);
        if (minisolve(P2)) {
            continue;
        }
        //            printf("solve P2 timestamp: %ld\n", getCurrentTime() % 10000);
        if (!is_empty(P1)) {
            L.push_back(P1);
        }
        if (!is_empty(P2)) {
            L.push_back(P2);
        }
    }

}

void MUC::print_data() {
    for (int i = 0; i < data.size(); ++i) {
        data_type* lits = data[i];
        for (int j = 0; j < lits->size(); ++j) {
            char sign = Minisat::sign((*lits)[j]) ? '-' : '+';
            printf("%c%d ", sign, Minisat::var((*lits)[j]));
        }
        printf("\n");
    }

    FILE* pFile = fopen("result.txt", "w");
    int num = M.size();
    fprintf(pFile, "p cnf aa %d\n", num);
    //printf("M_size:%d\n", num);
    for (int a = num - 1; a >= 0; a--) {
        // printf("(%d,%d)\n",M[a].first, M[a].second);
        data_type *item = data[M[a].first];
        for (int i = item->size() - 1; i >= 0; i--) {
            char sign = Minisat::sign((*item)[i]) ? '-' : ' ';
            fprintf(pFile, "%c%d ", sign, Minisat::var((*item)[i]) + 1);
        }
        fprintf(pFile, "0\n");
    }
}

void MUC::get_data(CNF& PUC) {
    int num = M.size();
    CNF_CLAUSE term;
    int var;
    for (int a = num - 1; a >= 0; a--) {
        // printf("(%d,%d)\n",M[a].first, M[a].second);
        data_type *item = data[M[a].first];
        for (int i = item->size() - 1; i >= 0; i--) {
            //            char sign = Minisat::sign((*item)[i]) ? '-' : ' ';
            //            fprintf(pFile, "%c%d ", sign, Minisat::var((*item)[i]) + 1);
            var = Minisat::var((*item)[i]) + 1;
            term.push_back(Minisat::sign((*item)[i]) ? -var : var);
        }
        //        fprintf(pFile, "0\n");
        PUC.data.push_back(term);
        term.clear();
    }
}

/**
 * clear data
 */
void MUC::clear() {
    for (int i = data.size() - 1; i >= 0; --i) {
        delete data[i];
    }

    data.clear();
    L.clear();
    M.clear();
}

/**
 * fun
 * @param P
 * @return 
 */
bool MUC::sat_check(item_type& P) {
    //    printf("sat check timestamp: %ld\n", getCurrentTime() % 10000);
    Minisat::SimpSolver S;
    int p_count = 0, m_count = 0, l_count = 0;
    for (int i = C_prime_num.first; i <= C_prime_num.second; i++) {
        // printf("print---");
        data_type *item_data = C_prime[i];
        for (int j = 0; j < item_data->size(); j++) {
            Minisat::Lit lit = (*item_data)[j];
            while (lit.x >= S.nVars()) S.newVar();
        }
        data_type copy;
        item_data->copyTo(copy);
        S.addClause_(copy);
    }
    add_clause(P, S);
    //    add_clause(C_prime_num, S);
    if (!this->is_empty(P))
        p_count += P.second - P.first + 1;
    for (int m = 0; m < M.size(); m++) {
        add_clause(M[m], S);
        if (!this->is_empty(M[m]))
            m_count += M[m].second - M[m].first + 1;
    }
    for (int n = 0; n < L.size(); n++) {
        add_clause(L[n], S);
        if (!this->is_empty(L[n]))
            l_count += L[n].second - L[n].first + 1;
    }
    //    printf(".........P-(%d) M-(%d & %d) L-(%d & %d) Sat size: %d\n", p_count,
    //            M.size(), m_count,
    //            L.size(), l_count,
    //            p_count + l_count + m_count);
    //    printf("M.size: %d,  L.size: %d\n", M.size(), L.size());
    //    printf("add clause timestamp: %ld\n", getCurrentTime() % 10000);
    bool res = sat(S);
    //    printf("sat timestamp: %ld\n", getCurrentTime() % 10000);
    return res;
}

/**
 * 
 * @param clause
 * @param S
 */
void MUC::add_clause(item_type& clause, Minisat::SimpSolver& S) {
    for (int i = clause.first; i <= clause.second; i++) {
        // printf("print---");
        data_type *item_data = data[i];
        for (int j = 0; j < item_data->size(); j++) {
            Minisat::Lit lit = (*item_data)[j];
            while (lit.x >= S.nVars()) S.newVar();
        }
        data_type copy;
        item_data->copyTo(copy);
        S.addClause_(copy);
    }

}

bool MUC::sat(Minisat::SimpSolver& S) {
    //    printf("sat timestamp: %ld\n", getCurrentTime() % 10000);
    Minisat::BoolOption solve("MAIN", "solve", "Completely turn on/off solving after preprocessing.", true);
    //    printf("solve timestamp: %ld\n", getCurrentTime() % 10000);
    S.eliminate(true); //------execute the main algorithm..saffie
    //    printf("eliminate timestamp: %ld\n", getCurrentTime() % 10000);
    if (!S.okay()) {
        return false;
        exit(20);
    }
    //    printf("okay timestamp: %ld\n", getCurrentTime() % 10000);
    Minisat::lbool ret = Minisat::l_Undef;
    if (solve) {
        Minisat::vec<Minisat::Lit> dummy;
        ret = S.solveLimited(dummy);
    }
    //    printf("solveLimited timestamp: %ld\n", getCurrentTime() % 10000);
    if (ret == Minisat::l_True) {
        return true;
    } else if (ret == Minisat::l_False) {
        return false;
    } else {
        return false;
    }
}

void MUC::split(item_type& P, item_type& P1, item_type& P2) {
    if (P.first != P.second) {
        float b = P.first;
        float a = (P.second - b) / 2;
        P1 = std::make_pair(P.first, P.first + ceil(a) - 1);
        P2 = std::make_pair(P1.second + 1, P.second);
    } else {
        P2 = P;
        P1 = std::make_pair(-1, -1);
    }
//    printf("___________P1:\n");
//    printf("first: %d, second: %d\n", P1.first, P1.second);
//    printf("___________P2:\n");
//    printf("first: %d, second: %d\n", P2.first, P2.second);
}

bool MUC::minisolve(item_type& P) {
//    printf("minisolve_P:\n");
//    printf("first: %d, second: %d\n", P.first, P.second);
    if (!is_empty(P) && !sat_check(P)) {
        if (is_atom(P)) {
            M.push_back(P);
        } else {
            L.push_back(P);
        }
        return true;
        //continue;
    }
    return false;
}

