// This header contains the experiment base class. This is the structure in the code that will group many
// measurements together, and allow for calculations to be made on them all.

#ifndef	EXPERIMENT_CLASS_H
#define EXPERIMENT_CLASS_H
#include<string>
#include<sstream>
#include<tuple>
#include<algorithm>
#include"measurementClass.h"

using namespace std;

// Template class for experiments
template <class V> class experiment{

protected:
	// Counter to track the number of measurements
	size_t measurementCounter;
	// Container of valid measurements in the experiment
	measurement<V>* measurements;
	// Integer counting the number of measurement errors in the experiment
	size_t errorCounter;
	// Container of error messages in the measurements. All inherited class will have this
	measurement<string>* measurementErrors;
	// String recording the title of an experiment
	string title;
	// A vector of pairs which can record the order of input of measurements
	// Pair specifies type and index in that order eg (numeric, 3),(error, 5)
	vector<pair<string, size_t>> orderVector;
	// A vector of experiment pointers for both types to track correlated measurements
	vector<experiment<double>*> numericCorrelatedVector;
	vector<experiment<string>*> stringCorrelatedVector;

public:
	// Default constructor
	experiment();

	// Paramatrised constructor
	experiment(const size_t mc, const size_t ec, const string tt);

	// Copy constructor
	experiment(const experiment &exp);

	// Move constructor
	experiment(experiment &&exp);

	// Destructor
	virtual ~experiment();

	// Copy assignment operator
	experiment & operator=(const experiment &exp);

	// Move assignment operator
	experiment & operator=(experiment &&exp);

	// Accesor functions (Most are self explanatory, simply giving access to the private variables)
	size_t getMeasurementCounter() const { return(measurementCounter); }

	measurement<V> & getMeasurement(const size_t & index);

	size_t getErrorCounter() const { return(errorCounter); }

	measurement<string> & getMeasurementError(const size_t & index);

	string getTitle() const { return(title); }

	void setTitle(const string & newTitle) { title = newTitle; }

	vector<pair<string, size_t>> & getOrderVector() { return orderVector; }

	vector<experiment<double>*> & getNumericCorrelatedVector() { return (numericCorrelatedVector); }

	vector<experiment<string>*> & getStringCorrelatedVector() { return (stringCorrelatedVector); }

	// This returns the number of correlated experiments, not including the experiment itself
	size_t getNumberOfCorrelated() const;

	// Returns a string stream with the correctly formatted data for a save file
	stringstream getSaveFileStream() const;

	// Data analysis functions
	// Average
	V getAverage() const;

	// Standard deviation
	V getStandardDeviation() const;

	// Error on mean
	V getErrorOnMean() const;

	// Function to perform a linear fit. Returns vector of form (gradient, graidentError, intercept, interceptError, chiSq)
	vector<double> linearFit(const experiment<double> & depExp) const;

	// Number of occurences of a word or number, and a percentage
	vector<tuple<V, size_t, double>> countOccurrences() const;

	// A function attempting to study any correlation between a number and a string outcome of an experiment
	// Returns pair of <string value, corresponding average numeric value>
	template <class T> vector<pair<string, double>> numericStringCorrelation( experiment<T> & exp);

	// Function to generate a report for this experiment
	stringstream makeReport(int indent);
};

// ############################## //
// #    Function definitions    # //
// ############################## //

// Default constructor
template<class V> experiment<V>::experiment() :
	measurementCounter{ 0 }, errorCounter{ 0 }, title{ "N/A" }, orderVector{} {
	try {
		measurements = new measurement<V>[measurementCounter];
		measurementErrors = new measurement<string>[errorCounter];
	}
	catch (bad_alloc) {
		cout << "Warning: Error when allocating memory in experiment class. Exiting program" << endl;
		exit(1);
	}
}

// Paramatrised constructor
template<class V> experiment<V>::experiment(const size_t mc, const size_t ec, const string tt) {
	measurementCounter = mc; errorCounter = ec; title = tt;
	try {
		measurements = new measurement<V>[measurementCounter];
		measurementErrors = new measurement<string>[errorCounter] ;
	}
	catch (bad_alloc) {
		cout << "Warning: Error when allocating memory in experiment class. Exiting program" << endl;
		exit(1);
	}
}

