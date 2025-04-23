#include "pch.h"
#include "CAircraftInfoPlugin.h"
#include "AircraftData.h"
#include <string>
#include <sstream>
#include <exception>

using namespace std;


const char* PLUGIN_NAME			= "Aircraft Info Plugin";
const char* PLUGIN_VERSION		= "1.5";
const char* PLUGIN_AUTHOR		= "Martin Bienert";
const char* PLUGIN_COPYRIGHT	= "Free to be distributed as source code";


/// Constants
const int TAG_ITEM_TYPE_ICAO			= 1001;
const int TAG_ITEM_TYPE_WTC				= 1002;
const int TAG_ITEM_TYPE_MTOW			= 1003;
const int TAG_ITEM_TYPE_WINGSPAN		= 1004;
const int TAG_ITEM_TYPE_ENGINES			= 1005;
const int TAG_ITEM_TYPE_NAME			= 1006;
const int TAG_ITEM_TYPE_AIRCRAFT_TYPE	= 1007;
const int TAG_ITEM_TYPE_RECAT_EU		= 1008;
const int TAG_ITEM_TYPE_FINAL_SPEED		= 1009;
const int TAG_ITEM_TYPE_INITIAL_ROC		= 1010;
const int TAG_ITEM_TYPE_ARC2			= 1011;

const char* COLUMN_NAME_CALLSIGN				= "C/S";
const char* COLUMN_NAME_ICAO					= "ICAO";
const char* COLUMN_NAME_WTC						= "WTC";
const char* COLUMN_NAME_MTOW					= "MTOW";
const char* COLUMN_NAME_WINGSPAN				= "Wingspan";
const char* COLUMN_NAME_ENGINES					= "Engines";
const char* COLUMN_NAME_NAME					= "Name";
const char* COLUMN_NAME_TYPE					= "Type";
const char* COLUMN_NAME_RECAT					= "Recat-EU";
const char* COLUMN_NAME_FINAL_APPROACH_SPEED	= "FAS";
const char* COLUMN_NAME_INITIAL_CLIMB_RATE		= "IROC";
const char* COLUMN_NAME_ARC2					= "ARC2";

const int COLUMN_WIDTH_CALLSIGN				= 10;
const int COLUMN_WIDTH_ICAO					= 6;
const int COLUMN_WIDTH_WTC					= 5;
const int COLUMN_WIDTH_MTOW					= 12;
const int COLUMN_WIDTH_WINGSPAN				= 12;
const int COLUMN_WIDTH_ENGINES				= 17;
const int COLUMN_WIDTH_NAME					= 17;
const int COLUMN_WIDTH_TYPE					= 12;
const int COLUMN_WIDTH_RECAT				= 14;
const int COLUMN_WIDTH_FINAL_APPROACH_SPEED = 7;
const int COLUMN_WIDTH_INITIAL_CLIMB_RATE	= 10;
const int COLUMN_WIDTH_ARC2					= 6;

const bool COLUMN_CENTER_CALLSIGN				= false;
const bool COLUMN_CENTER_ICAO					= false;
const bool COLUMN_CENTER_WTC					= false;
const bool COLUMN_CENTER_MTOW					= false;
const bool COLUMN_CENTER_WINGSPAN				= false;
const bool COLUMN_CENTER_ENGINES				= false;
const bool COLUMN_CENTER_NAME					= false;
const bool COLUMN_CENTER_TYPE					= false;
const bool COLUMN_CENTER_RECAT					= false;
const bool COLUMN_CENTER_FINAL_APPROACH_SPEED	= false;
const bool COLUMN_CENTER_INITIAL_CLIMB_RATE		= false;
const bool COLUMN_CENTER_ARC2					= false;


void CAircraftInfoPlugin::sendMessage(string message) {
	DisplayUserMessage("AIP", "", message.c_str(), true, true, false, true, false);
}


CAircraftInfoPlugin* CAircraftInfoPlugin::instance = nullptr;

