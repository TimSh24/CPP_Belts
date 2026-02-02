#include "date.h"

ostream& operator<<(ostream& stream, const Date& parsed_date) {
    stream << setfill('0');
    stream << setw(4) << parsed_date.year << '-' << setw(2) << parsed_date.month <<
           '-' << setw(2) << parsed_date.day;
    return stream;
}

bool operator<(const Date& lhs, const Date& rhs) {
    return tie(lhs.year, lhs.month, lhs.day) <
           tie(rhs.year, rhs.month, rhs.day);
}
bool operator<=(const Date& lhs, const Date& rhs) {
    return tie(lhs.year, lhs.month, lhs.day) <=
           tie(rhs.year, rhs.month, rhs.day);
}
bool operator==(const Date& lhs, const Date& rhs) {
    return tie(lhs.year, lhs.month, lhs.day) ==
           tie(rhs.year, rhs.month, rhs.day);
}
bool operator!=(const Date& lhs, const Date& rhs) {
    return tie(lhs.year, lhs.month, lhs.day) !=
           tie(rhs.year, rhs.month, rhs.day);
}
bool operator>(const Date& lhs, const Date& rhs) {
    return tie(lhs.year, lhs.month, lhs.day) >
           tie(rhs.year, rhs.month, rhs.day);
}
bool operator>=(const Date& lhs, const Date& rhs) {
    return tie(lhs.year, lhs.month, lhs.day) >=
           tie(rhs.year, rhs.month, rhs.day);
}

Date ParseDate(istream& input_stream) {
    string date_str;
    input_stream >> date_str;
    stringstream is(date_str);
    Date date;
    if (!(is >> date.year) or is.peek() != '-') {
        stringstream err;
        err << "Wrong date format: " << date_str << endl;
        throw runtime_error(err.str());
    }
    is.ignore(1);
    if (!(is >> date.month) or is.peek() != '-') {
        stringstream err;
        err << "Wrong date format: " << date_str << endl;
        throw runtime_error(err.str());
    }
    is.ignore(1);
    if (!(is >> date.day) or !is.eof()) {
        stringstream err;
        err << "Wrong date format: " << date_str << endl;
        throw runtime_error(err.str());
    }

    if (date.month < 1 or date.month > 12) {
        stringstream err;
        err << "Month value is invalid: " << date.month << endl;
        throw runtime_error(err.str());
    }

    if (date.day < 1 or date.day > 31) {
        stringstream err;
        err << "Day value is invalid: " << date.day << endl;
        throw runtime_error(err.str());
    }
    return date;
}
