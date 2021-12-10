#include <iostream>
#include "menu.h"
#include "grammar.h"

Menu::Menu()
{

}
int main_choice(vector<string> points){
    unsigned int input;
    for(unsigned int i = 0; i < points.size(); i++)
        cout << i << ". " << points[i] << endl;
    cout << endl << "> ";
    cin >> input;
    if(input >= points.size())
        return -1;
    else
        return input;
}
int Menu::choice(vector<string> points){
    int response = main_choice(points);
    while(response < 0){
        cout << inv << endl << endl;
        response = main_choice(points);
    }
    return response;
}
int Menu::start(){
    cout << "Здравствуйте!" << endl;
    int chc;
    Grammar gr;
    gr.set();
    Grammar cash = gr;
    while (true) {
        cout << gr.print() << endl << endl;
        if(gr.getCheck()){
            chc = choice({"Ввести новую грамматику",
                          "Проверить существование языка",
                          "Устранить нетерминалы, не порождающие терминальные строки",
                          "Устранить недостижимые символы",
                          "Устранить епсилон-правила",
                          "Устранить цепные правила",
                          "Устранить левые факторизации правил",
                          "Устранить прямые левые рекурсии",
                          "Выход"});
            switch (chc) {
            case 0:
                gr.set();
                break;
            case 1:
                if(gr.Check_the_existence_of_a_language())
                    cout << endl << "Язык грамматики существует" << endl;
                else
                    cout << endl << "Языка грамматики НЕ существует" << endl;
                break;
            case 2:
                cash.Eliminating_nonterminals_that_do_not_generate_terminal_strings();
                break;
            case 3:
                cash.Eliminate_unreachable_characters();
                break;
            case 4:
                cash.Eliminate_epsilon_precepts();
                break;
            case 5:
                cash.Eliminating_chain_precepts();
                break;
            case 6:
                cout << "Ещё не готово";
                break;
            case 7:
                cout << "Ещё не готово";
                break;
            case 8:
                cout << endl << "До свидания" << endl;
                break;
            default:
                cout << inv << endl;
                break;
            }
            cout << endl;
            if(chc >= 2 && chc <= 5){
                cout << "Новая грамматика:" << endl;
                cout << cash.print() << endl << endl;
                cout << "Хотите ли применить изменения?" << endl;
                while (chc != 0 && chc != 1) {
                    chc = choice({"Да", "Нет"});
                    switch (chc) {
                    case 0:
                        gr = cash;
                        break;
                    case 1:
                        break;
                    default:
                        cout << inv << endl << endl;
                        break;
                    }
                }
                cout << endl;
            }
            if(chc == 8)
                break;
        }else{
            chc = choice({"Ввести новую грамматику",
                          "Выход"});
            switch (chc) {
            case 0:
                gr.set();
                break;
            case 1:
                cout << "До свидания" << endl;
                break;
            default:
                cout << inv << endl;
                break;
            }
            cout << endl;
            if(chc == 1)
                break;
        }
        cash = gr;
    }
    return 0;
}
