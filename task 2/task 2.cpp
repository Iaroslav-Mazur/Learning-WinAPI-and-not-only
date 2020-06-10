#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>

DWORD WINAPI Worker(IN LPVOID lpParam)
{
	if (lpParam != nullptr)
	{
		const int workerID{ *(static_cast<int*>(lpParam)) };
		std::cout << std::endl << "nWorkerId " << workerID << " started to work" << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		std::cout << std::endl << "nWorkerId " << workerID << " finished working" << std::endl;
	}
	return 0;
}

int WorkManagement()
{
	std::vector<std::thread> workingThreads{ };
	std::vector<int> workersIDs(10);
	std::iota(workersIDs.begin(), workersIDs.end(), 1);
	
	for (auto& id : workersIDs)
	{
		workingThreads.emplace_back(Worker, &id);
	}
	
	for (auto& thread : workingThreads)
	{
		thread.join();
	}

	return 0;
}

int main()
{
	WorkManagement();
	return 0;
}