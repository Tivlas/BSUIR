#include <windows.h>
#include <stdio.h>
#include "KWnd.h"
#define MAX_STR_SIZE 80
#define MAX_N 10 
int N = 0;
HANDLE hEvtRecToServ[MAX_N];
char eventName[MAX_N][MAX_STR_SIZE + 8];
HANDLE hEvtServIsExist;
HANDLE hEvtServIsFree;
HANDLE hEvtServIsDone;
HANDLE hFileMap = NULL;
struct ThreadManager {
	HWND hwndParent;
};
ThreadManager tm;
DWORD WINAPI ThreadFunc(LPVOID);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//====================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
 LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	KWnd mainWnd("ServerApp", hInstance, nCmdShow, WndProc,
	NULL, 100, 100, 450, 100);
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC;
	PAINTSTRUCT ps;
	COPYDATASTRUCT* pCds;
	COPYDATASTRUCT cds;
	static char request[MAX_STR_SIZE];
	char suffix[8];
	HWND hwndClient;
	static HANDLE hThread;
	HWND* pHwnd = &hWnd;
	int i;
	switch(uMsg) {
		case WM_CREATE:
			hEvtServIsExist = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Server");
			if(hEvtServIsExist) {
				MessageBox(hWnd, "Server is already running, second instance started.",
				"ServerApp", MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL);
				PostQuitMessage(0);
			}
			else
				hEvtServIsExist = CreateEvent(NULL, FALSE, FALSE, "Server");
			break;
		case WM_COPYDATA:
			if(N == MAX_N) {
				MessageBox(hWnd, "Server is overloaded. Access denied!", "ServerApp", MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL);
				break;
			}
			pCds = (COPYDATASTRUCT*)lParam;
			strncpy_s(request, (char*)pCds->lpData, pCds->cbData);
			strcpy_s(eventName[N], request);
			sprintf_s(suffix, "_%d", N);
			strcat_s(eventName[N], suffix);
			hEvtRecToServ[N] = CreateEvent(NULL, FALSE, FALSE, eventName[N]);
			if(!N) {
				for(i = 0; i < MAX_N; ++i) {
					hEvtRecToServ[i] = hEvtRecToServ[0];
				}
				hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4 * 1024, "SharedData");
				hEvtServIsFree = CreateEvent(NULL, FALSE, TRUE, "ServerIsFree");
				hEvtServIsDone = CreateEvent(NULL, FALSE, FALSE, "ServerIsDone");
				tm.hwndParent = hWnd;
				hThread = CreateThread(NULL, 0, ThreadFunc, &tm, 0, NULL);
			}
			hwndClient = FindWindow(NULL, request);
			cds.dwData = N;
			cds.lpData = &eventName[N];
			cds.cbData = strlen(eventName[N]);
			SendMessage(hwndClient, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
			if(N < MAX_N) N++;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			if(N) {
				char text[40];
				sprintf_s(text, "Number of clients: %d", N);
				TextOut(hDC, 10, 20, text, strlen(text));
			}
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			CloseHandle(hFileMap);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}


DWORD WINAPI ThreadFunc(LPVOID lpv) {
	ThreadManager* pTm = (ThreadManager*)lpv;
	HWND hParent = pTm->hwndParent;
	static PVOID pView;
	char suffix[40];
	int k;
	char text[100];
	DWORD dw;
	while(1) {
		dw = WaitForMultipleObjects(MAX_N, hEvtRecToServ, FALSE, INFINITE);
		switch(dw) {
			case WAIT_FAILED:
				MessageBox(hParent, "Error in call of WaitForMultipleObjects", "ServerApp", MB_OK);
				return 0;
			default:
				k = dw - WAIT_OBJECT_0;
				pView = MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 0);
				strcpy_s(text, (PTSTR)pView);
				sprintf_s(suffix, " - %s\0", eventName[k]);
				strcat_s(text, suffix);
				strcpy((PTSTR)pView, text);
				UnmapViewOfFile(pView);
				SetEvent(hEvtServIsDone);
				break;
		}
	}
	return 0;
}