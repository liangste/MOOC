#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Query {
		string type, s;
		size_t ind;
};

class QueryProcessor {
		int bucket_count;
		// store all strings in one vector
		vector<string> elems;
		vector<vector<string> > data_;

		size_t hash_func(const string& s) const {
				static const size_t multiplier = 263;
				static const size_t prime = 1000000007;
				unsigned long long hash = 0;
				for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i)
						hash = (hash * multiplier + s[i]) % prime;
				return hash % bucket_count;
		}

public:
		explicit QueryProcessor(int bucket_count): bucket_count(bucket_count) {
			data_.resize(bucket_count);
		}

		Query readQuery() const {
				Query query;
				cin >> query.type;
				if (query.type != "check")
						cin >> query.s;
				else
						cin >> query.ind;
				return query;
		}

		void writeSearchResult(bool was_found) const {
				std::cout << (was_found ? "yes\n" : "no\n");
		}

		void processQuery(const Query& query) {
				if (query.type == "check") {
						// use reverse order, because we append strings to the end
						for (int i = 0; i < data_[query.ind].size(); i++) {
							cout << data_[query.ind][i] << " ";
						}
						cout << endl;
				} else if (query.type == "add") {
					size_t idx = hash_func(query.s);
					if (data_[idx].size() == 0) {
						data_[idx].insert(data_[idx].begin(), query.s);
					}
					else {
						bool found = false;

						for (int i = 0; i < data_[idx].size(); i++) {
							if (data_[idx][i] == query.s) {
								found = true;
								break;
							}
						}

						if (!found) {
							data_[idx].insert(data_[idx].begin(), query.s);
						}
					}
				} else if (query.type == "del") {
					size_t idx = hash_func(query.s);
					if (data_[idx].size() == 0) {
						return; // do nothing
					} else {
						for (int i = 0; i < data_[idx].size(); i++) {
							if (data_[idx][i] == query.s) {
								data_[idx].erase(data_[idx].begin() + i);
								return;
							}
						}
					}
				} else if (query.type == "find") {
					bool found = false;
					size_t idx = hash_func(query.s);

					for (int i = 0; i < data_[idx].size(); i++) {
						if (data_[idx][i] == query.s) {
							found = true;
							break;
						}
					}

					writeSearchResult(found);
				}
		}

		void processQueries() {
				int n;
				cin >> n;
				for (int i = 0; i < n; ++i)
						processQuery(readQuery());
		}
};

int main() {
		std::ios_base::sync_with_stdio(false);
		int bucket_count;
		cin >> bucket_count;
		QueryProcessor proc(bucket_count);
		proc.processQueries();
		return 0;
}
