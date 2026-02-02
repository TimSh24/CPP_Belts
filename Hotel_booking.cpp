#include <iostream>
#include <map>
#include <queue>
#include <string>

using namespace std;

struct booking_item {
    int64_t time_;
    int client_id_;
    int room_count_;
    booking_item(int64_t time,  int client_id, int room_count):
        time_(time), client_id_(client_id), room_count_(room_count) {}
};

class booking_hotels {
public:
    void Book(int64_t time, string hotel_name, int client_id, int room_count) {
        booking_item b(time, client_id, room_count);
        hotels[hotel_name].push(b);
        if (clients_duplicates[{hotel_name, client_id}] == 0) {
            client_count[hotel_name]++;
        }
        clients_duplicates[{hotel_name, client_id}]++;
        rooms_occupied[hotel_name] += room_count;
        last_booking_time = time;
        Clients_and_rooms_edit(hotel_name);

    }
    int Clients_last_day(string hotel_name) {
        if (client_count.count(hotel_name)) {
            Clients_and_rooms_edit(hotel_name);
            return client_count[hotel_name];
        }
        else {
            return 0;
        }
    };
    int Rooms_last_day(string hotel_name) {
        if (rooms_occupied.count(hotel_name)) {
            Clients_and_rooms_edit(hotel_name);
            return rooms_occupied[hotel_name];
        }
        else {
            return 0;
        }
    };

private:
    int64_t last_booking_time = 0;
    map<string,queue<booking_item>> hotels;
    map<pair<string,int>,int> clients_duplicates;
    map<string,int> rooms_occupied;
    map<string,int> client_count;

    void Clients_and_rooms_edit(string hotel_name) {
        while (!hotels[hotel_name].empty() && last_booking_time - hotels[hotel_name].front().time_ >= 86400) {
            auto t = hotels[hotel_name].front();
            clients_duplicates[{hotel_name, t.client_id_}]--;
            rooms_occupied[hotel_name]-= t.room_count_;
            if (clients_duplicates[{hotel_name, t.client_id_}] == 0) {
                client_count[hotel_name]--;
            }
            hotels[hotel_name].pop();
        }
    }
};

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    booking_hotels booking;

    int query_count;
    cin >> query_count;
    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        if (query_type == "BOOK") {
        int64_t time;
        cin >> time;
        string hotel_name;
        cin >> hotel_name;
        int client_id;
        cin >> client_id;
        int room_count;
        cin >> room_count;
        booking.Book(time, hotel_name, client_id, room_count);
        }
        else if (query_type == "CLIENTS") {
            string hotel_name;
            cin >> hotel_name;
            cout << booking.Clients_last_day(hotel_name) << "\n";
        }
        else if (query_type == "ROOMS") {
            string hotel_name;
            cin >> hotel_name;
            cout << booking.Rooms_last_day(hotel_name) << "\n";
        }
    }
    return 0;
}