// Copy constructor
template<class V> experiment<V>::experiment(const experiment &exp) {
	// copy data
	measurementCounter = exp.measurementCounter; errorCounter = exp.errorCounter;
	title = exp.title; orderVector = exp.orderVector;
	try {
		measurements = new measurement<V>[measurementCounter];
		measurementErrors = new measurement<string>[errorCounter];
	}
	catch (bad_alloc) {
		cout << "Warning: Error when allocating memory in experiment class. Exiting program" << endl;
		exit(1);
	}
	for (size_t i{ 0 }; i < measurementCounter; i++) {
		measurements[i] = exp.measurements[i];
	}
	for (size_t i{ 0 }; i < errorCounter; i++) {
		measurementErrors[i] = exp.measurementErrors[i];
	}
}

// Move constructor
template<class V> experiment<V>::experiment(experiment &&exp) {
	// copy data
	measurementCounter = exp.measurementCounter; errorCounter = exp.errorCounter;
	title = exp.title; orderVector = exp.orderVector;
	try {
		measurements = new measurement<V>[measurementCounter];
		measurementErrors = new measurement<string>[errorCounter];
	}
	catch (bad_alloc) {
		cout << "Warning: Error when allocating memory in experiment class. Exiting program" << endl;
		exit(1);
	}
	for (size_t i{ 0 }; i < measurementCounter; i++) {
		measurements[i] = exp.measurements[i];
	}
	for (size_t i{ 0 }; i < errorCounter; i++) {
		measurementErrors[i] = exp.measurementErrors[i];
	}
	// Delete exp data
	exp.measurementCounter = 0; exp.errorCounter = 0; exp.title = "N/A";
	measurements = NULL; measurementErrors = NULL; exp.orderVector.clear();
}

template<class V> experiment<V>::~experiment() {
	for (size_t i{ 0 }; i < numericCorrelatedVector.size(); i++) {
		delete numericCorrelatedVector[i];
	}
	for (size_t i{ 0 }; i < stringCorrelatedVector.size(); i++) {
		delete stringCorrelatedVector[i];
	}
	delete[] measurements; delete[] measurementErrors;
}

// Copy assignment operator
template<class V> experiment<V> & experiment<V>::operator=(const experiment &exp) {
	// Check for self assignment
	if (&exp == this) { return(*this); }
	// Delete data in arrays
	delete[] measurements; delete[] measurementErrors;
	// copy data
	measurementCounter = exp.measurementCounter; errorCounter = exp.errorCounter;
	try {
		measurements = new measurement<V>[measurementCounter];
		measurementErrors = new measurement<string>[errorCounter];
	}
	catch (bad_alloc) {
		cout << "Warning: Error when allocating memory in experiment class. Exiting program" << endl;
		exit(1);
	}
	title = exp.title; orderVector = exp.orderVector;
	for (size_t i{ 0 }; i < measurementCounter; i++) { measurements[i] = exp.measurements[i]; }
	for (size_t i{ 0 }; i < errorCounter; i++) { measurementErrors[i] = exp.measurementErrors[i]; }
	return (*this);
}

// Move assignment operator
template<class V> experiment<V> & experiment<V>::operator=(experiment &&exp) {
	// Check for self assignment
	if (&exp == this) { return(*this); }
	// Delete data in arrays
	delete[] measurements; delete[] measurementErrors;
	// copy data
	measurementCounter = exp.measurementCounter; errorCounter = exp.errorCounter;
	try {
		measurements = new measurement<V>[measurementCounter];
		measurementErrors = new measurement<string>[errorCounter];
	}
	catch (bad_alloc) {
		cout << "Warning: Error when allocating memory in experiment class. Exiting program" << endl;
		exit(1);
	}
	title = exp.title; orderVector = exp.orderVector;
	for (size_t i{ 0 }; i < measurementCounter; i++) {
		measurements[i] = exp.measurements[i];
	}
	for (size_t i{ 0 }; i < errorCounter; i++) {
		measurementErrors[i] = exp.measurementErrors[i];
	}
	// Delete exp data
	exp.measurementCounter = 0; exp.errorCounter = 0; exp.title = "N/A";
	measurements = NULL; measurementErrors = NULL; exp.orderVector.clear();
	return (*this);
}

