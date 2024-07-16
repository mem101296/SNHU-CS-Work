//This file will call the header files, which call the other classes. 
#include <iostream>
#include <iomanip>
#include <ios>
#include "screen1.h"

using namespace std;

int main(){

    InitialInvestment invest;
    int userInverstment;

    invest.SetInitialInvestment(userInverstment);
    cout << "Would you like to test an investment or apply an investment?" << endl;
    cout << "";

    return 0;
}