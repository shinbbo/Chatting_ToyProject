#include "pch.h"
#include <iostream>
#include <cstdio>
#include "MapData.h"


CMapFile::CMapFile()
{
	data = new MapData;
}
CMapFile::~CMapFile()
{
	m_vGridID.clear();
	
}

void CMapFile::FileOpen(CString strPathName1, CString path)
{
	filePath = strPathName1 + path;
	m_vGridID.push_back(path);
	m_vFilePath.push_back(filePath);
	
	_wfopen_s(&fp, filePath, L"rb");
	if (NULL == fp)
	{
		printf("File Open Fail\n");
		return;
	}

	data = pMapDataRead(fp);

	fclose(fp);
}


/*
void CMapFile::MapDataRead(FILE* fp)
{
	//그리드 정보 read & vector 저장
	fread(&m_MapData.stGridInfo, sizeof(GridInfo), 1, fp);
	m_vGridInfo.push_back(m_MapData.stGridInfo);
	
	//객체 종류 테이블 read
	fread(&m_MapData.stObjectTable, sizeof(ObjectTable), 1, fp);

	//객체 종류 정보 read
	fread(&m_MapData.stRoadObjData.stObjTypeInfo, sizeof(ObjTypeInfo), 1, fp);


	//도로객체의 수
	UINT NumOfRoad = m_MapData.stGridInfo.iNumOfRoad;

	//도로 옵셋 테이블 동적 할당
	m_MapData.stRoadObjData.pstRoadOffsetTable = (RoadOffsetTable*)malloc(sizeof(RoadOffsetTable) * NumOfRoad);
	memset(m_MapData.stRoadObjData.pstRoadOffsetTable, 0, sizeof(RoadOffsetTable) * NumOfRoad);

	for (int i = 0; i < NumOfRoad; i++)
	{
		//도로 옵셋 테이블 read
		fread(&m_MapData.stRoadObjData.pstRoadOffsetTable[i], sizeof(RoadOffsetTable), 1, fp);
	}

	m_MapData.stRoadObjData.pstRoadData = (RoadData*)malloc(sizeof(RoadData) * NumOfRoad);
	memset(m_MapData.stRoadObjData.pstRoadData, 0, sizeof(RoadData) * NumOfRoad);

	for (int i = 0; i < NumOfRoad; i++)
	{
		//도로 데이터 read & 도로 데이터 vector 저장
		fread(&m_MapData.stRoadObjData.pstRoadData[i].stRoadDataInfo, sizeof(RoadDataInfo), 1, fp);

		//진입 도로 개수
		int8_t PrevRoadNum = m_MapData.stRoadObjData.pstRoadData[i].stRoadDataInfo.nPrevRoadNum;

		//진입 도로 연결성 데이터 동적할당
		m_MapData.stRoadObjData.pstRoadData[i].pstPrevConnectData = (PrevConnectData*)malloc(sizeof(PrevConnectData) * PrevRoadNum);
		memset(m_MapData.stRoadObjData.pstRoadData[i].pstPrevConnectData, 0, sizeof(PrevConnectData) * PrevRoadNum);

		for (int j = 0; j < PrevRoadNum; j++)
		{
			//진입 도로 연결성 데이터 read
			fread(&m_MapData.stRoadObjData.pstRoadData[i].pstPrevConnectData[j], sizeof(PrevConnectData), 1, fp);
		}

		//진출 도로 개수
		int8_t NextRoadNum = m_MapData.stRoadObjData.pstRoadData[i].stRoadDataInfo.nNextRoadNum;

		//진출 도로 연결성 데이터 동적할당
		m_MapData.stRoadObjData.pstRoadData[i].pstNextConnectData = (NextConnectData*)malloc(sizeof(NextConnectData) * NextRoadNum);
		memset(m_MapData.stRoadObjData.pstRoadData[i].pstNextConnectData, 0, sizeof(NextConnectData) * NextRoadNum);

		for (int j = 0; j < NextRoadNum; j++)
		{
			//진출 도로 연결성 데이터 read
			fread(&m_MapData.stRoadObjData.pstRoadData[i].pstNextConnectData[j], sizeof(NextConnectData), 1, fp);
		}

		//도로 내 차로 개수
		int8_t LaneNum = m_MapData.stRoadObjData.pstRoadData[i].stRoadDataInfo.nLaneNum;

		//도로 내 차로 ID 리스트 동적할당
		m_MapData.stRoadObjData.pstRoadData[i].pstRoadIDList = (RoadIDList*)malloc(sizeof(RoadIDList) * LaneNum);
		memset(m_MapData.stRoadObjData.pstRoadData[i].pstRoadIDList, 0, sizeof(RoadIDList) * LaneNum);

		for (int j = 0; j < LaneNum; j++)
		{
			//도로 내 차로 ID리스트 read
			fread(&m_MapData.stRoadObjData.pstRoadData[i].pstRoadIDList[j], sizeof(RoadIDList), 1, fp);
		}
	}


	//차로 객체 종류 정보read
	fread(&m_MapData.stLaneObjData.stLaneObjTypeInfo, sizeof(LaneObjTypeInfo), 1, fp);

	//차로 옵셋 테이블 개수
	UINT NumOfLane = m_MapData.stGridInfo.iNumOfLane;
	m_MapData.stLaneObjData.pstLaneOffsetTable = (LaneOffsetTable*)malloc(sizeof(LaneOffsetTable) * NumOfLane);
	memset(m_MapData.stLaneObjData.pstLaneOffsetTable, 0, sizeof(LaneOffsetTable) * NumOfLane);

	for (int i = 0; i < NumOfLane; i++)
	{
		//차로 옵셋 테이블 read
		fread(&m_MapData.stLaneObjData.pstLaneOffsetTable[i], sizeof(LaneOffsetTable), 1, fp);
	}

	m_MapData.stLaneObjData.pstLaneData = (LaneData*)malloc(sizeof(LaneData) * NumOfLane);
	memset(m_MapData.stLaneObjData.pstLaneData, 0, sizeof(LaneData) * NumOfLane);

	for (int i = 0; i < NumOfLane; i++)
	{
		//차로 데이터 시작 위치로 fseek
		fseek(fp, m_MapData.stLaneObjData.pstLaneOffsetTable[i].iStart, SEEK_SET);

		//차로 데이터 read 
		fread(&m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo, sizeof(LaneDataInfo), 1, fp);

		//진입 차로 연결성 데이터 개수
		int8_t PrevLaneNum = m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo.nPrevLaneNum;

		//진입 차로 연결성 데이터 동적할당
		m_MapData.stLaneObjData.pstLaneData[i].pstPrevLaneConnectData = (PrevLaneConnectData*)malloc(sizeof(PrevLaneConnectData) * PrevLaneNum);
		memset(m_MapData.stLaneObjData.pstLaneData[i].pstPrevLaneConnectData, 0, sizeof(PrevLaneConnectData) * PrevLaneNum);

		for (int j = 0; j < PrevLaneNum; j++)
		{
			fread(&m_MapData.stLaneObjData.pstLaneData[i].pstPrevLaneConnectData[j], sizeof(PrevLaneConnectData), 1, fp);
		}

		//진출 차로 연결성 데이터 개수
		int8_t NextLaneNum = m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo.nNextLaneNum;

		//진출 차로 연결성 데이터 동적할당
		m_MapData.stLaneObjData.pstLaneData[i].pstNextLaneConnectData = (NextLaneConnectData*)malloc(sizeof(NextLaneConnectData) * NextLaneNum);
		memset(m_MapData.stLaneObjData.pstLaneData[i].pstNextLaneConnectData, 0, sizeof(NextLaneConnectData) * NextLaneNum);

		for (int j = 0; j < NextLaneNum; j++)
		{
			fread(&m_MapData.stLaneObjData.pstLaneData[i].pstNextLaneConnectData[j], sizeof(NextLaneConnectData), 1, fp);
		}

		//좌측 차선ID 개수
		int8_t LeftLineNum = m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo.nLeftLineNum;

		//좌측 차선ID 동적할당
		m_MapData.stLaneObjData.pstLaneData[i].pstLeftLaneData = (LeftLaneData*)malloc(sizeof(LeftLaneData) * LeftLineNum);
		memset(m_MapData.stLaneObjData.pstLaneData[i].pstLeftLaneData, 0, sizeof(LeftLaneData) * LeftLineNum);

		for (int j = 0; j < LeftLineNum; j++)
		{
			fread(&m_MapData.stLaneObjData.pstLaneData[i].pstLeftLaneData[j], sizeof(LeftLaneData), 1, fp);
		}


		//우측 차선ID 개수
		int8_t RightLineNum = m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo.nRightLineNum;

		//우측 차선ID 동적할당
		m_MapData.stLaneObjData.pstLaneData[i].pstRigthLaneData = (RigthLaneData*)malloc(sizeof(RigthLaneData) * RightLineNum);
		memset(m_MapData.stLaneObjData.pstLaneData[i].pstRigthLaneData, 0, sizeof(RigthLaneData) * RightLineNum);

		for (int j = 0; j < RightLineNum; j++)
		{
			fread(&m_MapData.stLaneObjData.pstLaneData[i].pstRigthLaneData[j], sizeof(RigthLaneData), 1, fp);
		}
	}
}*/