CAircraftInfoPlugin::CAircraftInfoPlugin(void) : EuroScopePlugIn::CPlugIn(
	EuroScopePlugIn::COMPATIBILITY_CODE,
	PLUGIN_NAME,
	PLUGIN_VERSION,
	PLUGIN_AUTHOR,
	PLUGIN_COPYRIGHT
) {
	instance = this;

	sendMessage(string("Loading " + string(PLUGIN_NAME) + " (version " + string(PLUGIN_VERSION) + ")..."));

	// Register Tags
	RegisterTagItemType("Aircraft ICAO",					TAG_ITEM_TYPE_ICAO);
	RegisterTagItemType("Aircraft WTC",						TAG_ITEM_TYPE_WTC);
	RegisterTagItemType("Aircraft MTOW",					TAG_ITEM_TYPE_MTOW);
	RegisterTagItemType("Aircraft Wingspan",				TAG_ITEM_TYPE_WINGSPAN);
	RegisterTagItemType("Aircraft Engines",					TAG_ITEM_TYPE_ENGINES);
	RegisterTagItemType("Aircraft Name",					TAG_ITEM_TYPE_NAME);
	RegisterTagItemType("Aircraft Type",					TAG_ITEM_TYPE_AIRCRAFT_TYPE);
	RegisterTagItemType("Aircraft RECAT-EU",				TAG_ITEM_TYPE_RECAT_EU);
	RegisterTagItemType("Aircraft Final Approach Speed",	TAG_ITEM_TYPE_FINAL_SPEED);
	RegisterTagItemType("Aircraft Initial Rate of Climb",	TAG_ITEM_TYPE_INITIAL_ROC);
	RegisterTagItemType("Aircraft ARC2",					TAG_ITEM_TYPE_ARC2);

	// Set up list
	m_aircraftInfoList = RegisterFpList("Aircraft Info");
	if (m_aircraftInfoList.GetColumnNumber() != 12) {
		m_aircraftInfoList.DeleteAllColumns();
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_CALLSIGN,	COLUMN_WIDTH_CALLSIGN, COLUMN_CENTER_CALLSIGN,
			NULL,					EuroScopePlugIn::TAG_ITEM_TYPE_CALLSIGN,
			NULL,					EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,					EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_ICAO,	COLUMN_WIDTH_ICAO, COLUMN_CENTER_ICAO,
			PLUGIN_NAME,		TAG_ITEM_TYPE_ICAO,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_WTC,	COLUMN_WIDTH_WTC, COLUMN_CENTER_WTC,
			PLUGIN_NAME,		TAG_ITEM_TYPE_WTC,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_RECAT, COLUMN_WIDTH_RECAT, COLUMN_CENTER_RECAT,
			PLUGIN_NAME, TAG_ITEM_TYPE_RECAT_EU,
			NULL, EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL, EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_ARC2, COLUMN_WIDTH_ARC2, COLUMN_CENTER_ARC2,
			PLUGIN_NAME, TAG_ITEM_TYPE_ARC2,
			NULL, EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL, EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_MTOW,	COLUMN_WIDTH_MTOW, COLUMN_CENTER_MTOW,
			PLUGIN_NAME,		TAG_ITEM_TYPE_MTOW,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_WINGSPAN,	COLUMN_WIDTH_WINGSPAN, COLUMN_CENTER_WINGSPAN,
			PLUGIN_NAME,			TAG_ITEM_TYPE_WINGSPAN,
			NULL,					EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,					EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_TYPE,	COLUMN_WIDTH_TYPE, COLUMN_CENTER_TYPE,
			PLUGIN_NAME,		TAG_ITEM_TYPE_AIRCRAFT_TYPE,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_ENGINES,	COLUMN_WIDTH_ENGINES, COLUMN_CENTER_ENGINES,
			PLUGIN_NAME,			TAG_ITEM_TYPE_ENGINES,
			NULL,					EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,					EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_FINAL_APPROACH_SPEED,	COLUMN_WIDTH_FINAL_APPROACH_SPEED, COLUMN_CENTER_FINAL_APPROACH_SPEED,
			PLUGIN_NAME,						TAG_ITEM_TYPE_FINAL_SPEED,
			NULL,								EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,								EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_INITIAL_CLIMB_RATE, COLUMN_WIDTH_CALLSIGN, COLUMN_CENTER_INITIAL_CLIMB_RATE,
			PLUGIN_NAME,					TAG_ITEM_TYPE_INITIAL_ROC,
			NULL,							EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,							EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
		m_aircraftInfoList.AddColumnDefinition(
			COLUMN_NAME_NAME,	COLUMN_WIDTH_NAME, COLUMN_CENTER_NAME,
			PLUGIN_NAME,		TAG_ITEM_TYPE_NAME,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO,
			NULL,				EuroScopePlugIn::TAG_ITEM_FUNCTION_NO
		);
	}

	m_currentFlightPlan = new EuroScopePlugIn::CFlightPlan();
	m_currentCallsign = "";

	sendMessage("Loaded version " + string(PLUGIN_VERSION));
}

