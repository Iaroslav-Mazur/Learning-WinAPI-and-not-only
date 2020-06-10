#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>


struct workerIdAndHandle {
	const int workerID{ };
	const HANDLE eventHandle{ };

	workerIdAndHandle(int workerID, HANDLE eventHandle) :
		workerID{ workerID },
		eventHandle{ eventHandle }
	{ }
};

DWORD WINAPI Worker(IN LPVOID lpParam)
{
	if (lpParam != nullptr)
	{
		const workerIdAndHandle idAndHandle{ *(static_cast<workerIdAndHandle*>(lpParam)) };

		std::cout << std::endl << "nWorkerId " << idAndHandle.workerID << " started to work" << std::endl;
		WaitForSingleObject(idAndHandle.eventHandle, INFINITE);
		std::cout << std::endl << "nWorkerId " << idAndHandle.workerID << " has had its work canceled" << std::endl;
	}

	return 0;
}

DWORD WINAPI CancelWork(IN LPVOID lpParam)
{
	if (lpParam != nullptr)
	{
		HANDLE event{ *(static_cast<HANDLE*>(lpParam)) };

		const auto errSetEvent{ SetEvent(event) };
		if (errSetEvent == 0)
		{
			std::cerr << "The event couldn't have been set when the work has been requested to be canceled" << std::endl;
		}
	}

	return 0;
}

int WorkManagement()
{
	std::vector<std::thread> workingThreads{ };
	std::vector<int> workersIDs(10);
	std::iota(workersIDs.begin(), workersIDs.end(), 1);

	std::vector<workerIdAndHandle> workerIdsAndHandles{ };
	HANDLE eventHandle{ CreateEventA(NULL, true, false, NULL) };
	for (const auto& id : workersIDs)
	{
		workerIdsAndHandles.emplace_back(id, eventHandle);
	}

	for (auto& idAndHandle : workerIdsAndHandles)
	{
		workingThreads.emplace_back(Worker, &idAndHandle);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	CancelWork(&eventHandle);

	for (auto& thread : workingThreads)
	{
		thread.join();
	}

	CloseHandle(eventHandle);

	return 0;
}

int main()
{
	WorkManagement();
	return 0;
}