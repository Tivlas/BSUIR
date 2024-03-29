#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <codecvt>

std::wstring StringToWString(const std::string& str) {
	std::wstring resw;
	for(int i = 0; i < str.size(); ++i) {
		resw += wchar_t(str[i]);
	}
	return resw;
}

std::string WideCharToString(LPWSTR wideCharString) {
	int narrowCharLength = WideCharToMultiByte(CP_UTF8, 0, wideCharString, -1, nullptr, 0, nullptr, nullptr);
	if(narrowCharLength == 0) {
		return "";
	}

	std::string narrowCharString(narrowCharLength, '\0');
	if(WideCharToMultiByte(CP_UTF8, 0, wideCharString, -1, &narrowCharString[0], narrowCharLength, nullptr, nullptr) == 0) {
		return "";
	}

	return narrowCharString;
}

std::vector<std::string> splitSentenceIntoWords(const std::string& str) {
	std::vector<std::string> words;
	std::istringstream iss(str);
	std::string word;

	while(iss >> word) {
		words.push_back(word);
	}

	return words;
}

std::vector<std::vector<std::string>> splitText(const std::string& text) {
	std::vector<std::vector<std::string>> sentences;
	std::istringstream iss(text);
	std::string sentence;

	while(std::getline(iss, sentence, '\n')) {
		sentences.push_back(splitSentenceIntoWords(sentence));
	}

	return sentences;
}

std::string readFile(const std::string& filename) {
	std::ifstream file(filename);
	std::string text;
	if(file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		text = buffer.str();
	}
	else {
		MessageBox(NULL, _T("Cannot read the file!"), _T("readFile."), NULL);
	}
	file.close();
	return text;
}

bool OpenFile(HWND hWnd, LPWSTR fileName, LPCWSTR filter) {
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));
	wchar_t szFileName[MAX_PATH] = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if(GetOpenFileName(&ofn) == TRUE) {
		wcscpy_s(fileName, MAX_PATH, szFileName);
		return true;
	}

	return false;
}