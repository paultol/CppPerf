#ifndef C3_H_INCLUDED
#define C3_H_INCLUDED 1
/**
 * @file c3.h
 * @brief C3, depth 3 test class
 */

#include "c2.h"

class C3: public C2 {
public:
	C3();
	virtual int f() const { return 3; }
};
class CNV3: public CNV2 {
public:
	CNV3();
	int f() const { return 3; }
};
#endif /* C3_H_INCLUDED */
