#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <limits>

using namespace std;

struct Line {
	int id;
	int freq;
	vector<string> stops;
	vector<int> times;
};

struct Driver {
	string name;
	int id;
	int max_hours_day;
	int max_hours_week;
	int min_rest;
};

static vector<Line> lines;
static vector<Driver> drivers;
static string lines_path;
static string drivers_path;
const int STARTING_HOUR = 7;
const int ENDING_HOUR = 23;

void trim(string &s) {
	if (s.find_first_not_of(' ') != -1) s = s.substr(s.find_first_not_of(' '));
	if (s.find_last_not_of(' ') != -1) s = s.substr(0, s.find_last_not_of(' ') + 1);
}

void next(string &piece, string &l) {
	int temp = l.find_first_of(';');
	if (temp != string::npos) {
		piece = l.substr(0, temp);
		l = l.substr(temp + 1, l.length() - 1);
	}
	else {
		piece = l;
	}
	trim(piece);
}

vector<string> treatStops(string &piece) {
	vector<string> stops;
	string stop;
	bool notFinished = true;
	while (piece.length() > 0 && notFinished) {
		int temp = piece.find_first_of(',');
		if (temp != string::npos) {
			stop = piece.substr(0, temp);
			piece = piece.substr(temp + 1, piece.length());
		}
		else {
			stop = piece.substr(0, piece.length());
			notFinished = false;
		}
		trim(stop);
		stops.push_back(stop);
	}
	return stops;
}

vector<int> treatTimes(string &piece) {
	vector<int> times;
	string p;
	bool notFinished = true;
	while (piece.length() > 0 && notFinished) {
		int temp = piece.find_first_of(',');
		if (temp != -1) {
			p = piece.substr(0, temp);
			piece = piece.substr(temp + 1, piece.length());
		}
		else {
			p = piece.substr(0, piece.length());
			notFinished = false;
		}
		trim(p);
		times.push_back(stoi(p));
	}
	return times;
}

Line readLine(string l) {
	Line newLine;
	string piece;

	next(piece, l);

	newLine.id = stoi(piece);

	next(piece, l);

	newLine.freq = stoi(piece);

	next(piece, l);

	newLine.stops = treatStops(piece);

	next(piece, l);

	newLine.times = treatTimes(piece);

	return newLine;
}

Driver readDriver(string l) {
	Driver newDriver;
	string piece;

	next(piece, l);

	newDriver.id = stoi(piece);

	next(piece, l);

	newDriver.name = piece;

	next(piece, l);

	newDriver.max_hours_day = stoi(piece);

	next(piece, l);

	newDriver.max_hours_week = stoi(piece);

	next(piece, l);

	newDriver.min_rest = stoi(piece);

	return newDriver;
}

void loadFile(string type) {
	ifstream input_file;
	string path;
	string currLine;

	cout << "Insert " << type << "' filename (e.g. 'lines.txt', 'drivers.txt'): ";
	cin >> path;
	input_file.open(path);

	while (input_file.fail()) {
		cout << "There was an error opening the file. Please try again.\n";
		cout << "Insert " << type << "' filename (e.g 'lines.txt'): ";
		cin >> path;
		input_file.open(path);
	}

	if (type == "lines") {
		lines_path = path;
	}
	else {
		drivers_path = path;
	}

	do {
		getline(input_file, currLine);
		if (type == "lines") {
			lines.push_back(readLine(currLine));
		}
		else {
			drivers.push_back(readDriver(currLine));
		}

	} while (!input_file.eof());

	input_file.close();
}

vector<string> linesToStrings() {
	vector<string> v;
	string next;
	Line l;
	for (int i = 0; i < lines.size(); i++) {
		next.clear();
		l = lines.at(i);
		next = to_string(l.id) + " ; " + to_string(l.freq) + " ; ";
		for (int x = 0; x < l.stops.size(); x++) {
			next += l.stops.at(x);
			if (x != l.stops.size() - 1) {
				next += ", ";
			}
			else {
				next += " ; ";
			}
		}
		for (int x = 0; x < l.times.size(); x++) {
			next += to_string(l.times.at(x));
			if (x != l.times.size() - 1) next += ", ";
		}
		v.push_back(next);
	}
	return v;
}

