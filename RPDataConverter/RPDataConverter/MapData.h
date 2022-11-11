#pragma once
#include <stdio.h>
#include <vector>
#include "MapDataStruct.h"

class CMapFile
{
public:
	CMapFile();
	~CMapFile();

public:
	void FileOpen(CString strPathName1, CString path);
	void MapDataRead(FILE* fp);
	MapData* pMapDataRead(FILE* fp);
	
	std::vector<CString> getGridIdList();
	std::vector< GridInfo> getGridInfo();
	std::vector<CString> getFilePath();
	std::vector<int> getFileSize();
	FILE* getFilePoint();
	

	MapData* data;
private:
	FILE* fp;
	MapData m_MapData;
	CString filePath;
	std::vector<CString> m_vGridID;
	std::vector< GridInfo> m_vGridInfo;							//�׸��� ���� vector ����
	std::vector<CString> m_vFilePath;
	std::vector<int> m_vFileSize;
};
