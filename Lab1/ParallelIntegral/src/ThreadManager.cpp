#include "ParallelIntegral/ThreadManager.hpp"


ThreadManager::ThreadManager(size_t countOfThreads) : std::vector<HANDLE>(countOfThreads, nullptr) {}

ThreadManager::ThreadManager(const ThreadManager& src) : std::vector<HANDLE>(src) {}

ThreadManager::ThreadManager(ThreadManager&& src) noexcept : std::vector<HANDLE>(src) {}

void ThreadManager::operator=(const ThreadManager& src) { std::vector<HANDLE>::operator=(src); }

void ThreadManager::operator=(ThreadManager&& src) noexcept { std::vector<HANDLE>::operator=(src); }


HANDLE ThreadManager::CreateThread(_beginthreadex_proc_type routine, void* argList) 
{
	return (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)routine, argList, 0, NULL);
}

DWORD ThreadManager::Rewrite(int i, HANDLE hThread) {
	CloseHandle((*this)[i]);
	(*this)[i] = hThread;

	return 0;
}

DWORD ThreadManager::Wait(BOOL bWaitAll) { return WaitForMultipleObjects(this->size(), this->data(), bWaitAll, INFINITE) - STATUS_WAIT_0; }

DWORD ThreadManager::CloseHandles()
{
	for (int i = 0; i < this->size(); i++)
	{
		if ((*this)[i] != nullptr) CloseHandle((*this)[i]);
	}

	return 0;
}

ThreadManager::~ThreadManager() {
	CloseHandles();
	this->clear();
}