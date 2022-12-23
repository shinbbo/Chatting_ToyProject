#pragma once
#include <atlstr.h>
#include <vector>


class CFileRead
{
public:
	CFileRead();
	~CFileRead();


public:
	char* FileOpen();
	CString FileDialog();
	void FileParse();

private:
	FILE* fp;
	CString FilePathName;
	int size;
	std::vector<char> m_vSave;
};