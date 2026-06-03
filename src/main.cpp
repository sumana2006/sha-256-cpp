#include <iostream>
#include <string>
#include "sha256.hpp"
using namespace std;

int main(){
string s;
cout<<"Enter input string: ";
if (!std::getline(std::cin, s)) return 1;

while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) {
        s.pop_back();
 }

cout<<endl<<"SHA-256 Hash: "<<sha256(s)<<endl;
return 0;
}