CAircraftInfoPlugin::~CAircraftInfoPlugin(void) {
	sendMessage(string("Unloading " + string(PLUGIN_NAME) + " (version " + string(PLUGIN_VERSION) + ")..."));
	sendMessage("Bye!");
}

string CAircraftInfoPlugin::getEngines(EuroScopePlugIn::CFlightPlanData fpd) {
	char engineType = fpd.GetEngineType();
	const char* engineTypeString;
	switch (engineType) {
	case 'P':
		engineTypeString = "Piston";
		break;
	case 'T':
		engineTypeString = "Turboprop";
		break;
	case 'J':
		engineTypeString = "Jet";
		break;
	case 'E':
		engineTypeString = "Electric";
		break;
	default:
		engineTypeString = "?";
		break;
	}

	ostringstream builder;
	builder << fpd.GetEngineNumber() << "x" << engineTypeString;


	return builder.str();
}

string CAircraftInfoPlugin::getName(EuroScopePlugIn::CFlightPlanData fpd) {
	return string(fpd.GetManufacturerType());
}


void CAircraftInfoPlugin::OnGetTagItem(
	EuroScopePlugIn::CFlightPlan FlightPlan,
	EuroScopePlugIn::CRadarTarget RadarTarget,
	int ItemCode,
	int TagData,
	char sItemString[16],
	int* pColorCode,
	COLORREF* pRGB,
	double* pFontSize
) {
	if (!FlightPlan.IsValid()) return;

	EuroScopePlugIn::CFlightPlanData fpd = FlightPlan.GetFlightPlanData();
	string icao = fpd.GetAircraftFPType();

	// Try to lookup aircraft data from ICAO
	map<string, string> info;
	try {
		info = AircraftData::lookupIcao(icao);
	} catch (const exception& e) {
		sendMessage("Error while trying to get aircraft data: " + string(e.what()));

		sItemString = (char *)"error";
		return;
	} catch (...) {
		sendMessage("Unknown error while trying to get aircraft data");

		sItemString = (char *)"error";
		return;
	}
	

	switch (ItemCode) {
	case TAG_ITEM_TYPE_ICAO:
		strncpy(sItemString, fpd.GetAircraftFPType(), 15);
		break;
	case TAG_ITEM_TYPE_WTC:
		snprintf(sItemString, 2, "%c", fpd.GetAircraftWtc());
		break;
	case TAG_ITEM_TYPE_RECAT_EU:
		strncpy(sItemString, AircraftData::getRecat(info).c_str(), 15);
		break;
	case TAG_ITEM_TYPE_MTOW:
		snprintf(sItemString, 15, "%st", AircraftData::getMtow(info).c_str());
		break;
	case TAG_ITEM_TYPE_ARC2:
		strncpy(sItemString, AircraftData::getARC2(info).c_str(), 15);
		break;
	case TAG_ITEM_TYPE_WINGSPAN:
		snprintf(sItemString, 15, "%sm", AircraftData::getWingspan(info).c_str());
		break;
	case TAG_ITEM_TYPE_AIRCRAFT_TYPE:
		strncpy(sItemString, AircraftData::getType(info).c_str(), 15);
		break;
	case TAG_ITEM_TYPE_ENGINES:
		strncpy(sItemString, getEngines(fpd).c_str(), 15);
		break;
	case TAG_ITEM_TYPE_FINAL_SPEED:
		snprintf(sItemString, 15, "%skt", AircraftData::getFinalSpeed(info).c_str());
		break;
	case TAG_ITEM_TYPE_INITIAL_ROC:
		snprintf(sItemString, 15, "%sfpm", AircraftData::getInitialRoc(info).c_str());
		break;
	case TAG_ITEM_TYPE_NAME:
		strncpy(sItemString, AircraftData::getShortName(info).c_str(), 15);
		break;
	default:
		sItemString = (char*)"";
		break;
	}

	// Make sure the text is always terminated
	sItemString[15] = '\0';
}

