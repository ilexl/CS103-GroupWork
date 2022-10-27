#pragma region preprocessor
// header files
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;

using std::getline;
using std::ws;
using std::endl;

using std::vector;
using std::string;

using grades = vector<grade> * ; // simplifies mutliple grades

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

/// <summary>
/// entry point to the program
/// </summary>
/// <returns>exit status</returns>
int main()
{
	user _user;

	school* _school = new school();
	int classrooms = _school->populateClassrooms();
	if (classrooms == 0) {
		_school->newSchoolScreen();
	}
	else {
		_school->Welcome();
	}

	// loop while program is active
	bool running = true;
	while (running) {
		_school->IntroScreen(_user.loggedIn); // intro screen for main options

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

	return 0; // Exit the program with no issues
}

#pragma region classes
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






		return; // -------------- temp return void to prevent error --------------
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
		delete reports;
		reports = nullptr;
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
	void EditRoll(login credentials) {
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
		for (student _student : *students) {
			_student.Exit();
		}
		delete students;
		students = nullptr;
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
	void Welcome() {
		cout << "*******************************************\n";
		cout << "Welcome to " << schoolName << endl;
		cout << "Contact details " << contactDetails << endl;
		cout << endl;
	}

	void newSchoolScreen() {
		// TODO implement new school screen









		Welcome(); // start welcome like normal after setup
	}

	int populateClassrooms() {
		// TODO load from file system

		return 0;
	}

	void IntroScreen(bool loggedIn) {
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
		for(classroom _classroom : *classrooms)
		{
			_classroom.Exit();
		}
		delete classrooms;
		classrooms = nullptr;
	}


};

/// <summary>
/// login information and functions
/// </summary>
class login {



public:

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
		return "ERROR - gtos couldn't convert gender to a string...";
		break;
	}
	return;
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
		return "ERROR - lptos couldn't convert learningProgress to a string...";
		break;
	}
	return;
}

/// <summary>
/// gets raw input from the console using getline (prevents crashes)
/// </summary>
/// <param name="prompt">display to the console before getting input</param>
/// <param name="newline">newline before input or not</param>
/// <returns>raw input from console</returns>
string GetRawInput(string prompt, bool newline = false) {
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