// Accesor function for accesing a particular measurement
template<class V> measurement<V> & experiment<V>::getMeasurement(const size_t & index) {
	if (index < measurementCounter) { return (measurements[index]); }
	else {
		cout << "Warning: Attempt to access out of range index: " << index;
		cout << " > " << measurementCounter << endl; exit(1);
	}
}

// Accesor function for accesing a particular error measurement
template<class V> measurement<string> & experiment<V>::getMeasurementError(const size_t & index) {
	if (index < errorCounter) { return (measurementErrors[index]); }
	else {
		cout << "Warning: Attempt to access out of range index: " << index;
		cout << " > " << errorCounter << endl; exit(1);
	}
}

// Accesor function returning total number of correlated measurements
template<class V> size_t experiment<V>::getNumberOfCorrelated() const {
	size_t output{ 0 };
	output += numericCorrelatedVector.size();
	output += stringCorrelatedVector.size();
	if (output != 0) {
		// Look for correlations in correlated experiments
		for (size_t i{ 0 }; i < numericCorrelatedVector.size(); i++) {
			output += numericCorrelatedVector[i]->getNumberOfCorrelated();
		}
		for (size_t i{ 0 }; i < stringCorrelatedVector.size(); i++) {
			output += stringCorrelatedVector[i]->getNumberOfCorrelated();
		}
	}
	return output;
}

// Accesor function that returns a string stream with all save data information correctly formatted
template<class V> stringstream experiment<V>::getSaveFileStream() const {
	stringstream ss;
	ss << "NUMERIC_EXPERIMENT_BEGIN" << endl;
	ss << title << ", " << measurementCounter << ", " << errorCounter << endl;
	for (size_t i{ 0 }; i < orderVector.size(); i++) {
		if (get<0>(orderVector[i]) == "numeric") {
			// Numeric measurement
			ss << measurements[get<1>(orderVector[i])] << endl;
		}
		else {
			// Error measurement
			ss << measurementErrors[get<1>(orderVector[i])] << endl;
		}
	}
	ss << "NUMERIC_EXPERIMENT_END" << endl;

	for (size_t i{ 0 }; i < numericCorrelatedVector.size(); i++) {
		ss << numericCorrelatedVector[i]->getSaveFileStream().str() << endl;
		ss << "correlate, " << title << ", " << numericCorrelatedVector[i]->getTitle() << endl;
	}
	for (size_t i{ 0 }; i < stringCorrelatedVector.size(); i++) {
		ss << stringCorrelatedVector[i]->getSaveFileStream().str() << endl;
		ss << "correlate, " << title << ", " << stringCorrelatedVector[i]->getTitle() << endl;
	}
	return ss;
}

template<> stringstream experiment<string>::getSaveFileStream() const {
	stringstream ss;
	ss << "STRING_EXPERIMENT_BEGIN" << endl;
	ss << title << ", " << measurementCounter << ", " << errorCounter << endl;
	for (size_t i{ 0 }; i < orderVector.size(); i++) {
		if (get<0>(orderVector[i]) == "string") {
			// String measurement
			ss << measurements[get<1>(orderVector[i])] << endl;
		}
		else {
			// Error measurement
			ss << measurementErrors[get<1>(orderVector[i])] << endl;
		}
	}
	ss << "STRING_EXPERIMENT_END" << endl;

	for (size_t i{ 0 }; i < numericCorrelatedVector.size(); i++) {
		ss << numericCorrelatedVector[i]->getSaveFileStream().str();
		ss << "correlate, " << title << ", " << numericCorrelatedVector[i]->getTitle() << endl;
	}
	for (size_t i{ 0 }; i < stringCorrelatedVector.size(); i++) {
		ss << stringCorrelatedVector[i]->getSaveFileStream().str();
		ss << "correlate, " << title << ", " << stringCorrelatedVector[i]->getTitle() << endl;
	}
	return ss;
}


