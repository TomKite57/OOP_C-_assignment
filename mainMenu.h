// A header file containing all the definitions useful in the main fuctioning of the code. These are contained in the namespace mainMenu
// Due to the fact that these functions work with template classes, and some are themselves templates, they have all been defined here in the header

#ifndef	MAIN_MENU_HEADER_H
#define MAIN_MENU_HEADER_H

using namespace std;
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<utility>
#include<stdio.h>

#include"generalHeader.h"
#include"experimentClass.h"

// Declare the essential experiment storage in the header so it is accessible to the whole program
extern vector<experiment<double>*> numericExperimentVector;
extern vector<experiment<string>*> stringExperimentVector;

// Create a namespace for the main manu section of the code
// A seperate namespace will help keep functions seperate given the names are quite generic
namespace mainMenu {

	//#######//
	// Notes //
	//#######//

	// In these functions the word "coordinates" refers to a pair returning an experiments type and index in the respective experiment vector
	
	// Most of these experiments return bool types with the idea that 'false' types indicate some kind of error, while 'true' indicates success
	// In many cases they could have been void types

	// This file is very long, the reason for this is I encountered issues with using instances of template classes in function definitions 
	// in seperate cpp files. That being said, I kept the declarations and definitions seperate, and the f12 shortcut allows quick access to
	// a definition from the declaration

	// A function to find an experiments coordinates given its name
	pair<string, size_t> getExperimentPair(const string & input);

	// A function to check if an experiment name already exists or not
	bool checkTitle(const string & input);

	// A function to show the user all the experiments, and give more actions for them
	bool seeExperiments();

	// A function to add an experiment either by file or by keyboard
	bool addExperiment();

	// A function to delete a given experiment and its correlated experiments
	bool deletExperiment(const pair<string, size_t> & exp);

	// A function to generate a report for an experiment to file or screen, or all experiments if default parameter used
	bool makeReport(const pair<string, size_t> & input = pair<string, size_t>{"null",0});

	// A function to make a save file of all data in the current instance of the program
	bool saveData();

	// A function to load a save file of data
	bool loadData();

	// A function to change the name of an experiment
	bool rename(const pair<string,size_t> & currentExperiment);

	// A function that allows experiments to be correlated, for example temperature and pressure measurements.
	// Order matters in this process. Exp2 will be stored inside of Exp1, with the idea that Exp1 is the independent variable
	bool correlate(const pair<string, size_t> & exp1, const pair<string, size_t> & exp2, const bool & checkOrder = true);

	// Collection of functions useful for the data extraction in this program
	namespace dataExtraction {
		// A function to take the first input of measurement from the file and check its type
		// Will return true for valid types, false for invalid, and automatically update counters
		bool checkMeasurementType(const string & input, vector<size_t> & counters);
	}
}

// #################### //
// Function definitions //
// #################### //

// A function to find an experiments coordinates given its name
pair<string, size_t> mainMenu::getExperimentPair(const string & input) {
	for (size_t i{ 0 }; i < numericExperimentVector.size(); i++) {
		if (input == numericExperimentVector[i]->getTitle()) { return(pair<string, size_t>{"numeric", i }); }
	}
	for (size_t i{ 0 }; i < numericExperimentVector.size(); i++) {
		if (input == stringExperimentVector[i]->getTitle()) { return(pair<string, size_t>{"string", i }); }
	}
	cout << "Warning: Tried to find experiment that doesn't exist. Exiting program." << endl;
	exit(1);
}

// A function to check if an experiment name already exists or not
bool mainMenu::checkTitle(const string & input){
	for (size_t i{ 0 }; i < numericExperimentVector.size(); i++) {
		if (numericExperimentVector[i]->getTitle() == input) { return true; }
	}
	for (size_t i{ 0 }; i < stringExperimentVector.size(); i++) {
		if (stringExperimentVector[i]->getTitle() == input) { return true; }
	}
	return false;
}

