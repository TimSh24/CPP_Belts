#pragma once

#include<vector>

vector<pair<int,int>> Head1(Priority_queue &q, int top) {
    vector<pair<int,int>> answer;
    auto q_size = q.size();
    for (int i = 0; i < min<int>(top,q_size); i++) {
        answer.push_back(q.ExtractMax());
    }
    return answer;
}