vector<string> driversToStrings() {
	vector<string> v;
	string next;
	Driver d;
	for (int i = 0; i < drivers.size(); i++) {
		next.clear();
		d = drivers.at(i);
		next = to_string(d.id) + " ; " + d.name + " ; " + to_string(d.max_hours_day) + " ; " + to_string(d.max_hours_week) + " ; " +
			to_string(d.min_rest);
		v.push_back(next);
	}
	return v;
}

void changeFile(string type) {
	ofstream output_file;
	string path;
	vector<string> newStrings;

	if (type == "lines") {
		path = lines_path;
		newStrings = linesToStrings();
	}
	else {
		path = drivers_path;
		newStrings = driversToStrings();
	}

	output_file.open(path);

	if (output_file.fail()) {
		cerr << "The output file could not be opened.\n";
	}
	else {

		for (int i = 0; i < newStrings.size(); i++) {
			output_file << newStrings.at(i);
			if (i != newStrings.size() - 1) output_file << endl;
		}

		output_file.close();
	}
}

void printDrivers() {
	cout << std::left << setw(4) << "ID" << setw(3) << " " << setw(30) << "NAME" << setw(3) << " " << setw(7) << "H/SHIFT"
		<< setw(3) << " " << setw(6) << "H/WEEK" << setw(3) << " " << setw(6) << "H/REST" << endl;
	for (int i = 0; i < drivers.size(); i++) {
		Driver d1 = drivers.at(i);
		cout << std::left << setw(4) << d1.id << setw(3) << " ";
		cout << setw(30) << d1.name << setw(3) << " ";
		cout << std::right << setw(7) << d1.max_hours_day << setw(3) << " ";
		cout << setw(6) << d1.max_hours_week << setw(3) << " ";
		cout << setw(6) << d1.min_rest << endl;
	}
}

int searchDriver(int id) {
	int val = -1;
	for (int i = 0; i < drivers.size(); i++) {
		if (id == drivers.at(i).id) {
			val = i;
			break;
		}
	}
	return val;
}

void addDriver() {
	Driver newDriver;
	string name;
	int temp;

	cout << "ID: "; cin >> temp;
	while (searchDriver(temp) != -1) {
		cout << "The driver " << temp << " already exists.\nPlease insert a new id or remove the driver first.\n";
		cout << "ID: ";
		cin >> temp;
	}
	newDriver.id = temp;
	cout << "NAME: "; 
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	getline(cin, name);
	trim(name);
	newDriver.name = name;
	cout << "H/SHIFT: "; cin >> temp;
	newDriver.max_hours_day = temp;
	cout << "H/WEEK: "; cin >> temp;
	newDriver.max_hours_week = temp;
	cout << "H/REST: "; cin >> temp;
	newDriver.min_rest = temp;
	drivers.push_back(newDriver);
	cout << "Driver " << newDriver.id << " - " << newDriver.name << " added successfully." << endl;
}

void deleteDriver(int driverID, int driverPos) {
	drivers.erase(drivers.begin() + driverPos);
	cout << "Driver " << driverID << " deleted successfully." << endl << endl;
}

void changeDriverName(int driverID, int driverPos) {
	string newName;
	cout << "The current name for driver " << driverID << " is '" << drivers.at(driverPos).name << "'.\n";
	cout << "Insert the new name: ";
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	getline(cin, newName);
	drivers.at(driverPos).name = newName;
	cout << "Driver's name updated successfully." << endl << endl;
}

void changeDriverShift(int driverID, int driverPos) {
	int newShift;
	cout << "The current shift for driver " << driverID << " is: " << drivers.at(driverPos).max_hours_day << "h.\n";
	cout << "Insert the new shift(h): "; cin >> newShift;
	drivers.at(driverPos).max_hours_day = newShift;
	cout << "Driver's shift updated successfully." << endl << endl;
}

void changeDriverWeeklyShift(int driverID, int driverPos) {
	int newWeeklyShift;
	cout << "The current weekly shift for driver " << driverID << " is: " << drivers.at(driverPos).max_hours_week << "h.\n";
	cout << "Insert the new weekly shift(h): "; cin >> newWeeklyShift;
	drivers.at(driverPos).max_hours_week = newWeeklyShift;
	cout << "Driver's weekly shift updated successfully." << endl << endl;
}

void changeDriverRest(int driverID, int driverPos) {
	int newRest;
	cout << "The current rest for driver " << driverID << " is: " << drivers.at(driverPos).min_rest << "h.\n";
	cout << "Insert the new rest shift(h): "; cin >> newRest;
	drivers.at(driverPos).min_rest = newRest;
	cout << "Driver's rest updated successfully." << endl << endl;
}