// A function to show the user all the experiments, and give more actions for them
bool mainMenu::seeExperiments() {
	bool keepRunning{ true };
	pair<string, size_t> currentExperiment;
	
	if (numericExperimentVector.size() + stringExperimentVector.size() == 0) { 
		cout << "No experiments are currently being stored" << endl;
		return (false);
	}

	while (keepRunning) {
		// Print out all experiments and show number of correlated measurements contained within each
		cout << endl;
		if (numericExperimentVector.size() != 0) {
			cout << "# Numeric experiments #" << endl;
			for (size_t i{ 0 }; i < numericExperimentVector.size(); i++) {
				cout << numericExperimentVector[i]->getTitle();
				size_t numCorrel{ numericExperimentVector[i]->getNumberOfCorrelated() };
				if (numCorrel != 0) { cout << " (" << numCorrel + 1 << ")"; }
				cout << endl;
			}
		}
		if (stringExperimentVector.size() != 0) {
			cout << "# String experiments #" << endl;
			for (size_t i{ 0 }; i < stringExperimentVector.size(); i++) {
				cout << stringExperimentVector[i]->getTitle();
				size_t numCorrel{ stringExperimentVector[i]->getNumberOfCorrelated() };
				if (numCorrel != 0) { cout << " (" << numCorrel + 1 << ")"; }
				cout << endl;
			}
		}

		// Allow user to input experiment names and access them
		cout << endl << "Type an experiment name for more actions or type 'cancel' to return to the main menu" << endl;
		bool foundExperiment;
		do {
			foundExperiment = true;
			string answer;
			getline(cin, answer);
			if (answer == "cancel") { return (false); }
			if (!checkTitle(answer)) { cout << "Experiment not found, try again" << endl; foundExperiment = false; }
			else {
				for (size_t i{ 0 }; i < numericExperimentVector.size(); i++) {
					if (numericExperimentVector[i]->getTitle() == answer) {
						get<0>(currentExperiment) = "numeric";
						get<1>(currentExperiment) = i;
					}
				}
				for (size_t i{ 0 }; i < stringExperimentVector.size(); i++) {
					if (stringExperimentVector[i]->getTitle() == answer) {
						get<0>(currentExperiment) = "string";
						get<1>(currentExperiment) = i;
					}
				}
			}
		} while (!foundExperiment);

		cout << "What do you want to do with this experiment?" << endl;
		// Print user options:
		cout << "######################################" << endl;
		cout << "#       **Actions (Command)**        #" << endl;
		cout << "######################################" << endl;
		cout << "#        Make report (report)        #" << endl;
		cout << "#     Rename experiment (rename)     #" << endl;
		cout << "#     Delete experiment (delete)     #" << endl;
		cout << "#  Correlate experiment (correlate)  #" << endl;
		cout << "#          Cancel (cancel)           #" << endl;
		cout << "######################################" << endl;
		cout << endl;

		int answer;
		answer = testUserInput({ {"report"},{"rename"},{"delete"},{"correlate"},{"cancel"} });
		if (answer == 0) { makeReport(currentExperiment); }
		else if (answer == 1) { rename(currentExperiment); }
		else if (answer == 2) { deletExperiment(currentExperiment); }
		else if (answer == 3) {
			// Allow user to give the name of the second experiment to correlate with
			cout << "What experiment do you want to correlate this with? Or type 'cancel' to go back" << endl;
			pair<string, size_t> secondExperiment;
			bool foundExperiment;
			do {
				foundExperiment = true;
				string answer;
				getline(cin, answer);
				if (answer == "cancel") { return (false); }
				if (!checkTitle(answer)) { cout << "Experiment not found, try again" << endl; foundExperiment = false; }
				else {
					for (size_t i{ 0 }; i < numericExperimentVector.size(); i++) {
						if (numericExperimentVector[i]->getTitle() == answer) {
							get<0>(secondExperiment) = "numeric";
							get<1>(secondExperiment) = i;
						}
					}
					for (size_t i{ 0 }; i < stringExperimentVector.size(); i++) {
						if (stringExperimentVector[i]->getTitle() == answer) {
							get<0>(secondExperiment) = "string";
							get<1>(secondExperiment) = i;
						}
					}
				}
			} while (!foundExperiment);
			// Check for self correlation
			if (currentExperiment == secondExperiment) { cout << "A function can't correlate with itself" << endl; return false; }
			// Correlate experiments
			correlate(currentExperiment, secondExperiment);
			return true;
		}
		else if (answer == 4) { return(true); }

	}
	return (true);
}

