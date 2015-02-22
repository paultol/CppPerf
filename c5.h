#ifndef C5_H_INCLUDED
#define C5_H_INCLUDED 1
/**
 * @file c5.h
 * @brief C4, depth 4 test class
 */

#include "c4.h"

class C5: public C4 {
public:
	C5();
	virtual int f() const { return 5; }
};
class CNV5: public CNV4 {
public:
	CNV5();
	int f() const { return 5; }
};
#endif /* C5_H_INCLUDED */
