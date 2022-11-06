#pragma region preprocessor
// header files
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <Windows.h>

using std::cin;
using std::cout;

using std::getline;
using std::ws;
using std::endl;

using std::vector;
using std::string;
using std::fstream;
using std::ios;


/// <summary>
/// holds a gender m, f, o
/// </summary>
enum class gender {
	male,
	female,
	other
};

/// <summary>
/// the learning progress of a student
/// </summary>
enum class learningProgress {
	achieved,
	progressing,
	needHelp
};

/// <summary>
/// holds grade information, class name and mark in class
/// </summary>
struct grade {
	string gradeName;
	int mark;

	/// <summary>
	/// constructor requires class name and class mark
	/// </summary>
	/// <param name="gradeName">the name of the class</param>
	/// <param name="mark">the mark in the class</param>
	grade(string gradeName, int mark)
	{
		this->gradeName = gradeName;
		this->mark = mark;
	}
};

using grades = vector<grade> * ; // simplifies mutliple grades

/// <summary>
/// Holds day and week as a date
/// </summary>
struct date {
	int week;
	int day;

	/// <summary>
	/// Constructor to make a date, needs day and week
	/// </summary>
	/// <param name="day">day of the week 1-7 for mon-sun</param>
	/// <param name="week">week number 1, 2, 3, etc</param>
	date(int day, int week)
	{
		this->day = day;
		this->week = week;
	}
};

class student;
class classroom;
class report;
class school;
class teacher;
class parent;
class admin;
class login;
class user;

string gtos(gender _gender);
string lptos(learningProgress _learningProgress);
string GetRawInput(string prompt, bool newline = false);
void Error(string msg);

#pragma endregion

#pragma region classes
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
			Error("Unable to open CSV files...");
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
			Error("Unable to save CSV file...");
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
/// <summary>
/// class which displays and holds a report
/// </summary>
class report {
	string _name;
	gender _gender;
	grades _grades;
	learningProgress _learningProgress;

	/// <summary>
	/// displays the grades within the report
	/// </summary>
	void outputGrades() {
		for (grade _grade : *_grades) {
			cout << _grade.gradeName << " : ";
			cout << _grade.mark << " / 100\n";
		}
	}

public:
	/// <summary>
	/// Exit code called for destructors
	/// </summary>
	void Exit() {

	}

	/// <summary>
	/// displays the report into the console
	/// </summary>
	void view() {
		cout << "------Report------\n";
		cout << "name : " << _name << endl;
		cout << "gender : " << gtos(_gender) << endl;
		cout << "grades : " << endl;
		outputGrades();
		cout << endl;
		cout << "overall learning progress : " << lptos(_learningProgress) << endl;
		cout << "---End of Report--\n";
	}

	/// <summary>
	/// constructor of the report, needs all variables
	/// </summary>
	/// <param name="_name">student name</param>
	/// <param name="_gender">student gender</param>
	/// <param name="_grades">student grades</param>
	/// <param name="_learningProgress">student learning progress</param>
	report(string _name, gender _gender, grades _grades, learningProgress _learningProgress) {
		this->_name = _name;
		this->_gender = _gender;
		this->_grades = _grades;
		this->_learningProgress = _learningProgress;
	}
};

/// <summary>
/// a student with records and basic info
/// <para>a logged in student can see its Records and School notices</para>
/// </summary>
class student {
	vector<report>* reports; // pointer to list of reports ** vector
	string name; // full name of student

	/// <summary>
	/// gets all reports from file using a student number
	/// </summary>
	/// <param name="studentNumber"></param>
	static vector<report>* reportGetAll(int studentNumber) {
		// TODO get file system to read student number from students folder and get raw data
		vector<report>* _reports = new vector<report>();


		// get reports here


		return _reports;
	}

public:
	int studentNumber; // student number 
	
	/// <summary>
	/// adds a report to the Student instance 
	/// </summary>
	void reportAdd() {

	}

