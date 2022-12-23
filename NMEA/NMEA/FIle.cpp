#include "pch.h"
#include "File.h"
#include <string>
#include <iostream>


#define MAX_PATH 256



CFileRead::CFileRead()
{

}

CFileRead::~CFileRead()
{

}


CString CFileRead::FileDialog()
{
	
	TCHAR szFilter[] = _T("All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())
	{
		FilePathName = dlg.GetPathName();
	}

	return FilePathName;
}


char* CFileRead::FileOpen()
{
	char* Buffer = NULL;
	
	_wfopen_s(&fp, FilePathName, L"rb");

	if (NULL == fp)
	{
		MessageBox(NULL, _T("File Open Fail"), _T("ERROR"), MB_OK);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	Buffer = (char*)malloc(sizeof(char)*size + 1);
	memset(Buffer, 0, sizeof(char)*size + 1);

	if (fread(Buffer, size, 1, fp) < 1)
	{
		free(Buffer);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	

	return Buffer;
}


void CFileRead::FileParse()
{
	int pos = 0;
	char* pDoc = FileOpen();
	std::string str;
	if (NULL == pDoc)
	{
		return;
	}
	while (pos < size)
	{
		if ('$' == pDoc[pos])
		{
			pos++;
			str += pDoc[pos];
			while (pos < size)
			{
				pos++;
				if (',' == pDoc[pos])
				{
					break;
				}
				str += pDoc[pos];
			}
			
			if ("GNRMC" == str)
			{
				str += pDoc[pos];
				while (pDoc[pos] != '\n')
				{
					pos++;
					str += pDoc[pos];
				}
				std::cout << str << std::endl;
			}
			else
			{
				str = "";
			}
		}
		pos++;
	}

	std::cout << "Á¾·á" << std::endl;
}
