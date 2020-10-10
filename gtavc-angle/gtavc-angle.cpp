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
									 * also invert th value because the values in the SCM is inverted for some reason
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