// A function to add an experiment either by file or by keyboard
bool mainMenu::addExperiment() {
	string experimentType, inputStyle, experimentTitle;
	vector<string> measurements;

	// Generally 2 types of input are expected: numeric and strings
	// There is also the possibility of an input being "error", in which case the data is still recorded
	// Define a set of counters which quantify what type of measurements are contained in the file or input via keyboard
	// These counters and vector will be checked after to determine what type of experiment class to create
	vector<size_t> counters{ 0,0,0 };
	// Format: numeric counter, string counter, error counter

	cout << "Do you want to input data from a file or the keyboard? (f/k)" << endl;
	int tempInt{ testUserInput({ { "file","f" },{ "keyboard","key","k" },{ "cancel","exit" } }) };
	if (tempInt == 0) { inputStyle = "file"; }
	else if (tempInt == 1) {
		inputStyle = "keyboard";
		// If keyboard, ask user what type of experiment they want to input
		// Inputs from file will be determined automatically
		cout << "Does your measurement contain numeric values or strings? (num/str)" << endl;
		if (testUserInput({ { "numeric","num","n" },{ "string","str","s" } }) == 0) { experimentType = "numeric"; }
		else { experimentType = "string"; }
	}
	else if (tempInt == 2) {
		// Exit data input stage
		return (false);
	}

	// Ask user for title and check if title already exists
	bool validTitle;
	do {
		validTitle = true;
		cout << "Please input a name for the experiment, or enter 'cancel' if you have changed your mind." << endl;
		getline(cin, experimentTitle);
		if (experimentTitle == "cancel") { return(false); }
		if (checkTitle(experimentTitle)) { 
			cout << "Invalid title. '" << experimentTitle << "' already exists. Try again." << endl; 
			validTitle = false;
		}
	} while (!validTitle);

	if (inputStyle == "file") {
		// Read data from file
		string fileName;	// Variable to store name of the file
		ifstream inputFile;	// File containing the data (Read only)

		// Ask user for file name, and make sure file opens properly
		do {
			cout << "Please input a file name (including file extension), or enter 'cancel' if you have changed your mind." << endl;
			getline(cin, fileName);
			if (fileName == "cancel") { return(false); }
			inputFile.open(path + fileName);
			if (!inputFile.good()) { cout << "Could not open file. Try again." << endl; }
		} while (!inputFile.good());
		
		// Loop through file getting all lines and storing them in a vector
		while (!inputFile.eof()) {
			string tempString;
			getline(inputFile, tempString);
			if (tempString.length() != 0) { measurements.push_back(tempString); }
		}
		inputFile.close();
		
		// Make iterators
		vector<string>::const_iterator fileBegin{ measurements.begin() }, fileEnd{ measurements.end() }, fileCurrent;

		// Check if all strings are valid, and check their types
		for (fileCurrent = fileBegin; fileCurrent < fileEnd; ++fileCurrent) {
			if (!dataExtraction::checkMeasurementType(*fileCurrent, counters)) {
				// Detected an error
				cout << "Warning: Invalid measurement found in '" << fileName << "'." << endl;
				cout << "Exiting data input stage." << endl;
				return(false);
			}
		}
		// Check counters make sense: Either numeric or string, not both
		if (counters[0] != 0 && counters[1] != 0) {
			cout << "Warning: Inconsistant measurements found in '" << fileName << "'." << endl;
			cout << "Mix of numeric and string inputs" << endl;
			cout << "Exiting data input stage." << endl;
			return(false);
		}
		else if (counters[0] == 0 && counters[1] == 0 && counters[2] == 0) {
			cout << "Warning: Inconsistant measurements found in '" << fileName << "'." << endl;
			cout << "No measurements found" << endl;
			cout << "Exiting data input stage." << endl;
			return(false);
		}

		// Set experiment type according to lines found
		if (counters[0] != 0) { experimentType = "numeric"; }
		else { experimentType = "string"; }
	}
	else {
		// Read data from keyboard
		if (experimentType == "numeric") {
			cout << "Numeric experiments take the following format: (comma-seperated values)" << endl;
			cout << "Value, Error, StatisticalError, Unit, timestamp" << endl;
		}
		else {
			cout << "String experiments take the following format: (comma-seperated values)" << endl;
			cout << "Value, timestamp" << endl;
		}
		cout << "You can also input error measurements. Just type 'error' followed by the timestamp" << endl;
		cout << "Timestamps have the following format: YYYY/MM/DD hh:mm:ss" << endl;

		// Take in data
		string tempString;
		do {
			cout << "Input measurement, or type finished" << endl;
			getline(cin, tempString);
			// Check if valid
			if (!dataExtraction::checkMeasurementType(tempString, counters)) {
				if (tempString != "finished") { cout << "Last measurement wasn't valid" << endl; }
			}
			else if (experimentType == "numeric" && counters[1] != 0) {
				cout << "Last measurement wasn't valid" << endl;
				counters[1] = 0;
			}
			else if (experimentType == "string" && counters[0] != 0) {
				cout << "Last measurement wasn't valid" << endl;
				counters[0] = 0;
			}
			else {
				measurements.push_back(tempString);
			}
		} while (tempString != "finished");

		if (measurements.size() == 0) {
			cout << "Warning: No measurements were input" << endl;
			cout << "Exiting data input stage." << endl;
			return(false);
		}
	}

	// Now we have finished reading in the data. Now create experiments
	// Create either double or string experiment
	if (experimentType == "numeric") {
		experiment<double>* tempExp{ new experiment<double>{counters[0], counters[2], experimentTitle} };
		numericExperimentVector.push_back(tempExp);
	}
	else {
		experiment<string>* tempExp{ new experiment<string>{counters[1], counters[2], experimentTitle } };
		stringExperimentVector.push_back(tempExp);
	}

	// Now create experiment with either file or keyboard input
	// Now add measurements using a lambda function and iterators
	// Make iterators
	vector<string>::const_iterator vecStart{ measurements.begin() }, vecEnd{ measurements.end() };
	// We need to record how many measurement have been put in to the experiment so far
	size_t measurementCounter{ 0 }; size_t errorCounter{ 0 };
	for_each(vecStart, vecEnd,
		[experimentType, &measurementCounter, &errorCounter]
	(string fullString)
	{	
		vector<string> substrings{ splitString(fullString) };
		bool errorFound{ false };
		// Check for error string
		for (size_t i{ 0 }; i < substrings.size(); i++) {
			if (substrings[i] == "error") { errorFound = true; }
		}
		if (errorFound) {
			// Error measurement
			measurement<string> tempMes{"error", "N/A", "N/A", "N/A", substrings.back() };
			if (experimentType == "numeric") { 
				numericExperimentVector.back()->getMeasurementError(errorCounter) = tempMes; 
				numericExperimentVector.back()->getOrderVector().push_back(pair<string, size_t>{"error", errorCounter});
			}
			else { 
				stringExperimentVector.back()->getMeasurementError(errorCounter) = tempMes;
				stringExperimentVector.back()->getOrderVector().push_back(pair<string, size_t>{"error", errorCounter});
			}
			errorCounter++;
		}
		else if (experimentType == "numeric") {
			// Numeric experiment
			measurement<double> tempMes{stod(substrings[0]), stod(substrings[1]), stod(substrings[2]), substrings[3], substrings[4] };
			numericExperimentVector.back()->getMeasurement(measurementCounter) = tempMes;
			numericExperimentVector.back()->getOrderVector().push_back(pair<string, size_t>{"numeric", measurementCounter});
			measurementCounter++;
		}
		else {
			// String experiment
			measurement<string> tempMes{substrings[0], "N/A", "N/A", "N/A", substrings[1] };
			stringExperimentVector.back()->getMeasurement(measurementCounter) = tempMes;
			stringExperimentVector.back()->getOrderVector().push_back(pair<string, size_t>{"string", measurementCounter});
			measurementCounter++;
		}
	});

	cout << "A total of " << counters[0] + counters[1] + counters[2] << " measurements have succesfully been input." << endl;

	return (true);
}

