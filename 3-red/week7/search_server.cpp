#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iostream>
#include <sstream>

vector<string_view> SplitIntoWords(string_view line)
{
    vector<string_view> result;
    line.remove_prefix(line.find_first_not_of(' '));

    while (line.size() > 0)
    {
        size_t sub_len = line.find(' ');
        result.emplace_back(line.substr(0, sub_len));
        line.remove_prefix(min(line.size(), line.find_first_not_of(' ', sub_len)));
    }

    return result;
}

SearchServer::SearchServer(istream &document_input)
{
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream &document_input)
{
    InvertedIndex new_index;

    for (string current_document; getline(document_input, current_document);)
    {
        new_index.Add(move(current_document));
    }

    lock_guard lock(index_mutex_);
    index_ = move(new_index);
}

void SearchServer::AddQueriesStream(
    istream &query_input, ostream &search_results_output)
{
    // Q <= 500 000 запросов
    // W <= 10 слов в запросе
    // L <= 100 символов в слове
    // V <= 10 000 различных слов во всех документах
    // N <= 1000 слов содержит каждый документ
    // D <= 50 000 документов
    for (string current_query; getline(query_input, current_query);) // Q*W*D ~ 3*10^11
    {
        lock_guard lock(index_mutex_);

        vector<size_t> docid_count(index_.Count(), 0);
        vector<int64_t> doc_ids(index_.Count(), 0);
        size_t doc_amount = 0;

        for (const auto &word : SplitIntoWords(current_query)) // W*D ~ 5*10^5
        {
            for (const auto &[docid, count] : index_.Lookup(word)) // D ~ 5*10^4
            {
                if (docid_count[docid] == 0)
                {
                    doc_ids[doc_amount++] = docid;
                }

                docid_count[docid] += count;
            }
        }

        vector<pair<size_t, int64_t>> search_results;
        search_results.reserve(docid_count.size());

        for (size_t doc_index = 0; doc_index < doc_amount; ++doc_index) // D ~ 5*10^4
        {
            search_results.emplace_back(docid_count[doc_ids[doc_index]], -doc_ids[doc_index]);
            docid_count[doc_ids[doc_index]] = 0;
            doc_ids[doc_index] = 0;
        }

        partial_sort(search_results.begin(),
                     search_results.begin() + min(search_results.size(),
                                                  static_cast<decltype(search_results)::size_type>(5)),
                     search_results.end(),
                     std::greater<>{}); // D ~ 5*10^4

        search_results_output << current_query << ':';
        for (const auto &[hitcount, docid] : Head(search_results, 5))
        {
            search_results_output << " {"
                                  << "docid: " << -docid << ", "
                                  << "hitcount: " << hitcount << '}';
        }
        search_results_output << endl;
    }
}

void InvertedIndex::Add(string &&document)
{
    const size_t docid = docs_.size();
    docs_.push_back(move(document));

    for (const auto &word : SplitIntoWords(docs_.back()))
    {
        if (auto &index_list = index_[word];
            !index_list.empty() && index_list.back().first == docid)
        {
            ++index_list.back().second;
        }
        else
        {
            index_list.emplace_back(docid, 1);
        }
    }
}

const vector<pair<size_t, size_t>> &InvertedIndex::Lookup(const string_view &word) const
{
    static const vector<pair<size_t, size_t>> empty{};

    if (auto it = index_.find(word); it != index_.end())
    {
        return it->second;
    }
    else
    {
        return empty;
    }
}

size_t InvertedIndex::Count() const
{
    return docs_.size();
}
