#ifndef C1_H_INCLUDED
#define C1_H_INCLUDED 1
/**
 * @file c1.h
 * @brief C1, depth 1 test class
 */

class C1 {
public:
	C1();
	virtual int f() const { return 1; }
};

class CNV1 {
public:
	CNV1();
	int f() const { return 1; }
private:
	void *dummy_;
};

#endif /* C1_H_INCLUDED */
