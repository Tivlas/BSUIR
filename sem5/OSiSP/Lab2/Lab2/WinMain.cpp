#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>
#include <wchar.h>
#include <tchar.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#define PI acos(-1)
#define ID_CHECK_BOX 228
#define WS_RADIOBUTTON_PARAMS WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON
CONST WCHAR* fileName = L"log.txt";
CONST long long FILESIZE = 1024 * 1024;

const wchar_t* colors[3] = {
	L"Red",
	L"Green",
	L"Blue"
};

const wchar_t* figures[4] = {
	L"Star",
	L"Circle",
	L"Rhombus",
	L"Rectangle"
};

const int colors_id[3] = {
	101,
	102,
	103
};

const int figures_id[4] = {
	104,
	105,
	106,
	107
};

int RGB[3];
int x_coord = 0, y_coord = 0;
HANDLE hFile = NULL;
HANDLE hMapFile = NULL;
LPVOID pMappedData = NULL;
size_t mappedDataSize = 0;

HWND check_box,
child_hwnd,
color_radio_buttons[3],
figure_radio_buttons[4];

HWND hwnd_main;

HINSTANCE hInst;

HHOOK mouseHook = NULL;

void UninitializeMappingFile() {
	if(pMappedData != NULL) {
		UnmapViewOfFile(pMappedData);
		pMappedData = NULL;
	}
	if(hMapFile != NULL) {
		CloseHandle(hMapFile);
		hMapFile = NULL;
	}
	if(hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
}

VOID InitializeMappingFile() {
	hFile = CreateFile(
		fileName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if(hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, L"CreateFile failed!", L"Error", MB_ICONERROR);
		return;
	}
	hMapFile = CreateFileMapping(
		hFile,
		NULL,
		PAGE_READWRITE,
		0,
		FILESIZE,
		NULL
	);

	if(hMapFile == NULL) {
		MessageBox(NULL, L"CreateFileMapping failed!", L"Error", MB_ICONERROR);
		CloseHandle(hFile);
		return;
	}
	pMappedData = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, FILESIZE);
	if(pMappedData == NULL) {
		MessageBox(NULL, L"MapViewOfFile failed!", L"Error", MB_ICONERROR);
		CloseHandle(hMapFile);
		CloseHandle(hFile);
		return;
	}
}

