#include <iostream>
#include <Windows.h>
#include <common.hpp>

void process1()
{
	HANDLE fileMappingHandle{ CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096, "IPC_FILE_MAPPING") };
	if (fileMappingHandle == NULL)
	{
		std::stringstream message{ {"Couldn't create the file mapping inside process1!"} };
		logERR(message);

		return;
	}

	HANDLE eventHandle{ };
	bool go_on{ true };
	while (go_on)
	{
		eventHandle = OpenEventA(EVENT_ALL_ACCESS, true, "FILE_MAPPING_FILLED");

		if (eventHandle != NULL || GetLastError() != ERROR_FILE_NOT_FOUND)
		{ go_on = false; }
	}

	if (eventHandle == NULL)
	{
		std::stringstream message{ {"Couldn't open the event handle inside process1!"} };
		logERR(message);

		CloseHandle(fileMappingHandle);
		return;
	}

	LPVOID mappedViewStartingAddr{ MapViewOfFile(fileMappingHandle, FILE_MAP_READ, 0, 0, 4096) };
	if (mappedViewStartingAddr == NULL)
	{
		std::stringstream message{ {"Couldn't map the view of the file mapping inside process1!"} };
		logERR(message);

		CloseHandle(eventHandle);
		CloseHandle(fileMappingHandle);
		return;
	}

	char textRead[4096];
	memcpy(textRead, mappedViewStartingAddr, 4096);
	std::cout << "Successfully read the following information from the view of the file mapping inside process1:" << std::endl;
	std::cout << textRead << std::endl;

	CloseHandle(eventHandle);
	UnmapViewOfFile(mappedViewStartingAddr);
	CloseHandle(fileMappingHandle);
}

void process2()
{
	HANDLE fileMappingHandle{ };

	bool go_on{ true };
	while (go_on)
	{
		fileMappingHandle = OpenFileMappingA(FILE_MAP_WRITE, false, "IPC_FILE_MAPPING");
		
		if (fileMappingHandle != NULL || GetLastError() != ERROR_FILE_NOT_FOUND)
		{ go_on = false; }
	}
	
	if (fileMappingHandle == NULL)
	{
		std::stringstream message{ {"Couldn't open the file mapping inside process2!"} };
		logERR(message);

		return;
	}

	LPVOID mappedViewStartingAddr{ MapViewOfFile(fileMappingHandle, FILE_MAP_WRITE, 0, 0, 4096) };
	if (mappedViewStartingAddr == NULL)
	{
		std::stringstream message{ {"Couldn't map the view of the file mapping inside process2!"} };
		logERR(message);

		CloseHandle(fileMappingHandle);
		return;
	}

	const char* textToWrite{ "a message for the other process" };
	memcpy(mappedViewStartingAddr, textToWrite, strlen(textToWrite));
	std::cout << "Successfully wrote the following information to the view of the file mapping inside process2:" << std::endl;
	std::cout << textToWrite << std::endl;

	HANDLE eventHandle{ CreateEventA(NULL, true, true, "FILE_MAPPING_FILLED") };
	if (eventHandle == NULL)
	{
		std::stringstream message{ {"Couldn't create the event handle inside process2!"} };
		logERR(message);

		UnmapViewOfFile(mappedViewStartingAddr);
		CloseHandle(fileMappingHandle);
		return;
	}

	UnmapViewOfFile(mappedViewStartingAddr);
	CloseHandle(fileMappingHandle);
	CloseHandle(eventHandle);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{ return -1; }

	if (strcmp(argv[1], "1") == 0)
	{ process1(); }

	if (strcmp(argv[1], "2") == 0)
	{ process2(); }
	
	return 0;
}