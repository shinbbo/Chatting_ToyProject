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

	std::vector< GridInfo> m_vGridInfo;							//그리드 정보 vector 저장
	std::vector< RoadDataInfo> m_vRoadDataInfo;		//도로 데이터 vector 저장
	std::vector< LaneDataInfo>m_vLaneDataInfo;			//차로 데이터 vector 저장


private:
	FILE* fp;
	MapData m_MapData;
};
