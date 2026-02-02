#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>
#include <set>
#include "date.h"
#include <functional>

using namespace std;

class Database {
public:
        void Add(const Date& key, const string& event);
        void Print(ostream& os) const;
        int RemoveIf(function<bool(const Date&, const string&)> predicate);
        vector<pair<Date, string>> FindIf(function<bool(const Date&, const string&)> predicate) const;
        pair <Date,string> Last(Date input_date) const;
private:
        map<Date, set<string>> dates_events_set;
        map<Date, vector<string>> dates_events;
};