// Data analysis functions
// Average
// Numeric
template <class V> V experiment<V>::getAverage() const {
	double returnValue{ 0 };
	for (size_t i{ 0 }; i < measurementCounter; i++) {
		returnValue += measurements[i].getValue() + measurements[i].getSystError();
	}
	return (returnValue / measurementCounter);
}
// String
template <> string experiment<string>::getAverage() const {
	cout << "Warning: Experiments of type 'string' have no 'getAverage()' function. Exiting program." << endl;
	exit(1);
}

// Standard deviation
// Numeric
template <class V> V experiment<V>::getStandardDeviation() const {
	double returnValue{ 0 };
	double average{ this->getAverage() };
	for (size_t i{ 0 }; i < measurementCounter; i++) {
		returnValue += pow(measurements[i].getValue() + measurements[i].getSystError() - average, 2);
	}
	returnValue /= measurementCounter - 1;
	returnValue = sqrt(returnValue);
	return(returnValue);
}
// String
template <> string experiment<string>::getStandardDeviation() const {
	cout << "Warning: Experiments of type 'string' have no 'getStandardDeviation()' function. Exiting program." << endl;
	exit(1);
}

// Error on mean
// Numeric
template <class V> V experiment<V>::getErrorOnMean() const {
	double returnValue{ this->getStandardDeviation() };
	returnValue /= sqrt(measurementCounter);
	return(returnValue);
}
// String
template <> string experiment<string>::getErrorOnMean() const {
	cout << "Warning: Experiments of type 'string' have no 'getErrorOnMean()' function. Exiting program." << endl;
	exit(1);
}

// Function to perform a linear fit. Returns vector of form (gradient, graidentError, intercept, interceptError, chiSq)
template<> vector<double> experiment<double>::linearFit(const experiment<double>& depExp) const {
	vector<double> xValues, yValues, yWeights;
	double sumW{ 0 }, sumWX{ 0 }, sumWXX{ 0 }, sumWY{ 0 }, sumWXY{ 0 };
	int validEntries{ 0 };
	vector<double> output;

	// Collect data in vectors and apply standard formulas
	for (size_t i{ 0 }; i < orderVector.size(); i++) {
		if (get<0>(orderVector[i]) == "numeric" && get<0>(depExp.orderVector[i]) == "numeric") {
			// Check for error of 0
			if (depExp.measurements[get<1>(orderVector[i])].getError() == 0) {
				cout << "Warning: Attempted a linear fit with an error of 0. Exiting program" << endl;
				exit(1);
			}
			xValues.push_back(measurements[get<1>(orderVector[i])].getValue() + measurements[get<1>(orderVector[i])].getSystError());
			yValues.push_back(depExp.measurements[get<1>(depExp.orderVector[i])].getValue() 
				+ depExp.measurements[get<1>(depExp.orderVector[i])].getSystError());
			yWeights.push_back(1/(pow(depExp.measurements[get<1>(depExp.orderVector[i])].getError(), 2)));
			sumW += yWeights.back();
			sumWX += yWeights.back() * xValues.back();
			sumWXX += yWeights.back() * pow(xValues.back(), 2);
			sumWY += yWeights.back() * yValues.back();
			sumWXY += yWeights.back() * xValues.back() * yValues.back();
			validEntries++;
		}
	}
	
	// Check for a minnmum of 3 measurements
	if (validEntries < 3) {
		cout << "Warning: Linear fit could not be performed with less than 3 valid measurements." << endl;
		return(vector<double>{0});
	}

	// Calculate relevant quantities
	double denom{ (sumW*sumWXX) - pow(sumWX, 2) };
	double gradient{ ((sumW*sumWXY) - (sumWX*sumWY)) / denom };
	output.push_back(gradient);
	double gradientError{ sqrt(sumW / denom) };
	output.push_back(gradientError);
	double intercept{ ((sumWY*sumWXX) - (sumWX*sumWXY)) / denom };
	output.push_back(intercept);
	double interceptError{ sqrt(sumWXX / denom) };
	output.push_back(interceptError);

	double chiSq{ 0 };

	// Find the chiSq value
	for (size_t i{ 0 }; i < orderVector.size(); i++) {
		double residual = gradient*xValues[i] + intercept - yValues[i];
		chiSq += yWeights[i] * pow(residual, 2);
	}
	output.push_back(chiSq);

	double reducedChiSq{ chiSq / double(validEntries - 2) };
	output.push_back(reducedChiSq);

	// Return all this data
	return (output);
}
// For strings
template<> vector<double> experiment<string>::linearFit(const experiment<double>& depExp) const {
	cout << "Warning: string measurements have no linear fit function. Exiting program" << endl;
	exit(1);
}