bool CAircraftInfoPlugin::OnCompileCommand(const char* sCommandLine) {
	if (strncmp(sCommandLine, ".aip", 4) != 0) return false;

	if (strlen(sCommandLine) == 4) {
		// Just the .aip
		sendMessage("Aircraft Info Plugin is up and running");
		sendMessage("Version:  " + string(PLUGIN_VERSION));
		sendMessage("Database: " + AircraftData::getLoadedDatabase());
		return true;
	}

	if (strlen(sCommandLine) < 6) return false;

	string cmd = sCommandLine + 5;
	try {
		transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
	} catch (...) {
		return false;
	}

	if (strcmp(cmd.c_str(), "HELP") == 0) {
		sendMessage("The following subcommands are available:");
		sendMessage("HELP          - Show this info");
		sendMessage("SHOW          - Show the aircraft info list, if hidden");
		sendMessage("HIDE          - Hide the aircraft info list, if shown");
		sendMessage("RELOAD	        - Reload the aircraft database file");
		sendMessage("LOOKUP [ICAO] - Look up and show the available information for aircraft with the given ICAO code");

		return true;
	}
	
	if (strcmp(cmd.c_str(), "SHOW") == 0) {
		m_aircraftInfoList.ShowFpList(true);
		return true;
	}

	if (strcmp(cmd.c_str(), "HIDE") == 0) {
		m_aircraftInfoList.ShowFpList(false);
		return true;
	}

	if (strcmp(cmd.c_str(), "RELOAD") == 0) {
		sendMessage("Reloading database...");
		if (AircraftData::reload()) {
			sendMessage("Success!");
		}

		return true;
	}

	if (strncmp(cmd.c_str(), "LOOKUP", 6) == 0) {
		if (strlen(cmd.c_str()) < 8) {
			sendMessage("lookup needs an aircraft ICAO");
			return true;
		}

		string icao = cmd.substr(7);
		try {
			transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
		} catch (...) {
			return false;
		}

		auto info = AircraftData::lookupIcao(icao);
		string gotIcao = AircraftData::getIcao(info);

		if (icao == gotIcao) {
			sendMessage("--------------------------");
			sendMessage("ICAO:     " + gotIcao);
			sendMessage("WTC:      " + AircraftData::getWtc(info));
			sendMessage("Recat-EU: " + AircraftData::getRecat(info));
			sendMessage("ARC2:     " + AircraftData::getARC2(info));
			sendMessage("MTOW:     " + AircraftData::getMtow(info));
			sendMessage("Wingspan: " + AircraftData::getWingspan(info));
			sendMessage("Type:     " + AircraftData::getType(info));
			sendMessage("FAS:      " + AircraftData::getFinalSpeed(info));
			sendMessage("IROC:     " + AircraftData::getInitialRoc(info));
			sendMessage("Name:     " + AircraftData::getShortName(info));
			sendMessage("--------------------------");
		} else {
			sendMessage("Could not find entry for ICAO '" + icao + "'");
		}
		
		return true;
	}

	return false;
}

void CAircraftInfoPlugin::OnTimer(int Counter) {
	EuroScopePlugIn::CRadarTarget selectedTarget = RadarTargetSelectASEL();
	const char* selectedCallsign = selectedTarget.GetCallsign();

	if (m_currentCallsign == selectedCallsign) return;

	m_aircraftInfoList.RemoveFpFromTheList(*m_currentFlightPlan);
	m_currentFlightPlan = new EuroScopePlugIn::CFlightPlan(selectedTarget.GetCorrelatedFlightPlan());
	m_currentCallsign = selectedCallsign;
	m_aircraftInfoList.AddFpToTheList(*m_currentFlightPlan);
}