void RecordPress(const std::string& actionString) {
	if(pMappedData != NULL) {
		SYSTEMTIME currentTime;
		GetLocalTime(&currentTime);
		std::ostringstream os;
		os << "[" << std::setw(2) << std::setfill('0') << std::to_string(currentTime.wHour) << ":" <<
			std::setw(2) << std::setfill('0') << std::to_string(currentTime.wMinute) << ":" << std::setw(2) << std::setfill('0') << std::to_string(currentTime.wSecond);
		std::string time = os.str();
		std::string keyInfo = time + "] - " + actionString + "\n";
		SIZE_T dataSize = keyInfo.size() * sizeof(CHAR);
		OutputDebugString(std::to_wstring(dataSize).c_str());
		OutputDebugString(L"\n");
		if(mappedDataSize + dataSize >= FILESIZE) {
			UninitializeMappingFile();
			InitializeMappingFile();
			mappedDataSize = 0;
		}
		memcpy((CHAR*)pMappedData + mappedDataSize, keyInfo.c_str(), dataSize);
		mappedDataSize += dataSize;
	}
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if(nCode == HC_ACTION) {
		if(wParam == WM_LBUTTONDOWN) {
			OutputDebugString(L"Clicked\n");
			RecordPress("Clicked");
		}
	}
	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void DrawFigure(HDC hdc, int type) {
	HBRUSH hBrush = CreateSolidBrush(RGB(RGB[0], RGB[1], RGB[2]));
	SelectObject(hdc, hBrush);
	switch(type) {
		case 0:
			{
				POINT point[5];
				for(int i = 0; i < 5; ++i) {
					point[i].x = x_coord + sin(2 * PI / 5 * (i * 2 % 5)) * 30;
					point[i].y = y_coord - cos(2 * PI / 5 * (i * 2 % 5)) * 30;
				}
				Polygon(hdc, point, 5);
				break;
			}
		case 1:
			{
				Ellipse(hdc, x_coord - 30, y_coord - 30, x_coord + 30, y_coord + 30);
				break;
			}
		case 2:
			{
				POINT point[4];
				point[0].x = x_coord - 30;
				point[0].y = y_coord;
				point[1].x = x_coord;
				point[1].y = y_coord - 30;
				point[2].x = x_coord + 30;
				point[2].y = y_coord;
				point[3].x = x_coord;
				point[3].y = y_coord + 30;
				Polygon(hdc, point, 4);
				break;
			}
		case 3:
			{
				Rectangle(hdc, x_coord - 30, y_coord - 30, x_coord + 30, y_coord + 30);
				break;
			}
	}
}

LRESULT CALLBACK WndProcChild(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch(msg) {
		case WM_LBUTTONDOWN:
			{
				x_coord = LOWORD(lParam);
				y_coord = HIWORD(lParam);
				InvalidateRect(child_hwnd, 0, 0);
				break;
			}
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				if(SendMessage(check_box, BM_GETCHECK, 1, NULL)) {
					for(int i = 0; i < 3; ++i) {
						if(SendMessage(color_radio_buttons[i], BM_GETCHECK, 1, 0)) {
							RGB[i] = 255;
						}
						else {
							RGB[i] = 0;
						}
					}
					for(int i = 0; i < 4; ++i) {
						if(SendMessage(figure_radio_buttons[i], BM_GETCHECK, 1, NULL)) {
							DrawFigure(hdc, i);
						}
					}
				}
				EndPaint(hwnd, &ps);
				break;
			}
		case WM_CLOSE:
			{
				SendMessage(hwnd_main, msg, 0, NULL);
				break;
			}
		default:
			{
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
	}
}

LRESULT CALLBACK WndProc(
	HWND   hwnd,
	UINT   msg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch(msg) {
		case WM_CREATE:
			{
				WNDCLASS wc;
				memset(&wc, 0, sizeof(WNDCLASS));
				wc.style = 0;
				wc.hInstance = hInst;
				wc.lpfnWndProc = WndProcChild;
				wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
				wc.lpszClassName = L"C Windows";
				wc.hCursor = LoadCursor(NULL, IDC_ARROW);
				RegisterClass(&wc);
				child_hwnd = CreateWindow(
					L"C Windows", L"Lab2. Painting.",
					WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
					CW_USEDEFAULT, CW_USEDEFAULT, 600, 480, NULL, NULL, hInst, NULL
				);
				ShowWindow(child_hwnd, SW_NORMAL);
				UpdateWindow(child_hwnd);
				if(child_hwnd == NULL) {
					MessageBox(hwnd, L"Can't create child window.", L"Error!", MB_ICONERROR | MB_OK);
				}
				for(size_t i = 0; i < 3; ++i) {
					color_radio_buttons[i] = CreateWindow(
						L"BUTTON", colors[i], WS_RADIOBUTTON_PARAMS,
						10, 10 + 15 * (i + 1), 100, 20, hwnd, (HMENU)colors_id[i], NULL, NULL
					);
				}
				for(size_t i = 0; i < 4; ++i) {
					figure_radio_buttons[i] = CreateWindow(
						L"BUTTON", figures[i], WS_RADIOBUTTON_PARAMS,
						10, 65 + 15 * (i + 1), 100, 20, hwnd, (HMENU)figures_id[i], NULL, NULL
					);
				}

				check_box = CreateWindow(
					L"BUTTON", L"Active", WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					10, 160, 100, 20, hwnd, (HMENU)ID_CHECK_BOX, NULL, NULL
				);
				break;
			}
		case WM_COMMAND:
			{
				int target_id = LOWORD(wParam);
				if(target_id == ID_CHECK_BOX) {

					bool status = SendDlgItemMessage(hwnd, ID_CHECK_BOX, BM_GETCHECK, 0, 0);
					SendMessage(check_box, BM_SETCHECK, status ^ true, NULL);

				}
				else if(target_id >= 101 && target_id <= 103) {

					for(int i = 0; i < 3; ++i) {
						SendMessage((HWND)color_radio_buttons[i], BM_SETCHECK, 0, NULL);
					}
					SendMessage(color_radio_buttons[target_id - 101], BM_SETCHECK, 1, NULL);

				}
				else if(target_id >= 104 && target_id <= 107) {

					for(int i = 0; i < 4; ++i) {
						SendMessage((HWND)figure_radio_buttons[i], BM_SETCHECK, 0, NULL);
					}
					SendMessage((HWND)figure_radio_buttons[target_id - 104], BM_SETCHECK, 1, NULL);
				}
				break;
			}
		case WM_CLOSE:
			{
				DestroyWindow(hwnd);
				break;
			}
		case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}
		default:
			{
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
	}
}

void SetKeyboardHook() {
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);
	if(mouseHook == NULL) {
		MessageBox(NULL, L"Failed to set mouse hook", L"Error", MB_ICONERROR);
	}
}

void UnhookKeyboardHook() {
	if(mouseHook != NULL) {
		UnhookWindowsHookEx(mouseHook);
		mouseHook = NULL;
	}
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
) {

	MSG        msg;
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"WindowClass";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Window registration is failed!", L"Error!", MB_ICONWARNING | MB_OK);
		return 0;
	}
	hwnd_main = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"WindowClass",
		L"Osisp",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 180, 250,
		NULL, NULL, hInstance, NULL
	);
	if(hwnd_main == NULL) {
		MessageBox(NULL, L"Window creation is failed!", L"Error!", MB_ICONWARNING | MB_OK);
		return 0;
	}
	hInst = hInstance;
	InitializeMappingFile();
	SetKeyboardHook();
	ShowWindow(hwnd_main, nCmdShow);
	UpdateWindow(hwnd_main);
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookKeyboardHook();
	UninitializeMappingFile();
	return msg.wParam;
}