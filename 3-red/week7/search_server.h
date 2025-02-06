#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <string_view>
#include <mutex>

using namespace std;

class InvertedIndex
{
public:
    void Add(string &&document);
    const vector<pair<size_t, size_t>> &Lookup(const string_view &word) const;
    size_t Count() const;

private:
    map<string_view, vector<pair<size_t, size_t>>> index_;
    list<string> docs_;
};

class SearchServer
{
public:
    SearchServer() = default;
    explicit SearchServer(istream &document_input);

    void UpdateDocumentBase(istream &document_input);
    void AddQueriesStream(istream &query_input, ostream &search_results_output);

private:
    InvertedIndex index_;
    mutex index_mutex_;
};
