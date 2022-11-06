#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//faster writting of std
using std::string;
using std::cout;
using std::cin;
using std::ios;
using std::vector;
//use protection against STD

void createUserAccount() //function for creating user, inputted elements will be pushed to a csv file: in this case file is called userDataBase
{
	string accountType = "0";
	string name, password;
	std::fstream userDataBase("userDataBase.csv", ios::in | ios::app);//allows us to read and to also 
	if (!userDataBase.is_open()) {
		cout << "Warning file is not open." << '\n';
		return;
	}

	cout << '\n';
	cout << "---------------------------------------------------------------------" << '\n';
	cout << "------------------------ create account -----------------------------" << '\n';//for aesethics 
	cout << "---------------------------------------------------------------------" << '\n';

	cout << "Enter a username: ";
	cin >> name;
	cout << "Enter a password: ";//Prompts for user to input their name and password
	cin >> password;

	userDataBase << name << "," << password << "," << accountType << '\n';//by default accountType = 0
	userDataBase.close();

	return;//exits to main 
}

void userLogin() {
	bool login = false;//password check 
	while (login == false) {


		std::fstream userDataBase;
		userDataBase.open("userDataBase.csv", ios::in | ios::app);//allows us to read from a file/ and append to file
		string line, word, name, password; //strings to store data from opened file 
		vector<string> row;
		vector<vector<string>> fileContent;
		if (!userDataBase.is_open()) {
			cout << "Warning file is not open" << '\n';
			return;
		}
		while (getline(userDataBase, line, '\n')) {
			row.clear();
			std::stringstream stream(line);

			while (getline(stream, word, ',')) {
				row.push_back(word);
			}
			fileContent.push_back(row);
		}

		cout << "Enter your username: ";
		cin >> name;
		cout << '\n';
		cout << "Enter your password: ";
		cin >> password;


		bool valid = false;
		for (vector<string> row : fileContent) {
			if (name == row.at(0) && password == row.at(1)) {
				valid = true;
			}
			
		}
		if (valid) {
			cout << "Welcome " << name << '\n';
			switch (stoi(row.at(2))) {
			case 0:
				cout << "logged in as user" << '\n';
				// run code for user functionality
				break;
			case 1:
				cout << "logged in as admin" << '\n';
				// run code for admin functionality
			}
			login = true;
		}
		else {
			cout << "login failed" << "\n";
		}
		

		userDataBase.close();//closes csv file
		fileContent.clear();//clears the filecontent vector
	}

}


//int main() {
//
//	int userChoice;
//	cout << "-------------------------------------------------------------" << '\n';
//	cout << "--------------------- some kind of company ------------------" << '\n';
//	cout << "-------------------------------------------------------------" << "\n\n";
//
//	do
//	{
//		cout << "please select the action you would like to take by entering the coressponding number." << '\n';
//		cout << "1) Login \t2) Create account \t 3) Exit" << '\n';//Creates a menu.
//		cout << "Enter choice: ";
//		cin >> userChoice;
//
//		switch (userChoice)
//		{
//		case 1:
//			cout << "logging in" << '\n';//if user inputs 1 UserLogin function will be called
//			userLogin();
//			break;
//		case 2:
//			cout << "creating account" << '\n';//if user inputs 2 createUser Function will be called
//			createUserAccount();
//			break;
//		case 3:
//			cout << "Thanks for using some kind of company\nHope to see you again soon\n";
//			cout << "application has closed\n";//Exits program
//			break;
//		}
//	} while (userChoice != 3);
//
//
//}








