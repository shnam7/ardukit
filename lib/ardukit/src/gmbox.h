/**
 *  @package Ardukit
 *
 *  @module gmbox - General purpose message box
 */

#pragma once
#include "gque.h"

namespace gcl {

//-----------------------------------------------------------------------------
//	class gmbox - thread safe queue (message box)
//-----------------------------------------------------------------------------
class gcl_api gmbox : protected gque {
protected:

public:
	gmbox() {}
	gmbox(unsigned capacity, unsigned msgSize) : gque(capacity, msgSize) {}
	~gmbox() {}

	void reset() { _lock(); gque::reset(); _unlock(); }
	void reset(unsigned capacity, unsigned msgSize)
        { _lock(); gque::reset(capacity, msgSize); _unlock(); }
	bool put(const void *msg)
        { _lock(); bool r = gque::put(msg); _unlock(); return r; }
	bool get(void *msg)
        { _lock(); bool r = gque::get(msg); _unlock(); return r; }
    bool push(const void *item)
        { _lock(); unsigned r = gque::push(item); _unlock(); return r; }
	bool pop(void *item=0)
        { _lock(); unsigned r = gque::pop(item); _unlock(); return r; }
    void clear() { _lock(); gque::clear(); _unlock(); }

	unsigned itemSize()
        { _lock(); unsigned r = gque::itemSize(); _unlock(); return r; }
    bool isEmpty()
        { _lock(); unsigned r = gque::isEmpty(); _unlock(); return r; }
    bool isFull()
        { _lock(); unsigned r = gque::isFull(); _unlock(); return r; }

    unsigned length()
        { _lock(); unsigned r = gque::length(); _unlock(); return r; }
    unsigned available()
        { _lock(); unsigned r = gque::available(); _unlock(); return r; }
    unsigned capacity()
        { _lock(); unsigned r = gque::capacity(); _unlock(); return r; }

protected:
    void _lock() {}
    void _unlock() {}
};


//--- type wrapper for gmbox
template <class T = void>
class gcl_api mbox : public gmbox {
public:
	mbox() {};
	mbox(unsigned capacity) : gmbox(capacity, sizeof(T)) {}
	~mbox() {}

    void reset(unsigned capacity) { gmbox::reset(capacity, sizeof(T)); }
	bool put(const T *msg) { return gmbox::put(msg); }
	bool put(const T &msg) { return gmbox::put(&msg); }
	bool get(T *msg) { return gmbox::get(msg); }
    bool push(const T *item) { return gmbox::push(item); }
	bool pop(void *item=0) { return gmbox::pop(item); }

protected:
	void reset(unsigned capacity, unsigned msgSize);    // disable
};

} // namespace gcl


using GMBox = gcl::gmbox;
