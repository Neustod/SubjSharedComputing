#pragma once
#include <windows.h>
#include <process.h>
#include <vector>


class ThreadManager : public std::vector<HANDLE>
{
public:
	explicit ThreadManager(size_t countOfThreads = 0);

	ThreadManager(const ThreadManager& src);

	ThreadManager(ThreadManager&& src) noexcept;

public:
	void operator=(const ThreadManager& src);

	void operator=(ThreadManager&& src) noexcept;

public:
	static HANDLE CreateThread(_beginthreadex_proc_type routine, void* argList);

	DWORD Rewrite(int i, HANDLE hThread);

	DWORD Wait(BOOL bWaitAll);

	DWORD CloseHandles();

	~ThreadManager();
};

