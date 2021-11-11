#include <iostream>
#include <Windows.h>
#include "driver.h"


int main()
{
	std::cout << "Make sure driver is loaded, F3 to continue" << std::endl;

	while (!GetAsyncKeyState(VK_F3) & 1)
		Sleep(400);

	std::cout << "Make sure assault cube is loaded, F3 to continue\n" << std::endl;

	while (!GetAsyncKeyState(VK_F3) & 1)
		Sleep(400);

	IoctlDriver Driver = IoctlDriver("\\\\.\\simpleioctl");

	std::cout << "F4 for ProcId, F5 for ModBase" << std::endl;

	while (true)
	{
		if (GetAsyncKeyState(VK_F4) & 1)
			std::cout << Driver.getProcId() << std::endl;

		if (GetAsyncKeyState(VK_F5) & 1)
			std::cout << Driver.getModBase() << std::endl;
	}

	return 0;
}