// Count number of occurences of a word
template<> vector<tuple<string, size_t, double>> experiment<string>::countOccurrences() const {
	vector<tuple<string, size_t, double>> output;
	// Loop through all experiments
	for (size_t i{ 0 }; i < measurementCounter; i++) {
		bool foundValue{ false };
		// Loop through values already encountered
		for (size_t j{ 0 }; j < output.size(); j++) {
			if (this->measurements[i].getValue() == get<0>(output[j])) {
				// Found value
				foundValue = true;
				get<1>(output[j])++;
				get<2>(output[j]) += 100 / double(measurementCounter);
			}
		}
		if (!foundValue) {
			// Not found value, create instance in output vector
			output.push_back(tuple<string, size_t, double>{this->measurements[i].getValue(), 1, 100 / double(measurementCounter)});
		}
	}

	// Before returning list, make sure it is ordered from most to least common
	vector<tuple<string, size_t, double>>::iterator vecBegin{ output.begin() }, vecEnd{ output.end() };
	sort(vecBegin, vecEnd,
		[](tuple<string, size_t, double> tupA, tuple<string, size_t, double> tupB) {
		return(get<2>(tupA) > get<2>(tupB));
	});

	return (output);
}
// Number of occurences of numbers by dividing data into bins
template<> vector<tuple<double, size_t, double>> experiment<double>::countOccurrences() const {
	vector<tuple<double, size_t, double>> output;
	double binSize;
	vector<double> values;
	// Loop through all experiments
	for (size_t i{ 0 }; i < measurementCounter; i++) {
		values.push_back(measurements[i].getValue());
	}
	
	// Sort by value
	sort(values.begin(), values.end());

	// Find binsize (max-min)/10
	binSize = (values.back() - values[0]) / 10;

	// Create bins
	for (size_t i{ 0 }; i < 10; i++) {
		output.push_back(tuple<double, size_t, double>{values[0] + i*binSize, 0, 0});
	}

	// Fill entries of output
	int counter{ 1 };
	for (size_t i{ 0 }; i < values.size(); i++) {
		if (values[i] > (values[0] + binSize*counter)) { counter++;}
		get<1>(output[counter - 1]) += 1;
		get<2>(output[counter - 1]) += 100 / double(measurementCounter);
	}

	return (output);
}

// A function attempting to study any correlation between a number and a string outcome of an experiment
// Returns pair of <string value, corresponding average numeric value>
template<> template<> vector<pair<string, double>> experiment<double>::numericStringCorrelation(experiment<string>& exp) {
	vector<pair<string, double>> output;
	vector<int> entries;

	// For each type of string, find the average numeric outcome
	for (size_t i{ 0 }; i < measurementCounter; i++) {
		bool stringFound{ false };
		for (size_t j{ 0 }; j < output.size(); j++) {
			if (exp.getMeasurement(i).getValue() == get<0>(output[j])) {
				// Measure one more count for this string
				stringFound = true;
				get<1>(output[j]) += measurements[i].getValue() + measurements[i].getSystError();
				entries[j]++;
			}
		}
		if (!stringFound) {
			// Create entry for this string
			output.push_back(pair<string, double>{exp.getMeasurement(i).getValue(), measurements[i].getValue() + measurements[i].getSystError()});
			entries.push_back(1);
		}
	}

	// Divide by entries to get the average
	for (size_t i{ 0 }; i < entries.size(); i++) {
		get<1>(output[i]) /= entries[i];
	}

	return (output);
}
// For strings call function the other way around
template<> template<> vector<pair<string, double>> experiment<string>::numericStringCorrelation(experiment<double>& exp) {
	vector<pair<string, double>> output{ exp.numericStringCorrelation(*this) };
	return (output);
}

