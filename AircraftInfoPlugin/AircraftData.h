#pragma once
#include <map>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;

static class AircraftData {
public:
	static map<string, string> lookupIcao(string icao);
	static vector<map<string, string>> lookupAny(string lookupText);

	// Column definitions
	static string cIcao;
	static string cManufacturer;
	static string cName;
	static string cShortName;
	static string cType;
	static string cWtc;
	static string cMtow;
	static string cWingspan;
	static string cRecat;
	static string cFinalSpeed;
	static string cInitialRoc;

	static string getIcao(map<string, string> info);
	static string getManufacturer(map<string, string> info);
	static string getName(map<string, string> info);
	static string getShortName(map<string, string> info);
	static string getType(map<string, string> info);
	static string getWtc(map<string, string> info);
	static string getMtow(map<string, string> info);
	static string getWingspan(map<string, string> info);
	static string getRecat(map<string, string> info);
	static string getFinalSpeed(map<string, string> info);
	static string getInitialRoc(map<string, string> info);
	static string getARC2(map<string, string> info);

	static string getLoadedDatabase();
	static bool reload();
protected:
	static std::filesystem::path filePath();
	static string filename;
	static char separator;
	static bool hasHeader;
	static int columnCount;

	static map<string, map<string, string>> data;
	static bool hasData;
	static bool loadData();

	static map<string, string> invalidDefault();
	static string defaultString;
private:
	static vector<string> readRow(string line);
	static void mapRow(vector<string> line, map<string, map<string, string>>* data, int lineNumber);
};


