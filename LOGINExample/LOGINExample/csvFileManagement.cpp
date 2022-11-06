//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//
//
//
//
//int main()
//{
//	std::fstream csvFileExample;
//	csvFileExample.open("csvFileExample.csv", std::ios::out);
//	// first name | last name | email | password
//
//	if (csvFileExample.is_open())
//	{
//		//csvFileExample << "John" << "Doe" << "johndoe@gmail.com" << "password1d1" ---- All the data is pushed into one cell
//		//In order to push to their individual cells seperate with "," (commas)
//		//csvFileExample << "John" << "," << "Doe" << "," << "johndoe@gmail.com" << "," << "password1d1"
//
//		csvFileExample << "John" << "," << "Doe" << "," << "johndoe@gmail.com" << "," << "password1d1";
//
//
//
//
//		csvFileExample.close();
//
//	}
//}