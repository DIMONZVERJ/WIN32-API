#include "WinMain.h"
#include "WorkFile.h"
#include "Animation.h"
#define KEY_SHIFTED     0x8000
#define KEY_TOGGLED     0x0001
using namespace std;
const int id1 = 1, id2 = 2, id3 = 3;
const TCHAR szWinClass[] = _T("Win32SampleApp");
TCHAR* szWinName;
int sizeWindowX = CW_USEDEFAULT, sizeWindowY = CW_USEDEFAULT, wigth = 320, height = 240, R = 255, G = 255, B = 255, size_field = 3;
LPWSTR* res;
void RunNotepad(void) //start the Notepad.exe 
{
	STARTUPINFO sInfo = { 0 };
	PROCESS_INFORMATION pInfo = { 0 };

	CreateProcess(_T("C:\\Windows\\Notepad.exe"),
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
	CloseHandle(pInfo.hThread);
	CloseHandle(pInfo.hProcess);
}
void SaveSetting()
{
	int mode = 0;
	if (res[1] != NULL)
	{
		mode = atoi((const char*)res[1]);
	}
	switch (mode)
	{
	case 1:
		fstream_out(); //файловые потоки
		break;
	case 2:
		FILE_var_out(); //файловые переменные
		break;
	case 3:
		funcWINAPI_out(); //Функции winAPI
		break;
	case 4:
		MappingFile_out(); //отображение на память
		break;
	default:
		funcWINAPI_out();
	}
}
int row_check(char** array, int& y)
{
	int sum = 0;
	int i, j;
	for (i = y, j = 0; j < size_field; j++)
		if (array[i][j] == 1)
			sum++;
		else if (array[i][j] == 0)
			sum--;
	if (sum == size_field) return 1;
	if (sum == -size_field) return -1;
	return 0;
}
int column_check(char** array, int& x)
{
	int sum = 0;
	int i, j;
	for (j = x, i = 0; i < size_field; i++)
		if (array[i][j] == 1)
			sum++;
		else if (array[i][j] == 0)
			sum--;
	if (sum == size_field) return 1;
	if (sum == -size_field) return -1;
	return 0;
}
int diagonal_check(char** array, int& x, int& y)
{
	int sum = 0;
	int i, j;
	if (x == y)
	{
		for (i = 0, j = 0; i < size_field; i++, j++)
			if (array[i][j] == 1) sum++;
			else if (array[i][j] == 0) sum--;
	}
	if (sum == size_field) return 1;
	if (sum == -size_field) return -1;
	return 0;
}
int antidiagonal_check(char** array, int& x, int& y)
{
	int sum = 0;
	int i, j;
	if (x + y == size_field-1)
	{
		for (i = 0, j = size_field - 1; i < size_field; i++, j--)
			if (array[i][j] == 1) sum++;
			else if (array[i][j] == 0) sum--;
	}
	if (sum == size_field) return 1;
	if (sum == -size_field) return -1;
	return 0;
}
bool fill_pole(char** array)
{
	for (int i = 0; i < size_field; i++)
		for (int j = 0; j < size_field; j++)
			if (array[i][j] == -1)
				return false;
	return true;
}
void EndGameCheck(int& x, int& y)
{
	int sum = 0;
	char* one_dimensional_array = GetMapSection();
	char** two_dimensional_array = new char* [size_field];
	int i, j;
	for (i = 0; i < size_field; i++)
		two_dimensional_array[i] = new char[size_field];
	for (i = 0; i < size_field; i++)
		for (j = 0; j < size_field; j++)
			two_dimensional_array[i][j] = one_dimensional_array[i * size_field + j];
	short r = row_check(two_dimensional_array, y);
	short c = column_check(two_dimensional_array, x);
	short d = diagonal_check(two_dimensional_array, x, y);
	short anti_d = antidiagonal_check(two_dimensional_array, x, y);
	bool fill = fill_pole(two_dimensional_array);
	if (abs(r) == 1 || abs(c) == 1 || abs(d) == 1 || abs(anti_d) == 1 || fill == true)
	{
		if (r == 1 || c == 1 ||	d == 1 || anti_d == 1)
		{
			win = "Победили нолики";
		}
		else if (r == -1 || c == -1 || d == -1 || anti_d == -1)
		{
			win = "Победили крестики";
		}
		else if (fill == true)
		{
			win = "Ничья";
		}
		for (i = 0; i < size_field; i++)
			delete[] two_dimensional_array[i];
		delete[] two_dimensional_array;
		if (WaitForSingleObject(semaphore_one_winner, 0) == WAIT_TIMEOUT) return;
		MessageBox(NULL, win.c_str(), "Notification", MB_OK);
		ReleaseSemaphore(semaphore_one_winner,1,NULL); //чтобы не показывало два уведомления о победе
		SendMessage(HWND_BROADCAST, MESSAGE_EXIT, NULL, NULL);
	}
}
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == MESSAGE_EXIT)
	{
		PostQuitMessage(0);
	}
	else if (message == MESSAGE_HOTKEY)
	{
		res_regist_hotkey_1 = RegisterHotKey(hwnd, id1, MOD_CONTROL | MOD_NOREPEAT, 0x51);
		res_regist_hotkey_2 = RegisterHotKey(hwnd, id2, MOD_SHIFT | MOD_NOREPEAT, 0x43);
	}
	else if (message == MESSAGE)
	{
		hdc = GetDC(hwnd);
		Redraw(hwnd, hdc, hPen,hBrush);
	}
	switch (message)
	{
	case WM_MOVING:
	{
		LPRECT rect = (LPRECT)lParam;
		sizeWindowX = rect->left;
		sizeWindowY = rect->top;
	}
	case WM_SIZING:
	{
		LPRECT rect = (LPRECT)lParam;
		sizeWindowX = rect->left;
		sizeWindowY = rect->top;
		wigth = rect->right - rect->left;
		height = rect->bottom - rect->top;
		break;
	}
	case WM_SIZE:
	{
		RECT window;
		if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
		{
			GetWindowRect(hwnd, &window);
			sizeWindowX = window.left;
			sizeWindowY = window.top;
			wigth = window.right - window.left;
			height = window.bottom - window.top;
		}
		sizeX = (int)round(LOWORD(lParam) / ((double)(size_field)));
		sizeY = (int)round(HIWORD(lParam) / ((double)(size_field)));
		Resize(sizeX, sizeY);
		return 0;
	}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &paint);
		PaintLine(hwnd,hdc);
		Redraw(hwnd, hdc, hPen, hBrush);
		EndPaint(hwnd, &paint);
		return 0;
	case WM_HOTKEY:
		if (wParam == id1) //ctrl+q
		{
			PostQuitMessage(0);
		}
		else if (wParam == id2) //shift+c
			RunNotepad();
		return 0;
	case WM_LBUTTONDOWN: 
	{
		sizeX = (int)round(clientArea.right / (double(size_field)));
		sizeY = (int)round(clientArea.bottom / (double(size_field)));
		int currentMouseX = GET_X_LPARAM(lParam);
		int currentMouseY = GET_Y_LPARAM(lParam);
		int i = currentMouseY / sizeY;
		int j = currentMouseX / sizeX;
		char* map_sections = GetMapSection();
		if (WaitForSingleObject(semaphore_one_winner, 0) == WAIT_TIMEOUT) return 0;
		else ReleaseSemaphore(semaphore_one_winner, 1, NULL);
		if (WaitForSingleObject(hEvent, 0) == WAIT_TIMEOUT) return 0;
		if (map_sections[i * size_field + j] == -1)
		{
			//if (WaitForSingleObject(semaphore_one_winner, 0) == WAIT_TIMEOUT) return 0;
			//else ReleaseSemaphore(semaphore_one_winner, 1, NULL);
			//if (WaitForSingleObject(hEvent1, 0) == WAIT_TIMEOUT) return 0;
			if (szWinName == "Player 1")
			{
				if (WaitForSingleObject(player1,0)==WAIT_TIMEOUT)
				{
					map_sections[i * size_field + j] = 1;
					SetEvent(player1);
					ResetEvent(player2);
				}
				else
				{
					ResetEvent(hEvent);
					MessageBox(NULL, "Сейчас не ваш ход", "Notification", MB_OK);
					SetEvent(hEvent);
					return 0;
				}

			}
			else if (szWinName == "Player 2")
			{
				if (WaitForSingleObject(player2,0) == WAIT_TIMEOUT)
				{
					map_sections[i * size_field + j] = 0;
					SetEvent(player2);
					ResetEvent(player1);
				}
				else
				{
					ResetEvent(hEvent);
					MessageBox(NULL, "Сейчас не ваш ход", "Notification", MB_OK);
					SetEvent(hEvent);
					return 0;
				}
			}
		}
		SendMessage(HWND_BROADCAST, MESSAGE, 0, lParam);
		EndGameCheck(j, i);
		return 0;
	}
	case WM_KEYUP:
		switch (wParam)
		{
		case 0x31:
			SetThreadPriority(thread_background, THREAD_PRIORITY_IDLE);
			MessageBox(NULL,"IDLE priority","Notification",MB_OK);
			return 0;
		case 0x32:
			SetThreadPriority(thread_background, THREAD_PRIORITY_LOWEST);
			MessageBox(NULL, "LOWEST priority", "Notification", MB_OK);
			return 0;
		case 0x33:
			SetThreadPriority(thread_background, THREAD_PRIORITY_BELOW_NORMAL);
			MessageBox(NULL, "BELOW NORMAL priority", "Notification", MB_OK);
			return 0;
		case 0x34:
			SetThreadPriority(thread_background, THREAD_PRIORITY_NORMAL);
			MessageBox(NULL, "NORMAL priority", "Notification", MB_OK);
			return 0;
		case 0x35:
			SetThreadPriority(thread_background, THREAD_PRIORITY_ABOVE_NORMAL);
			MessageBox(NULL, "ABOVE NORMAL priority", "Notification", MB_OK);
			return 0;
		case 0x36:
			SetThreadPriority(thread_background, THREAD_PRIORITY_HIGHEST);
			MessageBox(NULL, "HIGHEST priority", "Notification", MB_OK);
			return 0;
		case 0x37:
			SetThreadPriority(thread_background, THREAD_PRIORITY_TIME_CRITICAL);
			MessageBox(NULL, "TIME CRITICAL priority", "Notification", MB_OK);
			return 0;
		case VK_ESCAPE: //esc
			PostQuitMessage(0);
			return 0;
		case VK_SPACE:
			if (thread_stop == FALSE)
				SuspendThread(thread_background);
			else
				ResumeThread(thread_background);
			thread_stop = !thread_stop;
			return 0;
		}
		return 0;
	case WM_DESTROY:
		UnregisterHotKey(hwnd, id1);
		UnregisterHotKey(hwnd, id2);
		SendMessage(HWND_BROADCAST, MESSAGE_HOTKEY, 0, 0);
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	semaphore = CreateSemaphore(NULL, 2, 2, _T("semaphore"));
	if (WaitForSingleObject(semaphore, 0) == WAIT_TIMEOUT) return NULL; //если открыто 2 окна то больше не открывать
	int mode = 0,a;
	thread_stop = FALSE;
	loadImage(hwnd);
	MSG message;            /* Here message to the application are saved */
	semaphore_one_winner = CreateSemaphore(NULL, 1, 1, _T("semaphore1"));
	if (GetLastError()==ERROR_ALREADY_EXISTS)          //если он создался снова то создаём семафор второго игрока
		szWinName = (TCHAR*)"Player 2";
	else
		szWinName = (TCHAR*)"Player 1";
	hEvent = CreateEvent(NULL, TRUE, TRUE, _T("event"));
	player1 = CreateEvent(NULL, TRUE, FALSE, _T("player1"));
	player2 = CreateEvent(NULL, TRUE, FALSE, _T("player2"));
	wincl = { 0 };/* Data structure for the windowclass */
	res = CommandLineToArgvW(GetCommandLineW(), &a);
	if (res[1] != NULL)
	{
		mode = atoi((const char*)res[1]);
	}
	switch (mode)
	{
	case 1:
		fstream_in();     //файловые потоки
		break;
	case 2:
		FILE_var_in();    //файловые переменные
		break;
	case 3:
		funcWINAPI_in();  //функции WinAPI
		break;
	case 4:
		MappingFile_in(); //отображение на память
		break;
	default:
		funcWINAPI_in();
	}
	ColorChange(R, G,B);
	wincl.style = CS_VREDRAW | CS_HREDRAW;
	wincl.hInstance = hInstance;
	wincl.lpszClassName = szWinClass;
	wincl.lpfnWndProc = WindowProcedure;                 //функция-обработчик сообщений
	hBrush = CreateSolidBrush(RGB(R, G, B));             //кисть для фона 
	hBrushSection = CreateSolidBrush(RGB(33, 189, 207)); //кисть для крестиков-ноликов по умолчанию
	hPen = CreatePen(PS_SOLID, 2, RGB(24, 240, 60));     //перо для крестиков-ноликов по умолчанию
	wincl.hbrBackground = hBrush;
	/* Register the window class, and if it fails quit the program */
	if (!RegisterClass(&wincl))
		return 0;
	/* The class is registered, let's create the program*/
	hwnd = CreateWindow(
		szWinClass,            /* Classname */
		szWinName,             /* Title Text */
		WS_OVERLAPPEDWINDOW,   /* default window */
		sizeWindowX,           /* Windows decides the position */
		sizeWindowY,           /* where the window ends up on the screen */
		wigth,                 /* The programs width */
		height,                /* and height in pixels */
		HWND_DESKTOP,          /* The window is a child-window to desktop */
		NULL,                  /* No menu */
		hInstance,             /* Program Instance handler */
		NULL                   /* No Window Creation data */
	);
	/* Make the window visible on the screen */
	GetClientRect(hwnd, &clientArea);
	GetWindowRect(hwnd, &rect); sizeWindowX = rect.left; sizeWindowY = rect.top;
	hdc = GetDC(hwnd);
	sizeX = (int)round(clientArea.right / (double(size_field))); //size of a rectangle horizontally
	sizeY = (int)round(clientArea.bottom / (double(size_field))); //size of a rectangle vertically
	/*array rectangle initialization*/
	InitializeLocation(sizeX, sizeY);
	ShowWindow(hwnd, nCmdShow);
	MESSAGE = RegisterWindowMessage(_T("message"));
	MESSAGE_HOTKEY = RegisterWindowMessage(_T("hotkey"));
	MESSAGE_EXIT = RegisterWindowMessage(_T("exit"));
	InitializeMapSection();
	res_regist_hotkey_1 = RegisterHotKey(hwnd, id1, MOD_CONTROL | MOD_NOREPEAT, 0x51);
	res_regist_hotkey_2 = RegisterHotKey(hwnd, id2, MOD_SHIFT | MOD_NOREPEAT, 0x43);
	listArg listArgChangeBackground = {hdc,hBrush,hwnd};
	thread_background = CreateThread(NULL, 0, ChangeBackground, (LPVOID)&listArgChangeBackground, 0,NULL);
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		/* Send message to WindowProcedure */
		DispatchMessage(&message);
	}
	DestroyMapSection();
	DestroyLocation();
	DestroyImage();
	ReleaseSemaphore(semaphore, 1, NULL);
	CloseHandle(semaphore);
	CloseHandle(semaphore_one_winner);
	CloseHandle(hEvent);
	CloseHandle(player1);
	CloseHandle(player2);
	CloseHandle(thread_background);
	LocalFree(res);
	DestroyWindow(hwnd);
	DeleteDC(hdc);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	DeleteObject(hBrushSection);
	UnregisterClass(szWinClass, hInstance);
	return NULL;
}