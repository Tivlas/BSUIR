#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <d3d11.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// Function to retrieve GPU information
std::string GetGPUInfo() {
	std::string gpuInfo;

	IDXGIFactory* pFactory;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if(SUCCEEDED(hr)) {
		UINT adapterIndex = 0;
		IDXGIAdapter* pAdapter;
		while(pFactory->EnumAdapters(adapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
			DXGI_ADAPTER_DESC adapterDesc;
			pAdapter->GetDesc(&adapterDesc);
			std::wstring gpuNameWide(adapterDesc.Description);
			std::string gpuName(gpuNameWide.begin(), gpuNameWide.end());

			gpuInfo += "GPU: " + gpuName + "\n";

			pAdapter->Release();
			++adapterIndex;
		}

		pFactory->Release();
	}

	return gpuInfo;
}



std::string GetProcessorName() {
	HKEY hKey;
	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if(result != ERROR_SUCCESS) {
		return "Unknown";
	}

	char processorName[255];
	DWORD dataSize = sizeof(processorName);
	result = RegQueryValueEx(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)processorName, &dataSize);
	RegCloseKey(hKey);

	if(result != ERROR_SUCCESS) {
		return "Unknown";
	}

	return processorName;
}

// Функция для получения информации о системе
std::string GetSystemInformation() {
	std::string systemInfo;


	// Get processor information
	SYSTEM_INFO systemInfoStruct;
	GetSystemInfo(&systemInfoStruct);
	std::string processorName = GetProcessorName();
	systemInfo += "Processor: " + processorName + "\n";

	std::string gpuInfo = GetGPUInfo();
	if(!gpuInfo.empty()) {
		systemInfo += gpuInfo;
	}


	MEMORYSTATUSEX memoryStatus;
	systemInfo += "Number of processors: " + std::to_string(systemInfoStruct.dwNumberOfProcessors) + "\n";
	// Get memory information
	memoryStatus.dwLength = sizeof(memoryStatus);
	GlobalMemoryStatusEx(&memoryStatus);
	systemInfo += "Physical Memory: ";
	systemInfo += std::to_string(memoryStatus.ullTotalPhys / (1024 * 1024)) + " MB\n";
	systemInfo += "Virtual Memory: ";
	systemInfo += std::to_string(memoryStatus.ullTotalVirtual / (1024 * 1024)) + " MB\n";

	// Get disk information
	DWORD drivesMask = GetLogicalDrives();
	char drive = 'A';
	while(drivesMask) {
		if(drivesMask & 1) {
			std::string drivePath = std::string(1, drive) + ":\\";
			ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
			if(GetDiskFreeSpaceEx(drivePath.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
				systemInfo += "Drive ";
				systemInfo += drivePath;
				systemInfo += " - Free Space: ";
				systemInfo += std::to_string(totalNumberOfFreeBytes.QuadPart / (1024 * 1024)) + " MB\n";
				systemInfo += "Drive ";
				systemInfo += drivePath;
				systemInfo += " - Total Size: ";
				systemInfo += std::to_string(totalNumberOfBytes.QuadPart / (1024 * 1024)) + " MB\n";
			}
		}
		drivesMask >>= 1;
		drive++;
	}
	return systemInfo;
}

int main() {
	std::string systemInfo = GetSystemInformation();
	std::cout << systemInfo << std::endl;
	return 0;
}