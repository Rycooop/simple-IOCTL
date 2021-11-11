#include <Windows.h>

//Define same codes from driver
#define IO_GET_MODBASE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x666, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IO_GET_PROCID CTL_CODE(FILE_DEVICE_UNKNOWN, 0x667, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

class IoctlDriver
{
public:
	HANDLE driverHandle;

	IoctlDriver(LPCSTR regPath)
	{
		driverHandle = CreateFileA(regPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	}

	DWORD getProcId()
	{
		DWORD procId;
		ULONG bytes;

		if (DeviceIoControl(driverHandle, IO_GET_PROCID, &procId, sizeof(procId), &procId, sizeof(procId), &bytes, NULL))
		{
			return procId;
		}

		return 0;
	}

	DWORD getModBase()
	{
		DWORD modBase;
		ULONG bytes;

		if (DeviceIoControl(driverHandle, IO_GET_MODBASE, &modBase, sizeof(modBase), &modBase, sizeof(modBase), &bytes, NULL))
		{
			return modBase;
		}

		return 0;
	}

};