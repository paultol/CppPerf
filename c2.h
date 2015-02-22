#ifndef C2_H_INCLUDED
#define C2_H_INCLUDED 1
/**
 * @file c2.h
 * @brief C2, depth 2 test class
 */

#include "c1.h"

class C2: public C1 {
public:
	C2();
	virtual int f() const { return 2; }
};
class CNV2: public CNV1 {
public:
	CNV2();
	int f() const { return 2; }
};
#endif /* C2_H_INCLUDED */