// A function to delete a given experiment and its correlated experiments
bool mainMenu::deletExperiment(const pair<string, size_t>& exp){

	if (get<0>(exp) == "numeric") {
		// Numeric experiment
		delete numericExperimentVector[get<1>(exp)];
		numericExperimentVector.erase(numericExperimentVector.begin() + get<1>(exp));
	}
	else {
		// String experiment
		delete stringExperimentVector[get<1>(exp)];
		stringExperimentVector.erase(stringExperimentVector.begin() + get<1>(exp));
	}

	return true;
}

// A function to generate a report for an experiment to file or screen
// Do this by collecting a string stream and then printing this to screen or file depending on user input
bool mainMenu::makeReport(const pair<string, size_t> & index) {
	// Ask user if they want to print the report to a file or a screen
	cout << "Do you want to print the report to the screen, or to a file? (s/f). Or type 'cancel' to go back." << endl;
	int answer{ testUserInput({{"screen","s"},{"file","f"},{"cancel","c"}}) };
	if (answer == 2) { return false; }
	stringstream ss;
	if (index == pair<string, size_t>{"null", 0}) {
		// All experiments must be in the report
		for (size_t i{ 0 }; i < numericExperimentVector.size(); i++) {
			ss << numericExperimentVector[i]->makeReport(0).str();
			ss << endl  << "#################################################" << endl << endl;
		}
		for (size_t i{ 0 }; i < stringExperimentVector.size(); i++) {
			ss << stringExperimentVector[i]->makeReport(0).str();
			ss << endl << "#################################################" << endl << endl;
		}
	}
	else {
		// Only one experiment must be in the report
		if (get<0>(index) == "numeric") {
			ss = numericExperimentVector[get<1>(index)]->makeReport(0);
		}
		else if (get<0>(index) == "string") {
			ss = stringExperimentVector[get<1>(index)]->makeReport(0);
		}
	}
	if (answer == 0) { 
		// Print to screen
		cout << ss.str(); 
		return true; 
	}
	if (answer == 1) {
		// Print to file
		string fileName;
		bool validName;

		cout << "What do you want to name your report? Or type 'cancel' to go back." << endl;
		do {
			getline(cin, fileName);
			if (fileName == "cancel") { return(false); }
			if (fileName.length() == 0) { cout << "Empty names are not valid" << endl; validName = false; }
			else {
				fileName = "REP_" + fileName + ".txt";
				validName = true;
				// Try to open file with corresponding name to see if it already exists
				ifstream reportFile;
				reportFile.open(path + fileName);
				if (reportFile.good()) {
					cout << "File already exists, do you want to overwrite it? (y/n)" << endl;
					if (testUserInput({ { "yes","y" },{ "no","n" } }) == 0) {
						// Overwrite file
						reportFile.close();
						if (remove((path+fileName).c_str()) != 0) {
							cout << "Could not overwrite file. Enter a new name, Or type 'cancel' to return to the main menu." << endl;
							validName = false;
						}
					}
					else {
						// Close file and ask for new name
						reportFile.close();
						cout << "Enter a new name, or type 'cancel' to return to the main menu" << endl;
						validName = false;
					}
				}
			}
		} while (!validName);

		// Now create report file
		// Open file
		ofstream reportFile{ path + fileName };
		// Check file has been created correctly
		if (!reportFile.good()) { cout << "File could not be created, returning to menu" << endl; return (false); }
		// Write report to file
		reportFile << ss.str();
		// Close file
		reportFile.close();
		return true;
	}
	return true;
}

