#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Priority_queue {
public:
    int Parent(int i) {
        return (i - 1) / 2;
    }
    int Lch(int i) {
        return 2 * i + 1;
    }
    int Rch(int i) {
        return 2 * i + 2;
    }
    void SiftUp(int i) {
        while (i > 0 && make_pair(H[Parent(i)].second, -H[Parent(i)].first) <
                                                        make_pair(H[i].second,-H[i].first)) {
            swap(H[Parent(i)], H[i]);
            i = Parent(i);
        }
    }
    void SiftDown(int i) {
        int maxindex = i;
        int l = Lch(i);
        if (l < H.size() && make_pair(H[l].second,-H[l].first) >
                                                    make_pair(H[maxindex].second,-H[maxindex].first)) {
            maxindex = l;
        }
        int r = Rch(i);
        if (r < H.size() && make_pair(H[r].second,-H[r].first) >
                            make_pair(H[maxindex].second,-H[maxindex].first)) {
            maxindex = r;
        }
        if (i != maxindex) {
            swap(H[i], H[maxindex]);
            SiftDown(maxindex);
        }

    }
    void Insert(const pair<int,int> &p) {
        H.push_back(move(p));
        SiftUp(H.size()-1);
    }
    pair<int,int> ExtractMax() {
        auto result = H[0];
        H[0] = H[H.size()-1];
        H.pop_back();
        SiftDown(0);
        return result;
    }
    void Remove(int i) {
        H[i].second = std::numeric_limits<int>::infinity();
        SiftUp(i);
        ExtractMax();
    }
    void Print() {
        for (auto i : H) {
            cout << i.first << ' ' << i.second << endl;
        }
        cout << endl;
    }
    size_t size() {
        return H.size();
    }
private:
    //vector<int> H;
    vector<pair<int,int>> H;
};