// Report function
// For numeric experiments
template<class V> stringstream experiment<V>::makeReport(int indent) {
	stringstream ss;
	size_t experimentCounter{ 1 };
	size_t correlatedExperimentCounter{ getNumberOfCorrelated() };
	if (correlatedExperimentCounter != 0) {
		ss << string(5 * indent, ' ') << "This is a set of " << correlatedExperimentCounter + 1 << " correlated experiments:" << endl;
		ss << string(5 * indent, ' ') << "Experiment " << experimentCounter << ":" << endl;
		indent++;
	}
	ss << string(5 * indent, ' ') << "Experiment: " << title << endl;
	ss << string(5 * indent, ' ') << "Type: numeric" << endl;
	ss << string(5 * indent, ' ') << "Contains " << measurementCounter << " valid measurements and " << errorCounter << " errors." << endl;
	ss << string(5 * indent, ' ') << "Average: " << this->getAverage() << "+/-" << this->getErrorOnMean() << " " 
		<< this->measurements[0].getUnit() << endl;
	ss << string(5 * indent, ' ') << "Standard deviation: " << this->getStandardDeviation() << " " 
		<< this->measurements[0].getUnit() << endl << endl;
	// Get vector of measurement occurences
	vector<tuple<V, size_t, double>> occurences{ this->countOccurrences() };
	ss << string(5 * indent, ' ') << "The distribution of the values is the following:" << endl;
	double binSize{ get<0>(occurences[1]) - get<0>(occurences[0]) };
	for (size_t i{ 0 }; i < occurences.size(); i++) {
		ss << string(5 * indent, ' ') << "(" << get<0>(occurences[0]) + i*binSize << "-" << get<0>(occurences[0]) + (i + 1)*binSize << "): "
			<< get<1>(occurences[i]) << " entries " << "(= " << get<2>(occurences[i]) << "%)" << endl;
	}
	ss << string(5 * indent, ' ') << "--------------------------------------------" << endl;
	experimentCounter++;

	if (correlatedExperimentCounter != 0) {
		for (size_t i{ 0 }; i < numericCorrelatedVector.size(); i++) {
			ss << string(5 * indent - 5, ' ') << "Experiment " << experimentCounter << ":" << endl;
			ss << numericCorrelatedVector[i]->makeReport(indent).str();
			experimentCounter++;
		}
		for (size_t i{ 0 }; i < stringCorrelatedVector.size(); i++) {
			ss << string(5 * indent - 5, ' ') << "Experiment " << experimentCounter << ":" << endl;
			ss << stringCorrelatedVector[i]->makeReport(indent).str();
			experimentCounter++;
		}
	}
	if (correlatedExperimentCounter == 1 && numericCorrelatedVector.size() == 1) {
		// Two numeric experiments, perform a linear fit
		ss << string(5 * indent - 5, ' ') << "A linear fit on these two experiments gives the following:" << endl;
		vector<double> linearFitParam{ this->linearFit(*(numericCorrelatedVector[0])) };
		if (linearFitParam.size() == 1) { ss << string(5 * indent, ' ') << "A linear fit can't be done. Less than 3 valid measurements" << endl; }
		else {
			ss << string(5 * indent - 5, ' ') << numericCorrelatedVector[0]->title << " = m x " << this->title << " + c" << endl;
			ss << string(5 * indent - 5, ' ') << "With m = " << linearFitParam[0] << " +/- " << linearFitParam[1] << endl;
			ss << string(5 * indent - 5, ' ') << "and c = " << linearFitParam[2] << " +/- " << linearFitParam[3] << "." << endl;
			ss << string(5 * indent - 5, ' ') << "This fit gives a chi squared value of " << linearFitParam[4] << " (when reduced "
				<< linearFitParam[5] << ")." << endl;
		}
	}
	if (correlatedExperimentCounter == 1 && stringCorrelatedVector.size() == 1) {
		// One numeric and one string, perform numericStringCorrelation function
		vector<pair<string, double>> stringAverages{ this->numericStringCorrelation(*(stringCorrelatedVector[0])) };

		ss << string(5 * indent - 5, ' ') << "The average numeric value corresponding to each string outcome is the following:" << endl;
		for (size_t i{ 0 }; i < stringAverages.size(); i++) {
			ss << string(5 * indent - 5, ' ') << get<0>(stringAverages[i]) << ": " << get<1>(stringAverages[i]) 
				<< " "<< this->measurements[0].getUnit() << endl;
		}
	}

	return ss;
}

