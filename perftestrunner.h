#ifndef PERFTESTRUNNER_H_INCLUDED
#define PERFTESTRUNNER_H_INCLUDED 1
/**
 * @file perftestrunner.h
 * @brief class PerfTestRunner that runs a set of 
 * 	functions returning int with given argument.
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <sys/time.h>

using std::endl;
using std::ios;
using std::ostream;
using std::random_shuffle;
using std::string;
using std::vector;

namespace {

const double OneMln = 1000000.0;

double
GetTimeMicros() {
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec * OneMln + tv.tv_usec;
}

} // namespace

template <typename ArgType>
class PerfTestRunner {
public:
	typedef int (*TestFuncPtr)(const ArgType &);

	PerfTestRunner(unsigned nRuns, const ArgType &argRef, unsigned nOps):
		argRef_(argRef), dummyTestPtr_(0), entries_(), nOps_(nOps), nRuns_(nRuns) {}

	void addTestFunc(TestFuncPtr tfp, const char* testName, bool isDummy=false) {
		entries_.push_back(Entry(tfp, testName));
		if(isDummy)
			dummyTestPtr_ = &entries_.back();
	}

	void run() {
		typedef vector<Entry *> EntryPtrVec;
		EntryPtrVec testPtVec(entries_.size());

		for(unsigned testIdx = entries_.size();  testIdx--;) {
			entries_[testIdx].resetResults();
		}

		for(unsigned runIdx = 0;  runIdx < nRuns_;  ++runIdx) {
			for(unsigned testIdx = entries_.size();  testIdx--;) {
				testPtVec[testIdx] = &entries_[testIdx];
			}
			random_shuffle(testPtVec.begin(), testPtVec.end());
			for(unsigned testIdx = testPtVec.size();  testIdx--;) {
				Entry *const entryPt = testPtVec[testIdx];
				const double startTimeMicros = GetTimeMicros();
				const int testRes = (entryPt->getTestFuncPtr())(argRef_);
				const double endTimeMicros = GetTimeMicros();
				const double runTimeMicros = endTimeMicros - startTimeMicros;
				entryPt->addResult(testRes, runTimeMicros);
			}
		}
	}

	void report(ostream &os) const {
		const double totalNOps = (double)nOps_ * nRuns_;
		double totalDummyRuntimeSecs = dummyTestPtr_ ?
				dummyTestPtr_->calcTotalRuntimeSec() : 0.0;
		os.setf(ios::fixed, ios::floatfield);
		for(unsigned testIdx = 0;  testIdx < entries_.size();  ++testIdx) {
			const Entry *const entry = &entries_[testIdx];
			os.precision(3);
			os << entry->getTestName() << "  ave-res: " << entry->calcAveRes();
			os.precision(0);
			const double totalRuntimeSecs = entry->calcTotalRuntimeSec();
			const double opsPerSec = totalNOps / (totalRuntimeSecs ? totalRuntimeSecs :
					0.001);
			const double opCostNanos = 1000000000.0 / (opsPerSec ? opsPerSec : 1);
			os << "  ops/sec: " << opsPerSec;
			os.precision(2);
			os << "  opCostNanos: " << opCostNanos;
			if (entry != dummyTestPtr_ && totalDummyRuntimeSecs) {
				const double residualRuntimeSecs = totalRuntimeSecs -
						totalDummyRuntimeSecs;
				const double pureOpsPerSec = totalNOps / (residualRuntimeSecs ?
						residualRuntimeSecs : 0.001);
				os << "  resid.ops/sec: " << opsPerSec;
			}
			os << endl;
		}
	}

private:
	class Entry {
	public:
		Entry(TestFuncPtr testFuncPtr, const char *testName):
			testFuncPtr_(testFuncPtr), testName_(testName), results_() {}
		void addResult(int result, double runTimeMicros)
			{ results_.push_back(RunResult(result, runTimeMicros)); }
		const string &getTestName() const { return testName_; }
		TestFuncPtr getTestFuncPtr() const { return testFuncPtr_; }
		void resetResults() { results_.clear(); }
		double calcAveRes() const {
			return accumulate(results_.begin(), results_.end(), 0.0, AddResult()) /
					(results_.size() ? results_.size() : 1);
		}
		double calcTotalRuntimeSec() const {
			return accumulate(results_.begin(), results_.end(), 0.0, AddTime()) /
					OneMln;
		}

		class RunResult {
		public:
			RunResult(int result, double runTimeMicros):
				result_(result), runTimeMicros_(runTimeMicros) {}
			int getResult() const { return result_; }
			double getRunTimeMicros() const { return runTimeMicros_; }
		private:
			int result_;
		private:
			double runTimeMicros_;
		};
	private:
		struct AddResult {
			double operator()(double acc, const Entry::RunResult &r) const
				{ return acc + r.getResult(); }
		};
		struct AddTime {
			double operator()(double acc, const Entry::RunResult &r) const
				{ return acc + r.getRunTimeMicros(); }
		};
		typedef vector<RunResult> ResultVec;

		ResultVec results_;
		TestFuncPtr testFuncPtr_;
		string testName_;
	};

	typedef vector<Entry> EntryVec;

	const ArgType &argRef_;
	Entry* dummyTestPtr_;
	EntryVec entries_;
	const unsigned nOps_;
	const unsigned nRuns_;
};
#endif /* PERFTESTRUNNER_H_INCLUDED */
