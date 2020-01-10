// Experimental data management system
// Code for storing, managing, manipulating and ultimately documenting experimental results
// Will use classes, multiple files, etc.
// Author: Tom Kite

using namespace std;

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>

// THIS IS THE PATH OF THE FOLDER IN WHICH THE DATA IS STORED, REPORTS WILL BE GENERATED, AND WHERE SAVE FILES ARE CREATED OR READ FROM.
// CHANGE THIS TO THE WORKING DIRECTORY OF YOUR PROJECT
const string path{ "\\\\nask.man.ac.uk\\home$\\Visual Studio 2013\\Projects\\dataProject\\dataProject\\" };

#include"timestampClass.h"
#include"experimentClass.h"
#include"generalHeader.h"
#include"mainMenu.h"

// Declare the global data storage (Note the extern declaration in mainMenu.h)
vector<experiment<double>*> numericExperimentVector;
vector<experiment<string>*> stringExperimentVector;

// Set startign value for static data
size_t timestamp::timestampCounter{ 0 };

int main() {

	bool runCode{ true };	// Variable that keeps the program in a loop till finished

	// Print opening message
	cout << "Welcome to the data managment system!" << endl;
	cout << "To access a section of the program just type the command for the corresponding action.";

	while (runCode) {

		// Print user options and number of loaded measurements:
		cout << endl;
		cout << timestamp::getTimestampCounter() << " measurements currently loaded" << endl;
		cout << "#################################" << endl;
		cout << "#     **Actions (Command)**     #" << endl;
		cout << "#################################" << endl;
		cout << "#     Load save file (load)     #" << endl;
		cout << "#     See experiments (see)     #" << endl;
		cout << "#    Add new experiment (new)   #" << endl;
		cout << "# Generate full report (report) #" << endl;
		cout << "#     Save current data (save)  #" << endl;
		cout << "#          Exit (exit)          #" << endl;
		cout << "#################################" << endl;
		cout << endl;

		int answer;
		answer = testUserInput({ {"load"},{"see"},{"new"},{"report"},{"save"},{"exit"} });
		if (answer == 0) { mainMenu::loadData(); }
		else if (answer == 1) { mainMenu::seeExperiments(); }
		else if (answer == 2) { mainMenu::addExperiment(); }
		else if (answer == 3) { mainMenu::makeReport(); }
		else if (answer == 4) { mainMenu::saveData(); }
		else if (answer == 5) { 
			cout << "Are you sure you want to exit the program? (y/n)" << endl;
			if (testUserInput({ {"yes","y"},{"no","n"} }) == 0) { runCode = false; }
		}
	}


	// Delete allocated data
	for (size_t i{ 0 }; i < numericExperimentVector.size(); i++) {
		delete numericExperimentVector[i];
	}
	for (size_t i{ 0 }; i < stringExperimentVector.size(); i++) {
		delete stringExperimentVector[i];
	}

	return(0);
}