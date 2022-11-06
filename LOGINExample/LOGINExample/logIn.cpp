#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>

using std::cin;
using std::cout;
using std::getline;
using std::ws;
using std::endl;
using std::vector;
using std::string;
using std::fstream; // general stream
using std::ios; // stream types

class file {
	fstream fileStreamer;

	void LoadCSV(string fileName, vector<vector<string>>* data) {

		fileStreamer.open(fileName, ios::in | ios::beg);
		if (!fileStreamer.is_open()) {
			// Save blank file if no file exists
			fileStreamer.open(fileName, ios::out); // overwrite save
			fileStreamer.close();
			fileStreamer.open(fileName, ios::in | ios::beg);
		}

		if (!fileStreamer.is_open()) {
			//Error("Unable to open CSV files...");
			return;
			// Error and return void if still unable to open file...
		}

		string line, word;
		vector<string> row;
		while (getline(fileStreamer, line, '\n')) { // Each row
			row.clear();
			std::stringstream stream(line);
			while (getline(stream, word, ',')) { // Each collumn
				row.push_back(word); // add cell to row vector
			}
			data->push_back(row); // add copy of row to fileContent vector
		}

		fileStreamer.close();
	}

	void SaveCSV(string fileName, vector<vector<string>>* data) {
		fileStreamer.open(fileName, ios::out); // overwrite
		if (fileStreamer.is_open()) {
			for (int vIndex = 0; (unsigned)vIndex < (unsigned)data->size(); vIndex++) {
				vector<string>* v = new vector<string>();
				for (string s : (*data)[vIndex]) {
					v->push_back(s);
				}

				for (int sIndex = 0; (unsigned)sIndex < (unsigned)v->size(); sIndex++) {
					string s = (*v)[sIndex];
					fileStreamer << s;
					if (sIndex != v->size() - 1) {
						fileStreamer << ',';
					}
				}
				fileStreamer << endl;


				v->clear();
				delete v;
				v = nullptr;
			}
			fileStreamer.close();
		}
		else {
			//Error("Unable to save CSV file...");
			return;
		}
	}
public:

	vector<vector<string>>* classroomData;
	vector<vector<string>>* schoolData;
	vector<vector<string>>* studentData;
	vector<vector<string>>* loginData;
	vector<vector<string>>* staffData;

	/// <summary>
	/// Loads csv file from Drive to Memory
	/// </summary>
	void LoadAll() {
		// close any existing file stream that may be open
		fileStreamer = fstream();

		if (fileStreamer.is_open()) {
			fileStreamer.close();
		}

		classroomData->clear();
		schoolData->clear();
		studentData->clear();
		loginData->clear();
		staffData->clear();

		this->LoadCSV("classroom.csv", classroomData);
		this->LoadCSV("school.csv", schoolData);
		this->LoadCSV("students.csv", studentData);
		this->LoadCSV("logins.csv", loginData);
		this->LoadCSV("staff.csv", staffData);
	}

	/// <summary>
	/// Saves all memory in file to csv file
	/// </summary>
	void SaveAll() {
		this->SaveCSV("classroom.csv", classroomData);
		this->SaveCSV("school.csv", schoolData);
		this->SaveCSV("students.csv", studentData);
		this->SaveCSV("logins.csv", loginData);
		this->SaveCSV("staff.csv", staffData);
	}

	file()
	{
		classroomData = new vector<vector<string>>();
		schoolData = new vector<vector<string>>();
		studentData = new vector<vector<string>>();
		loginData = new vector<vector<string>>();
		staffData = new vector<vector<string>>();
	}
};

void login() {
	bool login = false;
	while (login == false)
	{
		string line, word, name, password;
		string userCheck, passwordCheck;
		file* allFiles = new file();//Pointer called allFiles 
		allFiles->LoadAll();


		//vector<string> temp;
		//temp.push_back("timmy");
		//temp.push_back("password");
		//temp.push_back("0");
		//allFiles->loginData->push_back(temp);

		//temp.clear();

		//temp.push_back("tony");
		//temp.push_back("password4");
		//temp.push_back("1");
		//allFiles->loginData->push_back(temp);

		//allFiles->SaveAll();

		vector<string> row;
		vector<vector<string>> fileContent = *allFiles->loginData;//loads csv into filecontent



		cout << "\n" << "\t\t" << "--Log-In--" << "\n\n\n";

		cout << "Username:";
		cin >> userCheck;

		cout << "Password:";
		cin >> passwordCheck;

		int index;

		bool valid = false;
		for (int i = 0; i < fileContent.size(); i++) {
			for (vector<string> row : fileContent)
			{
				if (userCheck == (*allFiles->loginData)[i][0] && passwordCheck == (*allFiles->loginData)[i][1])
				{
					//cout << "Login Successful";
					valid = true;
					index = i;
				}
			}
		}
		if (valid) {
			cout << "Welcome " << userCheck << "\n";
			switch (stoi((*allFiles->loginData)[index][2])) {
			case 0:
				cout << "Logged in as user" << "\n\n";
				break;

			case 1:
				cout << "logged in as admin" << "\n";


			}
			login = true;
		}
		else {
			cout << "login failed" << "\n";
		}

		fileContent.clear();

	}
}

int main() {
	//(*allFiles->loginData)[0][0] == string
	// [0] -> usernames
	// [1] -> passwords
	// [2] -> account type

	//(*allFiles->loginData)[0][0] == username;
	//(*allFiles->loginData)[0][1] == password;
	//(*allFiles->loginData)[0][2] == userType;

	login();

}