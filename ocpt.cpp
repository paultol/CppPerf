/** Object creation performance test */

#include <iostream>

using namespace std;

#include "perftestrunner.h"
#include "c1.h"
#include "c5.h"

template<class C>
int
CreateObjects(const vector<int> &data)
{
	int count = 0;
	const unsigned nOps = data.size();
	for (unsigned i = 0;  i < nOps;  ++i)
		count += (C().f() == data[i]);
	return count;
}

int
CreateDepth1NVObjects(const vector<int> &data)
{
	return CreateObjects<CNV1>(data);
}

int
CreateDepth5NVObjects(const vector<int> &data)
{
	return CreateObjects<CNV5>(data);
}

int
CreateDepth1Objects(const vector<int> &data)
{
	return CreateObjects<C1>(data);
}

int
CreateDepth5Objects(const vector<int> &data)
{
	return CreateObjects<C5>(data);
}

int
main(int, char*[])
{
	const unsigned N_OPS = 100000000;
	const unsigned N_RUNS = 10;
	vector<int> testData;
	testData.reserve(N_OPS);
	for(unsigned n = 0;  n < N_OPS;  ++n)
		testData.push_back(rand() % 100);

	cout << "sizeofs:  C1:" << sizeof(C1) << "  C2:" << sizeof(C2) <<
		"  CNV1:" << sizeof(CNV1) << "  CNV5:" << sizeof(CNV5) << endl;
	PerfTestRunner<vector<int> > runner(N_RUNS, testData, N_OPS);
	runner.addTestFunc(CreateDepth1Objects, "Depth 1   ");
	runner.addTestFunc(CreateDepth5Objects, "Depth 5   ");
	runner.addTestFunc(CreateDepth1NVObjects, "NV Depth 1");
	runner.addTestFunc(CreateDepth5NVObjects, "NV Depth 5");

	runner.run();

	runner.report(cout);

	return 0;
}
