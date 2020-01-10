// This file contains the definitions for the generalHeader.h file

using namespace std;
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<tuple>

#include"generalHeader.h"

// A function to test if a string can be made an integer with the std::stoi function
bool isStringInt(const string & input) {
	bool isInt{ true };
	for (size_t i{ 0 }; i < input.length(); i++) {
		if (!isdigit(input[i])) { isInt = false; }
	}
	return(isInt);
}

// Function to determine if a string corresponds to a number or not with the std::stod function (Works for integers and non integers)
bool isStringNum(const string & input) {
	bool isNum{ true };
	size_t decimalPoints{ 0 }, numberOfDigits{ 0 };
	size_t counter{ 0 };
	// Check for - or + signs at the beggining (Only accepts 1, to match the std::stod function)
	if (input[counter] == '+' || input[counter] == '-') { counter++; }
	// Check for decimal points
	while (counter < input.size()) {
		if (input[counter] != '.') {
			if (isdigit(input[counter])) { numberOfDigits++; }
			else { isNum = false; }
		}
		else { decimalPoints++; }
		counter++;
	}
	if (decimalPoints > 1 || numberOfDigits == 0) { isNum = false; }
	return (isNum);
}

// Function to remove any whitespace from a string
string removeWhitespace(const string & input) {
	string output;
	for (size_t i{ 0 }; i < input.size(); i++) {
		if (input[i] != ' ') { output += input[i]; }
	}
	return (output);
}

// A function that can make any string all lower case
string makeLowerCase(const string & input) {
	string output{ input };
	for (size_t i{ 0 }; i < input.length(); i++) {
		output[i] = (char)tolower(input[i]);
	}
	return(output);
}

// A function that can discriminate between types of answer returning an integer corresponding to which type
// Input will me a vector of vectors, like a matrix where the nth row will be the nth type of response. Integer return type contains the 
// number of row that the users input belonged to. Starts conting from 0
int testUserInput(const vector<vector<string>> & validInputs) {
	string input;
	int output;
	bool valid{ false };
	do {
		getline(cin, input);
		input = makeLowerCase(input);
		if (!cin.fail()) {
			for (size_t i{ 0 }; i < validInputs.size(); i++) {
				for (size_t j{ 0 }; j < int(validInputs[i].size()); j++) {
					if (input == validInputs[i][j]) { valid = true; output = i; }
				}
			}
		}
		if (cin.fail() || !valid) { cout << "Input not valid. Try again:" << endl; }
	} while (cin.fail() || !valid);

	return(output);
}

// A function to split a sting up into substrings by commas
vector<string> splitString(string input) {
	vector<string> output;
	while (input.find(',') != string::npos) {
		output.push_back(input.substr(0, input.find(',')));
		input.erase(0, input.find(',') + 1);
		while (input[0] == ' ' || input[0] == '\t' || input[0] == ',') { input.erase(0, 1); }
	}
	output.push_back(input);
	return output;
}

