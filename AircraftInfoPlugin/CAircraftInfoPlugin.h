#pragma once
#include "EuroScopePlugIn.h"
#include <string>

using namespace std;


class CAircraftInfoPlugin : public EuroScopePlugIn::CPlugIn {
public:
    static CAircraftInfoPlugin* instance;

	CAircraftInfoPlugin();
	virtual ~CAircraftInfoPlugin(void);

    virtual void OnGetTagItem(
        EuroScopePlugIn::CFlightPlan FlightPlan,
        EuroScopePlugIn::CRadarTarget RadarTarget,
        int ItemCode,
        int TagData,
        char sItemString[16],
        int* pColorCode,
        COLORREF* pRGB,
        double* pFontSize
    );
    virtual bool OnCompileCommand(const char* sCommandLine);

    virtual void OnTimer(int Counter);

    void sendMessage(string message);
protected:
    EuroScopePlugIn::CFlightPlanList    m_aircraftInfoList;
    EuroScopePlugIn::CFlightPlan*       m_currentFlightPlan;
    CString m_currentCallsign;

    string getMTOW(EuroScopePlugIn::CFlightPlan fp);
    string getWingspan(EuroScopePlugIn::CFlightPlan fp);
    string getEngines(EuroScopePlugIn::CFlightPlanData fpd);
    string getName(EuroScopePlugIn::CFlightPlanData fpd);
};