	/// <summary>
	/// edits an existing report in the Student instance
	/// </summary>
	void reportEdit() {

	}

	/// <summary>
	/// deletes a report within the Student instance
	/// </summary>
	void reportDelete(){

	}

	/// <summary>
	/// displays a report from the Student instance
	/// </summary>
	void reportView(int indexReport) {
		reports->at(indexReport).view(); // Report class displays the report
	}

	/// <summary>
	/// returns a report from the student instance
	/// </summary>
	report reportGet() {




		report temp = report("test", gender::male, NULL, learningProgress::achieved);

		return temp; // -------------- temp return void to prevent error --------------
	}

	/// <summary>
	/// Constructor for student instance
	/// </summary>
	/// <param name="name">student name</param>
	/// <param name="studentNumber">student number</param>
	/// <param name="newStudent">is a new student</param>
	student(string name, int studentNumber, bool newStudent = false) {
		this->name = name;
		this->studentNumber = studentNumber;
		reports = new vector<report>();
		
		if (!newStudent) { // if student is already in system read contents from file
			reports = reportGetAll(this->studentNumber);
		}
		else {
			reports = new vector<report>();
		}

	}

	/// <summary>
	/// Exit code called for destructors
	/// </summary>
	void Exit() {
		if (reports != nullptr) {
			reports->erase(reports->begin(), reports->end());
			delete reports;
			reports = nullptr;
		}
	}
};

/// <summary>
/// classroom which holds students
/// </summary>
class classroom {
	vector<student>* students; // all student in the classroom
	int number; // classroom number


public:
	/// <summary>
	/// checks if a student is in this classroom
	/// </summary>
	/// <param name="studentNumber">the student number you are checking for</param>
	/// <returns>bool true if found, false if not</returns>
	bool studentExists(int studentNumber) {
		for (student _student : *students) {
			if (_student.studentNumber == studentNumber) {
				return true; // student found
			}
		}

		return false; // no student found
	}
	
	/// <summary>
	/// Gets the roll of students in that class
	/// </summary>
	/// <returns></returns>
	vector<student>* getRoll() {
		return students;
	}

	/// <summary>
	/// Edits the roll if the logged in user has the correct credentials
	/// </summary>
	/// <param name="credentials">the login details</param>
	void EditRoll() {
		// TODO implement the edit roll feature
	}

	/// <summary>
	/// <para>gets a student with student number</para>
	/// <para>WARNING - call studentExists before to prevent crash</para> 
	/// </summary>
	/// <param name="studentNumber"></param>
	/// <returns></returns>
	student getStudent(int studentNumber) {
		for (student _student : *students) {
			if (_student.studentNumber == studentNumber) {
				return _student; // student found
			}
		}

		exit(1); // exit program with error code 1
	}

	/// <summary>
	/// Exit code called for destructors
	/// </summary>
	void Exit() {
		if (students != nullptr) {
			for (student _student : *students) {
				_student.Exit();
			}
			students->erase(students->begin(), students->end());
			delete students;
			students = nullptr;
		}
	}

};

/// <summary>
/// school hold all the information and related functions
/// </summary>
class school {
	vector<classroom>* classrooms;
	vector<string>* notices;
	string schoolName;
	string contactDetails;
	vector<string>* datesEvents;

public:
	/// <summary>
	/// welcome screen
	/// </summary>
	void Welcome() {
		cout << "*******************************************\n";
		cout << "Welcome to " << schoolName << endl;
		cout << "Contact details " << contactDetails << endl;
		cout << endl;
	}

	/// <summary>
	/// sets up the school
	/// </summary>
	void NewSchoolScreen() {
		// TODO implement new school screen








		
		Welcome(); // start welcome like normal after setup
	}

	/// <summary>
	/// loads school classrooms from files
	/// </summary>
	/// <returns>the amount of classrooms</returns>
	int PopulateClassrooms() {
		// TODO load from file system

		return 0; // 0 default
	}

