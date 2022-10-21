#pragma once
#include <stdio.h>
#include "MapDataStruct.h"

class CMapFile
{
public:
	CMapFile();
	~CMapFile();

public:
	void FileOpen(CString strPathName1, CString path);
	void MapDataRead(FILE* fp);



private:
	FILE* fp;
	MapData m_MapData;
};