// Function to save data in a file named by the user
bool mainMenu::saveData(){
	// Check if any experiments are in the program
	if (numericExperimentVector.size() + stringExperimentVector.size() == 0) {
		cout << "No experiments are currently being stored" << endl;
		return (false);
	}
	string saveName;
	bool validName;
	// Ask user for name of save file
	cout << "What do you want to name your save file? Or type cancel to return to the main menu." << endl;
	do {
		getline(cin, saveName);
		if (saveName == "cancel") { return(false); }
		if (saveName.length() == 0) { cout << "Empty names are not valid" << endl; validName = false; }
		else {
			saveName = "SAV_" + saveName + ".dat";
			validName = true;
			// Try to open file with corresponding name to see if it already exists
			ifstream saveFile;
			saveFile.open(path+saveName);
			if (saveFile.good()) {
				cout << "File already exists, do you want to overwrite it? (y/n)" << endl;
				if (testUserInput({ { "yes","y" },{ "no","n" } }) == 0) {
					// Overwrite file
					saveFile.close();
					if (remove((path + saveName).c_str()) != 0) {
						cout << "Could not overwrite file. Enter a new name, Or type 'cancel' to return to the main menu." << endl;
						validName = false;
					}
				}
				else {
					// Close file and ask for new name
					saveFile.close();
					cout << "Enter a new name, or type 'cancel' to return to the main menu" << endl;
					validName = false;
				}
			}
		}
	} while (!validName);

	// Now create save file
	// Open file
	ofstream saveFile{path+saveName};

	if (!saveFile.good()) { cout << "File could not be created, returning to main menu" << endl; return (false); }
	// Loop through experiments creating the appropriate code in the save file
	for (size_t i{ 0 }; i < numericExperimentVector.size(); i++) {
		saveFile << numericExperimentVector[i]->getSaveFileStream().str();
	}
	for (size_t i{ 0 }; i < stringExperimentVector.size(); i++) {
		saveFile << stringExperimentVector[i]->getSaveFileStream().str();
	}
	return (true);
}

