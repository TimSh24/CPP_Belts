#include "database.h"

void Database::Add(const Date& key, const string& event) {
        if (dates_events_set[key].insert(event).second) {
            dates_events[key].push_back(event);
        }
    }
void Database::Print(ostream& os) const {
        for (const auto &item: dates_events) {
            for (auto &j: item.second) {
                os << item.first << " " << j << endl;
            }
        }
    }
int Database::RemoveIf(function<bool(const Date&, const string&)> predicate) {
    int counter{0};
    for (auto it = dates_events.begin(); it != dates_events.end();) {
        auto border = stable_partition(it->second.begin(), it->second.end(), [it, predicate] (string& event) {
            return predicate(it->first, event);});
        auto set_iterator = dates_events_set.find(it->first);
        for (auto set_iterator2 = it->second.begin(); set_iterator2 <= border; set_iterator2++) {
            set_iterator->second.erase(*set_iterator2);
        }
        it->second.erase(it->second.begin(), border);
        counter += border - it->second.begin();
        if (it->second.empty()) {
            dates_events_set.erase(it->first);
            it = dates_events.erase(it);
        }
        else {
            ++it;
        }
    }
    return counter;
}
vector<pair<Date, string>> Database::FindIf(function<bool(const Date&, const string&)> predicate) const {
    vector<pair<Date, string>> entries;
    for (auto it = dates_events.begin(); it != dates_events.end(); ++it) {
        for (auto item = it->second.begin(); item != it->second.end(); ++item) {
            if (predicate(it->first, *item)) {
                entries.push_back({it->first, *item});
            }
        }
    }
    return entries;
}
pair <Date,string> Database::Last(Date input_date) const {
    auto first_not_less = dates_events.lower_bound(input_date);
    if (first_not_less->first == input_date) {
        return {first_not_less->first, *first_not_less->second.rbegin()};
    }
    else if (first_not_less == dates_events.begin()) {
        throw invalid_argument("");
    }
    else {
        auto last_less = prev(first_not_less);
        return {last_less->first, *last_less->second.rbegin()};
    }
}
