// This header contains useful functions for use in the main code
// Functions defined in the generalHeader.cpp file
// These functions don't contain any uage of measurement or experiment. These are in the mainMenu.h file
// They are seperate since they were problematic both in terms of linking and defining template functions

#ifndef	GENERAL_HEADER_H
#define GENERAL_HEADER_H

using namespace std;
#include<string>
#include<vector>
#include<iostream>

// A function to test if a string can be made an integer with the std::stoi function
bool isStringInt(const string & input);

// Function to determine if a string corresponds to a number or not with the std::stod function (Works for integers and non integers)
bool isStringNum(const string & input);

// Function to remove any whitespace from a string
string removeWhitespace(const string & input);

// A function that can make any string all lower case
string makeLowerCase(const string & input);

// A function that can discriminate between types of answer returning an integer corresponding to which type
// Input will me a vector of vectors, like a matrix where the nth row will be the nth type of response. Integer return type contains the 
// number of row that the users input belonged to. Starts conting from 0
int testUserInput(const vector<vector<string>> & validInputs);

// A function to split a sting up into substrings by commas
vector<string> splitString(string input);

#endif