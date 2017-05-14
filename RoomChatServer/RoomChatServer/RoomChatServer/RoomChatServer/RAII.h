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
		cout << "mutex ������ ȣ��" << endl;
	}
	~MUTEX()
	{
		cout << "mutex �Ҹ��� ȣ��" << endl;
	}
	void lock()
	{
		m.lock();
		cout << "mutex lock �Ϸ�" << endl;
	}
	void unlock()
	{
		m.unlock();
		cout << "mutex nulock �Ϸ�" << endl;
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
		cout << "critical ��ü ����" << endl;
	}
	void lock()
	{
		EnterCriticalSection(&CS);
		cout << "critical lock �Ϸ�" << endl;
	}
	void unlock()
	{
		LeaveCriticalSection(&CS);
		cout << "critical unlock �Ϸ�" << endl;
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