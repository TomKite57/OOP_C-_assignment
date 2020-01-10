// This header contains the measurement base class. This is an abstract base class to be specialised later
// No complicated functions are defined here, so no associated .cpp file will be used

#ifndef MEASUREMENT_CLASS_H
#define MEASUREMENT_CLASS_H

using namespace std;
#include"timestampClass.h"
#include"generalHeader.h"

// Forward declare class to make a friend fucntion for the >> operator
template <class V> class measurement;
template <class V> istream & operator>>(istream &is, measurement<V> & mes);

// Class template for all measurements. Make template so numeric values, vectors and strings (eg. N/A) can be inputs
template <class V> class measurement {
protected:
	V value, error, systError;	// The value, error and statistical error stored in this measurement
	string unit;	// A string storing the unit the values are measured in
	timestamp time;	// A timestamp storing the date and time of measurement

public:
	// Default constructor
	measurement();

	// Paramatrised constructor (For all different timestamp inputs, use template for this)
	template <class T> measurement(const V vv, const V ee, const V se, const string uu, const T tt);

	// Copy constructor
	measurement(const measurement<V> &mes);

	// Move constructor
	measurement(measurement<V> &&mes);

	// Destructor
	virtual ~measurement();

	// Copy assignment operator
	measurement & operator=(const measurement<V> &mes);

	// Move assignment operator
	measurement & operator = (measurement<V> &&mes);

	// Call >> operator friend of class
	template <class T> friend istream & operator >> (istream &is, measurement<T> & mes);
	template <class T> friend ostream & operator << (ostream &os, measurement<T> & mes);

	// Accesor functions
	V getValue() const { return(value); }
	V getError() const { return(error); }
	V getSystError() const { return(systError); }
	string getUnit() const { return(unit); }
	timestamp getTime() const { return(time); }

	// Function to show all the data in a measurement. This is printed directly to the screen
	void show() const;
};

// ############################## //
// #    Function definitions    # //
// ############################## //

// Default constructor
template<class V> measurement<V>::measurement(){
	value = V{}; error = V{}; systError = V{}; unit = "N/A"; time = timestamp{};
}

// Paramatrised constructor (For all different timestamp inputs, use template for this)
template <class V> template<class T> measurement<V>::measurement(const V vv, const V ee, const V se, const string uu, const T tt) {
	value = vv; error = ee; systError = se; unit = uu; time = tt;
}

// Copy constructor
template<class V> measurement<V>::measurement(const measurement<V> &mes) {
	value = mes.value; error = mes.error; systError = mes.systError; unit = mes.unit; time = mes.time;
}

// Move constructor
template<class V> measurement<V>::measurement(measurement<V> &&mes) {
	// Copy data
	value = mes.value; error = mes.error; systError = mes.systError; unit = mes.unit; time = mes.time;
	// Delete rvalue data
	mes.value = 0; mes.error = 0; mes.systError = 0; mes.unit = "N/A"; mes.time = timestamp();
}

// Destructor
template<class V> measurement<V>::~measurement(){}

// Copy assignment operator
template<class V> measurement<V> & measurement<V>::operator=(const measurement<V> &mes) {
	// Check for self assignment
	if (&mes == this) { return (*this); }
	value = mes.value; error = mes.error; systError = mes.systError; unit = mes.unit; time = mes.time;
	return (*this);
}

// Move assignment operator
template<class V> measurement<V> & measurement<V>::operator = (measurement<V> &&mes) {
	// Check for self assignment
	if (&mes == this) { return (*this); }
	// copy data
	value = mes.value; error = mes.error; systError = mes.systError; unit = mes.unit; time = mes.time;
	// Delete rvalue data
	mes.value = 0; mes.error = 0; mes.systError = 0; mes.unit = "N/A"; mes.time = timestamp();
	return (*this);
}

// Define >> operator
// For numbers
template<class V> istream & operator >> (istream & is, measurement<V>& mes){
	// Assume input is of form "type,value,error,systError,unit,timestamp"
	try {
		string fullLine;
		getline(is, fullLine);
		vector<string> substrings{ splitString(fullLine) };
		mes.value = substrings[0]; mes.error = substrings[1]; 
		mes.systError = substrings[2]; mes.unit = substrings[3]; mes.time = timestamp(substrings[4]);
		return (is);
	}
	catch (...) {
		cout << "Warning: Error while inserting data from ostream into measurement. Exiting program" << endl;
		exit(1);
	}
}
// For strings
template<> istream & operator >> (istream & is, measurement<string>& mes) {
	// Assume input is of form "type,value,timestamp"
	try {
		string fullLine;
		getline(is, fullLine);
		vector<string> substrings{ splitString(fullLine) };
		mes.value = substrings[0]; mes.error = "N/A"; 
		mes.systError = "N/A"; mes.unit = "N/A"; mes.time = timestamp(substrings.back());
		return (is);
	}
	catch (...) {
		cout << "Warning: Error while inserting data from ostream into measurement. Exiting program" << endl;
		exit(1);
	}
}

// Define << operator
template<class V> ostream & operator << (ostream & os, measurement<V>& mes) {
	os << mes.value << ", " << mes.error << ", " << mes.systError << ", " << mes.unit << ", " << mes.time;
	return os;
}


// Accesor functions to see the data in a measurement for numeric values
template<class V> void measurement<V>::show() const {
	cout << value << "+/-" << error << " " << unit << " with a statistical error of ";
	cout << systError << " " << unit << " measured at " << time << "." << endl;
}

// Accesor functions to see the data in a measurement for strings
template<> void measurement<string>::show() const {
	if (value == "error") {
		cout << "This measurement gave an error at " << time << endl;
	}
	else{
		cout << value << " measured at " << time << endl;
	}
}

#endif