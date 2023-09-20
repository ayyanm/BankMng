

#include "hashMap.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;


string lowerCase(const string& inpStr);

struct bankType
{
	struct transactionType
	{
		transactionType(bool t, double a)
		{
			type = t;
			amount = a;
		}

		bool type;
		double amount;
	};

	bankType(std::string first, std::string last, std::string acctNo,
		double balance,unsigned int pin)
	{
		name = last + ", " + first;
		this->balance = balance;
		this->pin = pin;
		this->acctNo = acctNo;
		locked = false;
	}

	bankType()
	{
		name = "";
		balance = 0;
		pin = 0;
		locked = false;
	}

	std::string name;
	std::string acctNo;
	double balance;
	unsigned int pin;
	bool locked;

	std::vector<transactionType> transactions;
};



int main()
{
	std::cout << std::fixed << std::setprecision(2);
	//creating map for bank
	hashMap< std::string, hashMap< std::string, bankType > > bank;

	//creating map for locations vector
	hashMap<string, bankType> locations;

	//initializing necessary vars
	int locAmt; 
	string fName, lName, acNum, location, balStr, pinStr, last4; 

	//loading data file into infile and opening 
    string filename = "data.csv";
    ifstream inFile(filename); 
	string line; 

	//looping thru data file to get parse thru string
	int itemNum = 0; 
	while (getline(inFile, line))
	{
		
		double bal; 
		unsigned int pinn; 

		//splitting string for each part of bankType()
		balStr = " ";

		//gets first name
		fName = line.substr(0, line.find(","));
		line.erase(0, line.find(",")+1);

		//gets last name
		lName = line.substr(0, line.find(","));
		line.erase(0, line.find(",")+1);


		//gets loc, sends to lowercase 
		location = line.substr(0, line.find(","));
		location = lowerCase(location);
		line.erase(0, line.find(",")+1);
		locations[location] = bankType(); 
		locations[location].name = "default";

		//get account num
		acNum = line.substr(0, line.find(","));
		line.erase(0, line.find(",")+1);

		//gets balance
		balStr = line.substr(0, line.find(","));
		line.erase(0, line.find(",")+1);

		//gets pin
		pinStr = line.substr(0, line.find(","));
		line.erase(0, line.find(",")+1);

		//gets last 4 to set up key
		if(!acNum.empty() ){
		last4 = acNum.substr(6, 9);
		}

		//converts to double if valid
		char check = balStr[0];
		if(isdigit(check)){
			bal = std::stod(balStr);
		}

		//converts to pin iff valid
		char checkInt = pinStr[0];
		if(isdigit(checkInt)){
			pinn = std::stoi(pinStr);
		}
		itemNum++;

		//if all is well, skipping first line, add to hashmap as banktype
		if (itemNum > 1){

		//adding to map 
		bank[location][last4] = bankType(fName, lName, acNum, bal, pinn);
		}	
	}
	string userLoc; 
	cout << endl; 
	bool tryAgain; 
	string input; 

	//loop to ask for branch
	do
	{
	
	//asking for branch
	cout << "Please enter bank branch location : "; 
	getline(cin, userLoc);	
    cout << endl; 
	userLoc = lowerCase(userLoc);
	//checking if location is in locations hashmap
	if (locations[userLoc].name ==  "default")
	{

	} else {
	//if not exits
		cout << "Bank branch not found" << endl;

		//asking to continue
		cout << "Continue? (Y/N): ";
			getline(cin, input);
			input = lowerCase(input);
			cout << endl; 
			if(input == "y")
			{
				continue;
			} else if(input == "n" ){
				continue; 
			}
	}

	//asking for account number
	string accNumStr;
	std::cout << "Enter last 4 digits of account: " << std::endl;
	getline(cin, accNumStr);

	//if wrong acc number
	if(bank[userLoc][accNumStr].name.empty()){
		cout << "Account not found" << endl;

		//asking to continue
		cout << "Continue? (Y/N): ";
			getline(cin, input);
			cout << endl; 
			input = lowerCase(input);
			if(input == "y")
			{
				continue;
			} else if(input == "n" || input == "N"){
				continue; 
			}

	//checking if locked or not
	} else if (bank[userLoc][accNumStr].locked == true)
	{
		std::cout << "Account has a lock" << std::endl;

		//asking to continue
		cout << "Continue? (Y/N): ";
			getline(cin, input);
			input = lowerCase(input);
			cout << endl; 
			if(input == "y")
			{
				continue;
			} else if(input == "n"){
				continue; 
			}
	}

	//check for pin
	bool valid = false;
	int i = 0; 
	string userPinStr; 

	//looping until user locks acc
	while (valid == false)
	{
		//asking for pin
		std::cout << "Enter a pin: " << std::endl;
		getline(cin, userPinStr);	
		int userPin = stoi(userPinStr); 


		//wrong pin
		if (bank[userLoc][accNumStr].pin != userPin)
		{

			std::cout << "Invalid pin" << endl;
			std::cout << std::endl;
			valid = false; 
			i++; 

			// too many attempts, locks account
			if (i > 2)
			{
				cout << "3 failed login attempts , account is now locked" << endl; 
				bank[userLoc][accNumStr].locked = true; 
				break;
			}

			continue;
			
		} else{
			valid = true;
			break; 
		}
	}

	if (valid == true)
	{	
	bool keepGoing = true;
/*
 * asking what to do with valid account
 *
 *
 * 
 */
	  do
	  {
		
	 
	  
	  string choice;
	  std::cout <<  "(D)eposit\n(W)ithdrawal\n(V)iew account" <<
	  "\n(E)xit account\nEnter choice: " 
	  << std::endl;

	  getline(cin, choice);

		//For Deposits
		if (choice == "D" || choice == "d")
		{	
			string depAmtStr; 
			std::cout << "Enter Deposit Amount: " << std::endl;
			getline(cin,depAmtStr);
			double depAmt = stod(depAmtStr);

			bank[userLoc][accNumStr].balance = depAmt + bank[userLoc][accNumStr].balance;

			bankType::transactionType newTrans(true, depAmt);
			bank[userLoc][accNumStr].transactions.push_back(newTrans);
			
			continue;

		//For withdrawals
		} else if (choice == "W" || choice == "w")
		{
			string withAmtStr; 
			std::cout << "Enter Withdrawal Amount: " << std::endl;
			getline(cin,withAmtStr);
			double withAmt = stod(withAmtStr);
			if (withAmt > bank[userLoc][accNumStr].balance)
			{
				std::cout << "not enough funds to make transaction" << std::endl;
				continue;
			}
			bank[userLoc][accNumStr].balance = bank[userLoc][accNumStr].balance - withAmt; 

			bankType::transactionType newTrans(false, withAmt);
			bank[userLoc][accNumStr].transactions.push_back(newTrans);
			
			continue;


		//For displaying	
		} else if (choice == "V" || choice == "v")
		{
			std::cout << "Name: " << bank[userLoc][accNumStr].name << std::endl;
			std::cout << "Balance: $" << bank[userLoc][accNumStr].balance << std::endl;
			std::cout << std::endl;

			if (!bank[userLoc][accNumStr].transactions.empty())
			{
				std::cout << "List of transactions" << std::endl;
				for (int i = 0; i < bank[userLoc][accNumStr].transactions.size(); i++) {
					if (bank[userLoc][accNumStr].transactions[i].type == false)
					{
						 std::cout << "Withdrawal amount $" <<
						 bank[userLoc][accNumStr].transactions[i].amount << endl;
					} else if(bank[userLoc][accNumStr].transactions[i].type == true){
						 std::cout << "Deposit amount $" <<
						 bank[userLoc][accNumStr].transactions[i].amount << endl;
					}
				}

				continue;

			} else {
				cout << "No Transactions"<< endl;
				continue;
			}
		//for exiting
		}  else if (choice == "E" || choice == "e")
		{
			keepGoing = false; 
			continue;

		//if no valid choice was made
		} else {
			cout << "Invalid Choice";
			continue; 
		}
	 } while (keepGoing == true);


	} else
	{
		//asking to continue
		cout << "Continue? (Y/N): ";
		getline(cin, input);
		cout << endl; 
		input = lowerCase(input);
		if(input == "y")
		{
			continue;
		} else if(input == "n"){
			continue; 
		}
	}
	
	cout << "Continue? (Y/N): ";
		getline(cin, input);
		cout << endl; 
		input = lowerCase(input);
		if(input == "y" )
		{
			continue;
		} else if(input == "n"){
			continue; 
		}
	
	
	} while (input == "y");
	
    

	return 0;
}



string lowerCase(const string& inpStr) {
    string result = inpStr;
    
    // iterate over each character and convert to uppercase
    for (int i = 0; i < result.length(); i++) {
        result[i] = tolower(result[i]);
    }
    
    return result;
}