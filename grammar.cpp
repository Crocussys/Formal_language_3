#include <iostream>
#include <fstream>
#include "grammar.h"
#include "precept.h"

string* DSP(string input, string separator){
    string a = "";
    string b = "";
    string* output = new string[3];
    if(separator == ""){
        output[0] = input;
        output[1] = b;
        output[2] = "FALSE";
        return output;
    }
    int ind = input.find(separator);
    if(ind == -1){
        a = input;
        output[2] = "FALSE";
    }
    else{
        a.assign(input, 0, ind);
        b.assign(input, ind + separator.length());
        output[2] = "TRUE";
    }
    output[0] = a;
    output[1] = b;
    return output;
}
Grammar::Grammar()
{
    fstream config;
    eps_symbol = 'e';
    pre_determinant = "-";
    pre_separator = "|";
    string cash;
    config.open("config.txt", ios::in);
    if(!config){
        cout << "Warning: Failed to open config file" << endl;
        cout << "Warning: All configurations are set by default" << endl << endl;
    }else
        while(getline(config, cash)){
            string* line = DSP(cash, "=");
            string name = line[0];
            string arg = line[1];
            if(name == "epsilon_symbol"){
                if(arg.length() == 1)
                    eps_symbol = arg[0];
                else
                    cout << "Warning: Epsilon symbol is set by default 'e'" << endl << endl;
            }else if(name == "precept")
                pre_determinant = arg;
            else if(name == "precept_separator")
                pre_separator = arg;
        }
    config.close();
}
template<typename  Type>
bool find_from_array(vector<Type> array, Type elem){
    bool flag = false;
    for (unsigned int i = 0; i < array.size(); i++)
        if(array[i] == elem){
            flag = true;
            break;
        }
    return flag;
}
void Grammar::addPrecepts(string a, string input){
    string* bs = DSP(input, pre_separator);
    Precept pr(a, bs[0]);
    if(!find_from_array(precepts, pr))
        precepts.push_back(pr);
    if (bs[2] == "FALSE")
        return;
    Precept pr2;
    string* bss;
    bool flag = false;
    if(bs[1].find(pre_separator) != -1){
        bss = DSP(bs[1], pre_separator);
        pr2.set(a, bss[0]);
        flag = true;
    }else
        pr2.set(a, bs[1]);
    if(!find_from_array(precepts, pr2))
        precepts.push_back(pr2);
    if (flag)
        addPrecepts(a, bss[1]);
}
void Grammar::set(){
    term_symbols = {};
    nonterm_symbols = {};
    precepts = {};
    string cash;
    cout << "Введите терминальные символы:" << endl;
    cin >> cash;
    cout << endl;
    for(unsigned int i = 0; i < cash.length(); i++){
        char symbol = cash[i];
        if(!find_from_array(term_symbols, symbol) && symbol != eps_symbol)
            term_symbols.push_back(symbol);
    }
    cout << "Введите нетерминальные символы:" << endl;
    cin >> cash;
    cout << endl;
    for(unsigned int i = 0; i < cash.length(); i++){
        char symbol = cash[i];
        if(!find_from_array(nonterm_symbols, symbol) && symbol != eps_symbol)
            nonterm_symbols.push_back(symbol);
    }
    cout << "Введите правила в фомате А" << pre_determinant << "а" << endl << "Каждое правило с новой строки. Закончите строкой: '" << pre_determinant << "'" << endl;
    cin >> cash;
    while(cash != pre_determinant){
        string* line = DSP(cash, pre_determinant);
        if(line[1].find(pre_separator) != -1){
            addPrecepts(line[0], line[1]);
        }else{
            Precept pr(line[0], line[1]);
            if(!find_from_array(precepts, pr))
                precepts.push_back(pr);
        }
        cin >> cash;
    }
    cout << endl << "Введите начальный символ:" << endl;
    cin >> start_symbol;
    cout << endl;
    if(check())
        cout << "Эта грамматика является контекстно-свободной" << endl << endl;
    else
        cout << "Эта грамматика НЕ является контекстно-свободной" << endl;
}
string Grammar::print(){
    string ret;
    ret += "Терминалы: ";
    for(unsigned int i = 0; i < term_symbols.size(); i++){
        ret += term_symbols[i];
        ret += " ";
    }
    if(term_symbols.size() != 0)
        ret.erase(ret.length()-1);
    ret += "\nНетерминалы: ";
    for(unsigned int i = 0; i < nonterm_symbols.size(); i++){
        ret += nonterm_symbols[i];
        ret += " ";
    }
    if(nonterm_symbols.size() != 0)
        ret.erase(ret.length()-1);
    ret += "\nНачальный символ: ";
    ret += start_symbol;
    ret += "\nПравила:";
    string a_pred = precepts[0].getA();
    for(unsigned int i = 0; i < precepts.size(); i++){
        string a = precepts[i].getA();
        if(a == a_pred && i != 0)
            ret += " " + pre_separator + " " + precepts[i].getB();
        else
            ret += "\n" + precepts[i].print();
        a_pred = a;
    }
    return ret;
}
bool Grammar::check(){
    bool flag = true;
    for(unsigned int i = 0; i < precepts.size(); i++){
        string a = precepts[i].getA();
        string b = precepts[i].getB();
        if(a.length() != 1 || !find_from_array(nonterm_symbols, a[0])){
            flag = false;
            break;
        }
        for(unsigned int j = 0; j < b.length(); j++)
            if(!(find_from_array(term_symbols, b[j]) || find_from_array(nonterm_symbols, b[j]) || b[j] == eps_symbol)){
                flag = false;
                break;
            }
        if(!flag)
            break;
    }
    CHECK = flag;
    return flag;
}
bool Grammar::getCheck(){
    return CHECK;
}
vector<char> Grammar::Nonterminals_spawning_terminal_strings(){
    vector<char> Np;
    vector<char> N = {};
    do{
        Np = N;
        for(unsigned int i = 0; i < precepts.size(); i++){
            char a = precepts[i].getA()[0];
            if(!find_from_array(N, a)){
                bool flag = true;
                string b = precepts[i].getB();
                for(unsigned int j = 0; j < b.length(); j++)
                    if(!(find_from_array(term_symbols, b[j]) || find_from_array(N, b[j]) || b[j] == eps_symbol)){
                        flag = false;
                        break;
                    }
                if(flag)
                    N.push_back(precepts[i].getA()[0]);
            }
        }
    }while(N != Np);
    return N;
}
bool Grammar::Check_the_existence_of_a_language(){
    vector<char> N = Nonterminals_spawning_terminal_strings();
    if(find_from_array(N, start_symbol))
        return true;
    else
        return false;
}
void Grammar::Remove_symbols(vector<char> del){
    for(vector<char>::iterator i = term_symbols.begin(); i != term_symbols.end(); i++)
        if(find_from_array(del, *i)){
            term_symbols.erase(i);
            i--;
        }
    for(vector<char>::iterator i = nonterm_symbols.begin(); i != nonterm_symbols.end(); i++)
        if(find_from_array(del, *i)){
            nonterm_symbols.erase(i);
            i--;
        }
    for(vector<Precept>::iterator i = precepts.begin(); i != precepts.end(); i++){
        Precept pr = *i;
        if(find_from_array(del, pr.getA()[0])){
            precepts.erase(i);
            i--;
            continue;
        }
        string b = pr.getB();
        for(unsigned int j = 0; j < b.length(); j++)
            if(find_from_array(del, b[j])){
                precepts.erase(i);
                i--;
                break;
            }
    }
}
void Grammar::Eliminating_nonterminals_that_do_not_generate_terminal_strings(){
    vector<char> deleted = {};
    vector<char> N = Nonterminals_spawning_terminal_strings();
    for(vector<char>::iterator i = nonterm_symbols.begin(); i != nonterm_symbols.end(); i++)
        if(!find_from_array(N, *i))
            deleted.push_back(*i);
    Remove_symbols(deleted);
}
void Grammar::Eliminate_unreachable_characters(){
    vector<char> deleted = {};
    vector<char> Wp;
    vector<char> W = {start_symbol};
    do{
        Wp = W;
        for(unsigned int i = 0; i < precepts.size(); i++)
            if(find_from_array(W, precepts[i].getA()[0])){
                string b = precepts[i].getB();
                for(unsigned int j = 0; j < b.length(); j++)
                    if(!find_from_array(W, b[j]))
                        W.push_back(b[j]);
            }
    }while(W != Wp);
    for(unsigned int i = 0; i < term_symbols.size(); i++){
        char ch = term_symbols[i];
        if(!find_from_array(W, ch))
            deleted.push_back(ch);
    }
    for(unsigned int i = 0; i < nonterm_symbols.size(); i++){
        char ch = nonterm_symbols[i];
        if(!find_from_array(W, ch))
            deleted.push_back(ch);
    }
    Remove_symbols(deleted);
}
char Grammar::new_nonterminal(){
    char nonterminal;
    cout << endl << "Необходим новый нетерминал. Введите его: ";
    while (true) {
        cin >> nonterminal;
        if(find_from_array(term_symbols, nonterminal) || find_from_array(nonterm_symbols, nonterminal) || nonterminal == eps_symbol)
            cout << "Этот символ уже используется. Введите другой: ";
        else
            break;
    }
    return nonterminal;
}
vector<string> Grammar::new_B(vector<char> N, string b){
    vector<string> ans;
    if(b.length() != 0 && !find_from_array(ans, b))
        ans.push_back(b);
    for(unsigned int j = 0; j < b.length(); j++)
        if(find_from_array(N, b[j])){
            string new_b = b;
            new_b.erase(j, 1);
            vector<string> plus_ans = new_B(N, new_b);
            for(unsigned int i = 0; i < plus_ans.size(); i++){
                string new_ans = plus_ans[i];
                if(!find_from_array(ans, new_ans))
                    ans.push_back(new_ans);
            }
        }
    return ans;
}
void Grammar::Eliminate_epsilon_precepts(){
    vector<char> N = {};
    for(unsigned int i = 0; i < precepts.size(); i++){
        char a = precepts[i].getA()[0];
        string b = precepts[i].getB();
        if(b.length() == 1 && b[0] == eps_symbol && !find_from_array(N, a))
            N.push_back(a);
    }
    vector<char> Np;
    do{
        Np = N;
        for(unsigned int i = 0; i < precepts.size(); i++){
            char a = precepts[i].getA()[0];
            string b = precepts[i].getB();
            bool flag = true;
            for(unsigned int j = 0; j < b.length(); j++)
                if(!find_from_array(N, b[j])){
                    flag = false;
                    break;
                }
            if(flag && !find_from_array(N, a))
                N.push_back(a);
        }
    }while(N != Np);
    vector<Precept> new_precepts = {};
    for(unsigned int i = 0; i < precepts.size(); i++){
        Precept pr = precepts[i];
        string a = pr.getA();
        string b = pr.getB();
        for(unsigned int j = 0; j < b.length(); j++){
            if(b[j] == eps_symbol)
                b.erase(j, 1);
        }
        vector<string> new_Bs = new_B(N, b);
        for(unsigned int j = 0; j < new_Bs.size(); j++){
            Precept new_precept(a, new_Bs[j]);
            new_precepts.push_back(new_precept);
        }
    }
    if(find_from_array(N, start_symbol)){
        char new_start_symbol = new_nonterminal();
        string a = "";
        string b = "";
        a += new_start_symbol;
        b += eps_symbol;
        Precept new_precept(a, b);
        new_precepts.insert(new_precepts.begin(), new_precept);
        b = "";
        b += start_symbol;
        Precept new_precept2(a, b);
        new_precepts.insert(new_precepts.begin(), new_precept2);
        nonterm_symbols.push_back(new_start_symbol);
        start_symbol = new_start_symbol;
    }
    precepts = new_precepts;
}
void Grammar::Eliminating_chain_precepts(){
    vector<vector<char>> N = {};
    vector<Precept> new_precepts = {};
    vector<char> Np;
    for(unsigned int i = 0; i < nonterm_symbols.size(); i++){
        N.push_back({nonterm_symbols[i]});
        do{
            Np = N[i];
            for(unsigned int j = 0; j < precepts.size(); j++){
                Precept pr = precepts[j];
                string b = pr.getB();
                if(find_from_array(N[i], pr.getA()[0]))
                    for(unsigned int k = 0; k < b.length(); k++){
                        char symbol = b[k];
                        if(find_from_array(nonterm_symbols, symbol)){
                            if(!find_from_array(N[i], symbol))
                                N[i].push_back(symbol);
                        }else{
                            new_precepts.push_back(pr);
                        }
                    }
            }
        }while(N[i] != Np);
    }
    vector<Precept> ans_precepts = {};
    for(unsigned int k = 0; k < N.size(); k++){
        vector<char> Nn = N[k];
        for(unsigned int i = 0; i < new_precepts.size(); i++){
            Precept pr = new_precepts[i];
            string a = pr.getA();
            if(Nn.size() == 1 && !find_from_array(ans_precepts, pr))
                ans_precepts.push_back(pr);
            else if(find_from_array(Nn, a[0]) && a[0] != Nn[0]){
                string new_a = "";
                new_a += Nn[0];
                Precept new_pr(new_a, pr.getB());
                if(!find_from_array(ans_precepts, new_pr))
                    ans_precepts.push_back(new_pr);
            }
        }
    }
    precepts = ans_precepts;
}
