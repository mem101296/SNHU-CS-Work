//This is going to be the first screen called

#include <iostream>
#include <iomanip>
#include <ios>

using namespace std;

#include "screen1.h"

void InitialInvestment::SetInitialInvestment(int invest){ //rename once other files are done

    //int initialInvestment = 0;
    int test = 2;

    setfill('-');
    cout << setfill('-') << setw(34) << "" << endl;
    cout << setfill('-') << setw(11) << "" << " Data Input " << setw(11) << "" << endl;

    

    if (invest == 0){ //prints values if initial investment is 0
        cout << left << "Initial Investment Amount: " << endl;
        cout << left << "Monthly Deposit: " << endl;
        cout << left << "Annual Interest: " << endl;
        cout << left << "Number of years: " << endl; 
        cout << left << "Press any key to continue. . . " << endl;
    }

    else{ //Prints the values if initial investment is greater than 0

    //remove test once header file is made
        cout << left << "Initial Investment Amount: " << invest << endl;
        cout << left << "Monthly Deposit: " << invest << endl;
        cout << left << "Annual Interest: " << invest << endl;
        cout << left << "Number of years: " << invest << endl; 
        cout << left << "Press any key to continue. . . " << endl;

    }


}