	/// <summary>
	/// displays the menu screen for user to see options
	/// </summary>
	/// <param name="loggedIn"></param>
	void MenuScreen(bool loggedIn) {
		cout << "Options:\n";
		if (loggedIn) {
			cout << "1. Logout\n";
			cout << "2. Exit\n";
			cout << "3. View more options\n";
		}
		else {
			cout << "1. Login/Signup:\n";
			cout << "2. Exit\n";
		}
	}

	/// <summary>
	/// Exit code called for destructors
	/// </summary>
	void Exit() {
		if (classrooms != nullptr) {
			for (classroom _classroom : *classrooms)
			{
				_classroom.Exit();
			}
			classrooms->erase(classrooms->begin(), classrooms->end());
			delete classrooms;
			classrooms = nullptr;
		}
	}


};

/// <summary>
/// login information and functions
/// </summary>
class login {
public:

	static void SignUpTemp(file* allFiles, string userType) {

		string username;
		string password;

		bool duplicate = true;
		while (duplicate) {
			username = GetRawInput("Enter your Username: ");
			password = GetRawInput("Enter your Password: ");

			duplicate = false;
			// add loop here
			for (vector<string> v : *allFiles->loginData) {
				if (username == v[0]) {
					cout << "Duplicate found\n";
					duplicate = true;
				}
			}
		}
		vector<string> newUserLoginData;

		newUserLoginData.push_back(username);
		newUserLoginData.push_back(password);
		newUserLoginData.push_back(userType);
		
		allFiles->loginData->push_back(newUserLoginData);


		cout << "Your Username and Password have been saved";
		allFiles->SaveAll();
	}

	static void LogInTemp(file* allFiles) {
		string username;
		string password;

		int index;
		bool success = false;

		bool duplicateUsername = true;
		while (duplicateUsername) {
			username = GetRawInput("Enter your Username: ");
			duplicateUsername = true;
			// add loop here
			for (int i = 0; i < (allFiles->loginData->size()); i++) {
				vector<string> v = (*allFiles->loginData)[i];
				if (username == v[0]) {
					cout << "Username found\n";
					duplicateUsername = false;
					index = i;
				}
			}
			if (duplicateUsername) {
				cout << "No username found";
			}
		}
		
		for (int i = 0; i < 3; i++) {
			password = GetRawInput("Enter your Password: ");
			if (password == (*allFiles->loginData)[index][1]) {
				cout << "password matches\n";
				success = true;
				break;
			}
			else {
				cout << "password wrong...\n";
			}
		}

		if (success) {
			cout << "Successfully logged in...\n";
		}
		else {
			cout << "unsuccessfully logged in, try again later\n";
			srand(time(NULL));
			int t = rand() % 300;
			cout << "You have been locked out for an random amount of time: " << t << " seconds";
			Sleep (t * 1000);
			cout << "\n";
		}
	}

	auto static Login() {

	}
	/// <summary>
	/// Exit code called for destructors
	/// </summary>
	void Exit() {

	}


};

/// <summary>
/// admin login
/// </summary>
class admin {



public:
	/// <summary>
	/// Exit code called for destructors
	/// </summary>
	void Exit() {

	}


};

/// <summary>
/// teacher login
/// </summary>
class teacher {



public:
	/// <summary>
	/// Exit code called for destructors
	/// </summary>
	void Exit() {

	}


};

/// <summary>
/// parent login
/// </summary>
class parent {




public:
	/// <summary>
	/// Exit code called for destructors
	/// </summary>
	void Exit() {

	}


};

/// <summary>
/// user represents the current user using union
/// </summary>
class user {
	union userType {
	public:
		parent _parent;
		teacher _teacher;
		admin _admin;
	};
public:
	userType _userType;
	bool loggedIn = false;
	void Login() {
		//auto _user = login::Login();
	}

	void Logout() {

	}

