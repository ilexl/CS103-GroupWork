#pragma region preprocessor

#pragma region header files

// header files
#include <iostream>  // console IO
#include <string>    // getline
#include <cstring>   // string management
#include <vector>    // memory
#include <fstream>   // file managment
#include <sstream>   // file managment
#include <chrono>    // sleeping console
#include <thread>    // holding time in thread
#include <Windows.h> // color in console text

#pragma endregion

#pragma region using

using std::cin;      // console input
using std::cout;     // console output

using std::getline;  // getline for files and cin
using std::ws;		 // white space " "
using std::endl;     // '\n'

using std::vector;   // std namespace vector
using std::string;   // std namespace string

using std::fstream; // general stream
using std::ios; // stream types

#pragma endregion

#pragma region data types

/// <summary>
/// holds a gender m, f, o
/// gtos converts gender to a string
/// </summary>
enum class gender {
	male,
	female,
	other
};

/// <summary>
/// the learning progress of a student
/// lptos converts lp to a string
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

// simplifies mutliple grades
using grades = vector<grade> * ; 

/// <summary>
/// Holds day and week as a date for a report
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

#pragma endregion

#pragma region function prototypes

// function prototypes used throughout program

string gtos(gender _gender);
string lptos(learningProgress _learningProgress);
string GetRawInput(string prompt, bool newline = false);
void Error(string msg);
void Title(string t, int len = 40);

#pragma endregion

#pragma endregion

#pragma region classes

/// <summary>
/// class which displays and holds a report
/// </summary>
class file {
	fstream fileStreamer; // file stream obj

	/// <summary>
	/// loads a csv from file
	/// </summary>
	/// <param name="fileName">the file name to load</param>
	/// <param name="data">the vector to hold the data</param>
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

	/// <summary>
	/// saves a csv from vector in memory to file as overwrite
	/// </summary>
	/// <param name="fileName">file name to save</param>
	/// <param name="data">data in memory</param>
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

public: // makes accessible outside the class{}

	// all of the vectors to store the data from numerous files

	vector<vector<string>>* classroomData;
	vector<vector<string>>* schoolData;
	vector<vector<string>>* studentData;
	vector<vector<string>>* loginData;
	vector<vector<string>>* staffData;
	vector<vector<string>>* parentData;

	/// <summary>
	/// loads csv file from Drive to Memory
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
		parentData->clear();

		this->LoadCSV("_classroom.csv", classroomData);
		this->LoadCSV("_school.csv", schoolData);
		this->LoadCSV("_students.csv", studentData);
		this->LoadCSV("_logins.csv", loginData);
		this->LoadCSV("_staff.csv", staffData);
		this->LoadCSV("_parent.csv", parentData);
	}

	/// <summary>
	/// saves all memory in file to csv file
	/// </summary>
	void SaveAll() {
		this->SaveCSV("_classroom.csv", classroomData);
		this->SaveCSV("_school.csv", schoolData);
		this->SaveCSV("_students.csv", studentData);
		this->SaveCSV("_logins.csv", loginData);
		this->SaveCSV("_staff.csv", staffData);
		this->SaveCSV("_parent.csv", parentData);
	}

	/// <summary>
	/// ctor for file instances
	/// </summary>
	file()
	{
		// creates memory in heap for all data types

		classroomData = new vector<vector<string>>();
		schoolData = new vector<vector<string>>();
		studentData = new vector<vector<string>>();
		loginData = new vector<vector<string>>();
		staffData = new vector<vector<string>>();
		parentData = new vector<vector<string>>();
	}
};

#pragma region function prototypes

// Signup/DeleteLogin/displayNotices function prototypes -> declaration order must be after file 
void SignUp(file* allFiles, string userType, int number = NULL);
void DeleteLogin(file* allData, int number, string type);
void displayNotices(file* allData);
void EditLogin(file* allData, int number, string type);
void ViewDetails(file* allData, string targetNumberString, string type);

#pragma endregion

/// <summary>
/// a student report with various functions and data
/// </summary>
class report {

	string _name;   // name of the student
	gender _gender; // gender of the student
	grades _grades; // vector of grades of the student
	learningProgress _learningProgress; // lp of the student
	
