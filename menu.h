#ifndef MENU_H
#define MENU_H
#include <string>
#include <vector>

using namespace std;


class Menu
{
private:
    string inv = "Warning: Invalid value!";
    int choice(vector<string> points);
public:
    Menu();
    int start();
};

#endif // MENU_H
