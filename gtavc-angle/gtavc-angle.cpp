/**
The MIT License (MIT)

Copyright (c) 2020 Deniz Ariyan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <windows.h>
#include <conio.h>
#include <iostream>
#include "tchar.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Press enter to start memory reading, press again to stop." << endl;
	cin.ignore();
	DWORD address = 0x7E48CC; // Memory offset for players left-right angle
	
	double multiplier = 57.2957779189; /** 
									    * Rockstar decided to divide the degrees to this holy number before writing to memory
									    * therefore we need to multiply the reading with this. Had to do some weird things to 
									    * find this number so it might not be perfectly accurate 
										*/
	float value = 0.0; 
	float degree = 0.0;
	DWORD pid; // Process ID
	HWND hwnd; // Windows handle
	hwnd = FindWindow(NULL, L"GTA: Vice City"); 
	if (!hwnd)
	{
		cout << "Could not find the Vice City window! Run the game before starting the exe!\n";
		cin.get();
		return -1;
	}

	GetWindowThreadProcessId(hwnd, &pid); 
	HANDLE phandle = OpenProcess(PROCESS_VM_READ, 0, pid);
	if (!phandle)
	{
		cout << "Could not get handle! Make sure you are running the exe with admin priv\n";
		cin.get();
		return -1;
		
	}
	bool loop = true;
	while (loop)
	{
		degree = (value * multiplier) + 90; // Have to add 90 because the degrees used in the SCM is 90 degrees off from the degrees given in the memory
		if (degree > 360)
		{
			degree = -(360-degree); /** 
									 * Subtract from 360 because it can go over 360 since we added 90 before to get in line with the SCM
									 * also invert the value because the values in the SCM is inverted
									 */
		}
		ReadProcessMemory(phandle, (void*)address, &value, sizeof(value), 0);
		cout << fixed <<degree << "\n"; //Fixed so it does not show in scientific notation

		if (_kbhit()) 
		{
			loop = false;
			return 0;
		}
		Sleep(200); // Don't set the sleep time to something less then 50ms, it lags the Vice City
	}
}