#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <CommCtrl.h>
#include <future>
#include <thread>
#include <chrono>

constexpr int IDC_OPEN_BUTTON = 101;

HWND g_hWnd = nullptr;

//void ProcessFileContent(const std::vector<char>& buffer, HWND hWnd) {
//	std::wstring message(buffer.begin(), buffer.end());
//	MessageBox(hWnd, message.c_str(), L"Содержимое файла", MB_OK);
//}

void ProcessFileContent(const char* buffer, DWORD bytesRead, HWND hWnd) {
	std::string content(buffer, bytesRead);
	std::wstring wideContent(content.begin(), content.end());
	MessageBox(hWnd, wideContent.c_str(), L"File Content", MB_OK | MB_ICONINFORMATION);
}

void ReadFileAsync(const std::wstring& filePath, HWND hWnd, std::function<void(HWND)> callback) {
	//HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	//if(hFile == INVALID_HANDLE_VALUE) {
	//	MessageBox(hWnd, L"Ошибка при открытии файла.", L"Ошибка", MB_OK | MB_ICONERROR);
	//	return;
	//}

	//LARGE_INTEGER fileSize;
	//GetFileSizeEx(hFile, &fileSize);

	//auto numThreads = std::thread::hardware_concurrency();
	//numThreads = 1;
	//auto numThreadsStr = std::to_wstring(numThreads);
	////MessageBox(hWnd, numThreadsStr.c_str(), L"Elapsed Time", MB_OK | MB_ICONINFORMATION);

	//std::streampos partSize = fileSize.QuadPart / numThreads;

	//auto startTime = std::chrono::high_resolution_clock::now();
	//std::vector<std::thread> threads;
	//for(int i = 0; i < numThreads; ++i) {
	//	std::streampos start = i * partSize;
	//	std::streampos end = (i == numThreads - 1) ? static_cast<std::streampos>(fileSize.QuadPart) : start + partSize;
	//	threads.emplace_back([start, end, &hWnd, &hFile]() {
	//		std::streamsize bufferSize = static_cast<std::streamsize>(end - start);
	//		std::vector<char> buffer(bufferSize);

	//		OVERLAPPED overlapped;
	//		ZeroMemory(&overlapped, sizeof(overlapped));
	//		overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//		overlapped.Offset = start & 0xFFFFFFFF;
	//		overlapped.OffsetHigh = (start >> 32) & 0xFFFFFFFF;

	//		DWORD bytesRead;
	//		if(!ReadFile(hFile, buffer.data(), static_cast<DWORD>(bufferSize), &bytesRead, &overlapped)) {
	//			if(GetLastError() != ERROR_IO_PENDING) {
	//				MessageBox(hWnd, L"Ошибка при чтении файла.", L"Ошибка", MB_OK | MB_ICONERROR);
	//				CloseHandle(hFile);
	//				return;
	//			}
	//		}

	//		DWORD result = WaitForSingleObject(overlapped.hEvent, INFINITE);
	//		if(result != WAIT_OBJECT_0) {
	//			MessageBox(hWnd, L"Ошибка при ожидании завершения операции чтения.", L"Ошибка", MB_OK | MB_ICONERROR);
	//			CloseHandle(hFile);
	//			return;
	//		}

	//		//ProcessFileContent(buffer, hWnd);
	//	});
	//}
	//for(auto& thread : threads) {
	//	thread.join();
	//}
	//CloseHandle(hFile);
	//auto endTime = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::microseconds> (endTime - startTime);
	//auto durationStr = std::to_wstring(duration.count());
	//MessageBox(hWnd, durationStr.c_str(), L"Elapsed Time", MB_OK | MB_ICONINFORMATION);
	//callback(hWnd);

	HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if(hFile == INVALID_HANDLE_VALUE) {
		MessageBox(hWnd, L"Ошибка при открытии файла.", L"Ошибка", MB_OK | MB_ICONERROR);
		return;
	}

	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize);

	//const DWORD bufferSize = 1048576;  // Размер буфера для чтения
	//const DWORD bufferSize = 268435456;  // Размер буфера для чтения
	const DWORD bufferSize = 268435456 * 2 * 2 * 2;  // Размер буфера для чтения

	std::vector<std::vector<char>> buffer(fileSize.QuadPart / bufferSize + 1, std::vector<char>(bufferSize));

	std::vector<OVERLAPPED> overlappeds;
	std::vector<HANDLE> events;

	for(DWORD i = 0; i < fileSize.QuadPart / bufferSize + 1; ++i) {
		OVERLAPPED overlapped;
		ZeroMemory(&overlapped, sizeof(overlapped));
		overlapped.Offset = i * bufferSize;
		events.push_back(CreateEvent(NULL, TRUE, FALSE, NULL));
		overlapped.hEvent = events.back();
		overlappeds.push_back(overlapped);
	}

	auto startTime = std::chrono::high_resolution_clock::now();

	DWORD bytesRead = 0;

	for(DWORD i = 0; i < overlappeds.size(); ++i) {
		if(!ReadFile(hFile, buffer[i].data(), bufferSize, &bytesRead, &overlappeds[i])) {
			if(GetLastError() != ERROR_IO_PENDING) {
				MessageBox(hWnd, L"Ошибка при чтении файла.", L"Ошибка", MB_OK | MB_ICONERROR);
				CloseHandle(hFile);
				return;
			}
		}
	}

	for(auto& event : events) {
		DWORD result = WaitForSingleObject(event, INFINITE);
		if(result != WAIT_OBJECT_0) {
			MessageBox(hWnd, L"Ошибка при ожидании завершения операции чтения.", L"Ошибка", MB_OK | MB_ICONERROR);
			CloseHandle(hFile);
			return;
		}
	}
	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
	auto durationStr = std::to_wstring(duration.count());
	MessageBox(hWnd, durationStr.c_str(), L"Elapsed Time", MB_OK | MB_ICONINFORMATION);
	int i = 0;
	/*for(auto& overlapped : overlappeds) {
		DWORD bytesRead;
		GetOverlappedResult(hFile, &overlapped, &bytesRead, TRUE);
		ProcessFileContent(buffer[i++].data(), bytesRead, hWnd);
	}*/

	CloseHandle(hFile);

	callback(hWnd);
}

