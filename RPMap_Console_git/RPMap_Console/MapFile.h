#pragma once
#include <stdio.h>
#include "MapHeaderStruct.h"
#include "MapDataStruct.h"

class CMapFile
{
public:
	CMapFile();
	~CMapFile();

public:
	void FileOpen();
	void MapHeaderRead(FILE* fp);
	void MapDataRead(FILE* fp);



private:
	FILE* fp;
	MapHeader m_MapHeader;
	MapData m_MapData;
};