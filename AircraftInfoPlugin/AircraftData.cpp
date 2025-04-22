#include "pch.h"
#include "AircraftData.h"
#include "CAircraftInfoPlugin.h"
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


/* ------------------- */
/* Static declarations */
/* ------------------- */

map<string, map<string, string>> AircraftData::data = map<string, map<string, string>>();
bool AircraftData::hasData = false;
string AircraftData::filename = "aircraft_data.csv";
char AircraftData::separator = ';';
bool AircraftData::hasHeader = true;
int AircraftData::columnCount = 11;
string AircraftData::defaultString = "-";

// Column definitions
string AircraftData::cIcao = "icao";
string AircraftData::cManufacturer = "manufacturer";
string AircraftData::cName = "name";
string AircraftData::cShortName = "short-name";
string AircraftData::cType = "type";
string AircraftData::cWtc = "wtc";
string AircraftData::cMtow = "mtow";
string AircraftData::cWingspan = "wingspan";
string AircraftData::cRecat = "recat-eu";
string AircraftData::cFinalSpeed = "final-speed";
string AircraftData::cInitialRoc = "initial-roc";


fs::path AircraftData::filePath() {
	char path[MAX_PATH];
	HMODULE hm = NULL;

	if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCWSTR)&filePath, &hm) == 0) {
		return filename;
	}
	if (GetModuleFileNameA(hm, path, MAX_PATH) == 0) {
		return filename;
	}

	return fs::path(path).parent_path() / filename;
}


bool AircraftData::loadData() {
	auto path = filePath();
	if (!fs::exists(path)) {
		CAircraftInfoPlugin::instance->sendMessage("Could not find aircraft database at expected path " + path.string() + ".");
		CAircraftInfoPlugin::instance->sendMessage("Continuing without aircraft database. Information will be incomplete.");

		data = map<string, map<string, string>>();
		return true;
	}

	CAircraftInfoPlugin::instance->sendMessage("Loading aircraft database from path " + path.string() + ".");
	ifstream file(path);
	
	string line;
	if (!getline(file, line)) {
		// No first row -> No data
		CAircraftInfoPlugin::instance->sendMessage("Error: No data in database.");
		CAircraftInfoPlugin::instance->sendMessage("Continuing without aircraft database. Information will be incomplete.");

		data = map<string, map<string, string>>();
		return true;
	}

	vector<string> firstLine = readRow(line);
	int rowSize = firstLine.size();

	if (rowSize != columnCount) {
		// Expected different number of columns
		CAircraftInfoPlugin::instance->sendMessage("Error: Header had unexpected number of columns.");
		CAircraftInfoPlugin::instance->sendMessage("Continuing without aircraft database. Information will be incomplete.");

		data = map<string, map<string, string>>();
		return false;
	}

	data = map<string, map<string, string>>();
	int lineNumber = 1;

	if (hasHeader) {
		// Interpret first line as header
		cIcao			= firstLine[0];
		cManufacturer	= firstLine[1];
		cName			= firstLine[2];
		cShortName		= firstLine[3];
		cType			= firstLine[4];
		cWtc			= firstLine[5];
		cMtow			= firstLine[6];
		cWingspan		= firstLine[7];
		cRecat			= firstLine[8];
		cFinalSpeed		= firstLine[9];
		cInitialRoc		= firstLine[10];
	} else {
		mapRow(firstLine, &data, lineNumber);
	}

	while (getline(file, line)) {
		lineNumber++;
		vector<string> currentLine = readRow(line);
		mapRow(currentLine, &data, lineNumber);
	}

	return true;
}

bool AircraftData::reload() {
	return AircraftData::loadData();
}

vector<string> AircraftData::readRow(string line) {
	vector<string> row;

	stringstream lineStream(line);
	string cell;
	while (getline(lineStream, cell, separator)) {
		row.push_back(cell);
	}

	return row;
}

void AircraftData::mapRow(vector<string> row, map<string, map<string, string>>* data, int lineNumber) {
	if (row.size() != columnCount) {
		CAircraftInfoPlugin::instance->sendMessage("Warning: Row " + to_string(lineNumber) + " had unexpected number of columns. Skipping this entry.");
		return;
	}

	string icao = row[0];

	map<string, string> rowData;
	rowData.insert_or_assign(cIcao, icao);
	rowData.insert_or_assign(cManufacturer, row[1]);
	rowData.insert_or_assign(cName, row[2]);
	rowData.insert_or_assign(cShortName, row[3]);
	rowData.insert_or_assign(cType, row[4]);
	rowData.insert_or_assign(cWtc, row[5]);
	rowData.insert_or_assign(cMtow, row[6]);
	rowData.insert_or_assign(cWingspan, row[7]);
	rowData.insert_or_assign(cRecat, row[8]);
	rowData.insert_or_assign(cFinalSpeed, row[9]);
	rowData.insert_or_assign(cInitialRoc, row[10]);

	data->insert_or_assign(icao, rowData);
}

map<string, string> AircraftData::lookupIcao(string icao) {
	if (!hasData) {
		hasData = loadData();

		if (!hasData) return invalidDefault();
	}

	auto iterator = data.find(icao);
	if (iterator != data.end()) {
		return iterator->second;
	}

	return invalidDefault();
}

map<string, string> AircraftData::invalidDefault() {
	map<string, string> def;

	def.insert_or_assign(cIcao, defaultString);
	def.insert_or_assign(cManufacturer, defaultString);
	def.insert_or_assign(cName, defaultString);
	def.insert_or_assign(cShortName, defaultString);
	def.insert_or_assign(cType, defaultString);
	def.insert_or_assign(cWtc, defaultString);
	def.insert_or_assign(cMtow, defaultString);
	def.insert_or_assign(cWingspan, defaultString);
	def.insert_or_assign(cRecat, defaultString);
	def.insert_or_assign(cFinalSpeed, defaultString);
	def.insert_or_assign(cInitialRoc, defaultString);

	return def;
}

string AircraftData::getIcao(map<string, string> info) {
	return info[cIcao];
}

string AircraftData::getManufacturer(map<string, string> info) {
	return info[cManufacturer];
}

string AircraftData::getName(map<string, string> info) {
	return info[cName];
}

string AircraftData::getShortName(map<string, string> info) {
	return info[cShortName];
}

string AircraftData::getType(map<string, string> info) {
	return info[cType];
}

string AircraftData::getWtc(map<string, string> info) {
	return info[cWtc];
}

string AircraftData::getMtow(map<string, string> info) {
	return info[cMtow];
}

string AircraftData::getWingspan(map<string, string> info) {
	return info[cWingspan];
}

string AircraftData::getRecat(map<string, string> info) {
	return info[cRecat];
}

string AircraftData::getFinalSpeed(map<string, string> info) {
	return info[cFinalSpeed];
}

string AircraftData::getInitialRoc(map<string, string> info) {
	return info[cInitialRoc];
}

string AircraftData::getLoadedDatabase() {
	if (hasData) return filePath().string();
	return "not loaded";
}

