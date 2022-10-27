// header files
#include <iostream>
#include <string>
#include <vector>

class Student;
class Classroom;
class Report;
class School;
class Teacher;
class Parent;
class Admin;
class Login;

using std::cin;
using std::cout;
using std::string;
using std::getline;
using std::ws;
using std::endl;
using std::vector;

// main function
int main()
{

}

class Report {
	


public:
	void view() {

	}
};

/// <summary>
/// a student with records and basic info
/// <para>a logged in student can see its Records and School notices</para>
/// </summary>
class Student {
	vector<Report>* reports; // pointer to list of reports ** vector
	int studentNumber; // student number 
	string name; // full name of student

	static void reportGetAll(int studentNumber) {
		// TODO get file system to read student number from students folder and get raw data
	}

public:
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
	Report reportGet() {






		return; // -------------- temp return void to prevent error --------------
	}

	Student(string name, int studentNumber, bool newStudent = false) {
		this->name = name;
		this->studentNumber = studentNumber;
		reports = new vector<Report>();
		
		if (!newStudent) { // if student is already in system read contents from file
			reportGetAll(this->studentNumber);
		}

	}
};
