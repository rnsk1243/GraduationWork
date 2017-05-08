#pragma once
#include<WinSock2.h>
#include<mutex>
#include<iostream>
using namespace std;

class MUTEX
{
private:
	mutex m;
public:
	MUTEX()
	{
		cout << "mutex 생성자 호출" << endl;
	}
	~MUTEX()
	{
		cout << "mutex 소멸자 호출" << endl;
	}
	void lock()
	{
		m.lock();
		cout << "mutex lock 완료" << endl;
	}
	void unlock()
	{
		m.unlock();
		cout << "mutex nulock 완료" << endl;
	}

};

class CRITICALSECTION
{
private:
	CRITICAL_SECTION CS;
public:
	CRITICALSECTION()
	{
		InitializeCriticalSection(&CS);
	}
	~CRITICALSECTION()
	{
		DeleteCriticalSection(&CS);
		cout << "critical 객체 해제" << endl;
	}
	void lock()
	{
		EnterCriticalSection(&CS);
		cout << "critical lock 완료" << endl;
	}
	void unlock()
	{
		LeaveCriticalSection(&CS);
		cout << "critical unlock 완료" << endl;
	}
};



template<typename T> class ScopeLock {

	T& obj;
public:

	ScopeLock(T* o) : obj(*o)
	{
		obj.lock();
	}

	ScopeLock(T& o) : obj(o)
	{
		obj.lock();
	}

	~ScopeLock()
	{
		obj.unlock();
		//obj.~T();
	}

};
//static MUTEX RAII_MUTEX;
//static CRITICALSECTION RAII_CRITICALSECTION;