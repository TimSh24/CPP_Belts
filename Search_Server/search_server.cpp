#include "search_server.h"
#include "iterator_range.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include "Head1.h"
#include "parse.h"

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(
        istream& query_input, ostream& search_results_output
) {
    vector<pair<size_t, size_t>> docid_count(index.doc_size(),{0,0});
    for (string current_query; getline(query_input, current_query); ) {
        const auto words = SplitIntoWords(current_query);
        for (const auto& word : words) {
            for (auto [docid,hitcount] : index.Lookup(word)) {
                docid_count[docid].first= docid;
                docid_count[docid].second+=hitcount;
            }
        }
        partial_sort(docid_count.begin(), Head(docid_count,5).end(),
                docid_count.end(),
                [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
                    int64_t lhs_docid = lhs.first;
                    auto lhs_hit_count = lhs.second;
                    int64_t rhs_docid = rhs.first;
                    auto rhs_hit_count = rhs.second;
                    return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
                }
        );

        search_results_output << current_query << ':';
        for (auto [docid, hitcount] : Head(docid_count, 5)) {
            if(hitcount!=0) {
            search_results_output << " {"
                                  << "docid: " << docid << ", "
                                  << "hitcount: " << hitcount << '}';}
        }
        search_results_output << endl;
        for (auto &i : docid_count) {
            i.first = i.second = 0;
        }
    }

}



/*void SearchServer::AddQueriesStream(
        istream& query_input, ostream& search_results_output
) {
    for (string current_query; getline(query_input, current_query); ) {
        const auto words = SplitIntoWords(current_query);
        vector<int> docid_count(50000);
        for (const auto &word: words) {
            for (const size_t docid: index.Lookup(static_cast<string>(word))) {
                docid_count[docid]++;
            }
        }

        Priority_queue search_results;
        for (int i = 0; i < docid_count.size(); i++) {
            if (docid_count[i]) search_results.Insert(make_pair(i,docid_count[i]));
        }
        search_results_output << current_query << ':';
        auto f = Head1(search_results, 5);
        for (auto[docid, hitcount]: f) {
            search_results_output << " {"
                                  << "docid: " << docid << ", "
                                  << "hitcount: " << hitcount << '}';
        }
        search_results_output << endl;
    }
}*/

void InvertedIndex::Add(const string& document) {
  docs.push_back(move(document));

  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(document)) {
      if (!index[word].empty() && index[word].back().first == docid) {
      index[word].back().first = docid;
      index[word].back().second++;}
      else index[word].push_back(make_pair(docid,1));
  }
}

 const vector<pair<size_t,size_t>>& InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return empty_vector;
  }
}
