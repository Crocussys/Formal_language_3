#ifndef PRECEPT_H
#define PRECEPT_H
#include <string>

using namespace std;


class Precept
{
private:
    string a;
    string b;
public:
    Precept();
    Precept(string x, string y);
    void set(string x, string y);
    string print();
    string getA();
    string getB();
    friend bool operator== (const Precept &pr1, const Precept &pr2){
        if (pr1.a == pr2.a && pr1.b == pr2.b)
            return true;
        else
            return false;
    }
};

#endif // PRECEPT_H