	/// <summary>
	/// Exit code called for destructors
	/// </summary>
	void Exit() {

	}

	/// <summary>
	/// Deals with options of each user type
	/// </summary>
	/// <param name="_school">reference of the school instance if required for operations</param>
	void Options(school * _school) {
		
	}
};
#pragma endregion

#pragma region functions

/// <summary>
/// gender to string
/// </summary>
/// <param name="_gender">gender</param>
/// <returns>string of gender</returns>
string gtos(gender _gender) {
	switch (_gender)
	{
	case gender::male:
		return "male";
		break;
	case gender::female:
		return "female";
		break;
	case gender::other:
		return "other";
		break;
	default:
		Error("ERROR - gtos couldn't convert gender to a string...");
		break;
	}
	return "";
}

/// <summary>
/// learningProgress to string
/// </summary>
/// <param name="_learningProgress">learning progress of studentr</param>
/// <returns>string of learningProgress</returns>
string lptos(learningProgress _learningProgress) {
	switch (_learningProgress)
	{
	case learningProgress::achieved:
		return "achieved";
		break;
	case learningProgress::progressing:
		return "progressing";
		break;
	case learningProgress::needHelp:
		return "NEEDS HELP";
		break;
	default:
		Error("ERROR - lptos couldn't convert learningProgress to a string...");
		break;
	}
	return "";
}

/// <summary>
/// gets raw input from the console using getline (prevents crashes)
/// </summary>
/// <param name="prompt">display to the console before getting input</param>
/// <param name="newline">newline before input or not</param>
/// <returns>raw input from console</returns>
string GetRawInput(string prompt, bool newline) {
	cout << prompt;
	
	if (newline) { cout << endl; }
	string rawInput;

	getline(cin >> ws, rawInput);

	return rawInput;
}

/// <summary>
/// displays and error message to the console
/// </summary>
/// <param name="msg">msg to display</param>
void Error(string msg) {
	cout << "*****ERROR*****\n";
	cout << msg << endl;
	cout << "***************\n";
}

#pragma endregion



/// <summary>
/// entry point to the program
/// </summary>
/// <returns>exit status</returns>
int main()
{
	bool DEBUG = true;

	if (DEBUG) {

		user _user;
		file* allFiles = new file();
		allFiles->LoadAll();
		login::LogInTemp(allFiles);
		login::SignUpTemp(allFiles, "admin");

		/*(*allFiles->loginData)[0][0] = "bob";
		(*allFiles->loginData)[0][0] = "password";

		allFiles->SaveAll();*/


	}
	else{

		user _user;

		school* _school = new school();
		int classrooms = _school->PopulateClassrooms();
		if (classrooms == 0) {
			_school->NewSchoolScreen();
		}
		else {
			_school->Welcome();
		}

		// loop while program is active
		bool running = true;
		while (running) {
			_school->MenuScreen(_user.loggedIn); // intro screen for main options

			string rawInput = GetRawInput("Your selection : "); // input
			try {
				int input = stoi(rawInput); // parsed input

				// deal with input accordingly
				switch (input)
				{
				case 1: { // login / logout
					if (_user.loggedIn) {
						_user.Logout();
					}
					else {
						_user.Login();
					}
					break;
				}
				case 2: { // exit
					// may have to save data?? TODO: make sure exit is safe
					_school->Exit();
					_user.Exit();
					running = false;
					break;
				}
				case 3: { // if a user is logged in, op 3 is more options
					if (_user.loggedIn) {
						_user.Options(_school);
					}
					else {
						Error("invalid input...");
					}
					break;
				}
				default: {
					Error("invalid input...");
					break;
				}
				}
			}
			catch (...) {
				Error("invalid input..."); // error is parse fails, i.e, rawInput isn't a string
			}
			// deal with input
			// exit if requried
		}

		delete _school;
		_school = nullptr;

	}

	return 0; // Exit the program with no issues
}

