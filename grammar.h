#ifndef GRAMMAR_H
#define GRAMMAR_H
#include <vector>
#include "precept.h"

using namespace std;


class Grammar
{
private:
    vector<char> term_symbols;
    vector<char> nonterm_symbols;
    vector<Precept> precepts;
    char start_symbol;
    bool CHECK;

    char eps_symbol;
    string pre_determinant;
    string pre_separator;

    bool check();
    vector<char> Nonterminals_spawning_terminal_strings();
    void Remove_symbols(vector<char> del);
    vector<string> new_B(vector<char> N, string b);
    char new_nonterminal();
    void addPrecepts(string a, string input);
public:
    Grammar();
    void set();
    string print();
    bool getCheck();
    bool Check_the_existence_of_a_language();
    void Eliminating_nonterminals_that_do_not_generate_terminal_strings();
    void Eliminate_unreachable_characters();
    void Eliminate_epsilon_precepts();
    void Eliminating_chain_precepts();
};

#endif // GRAMMAR_H
