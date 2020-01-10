// This file contains the definitions for the timestampClass.h file

using namespace std;
#include<iostream>
#include"timestampClass.h"
#include"generalHeader.h"

// Default constructor
timestamp::timestamp() {
	date = { 0, 0, 0, 0, 0, 0 };
	timestampCounter++;
}

// Paramatrised constructor with 6 ints
timestamp::timestamp(const int & YY, const int & MM, const int & DD, const int & hh, const int & mm, const int & ss) {
	date = { YY, MM, DD, hh, mm, ss };
	timestampCounter++;
}

// Paramatrised constructor with a vector
timestamp::timestamp(const vector<int> & input): timestamp() {
	if (input.size() == 6) { date = input; }
	else { cout << "Warning: timestamp input was invalid. Setting to default 0000/00/00 00:00:00" << endl; }
}

// Paramatrised constructor with a string
timestamp::timestamp(const string & input): timestamp() {

	// Check string is valid
	if (!timestamp::validTimeStamp(input)) {
		cout << "Warning: timestamp input was invalid. Setting to default 0000/00/00 00:00:00" << endl;
	}
	else {
		// Vector of strings to store all the substrings corresponding to parts of the date and time
		vector<string> timeStampStrings;	// <year,month,day,hour,minute,second>
		// Vector of ints giving important positions in the line (Colons, slashes, spaces)
		vector<int> linePositions;	// <slash1,slash2,space,colon1,colon2>

		// Get relevant positions (The last position being the end of the string)
		// Note the -1 at beginning is to make loop later work. +1 must be added to some positions to miss the / and : in substrings
		linePositions.push_back(-1);
		linePositions.push_back(input.find('/'));
		linePositions.push_back(input.find('/', linePositions[1] + 1));
		linePositions.push_back(input.find(' '));
		linePositions.push_back(input.find(':'));
		linePositions.push_back(input.find(':', linePositions[4] + 1));
		linePositions.push_back(input.size());
		
		// Get the substrings, removing white space
		for (int i{ 0 }; i < 6; i++) {
			string tempString{ input.substr(linePositions[i] + 1, linePositions[i + 1] - linePositions[i] - 1) };
			timeStampStrings.push_back(removeWhitespace(tempString));
		}

		// Now check if all strings are valid numbers
		for (size_t i{ 0 }; i < timeStampStrings.size(); i++) {
			date[i] = stoi(timeStampStrings[i]);
		}
	}
}

// Copy constructor
timestamp::timestamp(const timestamp &tt): timestamp() { 
	date = tt.date; 
}

// Move constructor
timestamp::timestamp(timestamp &&tt): timestamp() {
	date = tt.date;
	// Destroy data in rvalue timestamp
	tt.date.clear();
}

// Destructor
timestamp::~timestamp(){
	timestampCounter--;
}

// Copy assignment operator
timestamp & timestamp::operator=(const timestamp & tt){
	// Check for self assignment 
	if (&tt == this) { return (*this); }
	date = tt.date;
	return (*this);
}

// Move assignment operator
timestamp & timestamp::operator=(timestamp &&tt) {
	// Check for self assignment 
	if (&tt == this) { return (*this); }
	date = tt.date;
	// Destroy data in rvalue timestamp
	tt.date.clear();
	return (*this);
}

// Accesor functions for specific parts of the date
int timestamp::getYear() const { return date[0]; }
int timestamp::getMonth() const { return date[1]; }
int timestamp::getDay() const { return date[2]; }
int timestamp::getHour() const { return date[3]; }
int timestamp::getMinute() const { return date[4]; }
int timestamp::getSecond() const { return date[5]; }

// Static accesor function to check the number of measurements in program (And hence number of measurements)
size_t timestamp::getTimestampCounter() { return timestampCounter; }

// Accesor function for a vector representing the full date
vector<int> timestamp::getTimeVector() const { return date; }

// Accesor function for a vector representing the full date
string timestamp::getTimeString() const {
	string output;
	output = to_string(date[0]) + "/" + to_string(date[1]) + "/" + to_string(date[2]) + "  ";
	output += to_string(date[3]) + ":" + to_string(date[4]) + ":" + to_string(date[5]);
	return output;
}

// Overload << operator for output to the ostream
ostream & operator<<(ostream &os, const timestamp &tim) {
	os << tim.getTimeString();
	return os;
}

// Overload >> operator for output to the istream
istream & operator >> (istream &is, timestamp &tt) {
	// Take following format: YYYY/MM/DD  hh:mm:ss
	string line;
	getline(is, line);
	tt = timestamp(line);
	return is;
}

// A function to check if a string is a valid timestamp. Made static to be called without an instance of timeclass
bool timestamp::validTimeStamp(const string input) {
	// Boolean value telling us if an error has been found with the input
	bool validTimeStamp{ true };
	// Vector of strings to store all the substrings corresponding to parts of the date and time
	vector<string> timeStampStrings;	// <year,month,day,hour,minute,second>
	// Vector of ints giving important positions in the line (Colons, slashes, spaces)
	vector<int> linePositions;	// <slash1,slash2,space,colon1,colon2>

	// Count number of / and :
	size_t numSlash, numColon;
	numSlash = count(input.begin(), input.end(), '/');
	numColon = count(input.begin(), input.end(), ':');
	// Check if there are two of each
	if (numSlash != 2 || numColon != 2) { return (false); }
	else {
		// Get relevant positions (The last position being the end of the string)
		// Note the -1 at beginning is to make loop later work. +1 must be added to some positions to miss the / and : in substrings
		linePositions.push_back(-1);
		linePositions.push_back(input.find('/'));
		linePositions.push_back(input.find('/', linePositions[1] + 1));
		linePositions.push_back(input.find(' '));
		linePositions.push_back(input.find(':'));
		linePositions.push_back(input.find(':', linePositions[4] + 1));
		linePositions.push_back(input.size());

		// Get the substrings, removing white space
		for (int i{ 0 }; i < 6; i++) {
			string tempString{ input.substr(linePositions[i] + 1, linePositions[i + 1] - linePositions[i] - 1) };
			timeStampStrings.push_back(removeWhitespace(tempString));
		}

		// Now check if all strings are valid numbers
		for (size_t i{ 0 }; i < timeStampStrings.size(); i++) {
			if (!isStringInt(timeStampStrings[i])) { return(false); }
		}

		// If code reaches this point timestamp is valid
		return (true);
	}
}
