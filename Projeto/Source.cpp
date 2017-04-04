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
		if (temp != -1) {
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

	cout << "Insert " << type <<"' filename (e.g. 'lines.txt', 'drivers.txt'): ";
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
	for (int i = 0; i < drivers.size(); i++) {
		Driver d1 = drivers.at(i);
		cout << "ID: " << d1.id << endl;
		cout << "NAME: " << d1.name << endl;
		cout << "SHIFT: " << d1.max_hours_day << "H" << endl;
		cout << "H/WEEK: " << d1.max_hours_week << "H" << endl;
		cout << "RESTING HOURS: " << d1.min_rest << "H" << endl;
		cout << endl << endl;
	}
}

void printLines() {
	for (int i = 0; i < lines.size(); i++) {
		Line l1 = lines.at(i);
		cout << "ID: " << l1.id << endl;
		cout << "FREQ: " << l1.freq << endl;
		cout << "STOPS: ";
		for (int i = 0; i < l1.stops.size(); i++) {
			cout << l1.stops.at(i);
			if (i != (l1.stops.size() - 1)) cout << ", ";
		}
		cout << endl << "TIMES: "; 
		for (int i = 0; i < l1.times.size(); i++) {
			cout << l1.times.at(i);
			if (i != (l1.times.size() - 1)) cout << ", ";
		}
		cout << endl << endl;
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
	int p_id;
	cout << "ID: "; cin >> p_id;
	while (searchLine(p_id) != -1) {
		cout << "The line " << p_id << " already exists.\nPlease insert a new id or remove the line first.\n";
		cout << "ID: ";
		cin >> p_id;
	}
	newLine.id = p_id;
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
}

int main() {
	//loadFile("drivers");
	loadFile("lines");
	addLine();
	changeFile("lines");
	//printDrivers();
	printLines();
	return 0;
}