// A function to load a save file of data
bool mainMenu::loadData(){
	// Find save file
	string fileName;	// Variable to store name of the file
	ifstream inputFile;	// File containing the data (Read only)

	// Ask user for file name, and make sure file opens properly
	do {
		cout << "What is the name of your save file? Or enter 'cancel' if you have changed your mind." << endl;
		getline(cin, fileName);
		if (fileName == "cancel") { return(false); }
		fileName = "SAV_" + fileName + ".dat";
		inputFile.open(path+fileName);
		if (!inputFile.good()) { cout << "Could not open file. Try again." << endl; }
	} while (!inputFile.good());

	string fileLine;
	vector<string> substrings;
	bool validTitle;
	// Now copy data from save file
	while (!inputFile.eof()) {
		do {
			getline(inputFile, fileLine);
		} while (fileLine.length() == 0 && !inputFile.eof());
		substrings = splitString(fileLine);

		// Check for correlation line
		if (substrings[0] == "correlate") {
			correlate(getExperimentPair(substrings[1]), getExperimentPair(substrings[2]), false);
		}

		// Check for numeric experiment line
		if (fileLine == "NUMERIC_EXPERIMENT_BEGIN") {
			getline(inputFile, fileLine);
			substrings = splitString(fileLine);
			// Check if title already exists
			do {
				validTitle = true;
				// Check if user has said to cancel process. Note this is safe since no experiment could ever have name 'cancel'
				if (substrings[0] == "cancel") { return(false); }
				if (checkTitle(substrings[0])) {
					cout << "You are trying to load an experiment with a name that already exists: " << substrings[0] << endl;
					cout << "Please input a new name, or type 'cancel' to stop loading in experiments." << endl;
					getline(cin, substrings[0]);
					validTitle = false;
				}
			} while (!validTitle);

			// Create new experiment
			experiment<double>* tempExp{ new experiment<double>{size_t(stoi(substrings[1])),size_t(stoi(substrings[2])),substrings[0]} };
			// Create counters to keep track of all measurements currently loaded in
			size_t measurementCounter{ 0 }; size_t errorCounter{ 0 };
			getline(inputFile, fileLine);
			substrings = splitString(fileLine);
			while (fileLine != "NUMERIC_EXPERIMENT_END") {
				if (isStringNum(substrings[0])) {
					// Numeric measurement
					measurement<double> tempMes{stod(substrings[0]),stod(substrings[1]),stod(substrings[2]),substrings[3],substrings[4] };
					tempExp->getMeasurement(measurementCounter) = tempMes;
					tempExp->getOrderVector().push_back(pair<string, size_t>{"numeric", measurementCounter});
					measurementCounter++;
				}
				else {
					// Error measurement
					measurement<string> tempMes{ substrings[0], "N/A", "N/A", "N/A", substrings.back() };
					tempExp->getMeasurementError(errorCounter) = tempMes;
					tempExp->getOrderVector().push_back(pair<string, size_t>{"error", errorCounter});
					errorCounter++;
				}
				getline(inputFile, fileLine);
				substrings = splitString(fileLine);
			}
			numericExperimentVector.push_back(tempExp);
		}
		else if (fileLine == "STRING_EXPERIMENT_BEGIN") {
			getline(inputFile, fileLine);
			substrings = splitString(fileLine);

			// Check if title already exists
			do {
				validTitle = true;
				if (substrings[0] == "cancel") { return(false); }
				if (checkTitle(substrings[0])) {
					cout << "You are trying to load an experiment with a name that already exists: " << substrings[0] << endl;
					cout << "Please input a new name, or type 'cancel' to stop loading in experiments." << endl;
					getline(cin, substrings[0]);
					validTitle = false;
				}
			} while (!validTitle);

			// Create new experiment
			experiment<string>* tempExp{ new experiment<string>{size_t(stoi(substrings[1])),size_t(stoi(substrings[2])),substrings[0] } };
			// Create counters to keep track of all measurements currently loaded in
			size_t measurementCounter{ 0 }; size_t errorCounter{ 0 };
			getline(inputFile, fileLine);
			substrings = splitString(fileLine);
			while (fileLine != "STRING_EXPERIMENT_END") {
				// String and error measurements
				measurement<string> tempMes{substrings[0], "N/A", "N/A", "N/A", substrings.back() };
				if (substrings[0] != "error") { 
					tempExp->getMeasurement(measurementCounter) = tempMes;
					tempExp->getOrderVector().push_back(pair<string, size_t>{"string", measurementCounter});
					measurementCounter++;
				}
				else { 
					tempExp->getMeasurementError(errorCounter) = tempMes;
					tempExp->getOrderVector().push_back(pair<string, size_t>{"error", errorCounter});
					errorCounter++;
				}
				getline(inputFile, fileLine);
				substrings = splitString(fileLine);
			}
			stringExperimentVector.push_back(tempExp);
		}
	}
	return true;
}

