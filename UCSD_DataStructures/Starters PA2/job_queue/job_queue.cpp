#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using std::vector;
using std::cin;
using std::cout;
using std::endl;

class JobWorker {
public:
	JobWorker& operator=(const JobWorker& rhs) {
		this->id = rhs.id;
		this->free_time = rhs.free_time;
	}

	int id;
	long long free_time;
};

class CmpWorker
{
public:
	bool operator() (const JobWorker& lhs, const JobWorker& rhs) const
	{
		if (lhs.free_time == rhs.free_time)
			return (lhs.id > rhs.id);
		return (lhs.free_time > rhs.free_time);
	}
};


class JobQueue {
 private:
	int num_workers_;
	vector<int> jobs_;

	vector<int> assigned_workers_;
	vector<long long> start_times_;

	void WriteResponse() const {
		for (int i = 0; i < jobs_.size(); ++i) {
			cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
		}
	}

	void ReadData() {
		int m;
		cin >> num_workers_ >> m;
		jobs_.resize(m);
		for(int i = 0; i < m; ++i)
			cin >> jobs_[i];
	}

	void AssignJobs() {
		// TODO: replace this code with a faster algorithm.
		std::priority_queue<JobWorker, std::vector<JobWorker>, CmpWorker> workQueue;

		JobWorker tmpWorker;

		for (int i = 0; i < num_workers_; i++) {
			JobWorker worker;
			worker.id = i;
			worker.free_time = 0;
			workQueue.push(worker);
		}

		assigned_workers_.resize(jobs_.size());
		start_times_.resize(jobs_.size());

		for (int i = 0; i < jobs_.size(); ++i) {
			int duration = jobs_[i];
			tmpWorker = workQueue.top();
			workQueue.pop();
			assigned_workers_[i] = tmpWorker.id;
			start_times_[i] = tmpWorker.free_time;
			tmpWorker.free_time += duration;
			workQueue.push(tmpWorker);
		}
	}

 public:
	void Solve() {
		ReadData();
		AssignJobs();
		WriteResponse();
	}
};

int main() {
	std::ios_base::sync_with_stdio(false);
	JobQueue job_queue;
	job_queue.Solve();
	return 0;
}
