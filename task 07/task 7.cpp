#include <iostream>
#include <Windows.h>
#include "common.hpp"

#define MAX_VALUE_NAME 16383


bool createRegKeyAndHandleErrors(_In_ HKEY hKey,
								const std::string& lpSubKey,
								_Reserved_ DWORD Reserved,
								_In_opt_ LPSTR lpClass,
								_In_ DWORD dwOptions,
								_In_ REGSAM samDesired,
								_In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								_Out_ PHKEY phkResult,
								_Out_opt_ LPDWORD lpdwDisposition)
{
	const LSTATUS errRegCreateKeyExA = RegCreateKeyExA(hKey,
														lpSubKey.c_str(),
														Reserved,
														lpClass,
														dwOptions,
														samDesired,
														lpSecurityAttributes,
														phkResult,
														lpdwDisposition);

	if (errRegCreateKeyExA != ERROR_SUCCESS)
	{
		std::stringstream message{ };
		message << "Could not create the " << lpSubKey.c_str() << " registry key!";
		logERR(message);

		return false;
	}

	std::cout << "Successfully created the " << lpSubKey.c_str() << " key." << std::endl;
	
	return true;
}

bool SetRegKeyValueAndHandleErrors(_In_ HKEY hKey,
									const std::string& lpSubKey,
									const std::string& lpValueName,
									_In_ DWORD dwType,
									_In_reads_bytes_opt_(cbData) LPCVOID lpData,
									_In_ DWORD cbData)
{
	const LSTATUS errRegSetKeyValueA = RegSetKeyValueA(hKey,
														lpSubKey.c_str(),
														lpValueName.c_str(),
														dwType,
														lpData,
														cbData);
	if (errRegSetKeyValueA != ERROR_SUCCESS)
	{
		std::stringstream message{ };
		message << "Could not set the " << lpValueName.c_str() << "value for the " << lpSubKey.c_str() << " registry key!";
		logERR(message);

		return false;
	}

	std::cout << "Successfully filled the " << lpSubKey.c_str() << " key with a value." << std::endl;

	return true;
}

bool createRegKeyAndFillWithValues(const std::string& regKeyName)
{
	HKEY keyHandle{ };
	DWORD dispositionValue{ };

	if (createRegKeyAndHandleErrors(HKEY_CURRENT_USER,
									regKeyName,
									0, NULL,
									REG_OPTION_NON_VOLATILE,
									KEY_CREATE_SUB_KEY,
									NULL,
									&keyHandle,
									&dispositionValue) == false )
	{
		RegCloseKey(keyHandle);
		return false;
	}
	
	RegCloseKey(keyHandle);

	const int data{ 10 };
	const std::string dwordValueName{ "dwValue" };
	if (SetRegKeyValueAndHandleErrors(HKEY_CURRENT_USER,
										regKeyName,
										dwordValueName,
										REG_DWORD,
										&data,
										sizeof(int)) == false)
	{ return false;	}

	const std::string szData{ "some value" };
	const std::string szValueName{ "szValue" };
	if (SetRegKeyValueAndHandleErrors(HKEY_CURRENT_USER,
										regKeyName,
										szValueName,
										REG_SZ,
										szData.c_str(),
										szData.size()) == false)
	{ return false;	}

	return true;
}

int main()
{
	const std::string regKey1Name{ "Temp1" };
	if (createRegKeyAndFillWithValues(regKey1Name) == false)
	{ return -1; }

	const std::string regKey2Name{ "Temp2" };
	if (createRegKeyAndFillWithValues(regKey2Name) == false)
	{ return -1; }

	HKEY keyHandle{ };
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
						NULL,
						0,
						KEY_READ,
						&keyHandle) != ERROR_SUCCESS)
	{
		std::stringstream message{ {"Could not open a handle to HKEY_CURRENT_USER"} };
		logERR(message);
		
		RegCloseKey(keyHandle);
		return -1;
	}

	DWORD cbMaxSubKey{ };
	RegQueryInfoKey(keyHandle,      // key handle 
					NULL,           // buffer for class name 
					NULL,           // size of class string 
					NULL,           // reserved 
					NULL,           // number of subkeys 
					&cbMaxSubKey,	// longest subkey size 
					NULL,           // longest class string 
					NULL,           // number of values for this key 
					NULL,			// longest value name 
					NULL,           // longest value data 
					NULL,           // security descriptor 
					NULL);          // last write time 
	
	char* subKeyName{ new char[cbMaxSubKey + 1]{} };
	DWORD keyID{ 0 }, retCode{ };
	char* valueName{ new char[MAX_VALUE_NAME] {} };
	
	while (retCode != ERROR_NO_MORE_ITEMS)
	{
		DWORD cbName{ cbMaxSubKey };
		retCode = RegEnumKeyExA(keyHandle, 
								keyID,
								subKeyName,
								&cbName,
								NULL,
								NULL,
								NULL,
								NULL);
		
		if (retCode != ERROR_SUCCESS && retCode != ERROR_NO_MORE_ITEMS)
		{
			std::stringstream message{ {"Failed when calling RegEnumKeyEx"} };
			logERR(message);

			RegCloseKey(keyHandle);
			delete[] subKeyName;
			delete[] valueName;
			return -1;
		}
		
		if (strcmp(subKeyName, regKey1Name.c_str()) == 0 || strcmp(subKeyName, regKey2Name.c_str()) == 0)
		{
			std::cout << "Found the " << subKeyName << " key!" << std::endl;

			HKEY subKeyHandle{ };
			if (RegOpenKeyEx(HKEY_CURRENT_USER,
							subKeyName,
							0,
							KEY_READ,
							&subKeyHandle) != ERROR_SUCCESS)
			{
				std::stringstream message{ };
				message << "Could not open a handle to " << subKeyName;
				logERR(message);

				RegCloseKey(subKeyHandle);
				RegCloseKey(keyHandle);
				delete[] subKeyName;
				delete[] valueName;
				return -1;
			}

			LSTATUS errEnumValue{ };
			DWORD maxValueName{ MAX_VALUE_NAME };
			DWORD valueID{ 0 };

			std::cout << "Here are the values of the key:" << std::endl;

			while (errEnumValue != ERROR_NO_MORE_ITEMS)
			{
				maxValueName = MAX_VALUE_NAME;
				valueName[0] = '\0';
				errEnumValue = RegEnumValue(subKeyHandle,
											valueID,
											valueName,
											&maxValueName,
											NULL,
											NULL,
											NULL,
											NULL);
				
				if (errEnumValue != ERROR_SUCCESS && errEnumValue != ERROR_NO_MORE_ITEMS)
				{
					std::stringstream message{ { "Failed when calling RegEnumValue" } };
					logERR(message);
					
					RegCloseKey(subKeyHandle);
					RegCloseKey(keyHandle);
					delete[] subKeyName;
					delete[] valueName;
					return -1;
				}

				if (errEnumValue == ERROR_SUCCESS)
				{ std::cout << "valueName: " << valueName << std::endl; }

				valueID++;
			}

			RegCloseKey(subKeyHandle);
		}

		keyID++;
	}

	RegCloseKey(keyHandle);
	delete[] subKeyName;
	delete[] valueName;
	return 0;
}