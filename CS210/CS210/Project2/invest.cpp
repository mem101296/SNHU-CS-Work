/*
 * invest.cpp
 *
 *  Created on: Nov 30, 2020
 *      Author: 1397032_snhu
 */

#include <iostream>
#include <iomanip>
#include <ios>

using namespace std;


void InitialInvestmentLooks(){ //used to make output look pretty

    setfill('-');
    cout << setfill('-') << setw(34) << "" << endl;
    cout << setfill('-') << setw(11) << "" << " Data Input " << setw(11) << "" << endl;

}

float InitialInvestment(float userInvestment){ ///Takes the input for investment value

    cout << left << "Initial Investment Amount: $";
    cin >> userInvestment;
    //userInvestment = 1.00; //used to troubleshoot code
    return userInvestment;
}

float MonthlyDeposit(float monthlyDeposit){ //Takes the input for monthlydeposit value

    cout << left << "Monthly Deposit: $";
    cin >> monthlyDeposit;
   // monthlyDeposit = 50.00; //used to troubleshoot code
    return monthlyDeposit;
}

float AnnualInterest(float annualInterest){ //Takes the input for annualInterest

    cout << left << "Annual Interest: %";
    cin >> annualInterest;
    //annualInterest = 5; //used to troubleshoot code
    return annualInterest;
}

float NumYears(float numYears){ //Takes the input for numYeas

    cout << left << "Number of years: ";
    cin >> numYears;
    //numYears = 5; //used to troubleshoot code
    return numYears;

}


void GetinvestmentNumbers(float userInvestment, float monthlyDeposit, float annualInterest, float numYears){ //rename once other files are done

	float numInterest;
	float yearEnd;
    float totalInvest = userInvestment;

    //Used to make output look pretty
    cout << setfill('_') << setw(64) << "" << endl;
    cout <<  "|" << setfill(' ') << setw(62) << "" <<  "|" << endl;
    cout <<  "|" << setfill(' ') << setw(4) << "" << "Balance and Interest Without Additional Monthly Deposits" << setw(2) << "" <<  "|" << endl;
    cout <<  "|" << setfill('_') << setw(62) << "" <<  "|" << endl;
    cout <<  "|" << setfill(' ') << setw(62) << "" <<  "|" << endl;
    cout <<  "|" << setfill(' ') << setw(3) << "" << "Year";
    cout << setfill(' ') << setw(8) << "" << "Year End Balance";
    cout << setfill(' ') << setw(5) << "" << "Year End Earned Interest" << setw(2) << "" <<  "|" << endl;
    cout <<  "|" << setfill('_') << setw(62) << "" <<  "|" << endl;
    cout <<  "|" << setfill(' ') << setw(62) << "" <<   "|" <<endl;\

    //Outputs the interest and investment based on numYears
    for (int i = 0; i < numYears; i++){
    	numInterest = (totalInvest) * ((annualInterest / 100)); //getting investment as a num instead of percentage
        totalInvest = totalInvest + numInterest;

    	cout <<  "|" << setfill(' ') << setw(4) << "" << (i + 1) << setw(19) << "" << fixed << setprecision(2) << totalInvest << setw(24) << "" << numInterest << setw(6) << "" <<   "|" << endl;
        cout <<  "|" << setfill(' ') << setw(62) << "" <<   "|" << endl;
    }

    //Resets totalInvest value to original value
    totalInvest = userInvestment;

    //Makes output look pretty
    cout << setfill('_') << setw(64) << "" << endl;
    cout <<  "|" << setfill(' ') << setw(62) << "" <<  "|" << endl;
    cout <<  "|" << setfill(' ') << setw(4) << "" << "Balance and Interest With Additional Monthly Deposits" << setw(5) << "" <<  "|" << endl;
    cout <<  "|" << setfill('_') << setw(62) << "" <<  "|" << endl;
    cout <<  "|" << setfill(' ') << setw(62) << "" <<  "|" << endl;
    cout <<  "|" << setfill(' ') << setw(3) << "" << "Year";
    cout << setfill(' ') << setw(8) << "" << "Year End Balance";
    cout << setfill(' ') << setw(5) << "" << "Year End Earned Interest" << setw(2) << "" <<  "|" << endl;
    cout <<  "|" << setfill('_') << setw(62) << "" <<  "|" << endl;
    cout <<  "|" << setfill(' ') << setw(62) << "" <<   "|" <<endl;

    //Assigns a value to yearEnd
    yearEnd = 0;

    //runs a loops based off numYears
    for (int i = 0; i < numYears; i++){
    	//runs a loop using 12 to represent months
        for (int j = 0; j < 12; j++){


        	numInterest = (totalInvest + monthlyDeposit) * ((annualInterest / 100) / 12); //converting interest into a value
            yearEnd = yearEnd + numInterest; //total amount per year
            totalInvest = totalInvest + monthlyDeposit + numInterest; //total investment
        }
        	//outputs values
        	cout <<  "|" << setfill(' ') << setw(5) << "" << (i + 1) << setw(15) << "" << fixed << setprecision(2) << totalInvest << setw(22) << "" << yearEnd << setw(6) << "" <<   "|" << endl;
            cout <<  "|" << setfill(' ') << setw(62) << "" <<   "|" << endl;


    }


}



int main(){

    float userInvestment = 0;
    float monthlyDeposit = 0;
    float annualInterest = 0;
    float numYears = 0;

    //calls functions and assigns values
    InitialInvestmentLooks();
    userInvestment = InitialInvestment(userInvestment);
    monthlyDeposit = MonthlyDeposit(monthlyDeposit);
    annualInterest = AnnualInterest(annualInterest);
    numYears = NumYears(numYears);
    system("PAUSE"); //pauses - ONLY WORKS ON WINDOWS

    //Passes values to final function
    GetinvestmentNumbers(userInvestment, monthlyDeposit, annualInterest, numYears);

    return 0;
}


