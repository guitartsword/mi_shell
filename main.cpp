#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[]){
    while(true) {
        cout << "mi_sh> ";
        string line;
        getline(cin,line);
        if(line=="exit")
            return 0;
        cout << line << ": comando no encontrado" << endl;
    };
    cerr << "algo extraño paso" << endl;
    return 0;
}
