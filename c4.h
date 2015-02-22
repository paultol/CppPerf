#ifndef C4_H_INCLUDED
#define C4_H_INCLUDED 1
/**
 * @file c4.h
 * @brief C4, depth 4 test class
 */

#include "c3.h"

class C4: public C3 {
public:
	C4();
	virtual int f() const { return 4; }
};
class CNV4: public CNV3 {
public:
	CNV4();
	int f() const { return 4; }
};

#endif /* C4_H_INCLUDED */
