#include "precept.h"

Precept::Precept()
{
    a = "";
    b = "";
}
Precept::Precept(string x, string y)
{
    a = x;
    b = y;
}
void Precept::set(string x, string y){
    a = x;
    b = y;
}
string Precept::print(){
    return a + " -> " + b;
}
string Precept::getA(){
    return a;
}
string Precept::getB(){
    return b;
}