void FileProcessingCompleteCallback(HWND hWnd) {
	MessageBox(hWnd, L"Обработка файла завершена!", L"Готово", MB_OK | MB_ICONINFORMATION);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
		case WM_CREATE:
			{
				CreateWindowW(L"BUTTON", L"Открыть", WS_VISIBLE | WS_CHILD, 10, 10, 100, 30, hwnd, (HMENU)IDC_OPEN_BUTTON, nullptr, nullptr);
				break;
			}
		case WM_COMMAND:
			{
				if(LOWORD(wParam) == IDC_OPEN_BUTTON && HIWORD(wParam) == BN_CLICKED) {
					OPENFILENAME ofn;
					wchar_t szFileName[MAX_PATH] = L"";

					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hwnd;
					ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
					ofn.lpstrFile = szFileName;
					ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
					ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

					if(GetOpenFileName(&ofn)) {
						HANDLE hFile;
						std::vector<char> buffer;
						std::wstring filePath(szFileName);
						std::thread readFileThread(ReadFileAsync, filePath, hwnd, FileProcessingCompleteCallback);
						readFileThread.detach();
					}
				}

				break;
			}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

// Получение класса окна
ATOM RegisterWindowClass(HINSTANCE hInstance) {
	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"AsyncFileProcessingWindowClass";

	return RegisterClassEx(&wc);
}

// Создание и отображение окна
HWND CreateAndShowWindow(HINSTANCE hInstance, int nCmdShow) {
	HWND hWnd = CreateWindowEx(0, L"AsyncFileProcessingWindowClass", L"Асинхронная обработка файла",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, nullptr, nullptr, hInstance, nullptr);

	if(!hWnd) {
		return nullptr;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

// Точка входа приложения
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, PWSTR /*lpCmdLine*/, int nCmdShow) {
	if(RegisterWindowClass(hInstance)) {
		g_hWnd = CreateAndShowWindow(hInstance, nCmdShow);

		if(g_hWnd) {
			MSG msg = {};

			while(GetMessage(&msg, nullptr, 0, 0)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	return 0;
}