MapData* CMapFile::pMapDataRead(FILE* fp)
{
	MapData* ppd = (MapData*)malloc(sizeof(MapData));
	//그리드 정보 read & vector 저장
	fread(&ppd->stGridInfo, sizeof(GridInfo), 1, fp);
	m_vGridInfo.push_back(ppd->stGridInfo);

	//객체 종류 테이블 read
	fread(&ppd->stObjectTable, sizeof(ObjectTable), 1, fp);

	//객체 종류 정보 read
	fread(&ppd->stRoadObjData.stObjTypeInfo, sizeof(ObjTypeInfo), 1, fp);


	//도로객체의 수
	UINT NumOfRoad = ppd->stGridInfo.iNumOfRoad;

	//도로 옵셋 테이블 동적 할당
	ppd->stRoadObjData.pstRoadOffsetTable = (RoadOffsetTable*)malloc(sizeof(RoadOffsetTable) * NumOfRoad);
	memset(ppd->stRoadObjData.pstRoadOffsetTable, 0, sizeof(RoadOffsetTable) * NumOfRoad);

	for (int i = 0; i < NumOfRoad; i++)
	{
		//도로 옵셋 테이블 read
		
		fread(&ppd->stRoadObjData.pstRoadOffsetTable[i], sizeof(RoadOffsetTable), 1, fp);
	}

	ppd->stRoadObjData.pstRoadData = (RoadData*)malloc(sizeof(RoadData) * NumOfRoad);
	memset(ppd->stRoadObjData.pstRoadData, 0, sizeof(RoadData) * NumOfRoad);

	for (int i = 0; i < NumOfRoad; i++)
	{
		//도로 데이터 read & 도로 데이터 vector 저장
		fread(&ppd->stRoadObjData.pstRoadData[i].stRoadDataInfo, sizeof(RoadDataInfo), 1, fp);

		//진입 도로 개수
		int8_t PrevRoadNum = ppd->stRoadObjData.pstRoadData[i].stRoadDataInfo.nPrevRoadNum;

		//진입 도로 연결성 데이터 동적할당
		ppd->stRoadObjData.pstRoadData[i].pstPrevConnectData = (PrevConnectData*)malloc(sizeof(PrevConnectData) * PrevRoadNum);
		memset(ppd->stRoadObjData.pstRoadData[i].pstPrevConnectData, 0, sizeof(PrevConnectData) * PrevRoadNum);

		for (int j = 0; j < PrevRoadNum; j++)
		{
			//진입 도로 연결성 데이터 read
			fread(&ppd->stRoadObjData.pstRoadData[i].pstPrevConnectData[j], sizeof(PrevConnectData), 1, fp);
		}

		//진출 도로 개수
		int8_t NextRoadNum = ppd->stRoadObjData.pstRoadData[i].stRoadDataInfo.nNextRoadNum;

		//진출 도로 연결성 데이터 동적할당
		ppd->stRoadObjData.pstRoadData[i].pstNextConnectData = (NextConnectData*)malloc(sizeof(NextConnectData) * NextRoadNum);
		memset(ppd->stRoadObjData.pstRoadData[i].pstNextConnectData, 0, sizeof(NextConnectData) * NextRoadNum);

		for (int j = 0; j < NextRoadNum; j++)
		{
			//진출 도로 연결성 데이터 read
			fread(&ppd->stRoadObjData.pstRoadData[i].pstNextConnectData[j], sizeof(NextConnectData), 1, fp);
		}

		//도로 내 차로 개수
		int8_t LaneNum = ppd->stRoadObjData.pstRoadData[i].stRoadDataInfo.nLaneNum;

		//도로 내 차로 ID 리스트 동적할당
		ppd->stRoadObjData.pstRoadData[i].pstRoadIDList = (RoadIDList*)malloc(sizeof(RoadIDList) * LaneNum);
		memset(ppd->stRoadObjData.pstRoadData[i].pstRoadIDList, 0, sizeof(RoadIDList) * LaneNum);

		for (int j = 0; j < LaneNum; j++)
		{
			//도로 내 차로 ID리스트 read
			fread(&ppd->stRoadObjData.pstRoadData[i].pstRoadIDList[j], sizeof(RoadIDList), 1, fp);
		}
	}


	//차로 객체 종류 정보read
	fread(&ppd->stLaneObjData.stLaneObjTypeInfo, sizeof(LaneObjTypeInfo), 1, fp);

	//차로 옵셋 테이블 개수
	UINT NumOfLane = ppd->stGridInfo.iNumOfLane;
	ppd->stLaneObjData.pstLaneOffsetTable = (LaneOffsetTable*)malloc(sizeof(LaneOffsetTable) * NumOfLane);
	memset(ppd->stLaneObjData.pstLaneOffsetTable, 0, sizeof(LaneOffsetTable) * NumOfLane);

	for (int i = 0; i < NumOfLane; i++)
	{
		//차로 옵셋 테이블 read
		fread(&ppd->stLaneObjData.pstLaneOffsetTable[i], sizeof(LaneOffsetTable), 1, fp);
	}

	ppd->stLaneObjData.pstLaneData = (LaneData*)malloc(sizeof(LaneData) * NumOfLane);
	memset(ppd->stLaneObjData.pstLaneData, 0, sizeof(LaneData) * NumOfLane);

	for (int i = 0; i < NumOfLane; i++)
	{
		//차로 데이터 시작 위치로 fseek
		fseek(fp, ppd->stLaneObjData.pstLaneOffsetTable[i].iStart, SEEK_SET);

		//차로 데이터 read 
		fread(&ppd->stLaneObjData.pstLaneData[i].stLaneDataInfo, sizeof(LaneDataInfo), 1, fp);

		//진입 차로 연결성 데이터 개수
		int8_t PrevLaneNum = ppd->stLaneObjData.pstLaneData[i].stLaneDataInfo.nPrevLaneNum;

		//진입 차로 연결성 데이터 동적할당
		ppd->stLaneObjData.pstLaneData[i].pstPrevLaneConnectData = (PrevLaneConnectData*)malloc(sizeof(PrevLaneConnectData) * PrevLaneNum);
		memset(ppd->stLaneObjData.pstLaneData[i].pstPrevLaneConnectData, 0, sizeof(PrevLaneConnectData) * PrevLaneNum);

		for (int j = 0; j < PrevLaneNum; j++)
		{
			fread(&ppd->stLaneObjData.pstLaneData[i].pstPrevLaneConnectData[j], sizeof(PrevLaneConnectData), 1, fp);
		}

		//진출 차로 연결성 데이터 개수
		int8_t NextLaneNum = ppd->stLaneObjData.pstLaneData[i].stLaneDataInfo.nNextLaneNum;

		//진출 차로 연결성 데이터 동적할당
		ppd->stLaneObjData.pstLaneData[i].pstNextLaneConnectData = (NextLaneConnectData*)malloc(sizeof(NextLaneConnectData) * NextLaneNum);
		memset(ppd->stLaneObjData.pstLaneData[i].pstNextLaneConnectData, 0, sizeof(NextLaneConnectData) * NextLaneNum);

		for (int j = 0; j < NextLaneNum; j++)
		{
			fread(&ppd->stLaneObjData.pstLaneData[i].pstNextLaneConnectData[j], sizeof(NextLaneConnectData), 1, fp);
		}

		//좌측 차선ID 개수
		int8_t LeftLineNum = ppd->stLaneObjData.pstLaneData[i].stLaneDataInfo.nLeftLineNum;

		//좌측 차선ID 동적할당
		ppd->stLaneObjData.pstLaneData[i].pstLeftLaneData = (LeftLaneData*)malloc(sizeof(LeftLaneData) * LeftLineNum);
		memset(ppd->stLaneObjData.pstLaneData[i].pstLeftLaneData, 0, sizeof(LeftLaneData) * LeftLineNum);

		for (int j = 0; j < LeftLineNum; j++)
		{
			fread(&ppd->stLaneObjData.pstLaneData[i].pstLeftLaneData[j], sizeof(LeftLaneData), 1, fp);
		}


		//우측 차선ID 개수
		int8_t RightLineNum = ppd->stLaneObjData.pstLaneData[i].stLaneDataInfo.nRightLineNum;

		//우측 차선ID 동적할당
		ppd->stLaneObjData.pstLaneData[i].pstRigthLaneData = (RigthLaneData*)malloc(sizeof(RigthLaneData) * RightLineNum);
		memset(ppd->stLaneObjData.pstLaneData[i].pstRigthLaneData, 0, sizeof(RigthLaneData) * RightLineNum);

		for (int j = 0; j < RightLineNum; j++)
		{
			fread(&ppd->stLaneObjData.pstLaneData[i].pstRigthLaneData[j], sizeof(RigthLaneData), 1, fp);
		}
	}
	int size = ftell(fp);
	m_vFileSize.push_back(size);
	return ppd;
}


std::vector<CString> CMapFile::getGridIdList()
{
	return m_vGridID;
}

std::vector<GridInfo> CMapFile::getGridInfo()
{
	return m_vGridInfo;
}

std::vector<CString> CMapFile::getFilePath()
{
	return m_vFilePath;
}

FILE* CMapFile::getFilePoint()
{
	return fp;
}

std::vector<int> CMapFile::getFileSize()
{
	return m_vFileSize;
}