	/// <summary>
	/// displays the grades within the report
	/// </summary>
	void outputGrades() {
		for (grade _grade : *_grades) {
			cout << _grade.gradeName << " : ";
			cout << _grade.mark << " / 100\n";
		}
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

public: // makes members accessible outside the class{}
	int week;
	
	/// <summary>
	/// exit code called for destructors
	/// </summary>
	void Exit() {
		if (_grades != NULL) {
			_grades->clear();
		}
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------


	/// <summary>
	/// displays the report into the console
	/// </summary>
	void view() {
		cout << "-----------------Report-----------------\n\n";
		cout << "Week :" << week << endl;
		cout << "Name : " << _name << endl;
		cout << "Gender : " << gtos(_gender) << endl;
		cout << "Grades : " << endl;
		outputGrades();
		cout << "Overall Learning Progress : " << lptos(_learningProgress) << endl;
		cout << "\n--------------End of Report-------------\n\n";

	}
	// ------------------------ ^ NOT TESTED ^-----------------------------


	/// <summary>
	/// constructor of the report, needs all variables
	/// </summary>
	/// <param name="_name">student name</param>
	/// <param name="_gender">student gender</param>
	/// <param name="_grades">student grades</param>
	/// <param name="_learningProgress">student learning progress</param>
	report(string _name, int week, gender _gender, grades _grades, learningProgress _learningProgress) {
		this->_name = _name;
		this->_gender = _gender;
		this->week = week;
		this->_grades = _grades;
		this->_learningProgress = _learningProgress;
	}

	/// <summary>
	/// Converts raw string to report data type for a student
	/// </summary>
	/// <param name="rawStringFromFile">raw data</param>
	report(string rawStringFromFile) {
		string word;
		vector<string> row;
		_grades = new vector<grade>;
		row.clear();
		std::stringstream stream(rawStringFromFile);
		while (getline(stream, word, '~')) { // Each collumn
			row.push_back(word); // add cell to row vector
		}
		
		_name = row[0];
		week = stoi(row[1]);
		if (row[2] == "male") {
			_gender = gender::male;
		}
		else if (row[2] == "female") {
			_gender = gender::female;
		}
		else if (row[2] == "other") {
			_gender = gender::other;
		}
		
		for (int i = 3; (unsigned)i < (unsigned)row.size() - 1; i++) {
			vector<string> innerRow;
			innerRow.clear();
			std::stringstream istream(row[i]);
			while (getline(istream, word, '/')) { // Each collumn
				innerRow.push_back(word); // add cell to row vector
			}

			string gName = innerRow[0];
			string gMark = innerRow[1];
			int gMarkI = stoi(gMark);
			_grades->push_back(grade(gName, gMarkI));
		}

		if (row[4] == "achieved") {
			_learningProgress = learningProgress::achieved;
		}
		else if (row[4] == "progressing") {
			_learningProgress = learningProgress::progressing;
		}
		else if (row[4] == "NEEDS HELP") {
			_learningProgress = learningProgress::needHelp;
		}

	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

	/// <summary>
	/// converts report instance to string to store in file
	/// </summary>
	/// <returns>string of report</returns>
	string to_string() {
		string out = _name + '~'+ std::to_string(week) + "~" + gtos(_gender) + '~';
		for (grade g : *_grades)
		{
			out += g.gradeName + '/' + std::to_string(g.mark) + '~';
		}
		out += lptos(_learningProgress);
		return out;
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------
};

/// <summary>
/// a student with records and basic info
/// <para>a logged in student can see its Records and School notices</para>
/// </summary>
class student {
	file* loggedInStudentAllData; // stores the pointer to all data for a logged in student
	vector<report>* reports; // pointer to list of reports ** vector
	string name; // full name of student

	/// <summary>
	/// gets all reports from file using a student number
	/// </summary>
	/// <param name="studentNumber"></param>
	static vector<report>* reportGetAll(int studentNumber, file* data) {
		// TODO get file system to read student number from students folder and get raw data
		vector<report>* _reports = new vector<report>();

		for (vector<string> studentData : *data->studentData) {
			if (stoi(studentData[0]) == studentNumber) {
				for (int i = 2; (unsigned)i < (unsigned)studentData.size(); i++) {
					// each i that executes is a different report
					string raw = studentData[i];
					report r = report(raw);
					_reports->push_back(r);
				}
			}
		}
		// get reports here


		return _reports;
	}

public:
	int studentNumber; // student number 

#pragma region Reports


	/// <summary>
	/// adds a report to the Student instance 
	/// </summary>
	void reportAdd() {
		int rWeek;
		string rName;
		gender rGender;
		grades rGrades = new vector<grade>;
		learningProgress rLP;

		bool getMoreInfo;
		
		// week
		getMoreInfo = true;
		while (getMoreInfo) {
			string week = GetRawInput("Enter Week For Report : ");
			try {
				rWeek = stoi(week);
				getMoreInfo = false;
			}
			catch (...) {
				Error("INVALID INPUT");
			}
		}
	
		// name
		rName = name;

		// gender
		getMoreInfo = true;
		while (getMoreInfo) {
			string in = GetRawInput("Enter Gender (1. male, 2. female, 3. other) : "); // raw input
			getMoreInfo = false; // stop loop

			// assign gender based on input
			if (in == "1") {
				rGender = gender::male;
			}
			else if (in == "2") {
				rGender = gender::female;
			}
			else if (in == "3") {
				rGender = gender::other;
			}
			else {
				Error("INVALID INPUT"); // error if valid
				getMoreInfo = true; // loop again
			}

		}

		// grades
		getMoreInfo = true;
		while (getMoreInfo) {
			if (rGrades->size() >= 1) {
				string gradeGet = GetRawInput("Add Another Grade y or n : ");
				if (gradeGet == "y") {

				}
				else if (gradeGet == "n") {
					getMoreInfo = false;
					break;
				}
				else {
					Error("INVALID INPUT");
					continue;
				}
			}
			try {
				string gName = GetRawInput("Enter Grade Name : ");
				int gMark = stoi(GetRawInput("Enter Grade Mark /100 : "));
				grade g = grade(gName, gMark);
				rGrades->push_back(g);
			}
			catch (...) {
				Error("INVALID INPUT");
			}
		}

		// learning progress
		getMoreInfo = true;
		while (getMoreInfo) {
			string input = GetRawInput("1. Achieved, 2. Progressing, 3. Needs Help\nEnter Learning Progress : ");
			if (input == "1") {
				rLP = learningProgress::achieved;
				getMoreInfo = false;
			}
			else if (input == "2") {
				rLP = learningProgress::progressing;
				getMoreInfo = false;

			}

			else if (input == "3") {
				rLP = learningProgress::needHelp;
				getMoreInfo = false;

			}
			else {
				Error("INVALID INPUT");
			}
		}

		report r = report(rName, rWeek, rGender, rGrades, rLP);

		reports->push_back(r);
		string toMem = r.to_string();

		for (int i = 0; i < (*loggedInStudentAllData->studentData).size(); i++){
			vector<string> student = (*loggedInStudentAllData->studentData)[i];
			if (student[0] == std::to_string(studentNumber)) {
				(*loggedInStudentAllData->studentData)[i].push_back(toMem);
				cout << "\nSuccessfully Saved\n\n";
				loggedInStudentAllData->SaveAll();
				return;
			}
		}

		Error("Not saved to student file... Only Saved to student object?");
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

	/// <summary>
	/// edits an existing report in the Student instance
	/// </summary>
	void reportEdit() {
		ListReports();
		string input = GetRawInput("Enter report number or 'e' to cancel: ");
		cout << endl;
		if (input == "e") {
			return;
		}
		try {
			int reportNum = stoi(input);
			if (reports->size() > 0) {
				if (reportNum >= 0 && reportNum < reports->size()) {
					reports->erase(reports->begin() + reportNum);
					reportAdd();
					cout << "Report Successfully Editted...\n\n";
					vector<string> studentThisData;
					studentThisData.push_back(std::to_string(studentNumber));
					studentThisData.push_back(name);

					for (report r : *reports) {
						studentThisData.push_back(r.to_string());
					}
					
					for (int i = 0; (unsigned)i < (unsigned)loggedInStudentAllData->studentData->size(); i++) {
						vector<string> v = (*loggedInStudentAllData->studentData)[i];
						if (v[0] == std::to_string(studentNumber)) {
							v.clear();
							loggedInStudentAllData->studentData->erase(loggedInStudentAllData->studentData->begin() + i);
						}
					}

					loggedInStudentAllData->studentData->push_back(studentThisData);
					loggedInStudentAllData->SaveAll();
				}
				else {
					Error("INVALID INPUT");
				}
			}
			else {
				Error("INVALID INPUT");
			}
		}
		catch (...) {
			Error("INVALID INPUT");
		}
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

	/// <summary>
	/// deletes a report within the Student instance
	/// </summary>
	void reportDelete(){
		ListReports();
		string input = GetRawInput("Enter report number or 'e' to cancel: ");
		cout << endl;

		if (input == "e") {
			return;
		}
		try {
			int reportNum = stoi(input);
			if (reports->size() > 0) {
				if (reportNum >= 0 && reportNum < reports->size()) {
					reports->erase(reports->begin() + reportNum);
					cout << "Report Successfully Deleted...\n\n";
					vector<string> studentThisData;
					studentThisData.push_back(std::to_string(studentNumber));
					studentThisData.push_back(name);

					for (report r : *reports) {
						studentThisData.push_back(r.to_string());
					}

					for (int i = 0; (unsigned)i < (unsigned)loggedInStudentAllData->studentData->size(); i++) {
						vector<string> v = (*loggedInStudentAllData->studentData)[i];
						if (v[0] == std::to_string(studentNumber)) {
							v.clear();
							loggedInStudentAllData->studentData->erase(loggedInStudentAllData->studentData->begin() + i);
						}
					}

					loggedInStudentAllData->studentData->push_back(studentThisData);
					loggedInStudentAllData->SaveAll();
				}
				else {
					Error("INVALID INPUT");
				}
			}
			else {
				Error("INVALID INPUT");
			}
		}
		catch (...) {
			Error("INVALID INPUT");
		}
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

	/// <summary>
	/// displays a report from the Student instance
	/// </summary>
	void reportView(int indexReport) {
		reports->at(indexReport).view(); // Report class displays the report
	}

	void ListReports() {
		Title("All Reports");
		for (int i = 0; i < reports->size(); i++) {
			cout << i << ". Week " << ((*reports)[i]).week << " Report\n";
		}
		if (reports->size() == 0) {
			cout << "No Reports To Show...\n";
		}
		cout << endl;
	}

	// ------------------------ ^ NOT TESTED ^-----------------------------


	/// <summary>
	/// menu for editting reports
	/// </summary>
	void EditReports() {
		// number avalibe as is requires to be a student obj with a num
		bool inReports = true;
		while (inReports) {
			cout << "Options:\n";
			cout << "0. Back\n";
			cout << "1. View Report\n";
			cout << "2. Add Report\n";
			cout << "3. Edit Report\n";
			cout << "4. Delete Report\n";
			string input = GetRawInput("\nSelection : ");
			cout << endl;
			if (input == "0") {
				inReports = false;
				return;
			}
			else if (input == "1") {
				reportOption();
			}
			else if (input == "2") {
				reportAdd();
			}
			else if (input == "3") {
				reportEdit();
			}
			else if (input == "4") {
				reportDelete();
			}
			else {
				Error("INVALID INPUT");
			}
		}
	}

	/// <summary>
	/// reports option for student or parent
	/// </summary>
	void reportOption() {
		ListReports();
		string input = GetRawInput("Enter report number or 'e' to cancel: ");
		cout << endl;

		if (input == "e") {
			return;
		}
		try {
			int reportNum = stoi(input);
			if (reports->size() > 0) {
				if (reportNum >= 0 && reportNum < reports->size()) {
					reportView(reportNum);
				}
				else {
					Error("INVALID INPUT");
				}
			}
			else {
				Error("INVALID INPUT");
			}
		}
		catch (...) {
			Error("INVALID INPUT");
		}

	}

#pragma endregion

	/// <summary>
	/// ctor for student instance
	/// </summary>
	/// <param name="name">student name</param>
	/// <param name="studentNumber">student number</param>
	/// <param name="newStudent">is a new student</param>
	student(string name, int studentNumber, file* data, bool newStudent = false) {
		this->name = name;
		this->studentNumber = studentNumber;
		reports = new vector<report>();
		
		if (!newStudent) { // if student is already in system read contents from file
			reports = reportGetAll(this->studentNumber, data);
		}
		else {
			reports = new vector<report>();
		}

	}

	/// <summary>
	/// exit called for destructors
	/// </summary>
	void Exit() {
		if (reports != nullptr) {
			for (report r : *reports) {
				r.Exit();
			}
			reports->erase(reports->begin(), reports->end());
			delete reports;
			reports = nullptr;
		}
	}

	/// <summary>
	/// creates a new student
	/// </summary>
	/// <param name="allData">data stored in memory</param>
	/// <returns>student number</returns>
	static int CreateNew(file* allData) {
		int studentNumber = 0; // allocate memory
		bool studentNumCorrect = false; // assume number is invalid
		while (studentNumCorrect == false) { // loop while number is invalid
			string s = GetRawInput("Enter a new student number (8 digits): "); // get raw input
			try {
				studentNumber = stoi(s); // try convert to number
				studentNumCorrect = true; // if converted number is valid
			}
			catch (...) {
				Error("INVALID INPUT"); // catch a conversion error
			}
		}

		// loop through each student in memory
		for (vector<string> v : *allData->studentData) {
			if (v.size() > 0) {
				if (v[0] == std::to_string(studentNumber)) { // look for matching student number
					Error("Student already exists..."); // error if found
					studentNumber = CreateNew(allData, true);
				}
			}
		}

		string studentName = GetRawInput("Enter student's full name : "); // get name of new student
		cout << endl;
		student s = student(studentName, studentNumber, allData, true); // create a new student instance

		// store new student in memory and to file
		vector<string> sv;
		sv.push_back(std::to_string(studentNumber));
		sv.push_back(studentName);
		allData->studentData->push_back(sv);
		allData->SaveAll();

		return studentNumber; // return the student number of new student
	}

	/// <summary>
	/// creates a new student
	/// </summary>
	/// <param name="allData">data stored in memory</param>
	/// <returns>student number</returns>
	static int CreateNew(file* allData, bool t) {
		int studentNumber = 0; // allocate memory
		bool studentNumCorrect = false; // assume number is invalid
		while (studentNumCorrect == false) { // loop while number is invalid
			string s = GetRawInput("Enter a new student number (8 digits): "); // get raw input
			try {
				studentNumber = stoi(s); // try convert to number
				studentNumCorrect = true; // if converted number is valid
			}
			catch (...) {
				Error("INVALID INPUT"); // catch a conversion error
			}
		}

		// loop through each student in memory
		for (vector<string> v : *allData->studentData) {
			if (v.size() > 0) {
				if (v[0] == std::to_string(studentNumber)) { // look for matching student number
					Error("Student already exists..."); // error if found
					studentNumber = CreateNew(allData, true);
				}
			}
		}

		return studentNumber; // return the student number of new student
	}

	static int CreateNew(file* allData, int parentNumber) {
		int studentNumber = 0; // allocate memory
		bool studentNumCorrect = false; // assume number is invalid
		while (studentNumCorrect == false) { // loop while number is invalid
			string s = GetRawInput("Enter a new student number (8 digits): "); // get raw input
			try {
				studentNumber = stoi(s); // try convert to number
				if (parentNumber == studentNumber) {
					studentNumCorrect = false;
					Error("Cannot be the same as the parent number");
				}
				else {
					studentNumCorrect = true; // if converted number is valid
				}
			}
			catch (...) {
				Error("INVALID INPUT"); // catch a conversion error
			}
		}

		// loop through each student in memory
		for (vector<string> v : *allData->studentData) {
			if (v[0] == std::to_string(studentNumber)) { // look for matching student number
				Error("Student already exists..."); // error if found
				studentNumber = CreateNew(allData, true);
			}
		}

		string studentName = GetRawInput("Enter student's full name : "); // get name of new student
		cout << endl;
		student s = student(studentName, studentNumber, allData, true); // create a new student instance

		// store new student in memory and to file
		vector<string> sv;
		sv.push_back(std::to_string(studentNumber));
		sv.push_back(studentName);
		allData->studentData->push_back(sv);
		allData->SaveAll();

		return studentNumber; // return the student number of new student
	}

	/// <summary>
	/// deletes a student from memory
	/// </summary>
	/// <param name="allData">data pointer</param>
	/// <param name="studentNumber">student number to delete</param>
	static void DeleteStudent(file* allData, int studentNumber) {
		bool confirm = true; // start loop confirm they want to delete the student
		while (confirm) {
			cout << "Confirm delete student -> " << studentNumber << endl;
			string rawInput = GetRawInput("y or n : "); // get user input for delete
			if (rawInput == "n") {
				cout << "\nStudent was NOT deleted...\n\n";
				return; // return and stop code if change mind
			}
			else if(rawInput == "y") {
				confirm = false;
			}
			else {
				Error("INVALID INPUT");
			}
		}

		// Delete student here
		for (int i = 0; (unsigned)i < (unsigned)allData->studentData->size(); i++) {
			vector<string> v = (*allData->studentData)[i];
			if (v[0] == std::to_string(studentNumber)) {
				v.clear();
				allData->studentData->erase(allData->studentData->begin() + i);
				allData->SaveAll();
				cout << "\nStudent Deleted...\n\n";

				// TODO delete their login

				return; // return once a student has successfully been deleted
			}
		}

		Error("Student not found..."); // error if no student was deleted
	}

	/// <summary>
	/// edits a student
	/// </summary>
	static void Edit(file* allData) {
		// displaying all students based on user input
		bool displayAllInput = true; // loop boolean
		while (displayAllInput) {
			string input = GetRawInput("Would you like to see a list of all students?\ny or n : "); // raw input
			cout << endl;
			if (input == "y" || input == "Y") {
				DisplayAllStudents(allData); // show all students if requested
				displayAllInput = false; // stop loop
			}
			else if (input == "n" || input == "N") {
				displayAllInput = false; // stop loop
			}
			else {
				Error("INVALID INPUT"); // inform user
			}
		}
		
		// getting valid student
		bool getStudent = true; // loop boolean 
		int studentTarget; // student number
		while (getStudent) {
			string input = GetRawInput("Enter Student Number or \'e\' to cancel: "); // raw input
			cout << endl;
			try {
				if (input == "e") { // exit input
					//cout << "\nStudent not edited\n\n"; // inform user
					return; // exit function
				}

				studentTarget = stoi(input); // try convert string to int
				getStudent = true; // assume student doesnt exist -> keep looping

				for (vector<string> v : *allData->studentData) {
					if (stoi(v[0]) == studentTarget) {
						getStudent = false; // stop loop if exist
					}
				}
				if (getStudent) { // if still doesnt exits
					Error("Student not found..."); // inform user
				}
			}
			catch (...) {
				Error("INVALID INPUT");
			}
		}

		int option = 0;
		bool editting = true;
		while (editting) {
			cout << "Options:\n";
			cout << "0. Back\n";
			cout << "1. Edit reports\n";
			cout << "2. Edit name\n";
			cout << "3. Edit Login\n";
			string edditingOptionString = GetRawInput("\nSelection : ");
			cout << endl;

			try {
				option = stoi(edditingOptionString);
			}
			catch (...) {
				Error("INVALID INPUT");
				continue;
			}

			switch (option) {
			case 0: return;
			case 1: {
				student* s = new student(allData, studentTarget);
				s->EditReports();
				allData->SaveAll();
				delete s;
				s = nullptr;
				break;
			}
			case 2: {
				string newName = GetRawInput("Enter new name for student : ");
				cout << endl;
				vector<vector<string>>* pointerToData = allData->studentData;
				for (int i = 0; (unsigned)i < (unsigned)pointerToData->size(); i++) {
					if ((*allData->studentData)[i][0] == std::to_string(studentTarget)) {
						(*allData->studentData)[i][1] = newName;
					}
				}
				cout << "Name SUCCESSFULLY Saved...\n\n";
				allData->SaveAll();
				break;
			}
			case 3: {
				EditLogin(allData, studentTarget, "student");
				allData->SaveAll();
				break;
			}
			default: {
				Error("INVALID INPUT");
				break;
			}
			}
		}
		allData->SaveAll();
	}

	/// <summary>
	/// displays all the students in the system
	/// </summary>
	/// <param name="allData">all the data in memory</param>
	static void DisplayAllStudents(file* allData) {
		vector<vector<string>>* studentData = allData->studentData; // get student data from memory
		Title("All Students"); // show title
		for (vector<string> v : *studentData) { // foreach student in memory
			cout << "Student " << v[0] << " - " << v[1] << endl; // display student number and name
		}
		cout << endl;
	}

	/// <summary>
	/// ctor of student LOGIN
	/// </summary>
	/// <param name="allData">data in memory</param>
	/// <param name="studentNumber">student that logged in</param>
	student(file* allData, int studentNumber) {
		this->loggedInStudentAllData = allData;
		this->reports = reportGetAll(studentNumber, allData);
		this->studentNumber = studentNumber;
		this->name = "NO NAME GIVEN...";
		for (vector<string> v : *allData->studentData) {
			if (v[0] == std::to_string(studentNumber)) {
				this->name = v[1];
			}
		}
	}

	/// <summary>
	/// displays a list of options a student can choose from once logged in
	/// </summary>
	void Options() {
		int choice;

		bool selectionNum = false; // assume number is invalid
		while (selectionNum == false) { // loop while number is invalid
			cout << "0. Back\n";
			cout << "1. View Reports\n";
			cout << "2. View Notices\n";
			string s = GetRawInput("\nSelection : ");
			cout << endl;
			try {
				choice = stoi(s); // try convert to number
				selectionNum = true; // if converted number is valid
			}
			catch (...) {
				Error("INVALID INPUT"); // catch a conversion error
			}
		}

		switch (choice)
		{
		case 0: return;
		case 1:
			reportOption();
			break;

		case 2:
			displayNotices(loggedInStudentAllData);
			break;
		}

	}
	// ------------------------ ^ NOT TESTED ^-----------------------------
};

/// <summary>
/// classroom which holds students
/// </summary>
class classroom {
	vector<student>* students; // all student in the classroom
	int number; // classroom number
	file* allData; // all data stored in memory -> pointer

public: // makes members accessible outside the class{}
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
	/// gets the roll of students in that class
	/// </summary>
	/// <returns></returns>
	vector<student>* getRoll() {
		return students; // return students list
	}

	/// <summary>
	/// edits the roll if the logged in user has the correct credentials
	/// </summary>
	/// <param name="credentials">the login details</param>
	void EditRoll() {
		// TODO implement the edit roll feature
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

	/// <summary>
	/// <para>gets a student with student number</para>
	/// <para>WARNING - call studentExists before to prevent crash</para> 
	/// </summary>
	/// <param name="studentNumber">student to get</param>
	/// <returns>a student</returns>
	student getStudent(int studentNumber) {
		for (student _student : *students) {
			if (_student.studentNumber == studentNumber) {
				return _student; // student found
			}
		}

		exit(1); // exit program if no student found
	}

	/// <summary>
	/// exit called for destructors
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

	/// <summary>
	/// adds a student to the class
	/// </summary>
	/// <param name="studentNumber">student to add</param>
	void AddStudent(int studentNumber) {
		bool studentFound = false; // assume student isn't found
		for (int i = 0; (unsigned)i < (unsigned)allData->studentData->size(); i++) { // loop through data
			if (stoi((*allData->studentData)[i][0]) == studentNumber) { // find student
				// add student to classroom
				studentFound = true;
				student s = student((*allData->studentData)[i][1], stoi((*allData->studentData)[i][0]), allData, false);
				students->push_back(s);
			}
		}

		if (!studentFound) { // if no matching student found
			// create a student with null attributes to the class for admin to edit later
			string name = "NULL";
			string number = std::to_string(studentNumber);
			vector<string> temp;
			temp.push_back(number);
			temp.push_back(name);

			// store temp student in data and file
			(*allData->studentData).push_back(temp);
			allData->SaveAll();

			AddStudent(studentNumber); // add the new temp student
			Error("Missing student data, creating NULL student in place..."); // display to admin
		}
	}

	/// <summary>
	/// ctor for a classroom
	/// </summary>
	/// <param name="classroomNumber">classroom number</param>
	/// <param name="alldata">data in memory</param>
	classroom(int classroomNumber, file* alldata)
	{
		this->allData = alldata;
		number = classroomNumber;
		students = new vector<student>();

		// TODO : loop through data in memory and create students in this classroom and add to roll
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

	/// <summary>
	/// creates a new classroom <- admin only
	/// </summary>
	/// <param name="allData">data in memory</param>
	static void CreateNew(file* allData) {
		int classroomNumber = 0; // allocate memory
		bool classNumCorrect = false; // assume classroom number is incorrect
		while (classNumCorrect == false) { // loop whiel classroom number is incorrect
			string s = GetRawInput("Enter the new classroom number (8 digits): "); // get raw input
			try {
				classroomNumber = stoi(s); // try convert input into number
				classNumCorrect = true; // if successful stop the loop -> classroom number is correct
			}
			catch (...) {
				Error("INVALID INPUT"); // catch conversion exception and infrom of error
			}
		}

		// loop through all classrooms already in memory
		for (vector<string> v : *allData->classroomData) {
			if (v[0] == std::to_string(classroomNumber)) { // check if classroom already exists
				Error("Classroom already exists...");  // inform if duplicate found
				return; // stop function if found duplicate
			}
		}

		// create new classroom instance
		classroom c = classroom(classroomNumber, allData);
		vector<string> toData;
		toData.push_back(std::to_string(classroomNumber)); // add to temp data
		
		// output instructions
		cout << "Instuctions : \n";
		cout << "Enter a student number to add a student to the classroom or \n";
		cout << "enter 'e' to stop entering students into the classroom.\n\n";
		
		// start looping until end = true
		bool end = false;
		while (end == false) {
			string s = GetRawInput("Enter student number : "); // get raw input
			int sNum = 0; // allocate memory to a number
			if (s[0] == 'e' || s[0] == 'E') { // check if char at index 0 is 'e'
				end = true; // stop looping if 'e' is entered
				break; // break the loop
			}
			else { // if char is not 'e'
				try {
					sNum = stoi(s); // try convert to a number
					c.AddStudent(sNum); // add student to classroom
					toData.push_back(std::to_string(sNum)); // add student number to temp data
				}
				catch (...) {
					Error("INVALID INPUT"); // catch conversion error and infrom user
				}
			}
		}
		cout << endl;
		allData->classroomData->push_back(toData); // store temp data and 
		allData->SaveAll(); // save all data to file
	}

	/// <summary>
	/// deletes a classroom from memory and file
	/// </summary>
	/// <param name="allData">current data in memory</param>
	/// <param name="classroomNumber">classroom to delete</param>
	static void DeleteClassroom(file* allData, int classroomNumber) {
		bool confirm = true; // start loop to confirm delete
		while (confirm) { // loop while confirming
			cout << "Confirm delete classroom -> " << classroomNumber << endl; // promtp user
			string rawInput = GetRawInput("y or n : "); // get raw input
			if (rawInput == "n" || rawInput == "N") { // if n or N
				cout << "\nClassroom was NOT deleted...\n\n"; // output not deleted
				return; // return to stop function
			}
			else if (rawInput == "y" || rawInput == "Y") { // if y or Y
				confirm = false; // stop confirm loop
			}
			else { // if neither n or y
				Error("INVALID INPUT"); // infrom user of invalid input
			}
		}

		// deletes classroom here
		for (int i = 0; (unsigned)i < (unsigned)allData->classroomData->size(); i++) {
			vector<vector<string>> ve = *allData->classroomData;
			vector<string> v = ve[i];
			if (v[0] == std::to_string(classroomNumber)) {// find match of classroom number with one in memory
				v.clear();
				allData->classroomData->erase(allData->classroomData->begin() + i);
				allData->SaveAll();
				cout << "\nClassroom Deleted...\n\n";
				return; // return once classroom was deleted
			}
		}

		Error("Classroom not found..."); // error if no user was deleted
	}

	/// <summary>
	/// edits a classroom
	/// </summary>
	static void Edit(file* allData) {
		// displaying all classrooms based on user input
		bool displayAllInput = true; // loop boolean
		while (displayAllInput) {
			string input = GetRawInput("Would you like to see a list of all classrooms?\ny or n : "); // raw input
			cout << endl;
			if (input == "y" || input == "Y") {
				DisplayList(allData); // show all students if requested
				displayAllInput = false; // stop loop
			}
			else if (input == "n" || input == "N") {
				displayAllInput = false; // stop loop
			}
			else {
				Error("INVALID INPUT"); // inform user
			}
		}

		// get classroom then edit
		string number = GetRawInput("Enter classroom number : ");
		cout << endl;

		for (int i = 0; (unsigned)i < (unsigned)allData->classroomData->size(); i++) {
			vector<vector<string>> ve = *allData->classroomData;
			vector<string> v = ve[i];
			if (v[0] == number) {// find match of classroom number with one in memory
				v.clear();
				allData->classroomData->erase(allData->classroomData->begin() + i);
				CreateNew(allData);
				cout << "Classroom Editted...\n\n";
				return; // return once classroom was deleted
			}
		}

		Error("Classroom not found..."); // error if no user was deleted

	}

	static void DisplayList(file* allData) {
		Title("All Classrooms");
		for (vector<string> v : *allData->classroomData) {
			cout << "Classroom " << v[0];
			int studentCount = 0;
			for (int i = 1; (unsigned)i < (unsigned)v.size(); i++) {
				studentCount++;
			}
			cout << " - " << studentCount << " students" << endl;
		}
		cout << endl;
	}
};

/// <summary>
/// school hold all the information and related functions
/// </summary>
class school {
	file* storedData; // pointer to stored data in memory
	vector<classroom>* classrooms; // pointer to all classrooms in memory
	vector<string>* notices; // pointer to list of notices
	string schoolName; // the schools name
	string contactDetails; // school contact details
	vector<string>* datesEvents; // pointer to list of dates and events

public:
	/// <summary>
	/// welcome screen
	/// </summary>
	void Welcome() {
		cout << "****************************************\n\n";
		cout << "Welcome to " << schoolName << endl;
		cout << "Contact details:\n" << contactDetails << endl;
		if (contactDetails == "") {
			//cout << endl;
		}
		cout << "****************************************\n\n";

		cout << "Select a number from options below to navigate this program\n\n";


	}

	/// <summary>
	/// sets up the school
	/// </summary>
	void NewSchoolScreen(file* allFiles) {
		cout << "Please create an ADMIN account to continue...\n"; // prompt first time setup
		allFiles->loginData->clear(); // clears any logins already existing -> school isnt setup so admin account is reset and all currect logins deleted
		allFiles->classroomData->clear();
		allFiles->parentData->clear();
		allFiles->studentData->clear();
		allFiles->staffData->clear();
		SignUp(allFiles, "admin"); // sign up a new admin for first time setup
		allFiles->LoadAll();
		schoolName = "(EDIT SCHOOL NAME)";       // temp insert for first time setup
		contactDetails = "(EDIT CONTACT INFO)";  // temp insert for first time setup

		cout << "Login as an admin to edit the school...\n";
		Welcome(); // start welcome like normal after setup
	}

	/// <summary>
	/// loads school classrooms from files
	/// </summary>
	/// <returns>the amount of classrooms</returns>
	int PopulateClassrooms() {
		classrooms->clear();

		//storedData
		int count = 0;
		for (int i = 0; (unsigned)i < (unsigned)storedData->classroomData->size(); i++) {
			vector<string> v = (*storedData->classroomData)[i];
			classroom* cr = new classroom(stoi(v[0]), storedData);
			for (int j = 1; (unsigned)j < (unsigned)v.size(); j++) {
				cr->AddStudent(stoi(v[j]));
			}
			classrooms->push_back(*cr); // make a copy of new classroom in list
			count++; // count classrooms in memory

			delete cr; // reset memory for use later
			cr = nullptr; // reset pointer
		}

		return count; // 0 default
	}

	/// <summary>
	/// displays the menu screen for user to see options
	/// </summary>
	/// <param name="loggedIn"></param>
	void MenuScreen(bool loggedIn) {
		cout << "Options:\n";
		if (loggedIn) { // logged in options
			cout << "1. Logout\n";
			cout << "2. Exit\n";
			cout << "3. View more options\n";
		}
		else { // logged out options
			cout << "1. Login\n";
			cout << "2. Exit\n";
			cout << "3. Parent Sign Up\n";
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

	/// <summary>
	/// ctor for school, holds data
	/// </summary>
	/// <param name="data">the data object</param>
	school(file* data)
	{
		// allocate memory for school
		this->storedData = data;
		classrooms = new vector<classroom>();
		notices = new vector<string>();
		datesEvents = new vector<string>();

		if (data->schoolData->size() >= 1 && (PopulateClassrooms() > 0)) {
			schoolName = (*data->schoolData)[0][0];
		}
		else {
			if (data->schoolData->size() == 0) {
				vector<string> temp;
				temp.push_back("(SETUP SCHOOL NAME)");
				(*data->schoolData).push_back(temp);
			}
			else {
				(*data->schoolData)[0][0] = "(SETUP SCHOOL NAME)";
			}
			data->SaveAll();
			schoolName = (*data->schoolData)[0][0];
		}

		if (data->schoolData->size() >= 1) {
			for (int i = 1; (unsigned)i < (unsigned)(*data->schoolData)[0].size(); i++) {
				string s = (data->schoolData->at(0)).at(i);
				contactDetails.append(s);
				contactDetails.append("\n");
			}
		}
	}
};

/// <summary>
/// parent login
/// </summary>
class parent {
	file* allData; // pointer to all stored data in memory
	int number; // parent number

public: // makes members accessible outside class{}
	/// <summary>
	/// ctor for parent logged in
	/// </summary>
	/// <param name="allData">data in memory</param>
	/// <param name="number">parent number</param>
	parent(file* allData, int number)
	{
		this->allData = allData; // store pointer for later reference
		this->number = number; // store number for use later
	}

	/// <summary>
	/// displays options for logged in parent
	/// </summary>
	void Options() {
		// TODO : show options
		int choice;


		bool selectionNum = false; // assume number is invalid
		while (selectionNum == false) { // loop while number is invalid
			cout << "0. Back\n";
			cout << "1. View Reports\n";
			cout << "2. View Notices\n";
			string s = GetRawInput("\nSelection : ");
			cout << endl;
			try {
				choice = stoi(s); // try convert to number
				selectionNum = true; // if converted number is valid
			}
			catch (...) {
				Error("INVALID INPUT"); // catch a conversion error
			}
		}

		switch (choice)
		{
		case 0: return;
		case 1: {
			//cout << "VIEW REPORTS\n";
			int num = 0;
			bool found = false;
			for (vector<string> v : *allData->parentData) {
				if (stoi(v[0]) == number) {
					num = stoi(v[1]);
					found = true;
				}
			}
			if (!found) {
				Error("Student not found, please check with an admin your data is correct...");
				return;
			}
			student* s = new student(allData, num);
			s->reportOption();

			delete s;
			s = nullptr;
			break;
		}

		case 2:
			displayNotices(allData);
			break;

		}
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------


	/// <summary>
	/// exit code called for destructors
	/// </summary>
	void Exit() {

	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

	/// <summary>
	/// creates a new parent in memory
	/// </summary>
	/// <param name="allData">data in memory</param>
	static int CreateNew(file* allData) {
		while (true) {
			bool startAgain = false;
			string rawParentNumber;
			int parentNumber;
			bool numberConfirm = true;
			while (numberConfirm) {
				try {
					rawParentNumber = GetRawInput("Enter a new parent number (8 digits): ");
					parentNumber = stoi(rawParentNumber);
					numberConfirm = false;
					for (vector<string> v : (*allData->loginData)) { // foreach student in memory / data
						if (v[3] == std::to_string(parentNumber)) { // if match
							numberConfirm = true; // restart loop
						}
					}
					if (numberConfirm) {
						Error("Parent number already exists, please pick a different one");
					}
				}
				catch (...) {
					Error("INVALID INPUT");
				}
			}

			int studentNumber; // reference to student number
			bool childInput = true;
			while (childInput) { // loop child number input
				string studentRawInput = GetRawInput("Is your child already enrolled with us?\ny or n : ");
				if (studentRawInput == "n" || studentRawInput == "N") {
					studentNumber = student::CreateNew(allData, parentNumber); // parent signup makes a new student for parent
					SignUp(allData, "student", studentNumber);
					childInput = false;
				}
				else if (studentRawInput == "y" || studentRawInput == "Y") {
					try {
						studentNumber = stoi(GetRawInput("Enter your child's student number : ")); // get student number from raw input
						childInput = false;
					}
					catch (...) {
						Error("INVALID INPUT"); // catch any conversion errors that may occur
					}
				}

				if (childInput == false) { // confirm student is in system
					childInput = true;
					for (vector<string> v : (*allData->studentData)) { // foreach student in memory / data
						if (v[0] == std::to_string(studentNumber)) { // if match
							childInput = false; // end loop
						}
					}
				}

				if (childInput) { // if loop again
					Error("Error getting student...");
				}
			}
			if (startAgain) {
				continue;
			}


			// TODO Get more info and store in parent file...
			//-----------------------------------------------
			vector<string> parentInfoData;

			parentInfoData.push_back(std::to_string(parentNumber));
			parentInfoData.push_back(std::to_string(studentNumber));
			parentInfoData.push_back(GetRawInput("Enter adult's name : "));
			parentInfoData.push_back(GetRawInput("Enter emergency contact phone number : "));
			parentInfoData.push_back(GetRawInput("Enter address : "));
			parentInfoData.push_back(GetRawInput("Enter DOB : "));
			parentInfoData.push_back(GetRawInput("Enter email : "));

			allData->parentData->push_back(parentInfoData);

			cout << "\nParent Created...\n\n"; // inform user
			allData->SaveAll();
			return parentNumber; // return parent unique number
		}

	}

	/// <summary>
	/// deletes a parent out of the system
	/// </summary>
	/// <param name="allData">all data reference</param>
	/// <param name="username">username to delete</param>
	static bool DeleteParent(file* allData, int number) {
		bool confirm = true; // confirmation loop boolean
		while (confirm) { // loop while confirming
			cout << "Confirm delete parent -> " << number << endl; // prompt
			string rawInput = GetRawInput("y or n : "); // raw input
			if (rawInput == "n" || rawInput == "N") { // if n or N
				cout << "\nParent was NOT deleted...\n\n"; // inform user
				return false; // return void to stop function
			}
			else if (rawInput == "y" || rawInput == "Y") { // if y or Y
				confirm = false; // stop loop
			}
			else {
				Error("INVALID INPUT"); // inform user of invalid input
			}
		}

		// Delete parent login here
		for (int i = 0; (unsigned)i < (unsigned)allData->loginData->size(); i++) { // foreach parent in data
			vector<string> v = (*allData->loginData)[i];
			if (v[3] == std::to_string(number)) { // if match
				int index = i;
				allData->loginData->erase(allData->loginData->begin() + index);
				index = 0;
				for (vector<string> vP : (*allData->parentData)) {
					if (vP[0] == std::to_string(number)) {
						vP.clear();
						allData->parentData->erase(allData->parentData->begin() + index);
						allData->SaveAll();
						cout << "\nParent Deleted...\n\n";
						return true; // stop function from running
					}
					index++;
				}
				
				// TODO delete parent data here
				//---------------------------------------------------------------------------------------------

			}
		}

		int index = 0;
		for (vector<string> vP : (*allData->parentData)) {
			if (vP[0] == std::to_string(number)) {
				vP.clear();
				allData->parentData->erase(allData->parentData->begin() + index);
				allData->SaveAll();
				cout << "\nParent Deleted...\n\n";
				return true; // stop function from running
			}
			index++;
		}


		Error("Parent not found..."); // error if no parent found to delete
		return false;
	}

	/// <summary>
	/// edits a parent
	/// </summary>
	static void Edit(file* allData) {
		// displaying all parents based on user input
		bool displayAllInput = true; // loop boolean
		while (displayAllInput) {
			string input = GetRawInput("Would you like to see a list of all parents?\ny or n : "); // raw input
			cout << endl;
			if (input == "y" || input == "Y") {
				DisplayList(allData); // show all students if requested
				displayAllInput = false; // stop loop
			}
			else if (input == "n" || input == "N") {
				displayAllInput = false; // stop loop
			}
			else {
				Error("INVALID INPUT"); // inform user
			}
		}

		string number = GetRawInput("Enter Parent Number or 'e' to cancel : ");
		cout << endl;

		if (number == "e") {
			return;
		}

		// get parent then edit
		for (int i = 0; (unsigned)i < (unsigned)allData->loginData->size(); i++) { // foreach parent in data
			vector<string> v = (*allData->loginData)[i];
			if (v[3] == number) { // if match
				int index = 0;
				for (vector<string> vP : (*allData->parentData)) {
					if (vP[0] == number) {
						vP.clear();
						allData->parentData->erase(allData->parentData->begin() + index);
						CreateNew(allData);
						return; // stop function from running
					}
					index++;
				}

				// TODO delete parent data here
				//---------------------------------------------------------------------------------------------

			}
		}

		Error("Parent NOT Found...");
	}

	static void DisplayList(file* allData) {
		Title("All Parents");
		for (vector<string> v : *allData->parentData) {
			cout << "Parent " << v[0] << " of student " << v[1] << endl;
		}
		cout << endl;
	}
};

/// <summary>
/// teacher login
/// </summary>
class teacher {
	file* allData; // pointer to data in memory

	void Attendance() {
		Error("Not Implemented Yet...");
	}



public: // makes members accessible outside class{}
	/// <summary>
	/// ctor for teacher login
	/// </summary>
	/// <param name="allData">data in memory</param>
	/// <param name="number">teacher that logged in</param>
	teacher(file* allData, int number)
	{
		this->allData = allData;
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

	/// <summary>
	/// exit code called for destructors
	/// </summary>
	void Exit() {
		// TODO make sure memory safe exit
	}

	/// <summary>
	/// shows options teacher logged in has
	/// </summary>
	void Options() {
		bool teacherOptions = true;
		while (teacherOptions) {
			Title("Teacher Options Menu");
			cout << "Options : \n";
			cout << "0. Back to main menu\n";
			cout << "1. Notices\n";
			cout << "2. Attendance\n";
			cout << "3. Reports\n";
			cout << "4. Person Details\n";

			string rawIn = GetRawInput("\nSelection : ");
			cout << endl;

			// error prevention with menus...
			try {
				int selection = stoi(rawIn); // try convert string to int
				bool inSubMenu = true; // bool for sub menu loop -> true if selection is valid
				switch (selection) { // switch int selection based on input
				case 0: { // main menu
					bool inSubMenu = false; // stop sub loop
					teacherOptions = false; // stop main loop -> takes you back out of function
					break; // break while loop instantly
				}
				case 1: { // show notices options
					notices(allData);
					break;
				}
				case 2: { // Show attendance options
					Attendance();
					break;
				}
				case 3: { // Show reports options
					while (inSubMenu) {
						student::DisplayAllStudents(allData);
						string studentNumInStr = GetRawInput("Enter Student Number : ");
						cout << endl;
						try {
							bool found = false;
							int studentNumber = stoi(studentNumInStr);
							for (vector<string> v : *allData->studentData) {
								if (v[0] == studentNumInStr) {
									found = true;
								}
							}
							if (found) {
								student* s = new student(allData, studentNumber);
								s->EditReports();
								allData->SaveAll();
								delete s;
								s = nullptr;
							}
							else {
								Error("No student found - " + studentNumInStr);
							}
							inSubMenu = false;
						}
						catch (...) {
							Error("INVALID INPUT");
						}
					}
					// get student number
					// make sure valid student
					// call student function EditReports
					break;
				}
				case 4: { // Show details options
					// student
					// parent
					// teacher
					// classroom

					while (inSubMenu) {
						cout << "0. Back to Teacher options\n";
						cout << "1. View Student Details\n";
						cout << "2. View Parent Details\n";
						cout << "3. View Teacher Details\n";
						cout << "4. View Classroom Details\n";

						string detailsInput = GetRawInput("\nSelection : ");
						cout << endl;
						try {
							int detailsSelection = stoi(detailsInput);
							string type;
							string numberTarget;
							switch (detailsSelection)
							{
							case 0: {
								inSubMenu = false;
								continue; // skip processing after switch
								break;
							}
							case 1: {
								student::DisplayAllStudents(allData);
								type = "student";
								break;
							}
							case 2: {
								parent::DisplayList(allData);
								type = "parent";
								break;
							}
							case 3: {
								teacher::DisplayList(allData);
								type = "teacher";
								break;
							}
							case 4: {
								classroom::DisplayList(allData);
								type = "classroom";
								break;
							}
							default: {
								// crash the program -> cause error input
								string crash = "Im gonna crash the try (;"; // string not int
								int crash_now = stoi(crash); // convert not int to int -> crash -> catch code to run
							}
							}

							// more processing
							numberTarget = GetRawInput("Enter " + type + " number : ");
							cout << endl;
							ViewDetails(allData, numberTarget, type);
							cout << endl;

							inSubMenu = false;
						}
						catch (...) {
							Error("INVALID INPUT");
						}
					}

					break;
				}
				default: { // crash the program -> cause error input
					string crash = "Im gonna crash the try (;"; // string not int
					int crash_now = stoi(crash); // convert not int to int -> crash -> catch code to run
				}
				}
				allData->SaveAll();

			}
			catch (...) { // catch any conversion errors in any menu or sub menu and loop from the teacher menu
				Error("INVALID INPUT"); // inform invalid input
			}
		}
	}
	// ------------------------ ^ NOT TESTED ^-----------------------------

	static void EditNotices(file* allData) {
		Title("Current Notices");
		if (allData->schoolData->size() >= 2) {
			for (int i = 0; (unsigned)i < (unsigned)((*allData->schoolData)[1]).size(); i++) {
				cout << i << ". " << (*allData->schoolData)[1][i] << endl;
				cout << endl;
			}
		}

		cout << "Enter notice number to replace or 'e' to go back\n";

		string input = GetRawInput("Notice # : ");
		cout << endl;

		if (input == "e") {
			return;
		}
		if (allData->schoolData->size() >= 2) {

			try {
				int noticeNum = stoi(input);
				if ((unsigned)((*allData->schoolData)[1]).size() == 0) {
					Error("Notice not found...");
				}
				else if (noticeNum >= 0 && (unsigned)noticeNum < (unsigned)((*allData->schoolData)[1]).size()) {
					vector<string> rawNotices = (*allData->schoolData)[1];
					for (int i = 0; (unsigned)i < (unsigned)rawNotices.size(); i++) {
						if (i == noticeNum) {
							rawNotices.erase(rawNotices.begin() + i);
							while ((*allData->schoolData).size() > 1) {
								(*allData->schoolData).pop_back();
							}
							(*allData->schoolData).push_back(rawNotices);
							AddNotices(allData);
							allData->SaveAll();
							cout << "Notice replaced...\n\n";
							return;
						}
					}
					Error("Notice not found...");
				}
				else {
					Error("Notice not found...");
				}
			}
			catch (...) {
				Error("INVALID INPUT");
			}
		}
		else {
			Error("Notice not found...");
		}
	}

	static void DeleteNotices(file* allData) {
		Title("Current Notices");
		if (allData->schoolData->size() >= 2) {
			for (int i = 0; (unsigned)i < (unsigned)((*allData->schoolData)[1]).size(); i++) {
				cout << i << ". " << (*allData->schoolData)[1][i] << endl;
				cout << endl;
			}
		}

		cout << "Enter notice number to delete or 'e' to go back\n";

		string input = GetRawInput("Notice # : ");
		cout << endl;
		if (input == "e") {
			return;
		}
		if (allData->schoolData->size() >= 2) {

			try {
				int noticeNum = stoi(input);
				if ((unsigned)((*allData->schoolData)[1]).size() == 0) {
					Error("Notice not found...");
				}
				else if (noticeNum >= 0 && (unsigned)noticeNum < (unsigned)((*allData->schoolData)[1]).size()) {
					vector<string> rawNotices = (*allData->schoolData)[1];
					for (int i = 0; (unsigned)i < (unsigned)rawNotices.size(); i++) {
						if (i == noticeNum) {
							rawNotices.erase(rawNotices.begin() + i);
							while ((*allData->schoolData).size() > 1) {
								(*allData->schoolData).pop_back();
							}
							(*allData->schoolData).push_back(rawNotices);
							allData->SaveAll();
							cout << "Notice Deleted...\n\n";
							return;
						}
					}
					Error("Notice not found...");
				}
				else {
					Error("Notice not found...");
				}
			}
			catch (...) {
				Error("INVALID INPUT");
			}
		}
		else {
			Error("Notice not found...");
		}
	}

	static void AddNotices(file* allData) {
		string notice = "";
		string input = "";
		int i = 1;
		while (input != "e") {
			input = GetRawInput("Enter notice line " + std::to_string(i) + " or 'e' to end notice : ");
			if (input != "e") {
				if (i != 1) {
					notice += "\n";
				}
				notice += (input);
				i++;
			}
			else {
				cout << endl;
			}
		}
		if (notice != "") {
			if ((unsigned)allData->schoolData->size() >= (unsigned)2) {
				(*allData->schoolData)[1].push_back(notice);
			}
			else if((unsigned)allData->schoolData->size() == 1){
				vector<string> v;
				allData->schoolData->push_back(v);
				(*allData->schoolData)[1].push_back(notice);
			}
			else  if((unsigned)allData->schoolData->size() == 0){
				vector<string> v;
				allData->schoolData->push_back(v);
				allData->schoolData->push_back(v);
				(*allData->schoolData)[1].push_back(notice);
			}
			else {
				Error("Data Error...");
				cout << "Notice NOT Added...\n\n";
			}
		}
		else {
			cout << "Notice NOT Added...\n\n";
		}
		allData->SaveAll();
	}

	static void notices(file* allData) {
		bool inNotices = true;
		while (inNotices) {
			Title("Notices - Menu");
			cout << "Options : \n";
			cout << "0. Back\n";
			cout << "1. View\n";
			cout << "2. Add\n";
			cout << "3. Edit\n";
			cout << "4. Delete\n";
		
			string input = GetRawInput("\nSelection : ");
			cout << endl;
			try {
				int selection = stoi(input);
				switch (selection)
				{
				case 0: {
					inNotices = false;
					return;
					break;
				}
				case 1: {
					displayNotices(allData);
					break;
				}
				case 2: {
					AddNotices(allData);
					break;
				}
				case 3: {
					EditNotices(allData);
					break;
				}
				case 4: {
					DeleteNotices(allData);
					break;
				}
				default:
					Error("INVALID INPUT");
					break;
				}
			}
			catch (...) {
				Error("INVALID INPUT");
			}
		}
	}

	static void DisplayList(file* allData) {
		Title("All Teachers");
		for (vector<string> v : *allData->staffData) {
			cout << "Teacher " << v[0] << " - " << v[1] << endl;
		}
		cout << endl;
	}

	/// <summary>
	/// creates a new teacher
	/// </summary>
	/// <param name="allData">program file management data</param>
	static int CreateNew(file* allData) {
		Title("Create Teacher"); // title of creation
		
		// variables needed

		int teacherNumber;
		string name;
		int classroom;
		gender _gender = gender::other; // allocate memory
		string DOB;
		string email;
		string contactNumber;
		int yearTeach;

		bool moreInfoNeeded; // main looping bool (reused)
		
		// teacher number input
		moreInfoNeeded = true; // loop
		// number
		while (moreInfoNeeded) { 
			string in = GetRawInput("Enter new teacher number (8 digits): "); // raw input
			try {
				int i = stoi(in); // try convert to int
				teacherNumber = i; // store in variable
				moreInfoNeeded = false; // stop loop

				// check if teacher number already exist
				for (vector<string> v : *allData->staffData) {
					if (v[0] == std::to_string(teacherNumber)) {
						Error("Teacher number already exists..."); // display if duplicate
						moreInfoNeeded = true; // start loop again
					}
				}
			}
			catch (...) {
				Error("INVALID INPUT"); // catch conversion error
			}
		}

		// teacher name input
		moreInfoNeeded = true; // loop
		// name
		while (moreInfoNeeded) {
			string in = GetRawInput("Enter teacher name : "); // raw input
			name = in; // store in variable
			moreInfoNeeded = false; // stop loop
		}

		moreInfoNeeded = true; // loop
		// classroom
		while (moreInfoNeeded) {
			// TODO print all classroom numbers??
			classroom::DisplayList(allData);
			string in = GetRawInput("Enter teacher classroom number : "); // raw input
			try {
				int i = stoi(in); // try convert to int
				classroom = i; // store int in variable
				moreInfoNeeded = false; // stop loop
			}
			catch (...) {
				Error("Not a valid classroom number..."); // catch conversion error
			}
		}

		moreInfoNeeded = true; // loop
		// gender
		while (moreInfoNeeded) {
			string in = GetRawInput("Enter gender (1. male, 2. female, 3. other) : "); // raw input
			moreInfoNeeded = false; // stop loop

			// assign gender based on input
			if (in == "1") {
				_gender = gender::male;
			}
			else if (in == "2") {
				_gender = gender::female;
			}
			else if (in == "3") {
				_gender = gender::other;
			}
			else {
				Error("INVALID INPUT"); // error if valid
				moreInfoNeeded = true; // loop again
			}
			
		}

		moreInfoNeeded = true; // loop
		// DOB
		while (moreInfoNeeded) {
			string in = GetRawInput("Enter teacher DOB \"DD/MM/YYYY\" : "); // raw input
			DOB = in; // store input in variable
			moreInfoNeeded = false; // stop loop
		}

		moreInfoNeeded = true; // loop
		// email
		while (moreInfoNeeded) {
			string in = GetRawInput("Enter teacher email : "); // raw input
			email = in; // store in variable
			moreInfoNeeded = false; // stop loop
		}

		moreInfoNeeded = true; // loop
		// contact number
		while (moreInfoNeeded) {
			string in = GetRawInput("Enter teacher contact number : "); // raw input
			contactNumber = in; // store in variable
			moreInfoNeeded = false; // stop loop
		}

		moreInfoNeeded = true; // loop
		// year teach 
		while (moreInfoNeeded) {
			// TODO print all classroom numbers??
			string in = GetRawInput("Enter teacher year level teaching : "); // raw input
			try {
				int i = stoi(in); // try convert to int
				yearTeach = i; // store int
				moreInfoNeeded = false; // stop loop
			}
			catch (...) {
				Error("INVALID INPUT"); // error if conversion fails
			}
		}

		// store all data in vector
		vector<string> newTeacherData;
		newTeacherData.push_back(std::to_string(teacherNumber));
		newTeacherData.push_back(name);
		newTeacherData.push_back(std::to_string(classroom));
		newTeacherData.push_back(gtos(_gender));
		newTeacherData.push_back(email);
		newTeacherData.push_back(contactNumber);
		newTeacherData.push_back(std::to_string(yearTeach));

		// add new data to memory and file
		allData->staffData->push_back(newTeacherData);
		allData->SaveAll();

		// output success
		cout << "\nTeacher Created\n\n";
		return teacherNumber;
	}

	/// <summary>
	/// Deletes a teacher
	/// </summary>
	static void DeleteTeacher(file* allData, int teacherNumber) {
		bool confirm = true; // loop for confirmation
		while (confirm) { // loop
			cout << "Confirm delete teacher -> " << teacherNumber << endl; // confirm prompt
			string rawInput = GetRawInput("y or n : "); // input
			if (rawInput == "n" || rawInput == "N") { // user enters n or N
				cout << "\nTeacher was NOT deleted...\n\n"; // cancel deletion
				return; // return stops the function
			}
			else if (rawInput == "y" || rawInput == "Y") { // user enters y or Y
				confirm = false; // stop the loop and start delete
			}
			else {
				Error("INVALID INPUT"); // error if invalid input
			}
		}

		// Delete teacher here
		for (int i = 0; (unsigned)i < (unsigned)allData->staffData->size(); i++) { // loop through current teachers
			vector<vector<string>> ve = *allData->staffData;
			vector<string> v = ve[i];
			if (v[0] == std::to_string(teacherNumber)) { // if teacehr match
				v.clear(); // clear memory
				allData->staffData->erase(allData->staffData->begin() + i); // erase memory
				allData->SaveAll(); // overwrite file without info
				cout << "\nTeacher Deleted...\n\n"; // inform user

				// TODO delete their login

				return; // returns on completion of delete
			}
		}

		Error("Teacher not found..."); // error if no teacher found
	}

	/// <summary>
	/// edits a teacher
	/// </summary>
	static void Edit(file* allData) {
		// displaying all teachers based on user input
		bool displayAllInput = true; // loop boolean
		while (displayAllInput) {
			string input = GetRawInput("Would you like to see a list of all teachers?\ny or n : "); // raw input
			cout << endl;
			if (input == "y" || input == "Y") {
				DisplayList(allData); // show all students if requested
				displayAllInput = false; // stop loop
			}
			else if (input == "n" || input == "N") {
				displayAllInput = false; // stop loop
			}
			else {
				Error("INVALID INPUT"); // inform user
			}
		}

		string number = GetRawInput("Enter Teacher Number or 'e' to cancel : ");
		cout << endl;
		if (number == "e") {
			return;
		}

		// Delete teacher here
		for (int i = 0; (unsigned)i < (unsigned)allData->staffData->size(); i++) { // loop through current teachers
			vector<vector<string>> ve = *allData->staffData;
			vector<string> v = ve[i];
			if (v[0] == number) { // if teacehr match
				v.clear(); // clear memory
				allData->staffData->erase(allData->staffData->begin() + i); // erase 
				CreateNew(allData);

				return;
			}
		}
		// get teacher then edit

		Error("Teacher NOT Found...");
	}


};

/// <summary>
/// admin login
/// </summary>
class admin {
	file* allData; // pointer to data in memory

#pragma region AdminOptionsFunctions

#pragma region Create
	/// <summary>
	/// creates a teacher
	/// </summary>
	void CreateTeacher() {
		int teacherNumber = teacher::CreateNew(allData); // creates teacher here
		SignUp(allData, "teacher", teacherNumber);
		// create login for teacher here
	}

	/// <summary>
	/// creates a parent
	/// </summary>
	void CreateParent() {
		int parentNumber = parent::CreateNew(allData); // creates parent
		SignUp(allData, "parent", parentNumber);
	}

	/// <summary>
	/// creates a classroom with input
	/// </summary>
	void CreateClassroom() {
		classroom::CreateNew(allData); // Creates a new classroom
	}

	/// <summary>
	/// creates a student with input
	/// </summary>
	void CreateStudent() {
		int studentNumber = student::CreateNew(allData);
		SignUp(allData, "student", studentNumber);
		// use studentNumber to create a login
		// Create login for student here
	}
#pragma endregion

#pragma region Edit
	/// <summary>
	/// Edits a teacher
	/// </summary>
	void EditTeacher() {
		teacher::Edit(allData);
	}

	/// <summary>
	/// Edits a parent
	/// </summary>
	void EditParent() {
		parent::Edit(allData);
	}

	/// <summary>
	/// Edits a classroom with input
	/// </summary>
	void EditClassroom() {
		classroom::Edit(allData);
	}

	/// <summary>
	/// Edits a student with input
	/// </summary>
	void EditStudent() {
		student::Edit(allData);
	}
#pragma endregion

#pragma region Delete
	/// <summary>
	/// deletes a teacher
	/// </summary>
	void DeleteTeacher() {
		bool pre = true; // loop pre delete
		while (pre) { // loop input
			cout << "Would you like to see a list of teachers\n"; // prompt
			string rawInput = GetRawInput("y or n : "); // raw input
			cout << endl;
			if (rawInput == "y" || rawInput == "Y") {
				teacher::DisplayList(allData);
				pre = false; // stop loop
			}
			else if (rawInput == "n") {
				pre = false; // stop loop
			}
			else {
				Error("INVALID INPUT"); // inform of invalid input
			}
		}

		pre = true; // reuse pre loop bool
		while (pre) { // loop for input
			string rawInput = GetRawInput("Enter teacher number or 'e' to cancel : "); // raw input
			if (rawInput == "e" || rawInput == "E") { // if cancel input
				return; // stop function
			}

			// otherwise 
			try {
				int number = stoi(rawInput); // try convert input to int
				teacher::DeleteTeacher(allData, number); // delete teacher with number
				DeleteLogin(allData, number, "teacher");
				pre = false;
			}
			catch (...) {
				Error("INVALID INPUT"); // catch conversion errors
			}
		}


		// TODO Delete login on delete
	}

	/// <summary>
	/// deletes a parent
	/// </summary>
	void DeleteParent() {
		bool pre = true; // loop pre delete
		while (pre) { // loop input
			cout << "Would you like to see a list of parents\n"; // prompt
			string rawInput = GetRawInput("y or n : "); // raw input
			cout << endl;
			if (rawInput == "y" || rawInput == "Y") { // if y or Y
				// TODO display all classrooms
				parent::DisplayList(allData);
				pre = false; // stop loop
			}
			else if (rawInput == "n" || rawInput == "N") { // if n or N
				pre = false; // stop loop
			}
			else {
				Error("INVALID INPUT"); // inform of invalid input
			}
		}

		pre = true; // reuse pre loop bool
		while (pre) { // loop input
			string rawInput = GetRawInput("Enter parent number or 'e' to cancel : "); // raw input
			cout << endl;
			if (rawInput == "E" || rawInput == "e") { // if c or C
				return; // stop function
			}

			// otherwise

			try {
				int number = stoi(rawInput);
				bool confirm = parent::DeleteParent(allData, number); // try deleting the parent with the given number
				if (confirm) {
					for (vector<string> v : (*allData->loginData)) {
						if (v[3] == rawInput) {
							DeleteLogin(allData, number, "parent");
						}
					}
					pre = false;
				}
			}
			catch (...) {
				Error("Deleting parent was NOT successful..."); // catch errors and inform user
			}
		}
	}

	/// <summary>
	/// deletes an admin
	/// </summary>
	void DeleteAdmin() {
		// TODO Delete admin login
		cout << "\nYou cannot delete admins...\n\n";
	}

	/// <summary>
	/// deletes a classroom
	/// </summary>
	void DeleteClassroom() {
		bool pre = true; // pre delete classroom bool
		while (pre) { // loop pre delete
			cout << "Would you like to see a list of classrooms\n"; // prompt
			string rawInput = GetRawInput("y or n : "); // raw input
			cout << endl;
			if (rawInput == "y" || rawInput == "Y") { // if y or Y
				classroom::DisplayList(allData);
				pre = false; // stop loop
			}
			else if (rawInput == "n") { // if n or N
				pre = false; // stop loop
			}
			else {
				Error("INVALID INPUT"); // error if invalid input
			}
		}

		pre = true; // reuse pre loop boolean
		while (pre) { // pre delete loop
			string rawInput = GetRawInput("Enter classroom number or 'e' to cancel : "); // raw input
			if (rawInput == "e" || rawInput == "E") { // if c or C
				return; // stop function
			}

			// otherwise

			try {
				int number = stoi(rawInput); // try convert raw input to int
				classroom::DeleteClassroom(allData, number); // delete classroom with int
				pre = false;
			}
			catch (...) {
				Error("INVALID INPUT"); // catch conversion error
			}
		}
	}

	/// <summary>
	/// deletes a student
	/// </summary>
	void DeleteStudent() {
		bool pre = true; // pre loop boolean
		while (pre) { // pre loop
			cout << "Would you like to see a list of students\n"; // prompt
			string rawInput = GetRawInput("y or n : "); // raw input
			cout << endl;
			if (rawInput == "y" || rawInput == "Y") { // if y or Y
				student::DisplayAllStudents(allData); // show all students
				pre = false; // stop loop
			}
			else if (rawInput == "n" || rawInput == "N") { // if n or N 
				pre = false; // stop loop
			}
			else {
				Error("INVALID INPUT"); // inform user of invalid input
			}
		}

		pre = true; // reuse pre loop boolean
		while (pre) { // pre loop
			string rawInput = GetRawInput("Enter student number or 'e' to cancel : "); // raw input
			if (rawInput == "e" || rawInput == "E") { // if c or C
				return; // stop function on cancel
			}

			// otherwise

			try {
				int number = stoi(rawInput); // try convert input to int
				student::DeleteStudent(allData, number); // delete student with int input
				DeleteLogin(allData, number, "student");
				pre = false;
			}
			catch (...) {
				Error("INVALID INPUT"); // catch conversion error
			}
		}
	}
#pragma endregion

#pragma endregion

public:
	/// <summary>
	/// exit code called for destructors
	/// </summary>
	void Exit() {

	}

	/// <summary>
	/// displays admin options menu and deals with user actions / input
	/// </summary>
	void Options() {
		bool inAdminOptions = true; // loop bool
		while (inAdminOptions) { // loop while in the admin options menu
			//cout << "\n"; // spacing
			Title("Admin Options Menu"); // admin options title
			
			// display main options

			cout << "Options : \n";
			cout << "0. Back to main menu\n";
			cout << "1. Student   Options\n";
			cout << "2. Parent    Options\n";
			cout << "3. Teacher   Options\n";
			cout << "4. Classroom Options\n";
			cout << "5. School    Options\n";
			cout << "6. Login     Options\n";
		
			string rawIn = GetRawInput("\nSelection : "); // raw input
			cout << endl; // spacing
			
			// error prevention with menus...
			try {
				int selection = stoi(rawIn); // try convert string to int
				bool inSubMenu = true; // bool for sub menu loop -> true if selection is valid
				switch (selection) { // switch int selection based on input
				
				case 0: { // main menu
					bool inSubMenu = false; // stop sub loop
					inAdminOptions = false; // stop main loop -> takes you back out of function
					break; // break while loop instantly
				}
				case 1: { // show student options
					while (inSubMenu) { // loop sub menu
						// show sub menu options 
						cout << "Student Options : \n";
						cout << "0. Back to Admin menu\n";
						cout << "1. Add New Student\n";
						cout << "2. Edit    Student\n";
						cout << "3. Delete  Student\n";

						string rawInSub = GetRawInput("\nSelection : ");
						cout << endl;
						int subSelection = stoi(rawInSub); // try convert to int -> error kick you out to admin menu
						//cout << endl; // spacing

						switch (subSelection) // switch sub menu selection
						{
						case 0: // admin menu
							inSubMenu = false; // stop sub menu loop 
							break;
						case 1: { // new student
							Title("Creating New Student"); // title of action
							CreateStudent(); // crea	te a student
							break;
						}
						case 2: { // edit student
							Title("Editing Student"); // title of action
							EditStudent(); // edit a student
							break;
						}
						case 3: { // delete student
							Title("Delete Student"); // title of action
							DeleteStudent(); // delete a student
							break;
						}
						default:// crash the program -> cause error input
							string crashSub = "Im gonna crash the try (;"; // not an int
							int crash_nowSub = stoi(crashSub); // crash on purpose to kick into main admin options and out of sub
							break;
						}
					}
					break;
				}
				case 2: { // Show parent options
					while (inSubMenu) { // loop sub menu parent options
						// display options
						cout << "Parent Options : \n";
						cout << "0. Back to Admin menu\n";
						cout << "1. Add New Parent\n";
						cout << "2. Edit    Parent\n";
						cout << "3. Delete  Parent\n";

						string rawInSub = GetRawInput("\nSelection : ");
						cout << endl;
						int subSelection = stoi(rawInSub); // try convert to int -> error kick you out to admin menu
						//cout << endl; // spacing

						switch (subSelection)
						{
						case 0: // back to admin menu
							inSubMenu = false; // stop sub menu loop
							break;
						case 1: { // add parent
							CreateParent(); // create parent
							break;
						}
						case 2: { // edit parent 
							EditParent(); // edit parent
							break;
						}
						case 3: { // delete parent
							DeleteParent(); // delete parent
							break;
						}
						default:// crash the program -> cause error input
							string crashSub = "Im gonna crash the try (;"; // string not int
							int crash_nowSub = stoi(crashSub); // convert not int to int -> crash -> kicks you out of sub menu
							break;
						}
					}
					break;
				}
				case 3: { // Show teacher options
					while (inSubMenu) { // loop sub menu parent options
						// display options
						cout << "Teacher Options : \n";
						cout << "0. Back to Admin menu\n";
						cout << "1. Add New Teacher\n";
						cout << "2. Edit    Teacher\n";
						cout << "3. Delete  Teacher\n";
						 
						string rawInSub = GetRawInput("\nSelection : ");
						cout << endl;
						int subSelection = stoi(rawInSub); // try convert to int -> error kick you out to admin menu
						//cout << endl; // spacing

						switch (subSelection)
						{
						case 0: // back to admin menu
							inSubMenu = false;
							break;
						case 1: { // create teacher
							CreateTeacher(); // create a new teacher
							break;
						}
						case 2: { // edit teacher
							EditTeacher(); // edits a teacher
							break;
						}
						case 3: { // delete a teacehr
							DeleteTeacher(); // deletes teacher
							break;
						}
						default:// crash the program -> cause error input
							string crashSub = "Im gonna crash the try (;"; // string not int
							int crash_nowSub = stoi(crashSub); // convert not int to int -> crash -> kicks you out of sub menu
							break; 
						}
					}
					break;
				}
				case 4: { // Show classroom options
					while (inSubMenu) { // loop sub menu parent options
						// display options
						cout << "Classroom Options : \n";
						cout << "0. Back to Admin menu\n";
						cout << "1. Add New Classroom\n";
						cout << "2. Edit    Classroom\n";
						cout << "3. Delete  Classroom\n";

						string rawInSub = GetRawInput("\nSelection : ");
						cout << endl;
						int subSelection = stoi(rawInSub); // try convert to int -> error kick you out to admin menu
						//cout << endl; // spacing

						switch (subSelection)
						{
						case 0: // back to admin menu
							inSubMenu = false; // stop sub menu loop
							break;
						case 1: { // create classroom
							CreateClassroom(); // creates a classroom
							break;
						}
						case 2: { // edit classroom
							EditClassroom(); // edits a classroom
							break;
						}
						case 3: { // delete classroom
							DeleteClassroom(); // deletes a classroom
							break;
						}
						default:// crash the program -> cause error input
							string crashSub = "Im gonna crash the try (;"; // string not int
							int crash_nowSub = stoi(crashSub); // convert not int to int -> crash -> kicks you out of sub menu
							break;
						}
					}
					break;
				}
				case 5: { // Show school options
					while (inSubMenu) { // loop sub menu parent options
						// display options
						cout << "School Options : \n";
						cout << "0. Back to Admin menu\n";
						cout << "1. Set School Name\n";
						cout << "2. Set School Contact Info\n";
						cout << "3. School Notices\n";


						string rawInSub = GetRawInput("\nSelection : ");
						cout << endl;
						int subSelection = stoi(rawInSub); // try convert to int -> error kick you out to admin menu
						//cout << endl; // spacing

						switch (subSelection)
						{
						case 0: // back to admin menu
							inSubMenu = false; // stop sub menu loop
							break;
						case 1: { // edit school 
							string newSchoolName = GetRawInput("Enter new school name : ");
							cout << endl;

							if (allData->schoolData->size() == 0) {
								vector<string> v;
								v.push_back(newSchoolName);
								allData->schoolData->push_back(v);
							}
							else {
								(allData->schoolData->at(0)).at(0) = newSchoolName;
							}
							allData->SaveAll();
							break;
						}
						case 2: { // edit school 
							Title("Editing school contact info");
							cout << "Type 'e' to stop\n";
							vector<string> contactInfo;
							string contactInfoInput = "";
							int line = 1;
							while (contactInfoInput != "e") {
								contactInfoInput = GetRawInput("Enter in line " + std::to_string(line++) + " of contact info : ");
								if (contactInfoInput != "e") {
									contactInfo.push_back(contactInfoInput);
								}
							}
							cout << endl;

							if (allData->schoolData->size() == 0) {
								contactInfo.insert(contactInfo.begin(), "EDIT NAME OF SCHOOL...");
								allData->schoolData->push_back(contactInfo);
							}
							else if (allData->schoolData->size() == 1 || allData->schoolData->size() == 2) {
								string schoolName = (*allData->schoolData)[0][0];
								(*allData->schoolData)[0].clear();
								(*allData->schoolData)[0].push_back(schoolName);
								for (string s : contactInfo) {
									(*allData->schoolData)[0].push_back(s);
								}
							}
							else {
								Error("School Data Corrupted...");
							}
							allData->SaveAll();
							break;
						}
						case 3: {
							teacher::notices(allData);
							break;
						}
						default:// crash the program -> cause error input
							string crashSub = "Im gonna crash the try (;"; // string not int
							int crash_nowSub = stoi(crashSub); // convert not int to int -> crash -> kicks you out of sub menu
							break;
						}
					}
					break;
				}
				case 6: {
					while (inSubMenu) { // loop sub menu parent options
						// display options
						cout << "Login Options : \n";
						cout << "0. Back to Admin menu\n";
						cout << "1. Edit Student Login\n";
						cout << "2. Edit Parent Login\n";
						cout << "3. Edit Teacher Login\n";
						cout << "4. CHANGE ADMIN PASSWORD\n";


						string rawInSub = GetRawInput("\nSelection : ");
						cout << endl;
						int subSelection = stoi(rawInSub); // try convert to int -> error kick you out to admin menu
						//cout << endl; // spacing

						switch (subSelection)
						{
						case 0: // back to admin menu
							inSubMenu = false; // stop sub menu loop
							break;
						case 1: { // edit logins 
							student::DisplayAllStudents(allData);
							bool getInt = true;
							while (getInt) {
								string userIntStringInput = GetRawInput("Enter Student Number : ");
								cout << endl;
								try {
									int userInt = stoi(userIntStringInput);
									EditLogin(allData, userInt, "student");
									cout << "Login Successfully changed...\n\n";
									getInt = false;
								}
								catch(...){
									Error("INVALID INPUT");
								}
							}
							break;
						}
						case 2: { // edit logins 
							parent::DisplayList(allData);
							bool getInt = true;
							while (getInt) {
								string userIntStringInput = GetRawInput("Enter Parent Number : ");
								cout << endl;

								try {
									int userInt = stoi(userIntStringInput);
									EditLogin(allData, userInt, "parent");
									cout << "Login Successfully changed...\n\n";
									getInt = false;
								}
								catch (...) {
									Error("INVALID INPUT");
								}
							}
							break;
						}
						case 3: { // edit logins 
							teacher::DisplayList(allData);
							bool getInt = true;
							while (getInt) {
								string userIntStringInput = GetRawInput("Enter Teacher Number : ");
								cout << endl;

								try {
									int userInt = stoi(userIntStringInput);
									EditLogin(allData, userInt, "teacher");
									cout << "Login Successfully changed...\n\n";
									getInt = false;
								}
								catch (...) {
									Error("INVALID INPUT");
								}
							}
							break;
						}
						case 4: { // edit logins 
							string username = GetRawInput("Enter NEW ADMIN username : ");
							string password = GetRawInput("Enter NEW ADMIN password : ");
							for (int i = 0; (unsigned)i < (unsigned)allData->loginData->size(); i++) {
								vector<string> v = (*allData->loginData)[i];
								if (v[2] == "admin") {
									(*allData->loginData)[i][0] = username;
									(*allData->loginData)[i][1] = password;
									allData->SaveAll();
								}
							}
							/*for (vector<string> v : *allData->loginData) {
								if (v[2] == "admin") {
									v[1] = password;
								}
							}*/
							allData->SaveAll();
							cout << "\nLogin Successfully changed...\n\n";
							break;
						}
						default:// crash the program -> cause error input
							string crashSub = "Im gonna crash the try (;"; // string not int
							int crash_nowSub = stoi(crashSub); // convert not int to int -> crash -> kicks you out of sub menu
							break;
						}
						
					}
					break;
				}
				default:{ // crash the program -> cause error input
					string crash = "Im gonna crash the try (;"; // string not int
					int crash_now = stoi(crash); // convert not int to int -> crash -> catch code to run
				}
				}
				allData->SaveAll();
			}
			catch (...) { // catch any conversion errors in any menu or sub menu and loop from the admin menu
				Error("INVALID INPUT"); // inform invalid input
			}
		}
	}

	/// <summary>
	/// ctor for the admin
	/// </summary>
	admin(file * alldata)
	{
		this->allData = alldata; // store data pointer
	}

};

/// <summary>
/// login information and functions
/// </summary>
class login {
	enum class CURRENT_TYPE {
		admin,
		teacher,
		parent,
		student,
		none
	};

	CURRENT_TYPE ct;

public: // makes members accessible outside class{}
	bool loggedIn; // bool for checking if logged in or not
	union userType { // union for memory allocation of 1 / 4 types of login types
	public:
		parent* _parent;
		teacher* _teacher;
		admin* _admin;
		student* _student;
		int* null; // temp for reseting the union
	}type; // union is called type and accessible anywhere
	
	/// <summary>
	/// ctor for login
	/// </summary>
	login()
	{
		ct = CURRENT_TYPE::none;
		// reset union
		type.null = new int;
		delete type.null;
		type.null = nullptr;

		// login reset
		loggedIn = false;
	}

	/// <summary>
	/// logins the user
	/// </summary>
	/// <param name="allFiles"></param>
	void Login(file* allFiles) {
		string username; // reference 
		string password; // reference 

		int index = -1, attempts = 3; // attempts 3 left, index inaccessable
		bool success = false; // success of login
		bool found = false; // username match

		for (int i = 0; i < attempts; i++) { // for 3 attempts
			index = -1;
			success = false;
			found = false;
			username = GetRawInput("Enter your Username: "); // raw input
			for (int i = 0; (unsigned)i < (unsigned)(allFiles->loginData->size()); i++) { // for each stored login
				vector<string> v = (*allFiles->loginData)[i];
				if (username == v[0]) { // check if matches user input
					found = true; // set found
					index = i; // set index
				}
			}

			password = GetRawInput("Enter your Password: "); // ask for password
			cout << endl;
			if (found) {
				// if found check the password
				if (password == (*allFiles->loginData)[index][1]) { // compare password at index
					success = true; // set success
					break; // break out of the loop
				}
				else {
					Error("INVALID LOGIN");
				}
			}
			else if(!success || !found){ // if not found username or no success
				Error("INVALID LOGIN");
			}
		}

		if (success) { // if logged in successfully
			cout << "Logged In Successfully\n\n";
			loggedIn = true; // set logged state

			// set union to correct user type based on stored user type
			if ((*allFiles->loginData)[index][2] == "admin") { // admin
				type._admin = { new admin(allFiles) };  // admin
				ct = CURRENT_TYPE::admin;
			}
			else if ((*allFiles->loginData)[index][2] == "teacher") { // teacher
				type._teacher = { new teacher(allFiles, stoi((*allFiles->loginData)[index][3])) }; // teacher with number
				ct = CURRENT_TYPE::teacher;
			}
			else if ((*allFiles->loginData)[index][2] == "parent") { // parent
				type._parent = { new parent(allFiles, stoi((*allFiles->loginData)[index][3])) }; // parent with number
				ct = CURRENT_TYPE::parent;
			}
			else if ((*allFiles->loginData)[index][2] == "student") { // student
				type._student = { new student(allFiles, stoi((*allFiles->loginData)[index][3])) }; // student with number
				ct = CURRENT_TYPE::student;
			}
			else {
				Error("Failed to login - user type not specified correctly\n" + (*allFiles->loginData)[index][1]); // if no type an error has occured
				loggedIn = false; // invalidate login
				ct = CURRENT_TYPE::none;
			}

		}
		else { // if 3 attempts used and failed all 3
			Error("You have been locked out for 5 minutes...\n"); // lock out for 5 minutes
			//Sleep(5 * 1000); // 5 minutes
			using namespace std::this_thread;     // sleep_for, sleep_until
			using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
			using std::chrono::system_clock;

			sleep_for(10ns);
			sleep_until(system_clock::now() + 300s);
			loggedIn = false; // force logged out
		}
	}

	/// <summary>
	/// logs out a user
	/// </summary>
	void Logout() {
		loggedIn = false; // set state

		// reset union
		if (ct == CURRENT_TYPE::admin) {
			ct = CURRENT_TYPE::admin;
			delete type._admin;
			type._admin = nullptr;
		}
		else if (ct == CURRENT_TYPE::teacher) {
			ct = CURRENT_TYPE::teacher;
			delete type._teacher;
			type._teacher = nullptr;
		}
		else if (ct == CURRENT_TYPE::parent) {
			ct = CURRENT_TYPE::parent;
			delete type._parent;
			type._parent = nullptr;
		}
		else if (ct == CURRENT_TYPE::student) {
			ct = CURRENT_TYPE::student;
			delete type._student;
			type._student = nullptr;
		}
		if (type.null == nullptr) {
			type.null = new int;
			delete type.null;
			type.null = nullptr;
		}
	}

	/// <summary>
	/// shows options a logged in user has for them
	/// </summary>
	void Options() {
		// display admin options if union has admin
		if (ct == CURRENT_TYPE::admin) {
			type._admin->Options();
		}
		// display teacher options if union has teacher
		if (ct == CURRENT_TYPE::teacher) {
			type._teacher->Options();
		}
		// display parent options if union has parent
		if (ct == CURRENT_TYPE::parent) {
			type._parent->Options();
		}
		// display student options if union has student
		if (ct == CURRENT_TYPE::student) {
			type._student->Options();
		}
	}

	/// <summary>
	/// exit code called for destructors
	/// </summary>
	void Exit() {
		// TODO make sure safe to exit
	}
};

#pragma endregion

#pragma region functions
/// <summary>
/// Shows the details of a type
/// </summary>
/// <param name="allData">data reference</param>
/// <param name="targetNumberString">target as string</param>
/// <param name="type">type of target</param>
void ViewDetails(file* allData, string targetNumberString, string type) {
	bool error = true;
	if (type == "student") {
		for (vector<string> v : *allData->studentData) {
			if (v[0] == targetNumberString) {
				Title("Student Info");
				cout << "Student Number : " << v[0] << endl;
				cout << "Student Name : " << v[1] << endl;
				error = false;
			}
		}
	}
	else if (type == "parent") {
		for (vector<string> v : *allData->parentData) {
			if (v[0] == targetNumberString) {
				Title("Parent Info");
				cout << "Parent Number : " << v[0] << endl;
				cout << "Name : " << v[2] << endl;
				cout << "Student Number : " << v[1] << endl;
				cout << "Student Name : ";
				bool studentFound = false;
				for (vector<string> vv : *allData->studentData) {
					if (vv[0] == v[1]) {
						cout << vv[1] << endl;
						studentFound = true;
					}
				}
				if (!studentFound) {
					cout << "NOT FOUND..." << endl;
				}
				cout << "Emergency Contact Indo : " << v[3] << endl;
				cout << "Address : " << v[4] << endl;
				cout << "DOB : " << v[5] << endl;
				cout << "Email : " << v[6] << endl;				
				error = false;
			}
		}
	}
	else if (type == "teacher") {
		for (vector<string> v : *allData->staffData) {
			if (v[0] == targetNumberString) {
				Title("Teacher Info");
				cout << "Teacher Number : " << v[0] << endl;
				cout << "Name : " << v[1] << endl;
				cout << "Classroom Teaching : " << v[2] << endl;
				cout << "Gender : " << v[3] << endl;
				cout << "Email : " << v[4] << endl;
				cout << "Contact Number : " << v[5] << endl;
				cout << "Year Level Teaching : " << v[6] << endl;
				error = false;
			}
		}
	}
	else if (type == "classroom") {
		for (vector<string> v : *allData->classroomData) {
			if (v[0] == targetNumberString) {
				Title("Classroom Info");
				cout << "Classroom Number : " << v[0] << endl;
				cout << "Students : \n";
				for (int i = 1; (unsigned)i < (unsigned)v.size(); i++) {
					cout << "Student - " << v[1] << endl;
				}
				error = false;
			}
		}
	}
	else if(error){
		Error("INVALID INPUT");
	}
}

/// <summary>
/// displays the notices from memory
/// </summary>
/// <param name="allData"></param>
void displayNotices(file* allData) {
	Title("School Notices");
	if (allData->schoolData->size() < 2) {
		cout << "There are no notices to display...\n\n";
		return;
	}
	else {
		int displayed = 0;
		for (int i = 0; (unsigned)i < (unsigned)((*allData->schoolData)[1]).size(); i++) {
			cout << (*allData->schoolData)[1][i] << endl;
			if ((unsigned)displayed < (unsigned)(*allData->schoolData).at(1).size() - 1) {
				cout << endl;
			}
			displayed++;
		}
		if (displayed == 0) {
			cout << "There are no notices to display...\n";
		}
		cout<< endl;
	}
}

void EditLogin(file* allData, int number, string type) {
	bool changed = false;
	for (vector<string> v : *allData->loginData) {
		if (v[2] == type) {
			if (v[3] == std::to_string(number)) {
				// match here
				
				DeleteLogin(allData, number, type);

				bool duplicateUsername = true;
				while (duplicateUsername) {
					string newUsername = GetRawInput("New username (can be old): ");
					string newPassword = GetRawInput("New password (can be old): ");
					duplicateUsername = false;
					for (vector<string> vv : *allData->loginData) {
						if (vv[0] == newUsername) {
							duplicateUsername = true;
						}
					}

					if (!duplicateUsername) {
						vector<string>* nl = new vector<string>;
						nl->push_back(newUsername);
						nl->push_back(newPassword);
						nl->push_back(type);
						nl->push_back(std::to_string(number));
						allData->loginData->push_back(*nl);
						allData->SaveAll();
						cout << "\nUsername and Password SUCCESSFULLY Updated...\n\n";
						delete nl;
						nl = nullptr;
						changed = true;
					}
				}
				if (changed) {
					break;
				}
			}
		}
	}

	if (!changed) {
		SignUp(allData, type, number);
	}
}

/// <summary>
/// deletes a user from login file
/// </summary>
/// <param name="allData">data pointer</param>
/// <param name="number">number identifier</param>
/// <param name="type">user type</param>
void DeleteLogin(file* allData, int number, string type) {
	int index;
	bool found = false;
	for (int i = 0; (unsigned)i < (unsigned)(allData->loginData->size()); i++) {
		if ((*allData->loginData)[i][2] == type) {
			if ((*allData->loginData)[i][3] == std::to_string(number)) {
				found = true;
				index = i;
			}
		}
	}
	if (found) {
		//((allData->loginData)[index].clear());
		allData->LoadAll();
		allData->loginData->erase(allData->loginData->begin() + index);
		allData->SaveAll();
	}
	else {
		Error("No login found to delete...");
	}
}

/// <summary>
/// signs a user up
/// </summary>
/// <param name="allFiles">data reference</param>
/// <param name="userType">user type to sign up</param>
void SignUp(file* allFiles, string userType, int number) {
	
	string  t = userType;
	t[0] = toupper(t[0]);

	Title("Create Login : " + t);
	string username; // reference
	string password; // reference

	bool duplicate = true; // assume is dupe
	while (duplicate) { // loop while duplicate username
		username = GetRawInput("Enter New Username: "); // raw input
		password = GetRawInput("Enter New Password: "); // raw input
		cout << endl;
		duplicate = false; // assume not a duplicate

		for (vector<string> v : *allFiles->loginData) { // foreach login in data
			if (username == v[0]) { // if username matches
				Error("Username ALREADY Exits");
				duplicate = true; // loop bc duplicate
			}
		}
	}

	vector<string> newUserLoginData; // vector to store login data

	// store data as relavent
	newUserLoginData.push_back(username);
	newUserLoginData.push_back(password);
	newUserLoginData.push_back(userType);
	if (number != NULL) {
		newUserLoginData.push_back(std::to_string(number)); // number from creation of type if possible
	}
	else {
		newUserLoginData.push_back(std::to_string(69));
	}

	// push new user into data
	allFiles->loginData->push_back(newUserLoginData);

	// save to file and data
	cout << "Login SAVED...\n\n";
	allFiles->SaveAll();
}

/// <summary>
/// gender to string
/// </summary>
/// <param name="_gender">gender</param>
/// <returns>string of gender</returns>
string gtos(gender _gender) {
	// returns enum gender as string or "" if failed
	
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
		Error("ERROR - gtos couldn't convert gender to a string..."); // invalid enum type error
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
	// returns enum lerning progress as string or "" if failed


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
		Error("ERROR - lptos couldn't convert learningProgress to a string..."); // invalid enum type error
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
	cout << prompt; // prompt
	
	if (newline) { cout << endl; } // newline if specified -> default not
	string rawInput; // reference

	getline(cin >> ws, rawInput); // raw input from getline including white space " "

	return rawInput; // return the raw input
}

/// <summary>
/// displays and error message to the console
/// </summary>
/// <param name="msg">msg to display</param>
void Error(string msg) {
	// change to red text b4
	int color = 4; // red
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color);

	cout << "******************ERROR*****************\n";
	cout << msg << endl;
	cout << "****************************************\n";

	color = 7; // white
	SetConsoleTextAttribute(hConsole, color);

	// change to normal after
}

/// <summary>
/// displays a title to the console
/// </summary>
/// <param name="t">title</param>
/// <param name="len">length of spacers</param>
void Title(string t, int len) {
	unsigned int sLength = (unsigned)t.length();

	for (int i = 0; i < len; i++) {
		cout << "-";
	}

	cout << endl;

	int count = 0;
	for (int i = 0; i < (int)len/2 - (int)(sLength / 2); i++) {
		cout << "-";
		count++;
	}

	cout << t;
	count += sLength;

	for (int i = count; i < len; i++) {
		cout << "-";
		count++;
	}

	cout << endl;

	for (int i = 0; i < len; i++) {
		cout << "-";
	}

	cout << endl;
}

#pragma endregion

/// <summary>
/// entry point to the program
/// </summary>
/// <returns>exit status</returns>
int main()
{
	const bool DEBUG = false; // debug for split functionallity testing 

	if (DEBUG) { // if debugging testing



	}
	else { // otherwise run the actual program

		file *allFiles = new file(); // create a new file pointer
		allFiles->LoadAll(); // load data from files

		login currentUser; // reference to user

		school* _school = new school(allFiles); // create new school
		int classrooms = _school->PopulateClassrooms(); // load classrooms from school

		if (classrooms == 0) { // if no classrooms setup
			_school->NewSchoolScreen(allFiles); // make school setup from scrap
		}
		else {
			_school->Welcome(); // welcome if school is setup
		}

		// loop while program is active
		bool running = true;
		while (running) {
			_school->MenuScreen(currentUser.loggedIn); // intro screen for main options

			string rawInput = GetRawInput("\nSelection : ");
			cout << endl;
			try {
				int input = stoi(rawInput); // parsed input

				// deal with input accordingly
				switch (input)
				{
				case 1: { // login / logout
					// TODO : Change to login &&&& Sign up
					if (currentUser.loggedIn) {
						currentUser.Logout(); // logout if logged in
					}
					else {
						currentUser.Login(allFiles); // login if logged out
					}
					
					break;
				}
				case 2: { // exit
					// may have to save data?? TODO: make sure exit is safe
					allFiles->SaveAll();
					_school->Exit();
					currentUser.Exit();
					running = false;
					break;
				}
				case 3: { // if a user is logged in, op 3 is more options
					if (currentUser.loggedIn) {
						currentUser.Options();
					}
					else {
						int parentNumber = parent::CreateNew(allFiles);
						SignUp(allFiles, "parent", parentNumber);
					}
					break;
				}
				default: {
					Error("INVALID INPUT");
					break;
				}
				}
			}
			catch (...) {
				Error("INVALID INPUT"); // error is parse fails, i.e, rawInput isn't a string
			}
			// deal with input
			// exit if requried
		}

		delete allFiles;
		allFiles = nullptr;

		delete _school;
		_school = nullptr;
	}

	return 0; // Exit the program with no issues
}