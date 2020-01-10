// This header contatins the timestamp class. This will be used to store the date and time of a measurement.
// Functions defined in the timestampClass.cpp file

#ifndef	TIMESTAMP_CLASS_H
#define TIMESTAMP_CLASS_H

using namespace std;
#include<vector>

// Class to store the timestamp of a measurement
class timestamp {
	// Overload << operator for output to the ostream
	friend ostream & operator<<(ostream &os, const timestamp &tim);
	// Overload >> operator for input from the istream
	friend istream & operator>>(istream &is, timestamp &tt);
private:
	// Integers storing the timestamp information (format YYYY/MM/DD  hh:mm:ss)
	vector<int> date;	// {year, month, day, hour, minute, second}
	// Static data to see the number of timestamps in the program (1:1 correspondance with measurement)
	static size_t timestampCounter;
public:
	// Default constructor
	timestamp();
	// Paramatrised constructor with 6 ints
	timestamp(const int & YY, const int & MM, const int & DD, const int & hh, const int & mm, const int & ss);
	// Paramatrised constructor with a vector
	timestamp(const vector<int> & input);
	// Paramatrised constructor with a string
	timestamp(const string & input);

	// Copy constructor
	timestamp(const timestamp &tt);
	// Move constructor
	timestamp(timestamp &&tt);
	// Destructor
	~timestamp();

	// Copy assignment operator
	timestamp & operator=(const timestamp &tt);
	// Move assignment operator
	timestamp & operator=(timestamp &&tt);

	// Accesor functions for specific parts of the date
	int getYear() const;
	int getMonth() const;
	int getDay() const;
	int getHour() const;
	int getMinute() const;
	int getSecond() const;
	// Static accesor function to check the number of measurements in program (And hence number of measurements)
	static size_t getTimestampCounter();
	// Accesor function for a vector representing the full date
	vector<int> getTimeVector() const;
	// Accesor function for a string representing the full date
	string getTimeString() const;
	// A function to check if a string is a valid timestamp. Made static to be called without an instance of timeclass
	static bool validTimeStamp(const string input);
};

#endif