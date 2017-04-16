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

struct Stop {
	string name;
	int linePos;
	int lineID;
	int posInLine;
	int stopH;
	int stopM;
	int freq;
	int direction;
};

struct Stops {
	vector<Stop> stopsDirect;
	vector<Stop> stopsInverse;
};

struct Route {
	int linePos;
	int lineID;
	int startingPos;
	int endingPos;
	int time;
	int direction;
	vector<string> stops;
};

static vector<Line> lines;
static vector<Driver> drivers;
static string lines_path;
static string drivers_path;
const int STARTING_HOUR = 10;
const int STARTING_MIN = 0;
const int ENDING_HOUR = 16;
const int ENDING_MIN = 0;
static bool linesChanged = false;
static bool driversChanged = false;

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
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	input_file.open(path);

	while (input_file.fail()) {
		cout << "There was an error opening the file. Please try again.\n";
		cout << "Insert " << type << "' filename (e.g 'lines.txt'): ";
		cin >> path;
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

	type.at(0) = toupper(type.at(0));
	cout << type << " file ('" << path << "') successfully loaded.\n";
	cout << "Press any key to continue...";
	getchar();

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

	if (type == "lines" && !linesChanged) {
		cout << "There are no changes to be deployed to " << type << " file ('" << lines_path << "').\n";
		cout << "Press any key to continue...";
		getchar();
	}
	else if (type == "drivers" && !driversChanged) {
		cout << "There are no changes to be deployed to " << type << " file ('" << drivers_path << "').\n";
		cout << "Press any key to continue...";
		getchar();
	}
	else {
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
			type.at(0) = toupper(type.at(0));
			cout << "Changes to " << type << " file ('" << path << "') successfully deployed.\n";
			if (type == "lines") linesChanged = false;
			else driversChanged = false;
			cout << "Press any key to continue...";
			getchar();

			output_file.close();
		}
	}
}