// A function to change the name of an experiment
bool mainMenu::rename(const pair<string, size_t> & currentExperiment){
	bool validTitle;
	string newTitle;
	string currentTitle;

	// find current title
	if (get<0>(currentExperiment) == "numeric") { currentTitle = numericExperimentVector[get<1>(currentExperiment)]->getTitle(); }
	else { currentTitle = stringExperimentVector[get<1>(currentExperiment)]->getTitle(); }

	// Ask user for valid title
	do {
		validTitle = true;
		cout << "Please input a name for the experiment, or enter 'cancel' if you have changed your mind." << endl;
		getline(cin, newTitle);
		if (newTitle == "cancel") { return(false); }
		// Check if current title is same as old one
		if (newTitle == currentTitle) { return(false); }
		if (checkTitle(newTitle)) { cout << "Invalid title. '" << newTitle << "' already exists. Try again." << endl; validTitle = false; }
	} while (!validTitle);

	// Set new title
	if (get<0>(currentExperiment) == "numeric") { numericExperimentVector[get<1>(currentExperiment)]->setTitle(newTitle); }
	else { stringExperimentVector[get<1>(currentExperiment)]->setTitle(newTitle); }

	return (true);
}


// A function that allows experiments to be correlated, for example temperature and pressure measurements.
// Order matters in this process. Exp2 will be stored inside of Exp1, with the idea that Exp1 is the independent variable
// The check order variable is there so when correlation happens from load function it won't ask the user questions
bool mainMenu::correlate(const pair<string, size_t>& exp1, const pair<string, size_t>& exp2, const bool & checkOrder){
	// Need to check the experiments have a 1:1 correspondance
	size_t mes1, err1, mes2, err2;	// Counters for the number of measurements and errors in exp1 and exp2 respectively
	string firstTitle, secondTitle;	// Name of first and second experiment respectively

	// find all counter values and experiment names
	if (get<0>(exp1) == "numeric") {
		mes1 = numericExperimentVector[get<1>(exp1)]->getMeasurementCounter();
		err1 = numericExperimentVector[get<1>(exp1)]->getErrorCounter();
		firstTitle = numericExperimentVector[get<1>(exp1)]->getTitle();
	}
	else {
		mes1 = stringExperimentVector[get<1>(exp1)]->getMeasurementCounter();
		err1 = stringExperimentVector[get<1>(exp1)]->getErrorCounter();
		firstTitle = stringExperimentVector[get<1>(exp1)]->getTitle();
	}
	if (get<0>(exp2) == "numeric") {
		mes2 = numericExperimentVector[get<1>(exp2)]->getMeasurementCounter();
		err2 = numericExperimentVector[get<1>(exp2)]->getErrorCounter();
		secondTitle = numericExperimentVector[get<1>(exp2)]->getTitle();
	}
	else {
		mes2 = stringExperimentVector[get<1>(exp2)]->getMeasurementCounter();
		err2 = stringExperimentVector[get<1>(exp2)]->getErrorCounter();
		secondTitle = stringExperimentVector[get<1>(exp2)]->getTitle();
	}
	// Check if there is a 1:1 ratio
	if (mes1 + err1 != mes2 + err2) { 
		cout << "Experiments cannot be correlated, they don't have a 1:1 correspondance of measurements" << endl; 
		return (false); 
	}

	// This won't be checked if the correlation is happening from the load function
	if (checkOrder) {
		// Check the user has the correct order of experiments
		cout << "This implies that " << firstTitle << " contains the independent measurements, and ";
		cout << secondTitle << " the dependent measurements." << endl;
		cout << "Is this right? (y/n)" << endl;
		if (testUserInput({ { "yes","y" },{ "no","n" } }) == 1) {
			// User said no
			cout << "Do you want to swap the order, or cancel the process? (s/c)" << endl;
			if (testUserInput({ { "swap","s" },{ "cancel","c" } }) == 0) {
				// User said swap
				if (correlate(exp2, exp1)) { return true; }
				else { return false; }
			}
			else {
				// User said cancel
				return false;
			}
		}
	}

	// Now put the pointer to second experiment into the correlation vector in first experiment,
	// and remove the pointer from the numeric/stringExperimentVector
	// If experiment 1 is numeric
	if (get<0>(exp1) == "numeric") {
		experiment<double>* firstExp{ numericExperimentVector[get<1>(exp1)] };
		if (get<0>(exp2) == "numeric") {
			experiment<double>* secondExp{ numericExperimentVector[get<1>(exp2)] };
			firstExp->getNumericCorrelatedVector().push_back(secondExp);
			numericExperimentVector.erase(numericExperimentVector.begin() + get<1>(exp2));
		}
		else if (get<0>(exp2) == "string") {
			experiment<string>* secondExp{ stringExperimentVector[get<1>(exp2)] };
			firstExp->getStringCorrelatedVector().push_back(secondExp);
			stringExperimentVector.erase(stringExperimentVector.begin() + get<1>(exp2));
		}
	}
	// If experiment 1 is string
	else if (get<0>(exp1) == "string"){
		experiment<string>* firstExp{ stringExperimentVector[get<1>(exp1)] };
		if (get<0>(exp2) == "numeric") {
			experiment<double>* secondExp{ numericExperimentVector[get<1>(exp2)] };
			firstExp->getNumericCorrelatedVector().push_back(secondExp);
			numericExperimentVector.erase(numericExperimentVector.begin() + get<1>(exp2));
		}
		else if (get<0>(exp2) == "string") {
			experiment<string>* secondExp{ stringExperimentVector[get<1>(exp2)] };
			firstExp->getStringCorrelatedVector().push_back(secondExp);
			stringExperimentVector.erase(stringExperimentVector.begin() + get<1>(exp2));
		}
	}

	// If succesful return true
	return true;
}


// ##################### //
// #  DATA EXTRACTION  # //
// ##################### //

// A function to take the first input of measurement from the file and check its type
// Will return false if error was found (Empty vectors, inconsistant vectors, etc...)
bool mainMenu::dataExtraction::checkMeasurementType(const string & input, vector<size_t> & counters) {
	vector<string> substrings{ splitString(input) };
	// Does measurement have a valid timestamp?
	if (!timestamp::validTimeStamp(substrings.back())) { return (false); }
	// Is input numeric, string or error?
	// Check error first
	for (size_t i{ 0 }; i < substrings.size(); i++) {
		if (makeLowerCase(substrings[i]) == "error") { counters[2]++; return (true); }
	}
	// Check numeric type
	if (isStringNum(substrings[0])) {
		// Is measurement valid? Need 5 entries (value, error, systError, unit, time)
		// First 3 should be numeric (First already checked)
		if (substrings.size() != 5) { return (false); }
		if (!isStringNum(substrings[1]) || !isStringNum(substrings[2])) { return (false); }
		counters[0]++;
		return (true);
	}
	// If none of previous are true, must be general string
	// For general string expect only value and timestamp
	if (substrings.size() != 2) { return (false); }
	counters[1]++; return (true);	// String type
}

#endif