void changeDriver() {
	int driverID, driverPos, op;
	cout << "Available driver IDs: ";
	for (int i = 0; i < drivers.size(); i++) {
		cout << drivers.at(i).id;
		if (i != (drivers.size() - 1)) {
			cout << ", ";
		}
		else {
			cout << endl;
		}
	}
	cout << "Choose driver (ID): ";
	cin >> driverID;
	driverPos = searchDriver(driverID);

	while (driverPos == -1) {
		cout << "ID not found." << endl;
		cout << "Choose driver (ID): ";
		cin >> driverID;
		driverPos = searchDriver(driverID);
	}

	do {
		cout << "Driver " << driverID << " selected." << endl << endl;
		cout << "1. Change driver's name." << endl;
		cout << "2. Change driver's shift." << endl;
		cout << "3. Change driver's weekly shifts." << endl;
		cout << "4. Change driver's minimum rest." << endl;
		cout << "5. Delete driver." << endl;
		cout << "0. Back." << endl << endl;
		cout << "Select an option: ";
		cin >> op;

		switch (op) {
		case 1:
			changeDriverName(driverID, driverPos);
			break;
		case 2:
			changeDriverShift(driverID, driverPos);
			break;
		case 3:
			changeDriverWeeklyShift(driverID, driverPos);
			break;
		case 4:
			changeDriverRest(driverID, driverPos);
			break;
		case 5:
			deleteDriver(driverID, driverPos);
			break;
		case 0:
			//Back
			break;
		default:
			cout << "Invalid opreation.";
		}

	} while (op != 0);

}

//void printLines() {
//	for (int i = 0; i < lines.size(); i++) {
//		Line l1 = lines.at(i);
//		cout << "ID: " << l1.id << endl;
//		cout << "FREQ: " << l1.freq << endl;
//		cout << "STOPS: ";
//		for (int i = 0; i < l1.stops.size(); i++) {
//			cout << l1.stops.at(i);
//			if (i != (l1.stops.size() - 1)) cout << ", ";
//		}
//		cout << endl << "TIMES: "; 
//		for (int i = 0; i < l1.times.size(); i++) {
//			cout << l1.times.at(i);
//			if (i != (l1.times.size() - 1)) cout << ", ";
//		}
//		cout << endl << endl;
//	}
//}

void printSummaryLines() {
	cout << "LINE SUMMARY" << endl << endl;
	cout << std::left << setw(5) << "ID" << setw(3) << " " << setw(5) << "FREQ" << setw(3) << " " << setw(50) << "PARAGENS" << endl;
	for (int i = 0; i < lines.size(); i++) {
		int x = 0;
		Line l1 = lines.at(i);
		cout << setw(5) << l1.id << setw(3) << " " << setw(5) << l1.freq << setw(3) << " ";
		for (int i = 0; i < l1.stops.size() ; i++) {
			x += l1.stops.at(i).size();
			x += 2;
			cout << l1.stops.at(i);
			if (i != (l1.stops.size() - 1) && x < 45) cout << ", ";
			if (x >= 45) {
				cout << ", etc.";
				break;
			}
		}
		cout << endl;
	}
}

int searchLine(int id) {
	int val = -1;
	for (int i = 0; i < lines.size(); i++) {
		if (id == lines.at(i).id) {
			val = i;
			break;
		}
	}
	return val;
}

void addLine() {
	Line newLine;
	string t;
	int lineID;
	cout << "ID: "; cin >> lineID;
	while (searchLine(lineID) != -1) {
		cout << "The line " << lineID << " already exists.\nPlease insert a new id or remove the line first.\n";
		cout << "ID: ";
		cin >> lineID;
	}
	newLine.id = lineID;
	cout << "FREQ: ";
	cin >> newLine.freq;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	cout << "STOPS (stop1, stop2, ...): ";
	getline(cin, t);
	newLine.stops = treatStops(t);
	t.clear();
	cout << "TIMES (time1, time2, ...): ";
	getline(cin, t);
	newLine.times = treatTimes(t);
	lines.push_back(newLine);
	cout << "Line " << newLine.id << " added successfully." << endl;
}

void deleteLine(int lineID, int linePos) {
	lines.erase(lines.begin() + linePos);
	cout << "Line " << lineID << " deleted successfully." << endl << endl;
}

