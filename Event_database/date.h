#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct Date {
    Date(const int& y = 0, const int& m = 0, const int& d = 0) : year(y), month(m), day(d) {};
    int year;
    int month;
    int day;
};

ostream& operator<<(ostream& stream, const Date& parsed_date);

bool operator<(const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);
bool operator!=(const Date& lhs, const Date& rhs);
bool operator>(const Date& lhs, const Date& rhs);
bool operator>=(const Date& lhs, const Date& rhs);

Date ParseDate(istream& input_stream);
