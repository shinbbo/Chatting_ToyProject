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
	void FileOpen(CString strPathName1, CString path, int i);
	void setGridCnt(int cnt);
	void pMapDataRead(FILE* fp, int num);
	//void MapDataRead(FILE* fp);

	std::vector<CString> getGridIdList();
	std::vector< GridInfo> getGridInfo();
	std::vector<CString> getFilePath();
	std::vector<int> getFileSize();
	FILE* getFilePoint();
	MapData* getMapData();
	
private:
	FILE* fp;
	MapData m_MapData;
	CString filePath;
	std::vector<CString> m_vGridID;
	std::vector< GridInfo> m_vGridInfo;							//그리드 정보 vector 저장
	std::vector<CString> m_vFilePath;
	std::vector<int> m_vFileSize;
	int m_nGridCnt;
	MapData* m_ppd;
};
