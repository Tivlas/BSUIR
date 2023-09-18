#pragma once
#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <stdio.h>
#include "FileReading.h"

#define FILE_OPEN 1

static TCHAR szWindowClass[] = _T("FileInTable");
static TCHAR szTitle[] = _T("File content in table");
LPWSTR fileName = new wchar_t[MAX_PATH];
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

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	free(fileName);

	return (int)msg.wParam;
}

HWND hButtonOpen;
HWND hListView;
const int openFileButtonWidth = 200;
const int openFileButtonHeight = 60;

LRESULT CALLBACK WndProc(
   _In_ HWND   hWnd,
   _In_ UINT   message,
   _In_ WPARAM wParam,
   _In_ LPARAM lParam
) {
	static bool fileWasOpened = false;
	static std::vector<std::vector<std::string>> sentences;
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
				   hWnd, HMENU(1), (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL
				);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			if(fileWasOpened) {
				int cols = 0;
				for(const auto& sent : sentences) {
					if(sent.size() > cols) {
						cols = sent.size();
					}
				}
				for(int row = 0; row < sentences.size(); ++row) {
					for(int col = 0; col < cols; ++col) {
						RECT cellRect = {
							col * cellWidth,
							row * cellHeight,
							(col + 1) * cellWidth,
							(row + 1) * cellHeight
						};

						DrawEdge(hdc, &cellRect, EDGE_RAISED, BF_RECT);

						wchar_t cellText[MAX_PATH];
						std::wstring format;
						if(sentences[row].size() < cols && col >= sentences[row].size()) {
							format = L"";
						}
						else {
							format = StringToWString(sentences[row][col]);
						}
						swprintf_s(cellText, format.c_str(), row, col);
						DrawText(hdc, cellText, -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					}
				}
			}

			EndPaint(hWnd, &ps);
			break;
		case WM_SIZE:
			{
				RECT clientRect;
				GetClientRect(hWnd, &clientRect);
				int clientWidth = clientRect.right - clientRect.left;
				int clientHeight = clientRect.bottom - clientRect.top;
				MoveWindow(hButtonOpen, clientWidth - openFileButtonWidth - 20, clientHeight - openFileButtonHeight - 20, openFileButtonWidth, openFileButtonHeight, true);
			}
			break;
		case WM_COMMAND:
			{
				switch(LOWORD(wParam)) {
					case FILE_OPEN:
						{
							LPCWSTR filter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";

							if(OpenFile(hWnd, fileName, filter)) {
								fileWasOpened = true;
								auto text = readFile(WideCharToString(fileName));
								sentences = splitText(text);
								InvalidateRect(hWnd, NULL, TRUE);
							}
							else {
								MessageBox(hWnd, L"No file selected.", L"File Selection", MB_OK);
							}
							return 0;
						}
				}
				break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			}
	}
	return 0;
}