/**
 *  @package Ardukit
 *
 *  @module gmbox - General purpose message box
 */

#pragma once
#include "gque.h"

namespace gcl {

//-----------------------------------------------------------------------------
//	class GMBoxCore - thread safe message box core
//-----------------------------------------------------------------------------
class gcl_api gmbox {
protected:
    gque        m_q;

public:
	gmbox() {}
	gmbox(unsigned capacity, unsigned msgSize, bool uselock=true) : m_q(capacity, msgSize) {}
	~gmbox() {}

	void reset(unsigned capacity, unsigned msgSize)
        { lock(); m_q.reset(capacity, msgSize); unlock(); }
	void *peek() { return m_q.peek(); }
	bool put(const void *msg)
        { lock(); bool r = m_q.put(msg); unlock(); return r; }
	bool get(void *msg)
        { lock(); bool r = m_q.get(msg); unlock(); return r; }
    unsigned length() { return m_q.length(); }
    unsigned available() { return m_q.available(); }
    unsigned capacity() { return m_q.capacity(); }

    void lock() {}
    void unlock() {}
};


//-----------------------------------------------------------------------------
//	class msgbox - thread safe message box core
//-----------------------------------------------------------------------------
template <class T = void>
class mbox : public gmbox {
public:
	mbox() {};
	mbox(unsigned capacity, bool uselock=true) : gmbox(capacity, sizeof(T), uselock) {}
	~mbox() {}

    void reset(unsigned capacity) { gmbox::reset(capacity, sizeof(T)); }

	T *peek() { return (T *)gmbox::peek(); }
	bool get(T *msg) { return gmbox::get(msg); }
	bool put(const T *msg) { return gmbox::put(msg); }
	bool put(const T &msg) { return gmbox::put(&msg); }
};

} // namespace gcl


typedef gcl::gmbox      GMbox;