// For string experiments
template<> stringstream experiment<string>::makeReport(int indent) {
	stringstream ss;
	size_t experimentCounter{ 1 };
	size_t correlatedExperimentCounter{ getNumberOfCorrelated() };

	if (correlatedExperimentCounter != 0) {
		ss << string(5 * indent, ' ') << "This is a set of " << correlatedExperimentCounter + 1 << " correlated experiments:" << endl;
		ss << string(5 * indent, ' ') << "Experiment " << experimentCounter << ":" << endl;
		indent++;
	}
	ss << string(5 * indent, ' ') << "Experiment: " << title << endl;
	ss << string(5 * indent, ' ') << "Type: string" << endl;
	ss << string(5 * indent, ' ') << "Contains " << measurementCounter << " valid measurements and " << errorCounter << " errors." << endl << endl;
	// Get vector of measurement occurences
	ss << string(5 * indent, ' ') << "The most commmon entries were the following:" << endl;
	vector<tuple<string, size_t, double>> occurences{ this->countOccurrences() };
	for (size_t i{ 0 }; i < occurences.size(); i++) {
		if (i > 5) { break; }
		ss << string(5 * indent, ' ') << "Value " << get<0>(occurences[i]) << " found ";
		ss << get<1>(occurences[i]) << " times (= " << get<2>(occurences[i]) << "%)" << endl;
	}
	ss << string(5 * indent, ' ') << "--------------------------------------------" << endl;
	experimentCounter++;

	if (correlatedExperimentCounter != 0) {
		for (size_t i{ 0 }; i < numericCorrelatedVector.size(); i++) {
			ss << string(5 * indent - 5, ' ') << "Experiment " << experimentCounter << ":" << endl;
			ss << numericCorrelatedVector[i]->makeReport(indent).str();
			experimentCounter++;
		}
		for (size_t i{ 0 }; i < stringCorrelatedVector.size(); i++) {
			ss << string(5 * indent - 5, ' ') << "Experiment " << experimentCounter << ":" << endl;
			ss << stringCorrelatedVector[i]->makeReport(indent).str();
			experimentCounter++;
		}
	}

	if (correlatedExperimentCounter == 1 && numericCorrelatedVector.size() == 1) {
		// One numeric and one string, perform numericStringCorrelation function
		vector<pair<string, double>> stringAverages{ this->numericStringCorrelation(*(numericCorrelatedVector[0])) };

		ss << string(5 * indent - 5, ' ') << "The average numeric value corresponding to each string outcome is the following:" << endl;
		for (size_t i{ 0 }; i < stringAverages.size(); i++) {
			ss << string(5 * indent - 5, ' ') << get<0>(stringAverages[i]) << ": " << get<1>(stringAverages[i])
				<< " " << numericCorrelatedVector[0]->getMeasurement(0).getUnit() << endl;
		}
	}

	return ss;
	
}

#endif
