#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <stdio.h>
#include "FileReading.h"
#include <commdlg.h>

static TCHAR szWindowClass[] = _T("FileInTable");
static TCHAR szTitle[] = _T("File content in table");
const int numColumns = 3;
const int numRows = 3;
const int cellWidth = 100;
const int cellHeight = 50;


LRESULT CALLBACK WndProc(_In_ HWND, _In_ UINT, _In_ WPARAM, _In_ LPARAM);

int WINAPI WinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ LPSTR lpCmdLine,
   _In_ int nCmdShow
) {

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if(!RegisterClassEx(&wcex)) {
		MessageBox(NULL,
		   _T("Call to RegisterClassEx failed!"),
		   _T("File in table."),
		   NULL);

		return 1;
	}

	HWND hWnd = CreateWindowEx(
				WS_EX_OVERLAPPEDWINDOW,
				szWindowClass,
				szTitle,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				500, 400,
				NULL,
				NULL,
				hInstance,
				NULL);
	if(!hWnd) {
		MessageBox(NULL,
		   _T("Call to CreateWindowEx failed!"),
		   _T("File in table."),
		   NULL);

		return 1;
	}

	ShowWindow(hWnd,
	   nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

HWND hButtonOpen;
const int openFileButtonWidth = 200;
const int openFileButtonHeight = 60;

LRESULT CALLBACK WndProc(
   _In_ HWND   hWnd,
   _In_ UINT   message,
   _In_ WPARAM wParam,
   _In_ LPARAM lParam
) {
	PAINTSTRUCT ps;
	HDC hdc;
	switch(message) {
		case WM_CREATE:
			{
				hButtonOpen = CreateWindow(
				   L"BUTTON", L"Open",
				   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				   0,
				   0, openFileButtonHeight, openFileButtonHeight,
				   hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL
				);
			}
			break;
		case WM_SIZE:
			{
				RECT clientRect;
				GetClientRect(hWnd, &clientRect);
				int clientWidth = clientRect.right - clientRect.left;
				int clientHeight = clientRect.bottom - clientRect.top;
				if(hButtonOpen == 0) {
					MessageBox(hWnd, L"Button == 0", L"case WM_SIZE", MB_OK);
				}
				MoveWindow(hButtonOpen, clientWidth - openFileButtonWidth, clientHeight - openFileButtonHeight, openFileButtonWidth, openFileButtonHeight, true);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			for(int row = 0; row < numRows; ++row) {
				for(int col = 0; col < numColumns; ++col) {
					RECT cellRect = {
						col * cellWidth,
						row * cellHeight,
						(col + 1) * cellWidth,
						(row + 1) * cellHeight
					};

					DrawEdge(hdc, &cellRect, EDGE_RAISED, BF_RECT);

					wchar_t cellText[16];
					swprintf_s(cellText, L"Cell %d,%d", row, col);
					DrawText(hdc, cellText, -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}

			EndPaint(hWnd, &ps);
			break;
		case WM_COMMAND:
			{
				//int wmId = LOWORD(wParam);
				//switch(wmId) {
					//case IDM_OPEN:
				{
					LPWSTR fileName = new wchar_t[MAX_PATH];
					LPCWSTR filter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";

					if(OpenFile(hWnd, fileName, filter)) {
						MessageBox(hWnd, fileName, L"Selected File", MB_OK);
					}
					else {
						MessageBox(hWnd, L"No file selected.", L"File Selection", MB_OK);
					}
					free(fileName);
					return 0;
				}
				//}
				break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
			}
	}
}