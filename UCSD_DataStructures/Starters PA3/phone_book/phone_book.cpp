#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <stdlib.h>

#define DEBUG 0

using std::string;
using std::list;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::pair;

// we know that phone numbers are max 5-digit long
unsigned gPrime = 100003;
unsigned gStartCardinality = 10;

template <class T>
class MyHashTable {
public:
	MyHashTable(unsigned prime) : prime_(prime), m_(gStartCardinality) {
			SetHashF();
			data_.resize(m_);
	}

	~MyHashTable() {};

	bool HasKey(int k) {

	};

	T Find(int k) {
		long long tmp = a_ * k + b_;
		tmp = tmp % prime_;
		int idx = tmp % m_;
#if DEBUG
		cout << "Finding index" << idx << endl;
#endif
		if (data_[idx].size() == 0) {
			return "not found";
		} else {
			for (int i = 0; i < data_[idx].size(); i++) {
				if (data_[idx][i].first == k) {
					return data_[idx][i].second;
				}
			}
		}
		return "not found";
	};

	void Add(int k, T val) {
		long long tmp = a_ * k + b_;
#if DEBUG
		cout << "tmp = " << tmp << endl;
#endif
		tmp = tmp % prime_;
#if DEBUG
		cout << "tmp = " << tmp << endl;
#endif
		int idx = tmp % m_;
#if DEBUG
		cout << "Adding to index" << idx << endl;
#endif
		bool found = false;

		for (int i = 0; i < data_[idx].size(); i++) {
			if (data_[idx][i].first == k) {
				found = true;
				data_[idx][i].second = val;
				return;
			}
		}

		if (!found) {
			data_[idx].push_back(make_pair(k, val));
		}
		return;
	};

	void Delete(int k) {
		long long tmp = a_ * k + b_;
		tmp = tmp % prime_;
		int idx = tmp % m_;

		if (data_[idx].size() == 0) {
			return;
		} else {
			for (int i = 0; i < data_[idx].size(); i++) {
				if (data_[idx][i].first == k) {
#if DEBUG
					cout << "Removing " << data_[idx][i].second << endl;
#endif
					data_[idx].erase(data_[idx].begin() + i);
#if DEBUG
					cout << "now this index contains " << data_[idx].size() << " elements" << endl;
#endif
					return;
				}
			}
		}
		return;
	}

private:
	void SetHashF() {
		srand(time(0));
		a_ = rand() % prime_;
		if (a_ == 0)
			a_ = 1;
		b_ = rand() % prime_;
#if DEBUG
		cout << "a = " << a_ << ", b = " << b_ << endl;
#endif
	}

	// growth function if cardinality grows too large
	bool ReHash_() {

	};

	unsigned a_; // first selector
	unsigned b_; // second selector
	unsigned prime_; // prime used
	unsigned m_; // cardinality
	vector<vector<std::pair<int, T> > > data_; //
};

struct Query {
		string type, name;
		int number;
};

vector<Query> read_queries() {
		int n;
		cin >> n;
		vector<Query> queries(n);
		for (int i = 0; i < n; ++i) {
				cin >> queries[i].type;
				if (queries[i].type == "add")
						cin >> queries[i].number >> queries[i].name;
				else
						cin >> queries[i].number;
		}
		return queries;
}

void write_responses(const vector<string>& result) {
		for (size_t i = 0; i < result.size(); ++i)
				std::cout << result[i] << "\n";
}

vector<string> process_queries(const vector<Query>& queries) {
		vector<string> result;
		MyHashTable<string>* hashTable;
		hashTable = new MyHashTable<string>(gPrime);

		for (int i = 0; i < queries.size(); i++) {
			if (queries[i].type == "add") {
				hashTable->Add(queries[i].number, queries[i].name);
			} else if (queries[i].type == "find") {
				result.push_back(hashTable->Find(queries[i].number));
			} else if (queries[i].type == "del") {
				hashTable->Delete(queries[i].number);
			}
		}

		delete hashTable;
		return result;
}

int main() {
		write_responses(process_queries(read_queries()));
		return 0;
}