void changeLineFreq(int lineID, int linePos) {
	int newFreq;
	cout << "The current line " << lineID << " frequency is " << lines.at(linePos).freq << "min.\n";
	cout << "Insert the new frequency: ";
	cin >> newFreq;
	lines.at(linePos).freq = newFreq;
	cout << "Line frequency updated successfully." << endl << endl;
}

void changeLineStops(int lineID, int linePos) {
	string stops;
	cout << "Line " << lineID << " has the following stops: ";
	for (int i = 0; i < lines.at(linePos).stops.size(); i++) {
		cout << lines.at(linePos).stops.at(i);
		if (i != lines.at(linePos).stops.size() - 1) {
			cout << ", ";
		}
		else {
			cout << "." << endl;
		}
	}
	cout << "Please insert the new line stops (stop1, stop2, ...): ";
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	getline(cin, stops);
	lines.at(linePos).stops = treatStops(stops);
	cout << "Line stops updated successfully." << endl;
}

void changeLineTimes(int lineID, int linePos) {
	string times;
	cout << "Line " << lineID << " has the following times: ";
	for (int i = 0; i < lines.at(linePos).times.size(); i++) {
		cout << lines.at(linePos).times.at(i);
		if (i != lines.at(linePos).times.size() - 1) {
			cout << ", ";
		}
		else {
			cout << "." << endl;
		}
	}
	cout << "Please insert the new line times (time1, time2, ...): ";
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	getline(cin, times);
	lines.at(linePos).times = treatTimes(times);
	cout << "Line times updated successfully." << endl;
}

void changeLine() {
	int lineID, linePos, op;
	cout << "Available line IDs: ";
	for (int i = 0; i < lines.size(); i++) {
		cout << lines.at(i).id;
		if (i != (lines.size() - 1)) {
			cout << ", ";
		}
		else {
			cout << endl;
		}
	}
	cout << "Choose line (ID): ";
	cin >> lineID;
	linePos = searchLine(lineID);

	while (linePos == -1) {
		cout << "ID not found." << endl;
		cout << "Choose line (ID): ";
		cin >> lineID;
		linePos = searchLine(lineID);
	}

	do {
		cout << "Line " << lineID << " selected." << endl << endl;
		cout << "1. Change line frequency." << endl;
		cout << "2. Change line stops." << endl;
		cout << "3. Change line times." << endl;
		cout << "4. Delete line." << endl;
		cout << "0. Back." << endl << endl;
		cout << "Select an option: ";
		cin >> op;

		switch (op) {
		case 1:
			changeLineFreq(lineID, linePos);
			break;
		case 2:
			changeLineStops(lineID, linePos);
			break;
		case 3:
			changeLineTimes(lineID, linePos);
			break;
		case 4:
			deleteLine(lineID, linePos);
			break;
		case 0:
			//Back
			break;
		default:
			cout << "Invalid opreation.";
		}

	} while (op != 0);

}

vector<int> searchStops(string stop) {
	vector<int> foundLines;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t x = 0; x < lines.at(i).stops.size(); x++) {
			if (lines.at(i).stops.at(x) == stop) {
				foundLines.push_back(lines.at(i).id);
				break;
			}
		}
	}
	return foundLines;
}

void searchStops() {
	string stop;
	vector<int> foundLines;
	cout << "Insert the stop name to search for: ";
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	getline(cin, stop);
	foundLines = searchStops(stop);
	if (foundLines.empty()) {
		cout << "The stop '" << stop << "' does not belong to any line.\n";
	}
	else {
		cout << "The stop '" << stop << "' belongs to the following lines: ";
		for (size_t i = 0; i < foundLines.size(); i++) {
			cout << foundLines.at(i);
			if (i != foundLines.size() - 1) {
				cout << ", ";
			}
			else {
				cout << ".\n";
			}
		}
	}
}

void displayStopSchedule() {
	vector<int> foundLines;
	string stop;
	cout << "Insert the stop to look up its daily schedule: ";
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	getline(cin, stop);
	foundLines = searchStops(stop);
	while (foundLines.empty()) {
		cout << "The stop '" << stop << "' does not belong to any line.\n";
		foundLines.clear();
		cout << "Insert the stop to look up its daily schedule: ";
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		getline(cin, stop);
		foundLines = searchStops(stop);
	}

}

int main() {
	loadFile("lines");
	searchStops();
	return 0;
}