void printDrivers() {
	cout << "DRIVER SUMMARY\n\n";
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

void printDriver(int driverID) {
	int driverPos = searchDriver(driverID);
	Driver driverToDisplay = drivers.at(driverPos);
	cout << "Showing detailed information for driver " << driverID << ":\n\n";
	cout << std::left;
	cout << setw(15) << "ID: ";
	cout << driverToDisplay.id << endl;
	cout << setw(15) << "Name: ";
	cout << driverToDisplay.name << endl;
	cout << setw(15) << "Daily shift: ";
	cout << driverToDisplay.max_hours_day << "h\n";
	cout << setw(15) << "Weekly shift: ";
	cout << driverToDisplay.max_hours_week << "h\n";
	cout << setw(15) << "Resting hours: ";
	cout << driverToDisplay.min_rest << "h\n\n";
}

void addDriver() {
	Driver newDriver;
	string name;
	int tempInt;
	char op;
	bool finished = false;

	do {
		do {
			cout << "ID: ";
			if (cin >> tempInt && tempInt > 0) {
				if (searchDriver(tempInt) == -1) {
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					break;
				}
				else {
					cerr << "The driver ID already exists. Please choose another ID or remove the driver first.\n";
					cin.clear();
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			}
			else {
				cerr << "Invalid driver ID.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);
		newDriver.id = tempInt;
		do {
			cout << "Name: ";
			getline(cin, name);
			if (!name.empty()) {
				trim(name);
				break;
			}
			else {
				cerr << "Please insert a valid name.\n";
			}
		} while (true);
		newDriver.name = name;
		do {
			cout << "Daily shift(h): ";
			if (cin >> tempInt && tempInt > 0) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid driver shift.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);
		newDriver.max_hours_day = tempInt;
		do {
			cout << "Weekly shift(h): ";
			if (cin >> tempInt && tempInt > 0) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid driver weekly shift.n\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);
		newDriver.max_hours_week = tempInt;
		do {
			cout << "Rest(h): ";
			if (cin >> tempInt && tempInt > 0) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid driver rest.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);
		newDriver.min_rest = tempInt;
		drivers.push_back(newDriver);
		driversChanged = true;
		cout << "Driver " << newDriver.id << " - " << newDriver.name << " added successfully." << endl;

		do {
			cout << "Would you like to add another driver (Y/N) ? ";
			if (cin >> op && (op == 'Y' || op == 'N')) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				if (op == 'Y') break;
				else {
					finished = true;
					break;
				}
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);

	} while (!finished);
}

void deleteDriver(int driverID, int driverPos) {
	drivers.erase(drivers.begin() + driverPos);
	driversChanged = true;
	cout << "Driver " << driverID << " deleted successfully.\n";
	cout << "Press any key to continue...";
	getchar();
}

void changeDriverName(int driverID, int driverPos) {
	string newName;
	cout << "The current name for driver " << driverID << " is '" << drivers.at(driverPos).name << "'.\n";
	cout << "Insert the new name: ";
	getline(cin, newName);
	drivers.at(driverPos).name = newName;
	driversChanged = true;
	cout << "Driver name updated successfully.\n";
	cout << "Press any key to continue...";
	getchar();
}

void changeDriverShift(int driverID, int driverPos) {
	int newShift;
	cout << "The current shift for driver " << driverID << " is: " << drivers.at(driverPos).max_hours_day << "h.\n";
	cout << "Insert the new shift(h): "; cin >> newShift;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	drivers.at(driverPos).max_hours_day = newShift;
	driversChanged = true;
	cout << "Driver shift updated successfully.\n";
	cout << "Press any key to continue...";
	getchar();
}

void changeDriverWeeklyShift(int driverID, int driverPos) {
	int newWeeklyShift;
	cout << "The current weekly shift for driver " << driverID << " is: " << drivers.at(driverPos).max_hours_week << "h.\n";
	cout << "Insert the new weekly shift(h): "; cin >> newWeeklyShift;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	drivers.at(driverPos).max_hours_week = newWeeklyShift;
	driversChanged = true;
	cout << "Driver weekly shift updated successfully.\n";
	cout << "Press any key to continue...";
	getchar();
}

void changeDriverRest(int driverID, int driverPos) {
	int newRest;
	cout << "The current rest for driver " << driverID << " is: " << drivers.at(driverPos).min_rest << "h.\n";
	cout << "Insert the new rest shift(h): "; cin >> newRest;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	drivers.at(driverPos).min_rest = newRest;
	driversChanged = true;
	cout << "Driver rest updated successfully.\n";
	cout << "Press any key to continue...";
	getchar();
}

int chooseDriver() {
	int driverID;
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

	do {
		cout << "Choose driver (ID): ";
		if (cin >> driverID && (searchDriver(driverID) != -1)) {
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
		else {
			cerr << "Invalid line ID.\n";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	} while (true);

	return driverID;
}

void changeDriver(int driverID) {
	int driverPos, op;
	driverPos = searchDriver(driverID);
	
	do {
		system("cls");
		cout << "Driver " << driverID << " selected: \n\n";
		printDriver(driverID);
		cout << "[DRIVER MANAGEMENT MENU]\n\n";
		cout << "[1]- Change driver name.\n";
		cout << "[2]- Change driver shift.\n";
		cout << "[3]- Change driver weekly shifts.\n";
		cout << "[4]- Change driver minimum rest.\n";
		cout << "[5]- Delete driver.\n";
		cout << "[9]- Back.\n\n";
		cout << "Select an option: ";

		do {
			cout << "Insert the desired option: ";
			if (cin >> op && ((op >= 1 && op <= 5) || op == 9)) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);

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
			op = 9;
			break;
		}

	} while (op != 9);
}

void printSummaryLines() {
	cout << "LINE SUMMARY\n\n";
	cout << std::left << setw(5) << "ID" << setw(3) << " " << setw(5) << "FREQ" << setw(3) << " " << "ROUTE" << endl;
	for (int i = 0; i < lines.size(); i++) {
		Line l1 = lines.at(i);
		cout << setw(5) << l1.id << setw(3) << " " << setw(5) << l1.freq << setw(3) << " ";
		cout << l1.stops.at(0) << " <--> " << l1.stops.at(l1.stops.size() - 1) << endl;
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

void printDetailedLineInfo(int lineID) {
	int linePos = searchLine(lineID);
	Line lineToDisplay = lines.at(linePos);
	cout << std::left;
	cout << "Showing detailed information for line " << lineID << ":\n\n";
	cout << setw(12) << "ID: ";
	cout << lineID << endl;
	cout << setw(12) << "Frequency: ";
	cout << lineToDisplay.freq << "min\n";
	cout << setw(12) << "Stops: ";
	for (int i = 0; i < lineToDisplay.stops.size(); i++) {
		cout << lineToDisplay.stops.at(i);
		if (i != (lineToDisplay.stops.size() - 1)) cout << ", ";
		else cout << endl;
	}
	cout << setw(12) << "Times: ";
	for (int i = 0; i < lineToDisplay.times.size(); i++) {
		cout << lineToDisplay.times.at(i);
		if (i != (lineToDisplay.times.size() - 1)) cout << ", ";
		else cout << endl << endl;
	}
}

void addLine() {
	Line newLine;
	string t;
	int lineID, freq;
	char op;
	bool finished = false;
	do {
		system("cls");
		cout << "[LINE MANAGEMENT MENU]\n\n";
		cout << "Insert the new line information: \n\n";
		do {
			cout << "ID: ";
			if (cin >> lineID && lineID > 0) {
				if (searchLine(lineID) == -1) {
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					break;
				}
				else {
					cerr << "The line ID already exists. Please choose another ID or remove the line first.\n";
					cin.clear();
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			}
			else {
				cerr << "Invalid line ID.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);

		newLine.id = lineID;
		do {
			cout << "Frequency (min): ";
			if (cin >> freq && freq > 0) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid line frequency.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);
		newLine.freq = freq;

		cout << "Stops (stop1, stop2, ...): ";
		getline(cin, t);
		newLine.stops = treatStops(t);
		t.clear();
		do {
			cout << "TIMES (time1, time2, ...): ";
			getline(cin, t);
			vector<int> v = treatTimes(t);
			if (v.size() == (newLine.stops.size() - 1)) { newLine.times = v; break; }
			else cerr << "There should be " << newLine.stops.size() - 1 << " times based on the number of stops.\n";
		} while (true);

		lines.push_back(newLine);
		linesChanged = true;
		cout << "Line " << newLine.id << " added successfully." << endl;
		do {
			cout << "Would you like to add another line (Y/N) ? ";
			if (cin >> op && (op == 'Y' || op == 'N')) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				if (op == 'Y') break;
				else {
					finished = true;
					break;
				}
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);

	} while (!finished);
}

void deleteLine(int lineID, int linePos) {
	lines.erase(lines.begin() + linePos);
	linesChanged = true;
	cout << "Line " << lineID << " deleted successfully.\n";
	cout << "Press any key to continue...";
	getchar();
}

void changeLineFreq(int lineID, int linePos) {
	int newFreq;
	cout << "The current line " << lineID << " frequency is " << lines.at(linePos).freq << "min.\n";
	do {
		cout << "Insert the new frequency (min): ";
		if (cin >> newFreq) {
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
		else {
			cerr << "Invalid frequency.\n";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

	} while (true);
	lines.at(linePos).freq = newFreq;
	linesChanged = true;
	cout << "Line frequency updated successfully.\n";
	cout << "Press any key to continue...";
	getchar();
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
	do {
		cout << "Please insert the new line times (time1, time2, ...): ";
		getline(cin, times);
		vector<int> v = treatTimes(times);
		if (v.size() == (lines.at(linePos).stops.size() - 1)) break;
		else cerr << "There should be " << lines.at(linePos).stops.size() - 1 << " times based on the number of stops.\n";
		lines.at(linePos).times = v;
	} while (true);
	linesChanged = true;
	cout << "Line times updated successfully." << endl;
	cout << "Press any key to continue...";
	getchar();
}

void changeLineStops(int lineID, int linePos) {
	string stops;
	vector<string> v;
	bool needsTimeChange;
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
	do {
		needsTimeChange = false;
		cout << "Please insert the new line stops (stop1, stop2, ...): ";
		getline(cin, stops);
		v.clear();
		v = treatStops(stops);
		if (v.size() <= 1) {
			cerr << "There must be more than 1 stop.\n";
			continue;
		}
		if (v.size() == lines.at(linePos).stops.size()) break;
		else {
			needsTimeChange = true;
			break;
		}
	} while (true);
	lines.at(linePos).stops = v;
	linesChanged = true;
	cout << "Line stops updated successfully." << endl;
	if (needsTimeChange) {
		cout << "Since the number of stops changed, you must update the times.\n\n";
		changeLineTimes(lineID, linePos);
	}
	else {
		cout << "Press any key to continue...";
		getchar();
	}
}

void changeLine(int lineID) {
	int linePos, op;
	linePos = searchLine(lineID);

	do {
		system("cls");
		cout << "Line " << lineID << " selected:\n\n";
		printDetailedLineInfo(lineID);
		cout << "[LINE MANAGEMENT MENU]\n\n";
		cout << "[1]- Change line frequency.\n";
		cout << "[2]- Change line stops.\n";
		cout << "[3]- Change line times.\n";
		cout << "[4]- Delete line.\n";
		cout << "[9]- Back.\n\n";

		do {
			cout << "Insert the desired option: ";
			if (cin >> op && ((op >= 1 && op <= 4) || op == 9)) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);

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
			op = 9;
			break;
		}

	} while (op != 9);

}

Stops searchStops(string stop) {
	Stops foundLines;
	vector<Stop> stopsD;
	vector<Stop> stopsI;
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t x = 0; x < lines.at(i).stops.size(); x++) {
			if (lines.at(i).stops.at(x) == stop) {
				// SENTIDO DIRETO
				Stop newStop;
				newStop.name = stop;
				newStop.linePos = i;
				newStop.posInLine = x;
				newStop.lineID = lines.at(i).id;
				newStop.freq = lines.at(i).freq;
				newStop.stopH = STARTING_HOUR;
				newStop.stopM = STARTING_MIN;
				newStop.direction = 0;
				for (size_t z = 0; z < x; z++) {
					newStop.stopM += lines.at(i).times.at(z);
				}
				stopsD.push_back(newStop);
				//SENTIDO INVRSO
				Stop newStop2;
				newStop2.name = stop;
				newStop2.linePos = i;
				newStop2.posInLine = x;
				newStop2.lineID = lines.at(i).id;
				newStop2.freq = lines.at(i).freq;
				newStop2.stopH = STARTING_HOUR;
				newStop2.stopM = STARTING_MIN;
				newStop2.direction = 1;
				for (size_t z = lines.at(i).times.size() - 1; z > x; z--) {
					newStop2.stopM += lines.at(i).times.at(z);
				}
				stopsI.push_back(newStop2);
				break;
			}
		}
	}
	foundLines.stopsDirect = stopsD;
	foundLines.stopsInverse = stopsI;
	return foundLines;
}

void searchStops() {
	string stop;
	Stops foundLines;
	do {
		cout << "Insert the stop name to search for (CTRL-Z to cancel): ";
		getline(cin, stop);
		if (cin.eof()) {
			cin.clear();
			return;
		}
		foundLines = searchStops(stop);

		if (foundLines.stopsDirect.empty() && foundLines.stopsInverse.empty()) {
			cout << "Invalid stop name.\n";
		}
		else break;
	} while (true);

	cout << "The stop '" << stop << "' belongs to the following lines: ";
	for (size_t i = 0; i < foundLines.stopsDirect.size(); i++) {
		cout << foundLines.stopsDirect.at(i).lineID;
		if (i != foundLines.stopsDirect.size() - 1) {
			cout << ", ";
		}
		else {
			cout << ".\n";
		}

	}
}

void treatTime(int &h, int &m) {
	while (m >= 60) {
		m -= 60;
		h++;
	}
	while (h >= 24) {
		h -= 24;
	}
}

string displayTime(int &h, int &m) {
	string time;
	treatTime(h, m);
	if (h < 10) {
		time = "0" + to_string(h);
	}
	else {
		time = to_string(h);
	}
	time += ":";
	if (m < 10) {
		time += "0" + to_string(m);
	}
	else {
		time += to_string(m);
	}
	return time;
}

void displayStopSchedule() {
	Stops foundLines;
	string stop;
	bool notFinished;
	do {
		cout << "Insert the stop name to search for (CTRL-Z to cancel): ";
		getline(cin, stop);
		if (cin.eof()) {
			cin.clear();
			return;
		}
		foundLines = searchStops(stop);

		if (foundLines.stopsDirect.empty() && foundLines.stopsInverse.empty()) {
			cout << "Invalid stop name.\n";
		}
		else break;
	} while (true);

	//MAIN DIRECTION

	cout << std::left;
	for (int i = 0; i < foundLines.stopsDirect.size(); i++) {
		cout << setw(5) << foundLines.stopsDirect.at(i).lineID;
		if (i != foundLines.stopsDirect.size() - 1) {
			cout << setw(5) << " ";
		}
		else {
			cout << endl;
		}
	}

	do {
		notFinished = true;
		for (int i = 0; i < foundLines.stopsDirect.size(); i++) {
			treatTime(foundLines.stopsDirect.at(i).stopH, foundLines.stopsDirect.at(i).stopM);
			if (foundLines.stopsDirect.at(i).stopH > ENDING_HOUR || (foundLines.stopsDirect.at(i).stopH == ENDING_HOUR && foundLines.stopsDirect.at(i).stopM >= ENDING_MIN)) {
				cout << setw(5) << " ";
			}
			else {
				cout << setw(5) << displayTime(foundLines.stopsDirect.at(i).stopH, foundLines.stopsDirect.at(i).stopM);
				notFinished &= false;
				foundLines.stopsDirect.at(i).stopM += foundLines.stopsDirect.at(i).freq;
			}
			if (i != foundLines.stopsDirect.size() - 1) {
				cout << setw(5) << " ";
			}
			else {
				cout << endl;
			}
		}
	} while (!notFinished);
}

int chooseLine() {
	int lineID;
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

	do {
		cout << "Choose line (ID): ";
		if (cin >> lineID && (searchLine(lineID) != -1)) {
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
		else {
			cerr << "Invalid line ID.\n";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	} while (true);

	return lineID;
}

void displayLineSchedule() {
	int lineH, lineM, stopH, stopM, lineFreq;
	int lineID = chooseLine();
	int linePos = searchLine(lineID);
	Line line = lines.at(linePos);
	system("cls");
	cout << "[LINE " << lineID << " SCHEDULE]\n\n";
	lineFreq = line.freq;

	// MAIN DIRECTION
	lineH = STARTING_HOUR;
	lineM = STARTING_MIN;
	cout << "MAIN DIRECTION: ";
	cout << line.stops.at(0) << " --> ";
	cout << line.stops.at(line.stops.size() - 1) << endl << endl;
	for (int i = 0; i < line.stops.size(); i++) {
		cout << setw(line.stops.at(i).size()) << line.stops.at(i);
		if (i != line.stops.size() - 1) {
			cout << setw(3) << " ";
		}
		else {
			cout << endl;
		}
	}
	cout << std::left;
	while (true) {
		treatTime(lineH, lineM);
		if (lineH > ENDING_HOUR || (lineH == ENDING_HOUR && lineM >= ENDING_MIN)) {
			break;
		}
		cout << setw(line.stops.at(0).size()) << displayTime(lineH, lineM) << setw(3) << " ";
		stopH = lineH;
		stopM = lineM;
		for (int x = 0; x < line.times.size(); x++) {
			stopM += line.times.at(x);
			cout << setw(line.stops.at(x + 1).size()) << displayTime(stopH, stopM);
			if (x != line.times.size() - 1) {
				cout << setw(3) << " ";
			}
			else {
				cout << endl;
			}
		}
		lineM += lineFreq;
	}

	// INVERSE DIRECTION
	lineH = STARTING_HOUR;
	lineM = STARTING_MIN;
	cout << endl << "INVERSE DIRECTION: ";
	cout << line.stops.at(line.stops.size() - 1) << " --> ";
	cout << line.stops.at(0) << endl << endl;
	for (int i = line.stops.size() - 1; i >= 0; i--) {
		cout << setw(line.stops.at(i).size()) << line.stops.at(i);
		if (i != 0) {
			cout << setw(3) << " ";
		}
		else {
			cout << endl;
		}
	}

	cout << std::left;
	while (true) {
		treatTime(lineH, lineM);
		if (lineH > ENDING_HOUR || (lineH == ENDING_HOUR && lineM >= ENDING_MIN)) {
			break;
		}
		cout << setw(line.stops.at(line.stops.size() - 1).size()) << displayTime(lineH, lineM) << setw(3) << " ";
		stopH = lineH;
		stopM = lineM;
		for (int x = line.times.size() - 1; x >= 0; x--) {
			stopM += line.times.at(x);
			cout << setw(line.stops.at(x).size()) << displayTime(stopH, stopM);
			if (x != 0) {
				cout << setw(3) << " ";
			}
			else {
				cout << endl;
			}
		}
		lineM += lineFreq;
	}

	cout << endl << "Press any key to continue...";
	getchar();

}

void displayRoute() {
	string stop1, stop2;
	vector<Route> routes;
	Stops foundLines1, foundLines2;

	do {
		cout << "Insert the first stop name (CTRL-Z to cancel): ";
		getline(cin, stop1);
		if (cin.eof()) {
			cin.clear();
			return;
		}
		foundLines1 = searchStops(stop1);

		if (foundLines1.stopsDirect.empty() && foundLines1.stopsInverse.empty()) {
			cout << "Invalid stop name.\n";
		}
		else break;
	} while (true);

	do {
		cout << "Insert the second stop name (CTRL-Z to cancel): ";
		getline(cin, stop2);
		if (cin.eof()) {
			cin.clear();
			return;
		}
		foundLines2 = searchStops(stop2);

		if (foundLines2.stopsDirect.empty() && foundLines2.stopsInverse.empty()) {
			cout << "Invalid stop name.\n";
		}
		else break;
	} while (true);

	// MAIN DIRECTION

	for (int i = 0; i < foundLines1.stopsDirect.size(); i++) {
		Route newRoute;
		for (int x = 0; x < foundLines2.stopsDirect.size(); x++) {
			if (foundLines1.stopsDirect.at(i).lineID == foundLines2.stopsDirect.at(x).lineID) {
				if (foundLines1.stopsDirect.at(i).posInLine < foundLines2.stopsDirect.at(x).posInLine) {
					newRoute.lineID = foundLines1.stopsDirect.at(i).lineID;
					newRoute.linePos = foundLines1.stopsDirect.at(i).linePos;
					newRoute.startingPos = foundLines1.stopsDirect.at(i).posInLine;
					newRoute.endingPos = foundLines2.stopsDirect.at(x).posInLine;
					newRoute.time = 0;
					newRoute.direction = 0;
					for (int z = newRoute.startingPos; z < newRoute.endingPos; z++) {
						newRoute.time += lines.at(newRoute.linePos).times.at(z);
					}
					vector<string> stops;
					for (int z = newRoute.startingPos; z <= newRoute.endingPos; z++) {
						stops.push_back(lines.at(newRoute.linePos).stops.at(z));
					}
					newRoute.stops = stops;
					routes.push_back(newRoute);
					break;
				}
			}
		}
	}

	// INVERSE DIRECTION

	for (int i = 0; i < foundLines1.stopsInverse.size(); i++) {
		Route newRoute2;
		for (int x = 0; x < foundLines2.stopsInverse.size(); x++) {
			if (foundLines1.stopsInverse.at(i).lineID == foundLines2.stopsInverse.at(x).lineID) {
				if (foundLines1.stopsInverse.at(i).posInLine > foundLines2.stopsInverse.at(x).posInLine) {
					newRoute2.lineID = foundLines1.stopsInverse.at(i).lineID;
					newRoute2.linePos = foundLines1.stopsInverse.at(i).linePos;
					newRoute2.startingPos = foundLines1.stopsInverse.at(i).posInLine;
					newRoute2.endingPos = foundLines2.stopsInverse.at(x).posInLine;
					newRoute2.time = 0;
					newRoute2.direction = 1;
					for (int z = newRoute2.startingPos - 1; z >= newRoute2.endingPos; z--) {
						newRoute2.time += lines.at(newRoute2.linePos).times.at(z);
					}
					vector<string> stops;
					for (int z = newRoute2.startingPos; z >= newRoute2.endingPos; z--) {
						stops.push_back(lines.at(newRoute2.linePos).stops.at(z));
					}
					newRoute2.stops = stops;
					routes.push_back(newRoute2);
					break;
				}
			}
		}
	}

	if (!routes.empty()) {
		cout << "There are " << routes.size() << " direct routes from " << stop1 << " to " << stop2 << ":\n";
		for (int i = 0; i < routes.size(); i++) {
			cout << "Line " << routes.at(i).lineID;
			cout << " - ";
			if (routes.at(i).direction == 0) cout << "MAIN DIRECTION";
			else cout << "INVERSE DIRECTION";
			cout << " - ETA: ";
			if (routes.at(i).time >= 60) {
				int t = routes.at(i).time;
				int h = 0;
				while (t >= 60) {
					t -= 60;
					h++;
				}
				cout << h << "h " << t << "min";
			}
			else cout << routes.at(i).time << "min";
			cout << " - (";
			for (int x = 0; x < routes.at(i).stops.size(); x++) {
				cout << routes.at(i).stops.at(x);
				if (x != routes.at(i).stops.size() - 1) {
					cout << ", ";
				}
				else {
					cout << ")\n";
				}
			}
		}
	}
	else {
		cout << "There are no direct routes from " << stop1 << " to " << stop2 << ".\n";
	}
}

void driverInfoMenu() {
	int op;

	do {
		system("cls");
		if (!drivers.empty()) printDrivers();
		cout << "\n[DRIVER INFO MENU]\n\n";
		cout << "[1]- View driver designated work. (NOT DEVELOPED)\n";
		cout << "[2]- List available drivers. (NOT DEVELOPED)\n";
		cout << "[3]- Number of drivers for line. (NOT DEVELOPED)\n";
		cout << "[9]- Back.\n\n";

		do {
			cout << "Insert the desired option: ";
			if (cin >> op && ((op >= 1 && op <= 3) || op == 9)) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);


	} while (op != 9);


}

void driverManagementMenu() {
	int op;
	do {
		system("cls");
		printDrivers();
		cout << "\n[DRIVER MANAGEMENT MENU]\n\n";
		cout << "[1]- Choose driver to change.\n";
		cout << "[2]- Add new driver.\n";
		cout << "[9]- Back.\n\n";

		do {
			cout << "Insert the desired option: ";
			if (cin >> op && ((op >= 1 && op <= 2) || op == 9)) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);

		switch (op) {
		case 1:
			changeDriver(chooseDriver());
			break;
		case 2:
			addDriver();
			break;
		}

	} while (op != 9);
}

void fileMenu() {
	int op;
	do {
		system("cls");
		bool validOp = false;
		cout << "[FILE MANAGEMENT MENU]\n\n";
		cout << "[1]- Load lines file ";
		if (lines_path.empty()) {
			cout << "(No file loaded).";
		}
		else {
			cout << "('" << lines_path << "' loaded).";
		}
		cout << endl;
		cout << "[2]- Load drivers file ";
		if (drivers_path.empty()) {
			cout << "(No file loaded).";
		}
		else {
			cout << "('" << drivers_path << "' loaded).";
		}
		cout << endl;
		cout << "[3]- Save lines changes to file ";
		if (linesChanged) {
			cout << "(Changes to be deployed).";
		}
		else {
			cout << "(No changes to be deployed).";
		}
		cout << endl;
		cout << "[4]- Save drivers changes to file ";
		if (driversChanged) {
			cout << "(Changes to be deployed).";
		}
		else {
			cout << "(No changes to be deployed).";
		}
		cout << endl;
		cout << "[5]- Save all changes to files.\n";
		cout << "[9]- Back.\n\n";

		do {
			cout << "Insert the desired option: ";
			if (cin >> op && (op >= 0 && op <= 5) || op == 9) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				validOp = true;
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (!validOp);

		switch (op) {
		case 1:
			loadFile("lines");
			break;
		case 2:
			loadFile("drivers");
			break;
		case 3:
			changeFile("lines");
			break;
		case 4:
			changeFile("drivers");
			break;
		case 5:
			changeFile("lines");
			changeFile("drivers");
			break;
		}

	} while (op != 9);
}

void lineManagementMenu() {
	int lineID, linePos, op;

	do {
		system("cls");
		printSummaryLines();
		cout << "\n[LINE MANAGEMENT MENU]\n\n";
		cout << "[1]- Choose line to change.\n";
		cout << "[2]- Add new line.\n";
		cout << "[9]- Back.\n\n";

		do {
			cout << "Insert the desired option: ";
			if (cin >> op && ((op >= 1 && op <= 2) || op == 9)) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);

		switch (op) {
		case 1:
			changeLine(chooseLine());
			break;
		case 2:
			addLine();
			break;
		}

	} while (op != 9);

}

void detailedInfoMenu() {
	int op;
	int lineID = chooseLine();
	system("cls");
	cout << "[DETAILED LINE INFO]\n\n";
	printDetailedLineInfo(lineID);
	cout << "Press any key to continue...";
	getchar();
}

void lineInfoMenu() {
	int op;
	do {
		system("cls");
		if (!lines.empty()) printSummaryLines();
		cout << "\n[LINES/STOPS INFO MENU]\n\n";
		cout << "[1]- View detailed information about a line.\n";
		cout << "[2]- Find a stop.\n";
		cout << "[3]- Display line schedule.\n";
		cout << "[4]- Display stop schedule.\n";
		cout << "[5]- Find a route.\n";
		cout << "[9]- Back.\n\n";


		do {
			cout << "Insert the desired option: ";
			if (cin >> op && ((op >= 1 && op <= 5) || op == 9)) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);

		switch (op) {
		case 1:
			detailedInfoMenu();
			break;
		case 2:
			searchStops();
			cout << "Press any key to continue...";
			getchar();
			break;
		case 3:
			displayLineSchedule();
			break;
		case 5:
			displayRoute();
			cout << "Press any key to continue...";
			getchar();
			break;
		}

	} while (op != 9);

}

void mainMenu() {
	int op;
	do {
		system("cls");
		cout << "[MAIN MENU]\n\n";
		cout << "[1]- File management.\n";
		cout << "[2]- Line management.\n";
		cout << "[3]- Driver management.\n";
		cout << "[4]- Line/stops information.\n";
		cout << "[5]- Driver information.\n";
		cout << "[0]- Quit.\n\n";

		do {
			cout << "Insert the desired option: ";
			if (cin >> op && op >= 0 && op <= 5) {
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
			else {
				cerr << "Invalid option.\n";
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		} while (true);


		switch (op) {
		case 1:
			fileMenu();
			break;
		case 2:
			lineManagementMenu();
			break;
		case 3:
			driverManagementMenu();
		case 4:
			lineInfoMenu();
			break;
		case 5:
			driverInfoMenu();
			break;
		}

	} while (op != 0);
}

int main() {
	mainMenu();
	return 0;
}
