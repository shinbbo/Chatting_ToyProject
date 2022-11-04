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

	std::vector< GridInfo> m_vGridInfo;							//�׸��� ���� vector ����
	std::vector< RoadDataInfo> m_vRoadDataInfo;		//���� ������ vector ����
	std::vector< LaneDataInfo>m_vLaneDataInfo;			//���� ������ vector ����


private:
	FILE* fp;
	MapData m_MapData;
};
