#pragma once
#include "EuroScopePlugIn.h"
#include <sstream>
#include <iostream>
#include <string>
#include "Constant.hpp"
#include <fstream>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "json/json.h"
#define CURL_STATICLIB
#include "curl/curl.h"

#define MY_PLUGIN_NAME      "CDM Plugin"
#define MY_PLUGIN_VERSION   "1.1.6"
#define MY_PLUGIN_DEVELOPER "Roger Puig"
#define MY_PLUGIN_COPYRIGHT "GPL v3"
#define MY_PLUGIN_VIEW_AVISO  "Euroscope CDM"

#define PLUGIN_WELCOME_MESSAGE	"Welcome to the CDM Plugin for Euroscope!"

using namespace std;
using namespace boost;
using namespace rapidjson;
using namespace EuroScopePlugIn;

class CDM :
	public EuroScopePlugIn::CPlugIn
{
public:
	CDM();
	virtual ~CDM();

	//Define OnGetTagItem function
	virtual void OnGetTagItem(CFlightPlan FlightPlan,
		CRadarTarget RadarTarget,
		int ItemCode,
		int TagData,
		char sItemString[16],
		int* pColorCode,
		COLORREF* pRGB,
		double* pFontSize);

	bool getRate();

	int rateForRunway(string airport, string depRwy, bool lvoActive);

	bool refreshTimes(CFlightPlan FlightPlan, string callsign, string EOBT, string TSATfinal, string TTOTFinal, string origin, int taxiTime, string remarks, string depRwy, double rateHour, bool hasCTOT, int ctotPos, int pos, bool aircraftFind);

	string EobtPlusTime(string EOBT, int time);

	string getTaxiTime(double lat, double lon, string origin, string depRwy);

	bool FindPoint(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double pointx, double pointy);

	int inPoly(int nvert, double* vertx, double* verty, double testx, double testy);

	string formatTime(string timeString);

	string calculateTime(string timeString, double minsToAdd);

	bool expiredCtot(string line);

	string calculateLessTime(string timeString, double minsToAdd);

	bool checkIsNumber(string str);

	string getCidByCallsign(string callsign);

	int GetVersion();

	bool getTaxiZonesFromUrl(string url);

	int GetdifferenceTime(string hour1, string min1, string hour2, string min2);

	string unixTimeToHumanReadable(long int seconds);

	template <typename Out>
	void split(const string& s, char delim, Out result) {
		istringstream iss(s);
		string item;
		while (getline(iss, item, delim)) {
			*result++ = item;
		}
	}

	vector<string> split(const string& s, char delim) {
		vector<string> elems;
		split(s, delim, back_inserter(elems));
		return elems;
	}

	string destArrayContains(const Value& a, string s) {
		for (SizeType i = 0; i < a.Size(); i++) {
			string test = a[i].GetString();
			SizeType x = static_cast<rapidjson::SizeType>(s.rfind(test, 0));
			if (s.rfind(a[i].GetString(), 0) != -1)
				return a[i].GetString();
		}
		return "";
	}

	bool arrayContains(const Value& a, string s) {
		for (SizeType i = 0; i < a.Size(); i++) {
			if (a[i].GetString() == s)
				return true;
		}
		return false;
	}

	bool arrayContains(const Value& a, char s) {
		for (SizeType i = 0; i < a.Size(); i++) {
			if (a[i].GetString()[0] == s)
				return true;
		}
		return false;
	}

	string arrayToString(const Value& a, char delimiter) {
		string s;
		for (SizeType i = 0; i < a.Size(); i++) {
			s += a[i].GetString()[0];
			if (i != a.Size() - 1)
				s += delimiter;
		}
		return s;
	}
	bool routeContains(string s, const Value& a) {
		for (SizeType i = 0; i < a.Size(); i++) {
			bool dd = contains(s, a[i].GetString());
			if (contains(s, a[i].GetString()))
				return true;
		}
		return false;
	}

	virtual void OnFlightPlanDisconnect(CFlightPlan FlightPlan);

	virtual void debugMessage(string type, string message);

	virtual void sendMessage(string type, string message);

	virtual void sendMessage(string message);

	void OnFunctionCall(int FunctionId, const char* ItemString, POINT Pt, RECT Area);

	string getFromXml(string xpath);

	bool addCtotToMainList(string lineValue);

	bool OnCompileCommand(const char* sCommandLine);

	virtual void OnTimer(int Count);

protected:
	Document config;
	map<string, rapidjson::SizeType> airports;
};

