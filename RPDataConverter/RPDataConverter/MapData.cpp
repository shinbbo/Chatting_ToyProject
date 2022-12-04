#include "pch.h"
#include <iostream>
#include <cstdio>
#include "MapData.h"


CMapFile::CMapFile()
{
	
}
CMapFile::~CMapFile()
{
	m_vGridID.clear();
	
}

void CMapFile::FileOpen(CString strPathName1, CString path, int num)
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

	pMapDataRead(fp, num);

	fclose(fp);
}



void CMapFile::pMapDataRead(FILE* fp, int num)
{
	//�׸��� ���� read & vector ����
	fread(&m_ppd[num].stGridInfo, sizeof(GridInfo), 1, fp);
	m_vGridInfo.push_back(m_ppd[num].stGridInfo);

	//��ü ���� ���̺� read
	fread(&m_ppd[num].stObjectTable, sizeof(ObjectTable), 1, fp);

	//��ü ���� ���� read
	fread(&m_ppd[num].stRoadObjData.stObjTypeInfo, sizeof(ObjTypeInfo), 1, fp);


	//���ΰ�ü�� ��
	UINT NumOfRoad = m_ppd[num].stGridInfo.iNumOfRoad;

	//���� �ɼ� ���̺� ���� �Ҵ�
	m_ppd[num].stRoadObjData.pstRoadOffsetTable = (RoadOffsetTable*)malloc(sizeof(RoadOffsetTable) * NumOfRoad);
	memset(m_ppd[num].stRoadObjData.pstRoadOffsetTable, 0, sizeof(RoadOffsetTable) * NumOfRoad);

	for (int i = 0; i < NumOfRoad; i++)
	{
		//���� �ɼ� ���̺� read
		
		fread(&m_ppd[num].stRoadObjData.pstRoadOffsetTable[i], sizeof(RoadOffsetTable), 1, fp);
	}

	m_ppd[num].stRoadObjData.pstRoadData = (RoadData*)malloc(sizeof(RoadData) * NumOfRoad);
	memset(m_ppd[num].stRoadObjData.pstRoadData, 0, sizeof(RoadData) * NumOfRoad);

	for (int i = 0; i < NumOfRoad; i++)
	{
		//���� ������ read & ���� ������ vector ����
		fread(&m_ppd[num].stRoadObjData.pstRoadData[i].stRoadDataInfo, sizeof(RoadDataInfo), 1, fp);

		//���� ���� ����
		int8_t PrevRoadNum = m_ppd[num].stRoadObjData.pstRoadData[i].stRoadDataInfo.nPrevRoadNum;

		//���� ���� ���Ἲ ������ �����Ҵ�
		m_ppd[num].stRoadObjData.pstRoadData[i].pstPrevConnectData = (PrevConnectData*)malloc(sizeof(PrevConnectData) * PrevRoadNum);
		memset(m_ppd[num].stRoadObjData.pstRoadData[i].pstPrevConnectData, 0, sizeof(PrevConnectData) * PrevRoadNum);

		for (int j = 0; j < PrevRoadNum; j++)
		{
			//���� ���� ���Ἲ ������ read
			fread(&m_ppd[num].stRoadObjData.pstRoadData[i].pstPrevConnectData[j], sizeof(PrevConnectData), 1, fp);
		}

		//���� ���� ����
		int8_t NextRoadNum = m_ppd[num].stRoadObjData.pstRoadData[i].stRoadDataInfo.nNextRoadNum;

		//���� ���� ���Ἲ ������ �����Ҵ�
		m_ppd[num].stRoadObjData.pstRoadData[i].pstNextConnectData = (NextConnectData*)malloc(sizeof(NextConnectData) * NextRoadNum);
		memset(m_ppd[num].stRoadObjData.pstRoadData[i].pstNextConnectData, 0, sizeof(NextConnectData) * NextRoadNum);

		for (int j = 0; j < NextRoadNum; j++)
		{
			//���� ���� ���Ἲ ������ read
			fread(&m_ppd[num].stRoadObjData.pstRoadData[i].pstNextConnectData[j], sizeof(NextConnectData), 1, fp);
		}

		//���� �� ���� ����
		int8_t LaneNum = m_ppd[num].stRoadObjData.pstRoadData[i].stRoadDataInfo.nLaneNum;

		//���� �� ���� ID ����Ʈ �����Ҵ�
		m_ppd[num].stRoadObjData.pstRoadData[i].pstRoadIDList = (RoadIDList*)malloc(sizeof(RoadIDList) * LaneNum);
		memset(m_ppd[num].stRoadObjData.pstRoadData[i].pstRoadIDList, 0, sizeof(RoadIDList) * LaneNum);

		for (int j = 0; j < LaneNum; j++)
		{
			//���� �� ���� ID����Ʈ read
			fread(&m_ppd[num].stRoadObjData.pstRoadData[i].pstRoadIDList[j], sizeof(RoadIDList), 1, fp);
		}
	}


	//���� ��ü ���� ����read
	fread(&m_ppd[num].stLaneObjData.stLaneObjTypeInfo, sizeof(LaneObjTypeInfo), 1, fp);

	//���� �ɼ� ���̺� ����
	UINT NumOfLane = m_ppd[num].stGridInfo.iNumOfLane;
	m_ppd[num].stLaneObjData.pstLaneOffsetTable = (LaneOffsetTable*)malloc(sizeof(LaneOffsetTable) * NumOfLane);
	memset(m_ppd[num].stLaneObjData.pstLaneOffsetTable, 0, sizeof(LaneOffsetTable) * NumOfLane);

	for (int i = 0; i < NumOfLane; i++)
	{
		//���� �ɼ� ���̺� read
		fread(&m_ppd[num].stLaneObjData.pstLaneOffsetTable[i], sizeof(LaneOffsetTable), 1, fp);
	}

	m_ppd[num].stLaneObjData.pstLaneData = (LaneData*)malloc(sizeof(LaneData) * NumOfLane);
	memset(m_ppd[num].stLaneObjData.pstLaneData, 0, sizeof(LaneData) * NumOfLane);

	for (int i = 0; i < NumOfLane; i++)
	{
		//���� ������ ���� ��ġ�� fseek
		fseek(fp, m_ppd[num].stLaneObjData.pstLaneOffsetTable[i].iStart, SEEK_SET);

		//���� ������ read 
		fread(&m_ppd[num].stLaneObjData.pstLaneData[i].stLaneDataInfo, sizeof(LaneDataInfo), 1, fp);

		//���� ���� ���Ἲ ������ ����
		int8_t PrevLaneNum = m_ppd[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nPrevLaneNum;

		//���� ���� ���Ἲ ������ �����Ҵ�
		m_ppd[num].stLaneObjData.pstLaneData[i].pstPrevLaneConnectData = (PrevLaneConnectData*)malloc(sizeof(PrevLaneConnectData) * PrevLaneNum);
		memset(m_ppd[num].stLaneObjData.pstLaneData[i].pstPrevLaneConnectData, 0, sizeof(PrevLaneConnectData) * PrevLaneNum);

		for (int j = 0; j < PrevLaneNum; j++)
		{
			fread(&m_ppd[num].stLaneObjData.pstLaneData[i].pstPrevLaneConnectData[j], sizeof(PrevLaneConnectData), 1, fp);
		}

		//���� ���� ���Ἲ ������ ����
		int8_t NextLaneNum = m_ppd[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nNextLaneNum;

		//���� ���� ���Ἲ ������ �����Ҵ�
		m_ppd[num].stLaneObjData.pstLaneData[i].pstNextLaneConnectData = (NextLaneConnectData*)malloc(sizeof(NextLaneConnectData) * NextLaneNum);
		memset(m_ppd[num].stLaneObjData.pstLaneData[i].pstNextLaneConnectData, 0, sizeof(NextLaneConnectData) * NextLaneNum);

		for (int j = 0; j < NextLaneNum; j++)
		{
			fread(&m_ppd[num].stLaneObjData.pstLaneData[i].pstNextLaneConnectData[j], sizeof(NextLaneConnectData), 1, fp);
		}

		//���� ����ID ����
		int8_t LeftLineNum = m_ppd[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nLeftLineNum;

		//���� ����ID �����Ҵ�
		m_ppd[num].stLaneObjData.pstLaneData[i].pstLeftLaneData = (LeftLaneData*)malloc(sizeof(LeftLaneData) * LeftLineNum);
		memset(m_ppd[num].stLaneObjData.pstLaneData[i].pstLeftLaneData, 0, sizeof(LeftLaneData) * LeftLineNum);

		for (int j = 0; j < LeftLineNum; j++)
		{
			fread(&m_ppd[num].stLaneObjData.pstLaneData[i].pstLeftLaneData[j], sizeof(LeftLaneData), 1, fp);
		}


		//���� ����ID ����
		int8_t RightLineNum = m_ppd[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nRightLineNum;

		//���� ����ID �����Ҵ�
		m_ppd[num].stLaneObjData.pstLaneData[i].pstRigthLaneData = (RigthLaneData*)malloc(sizeof(RigthLaneData) * RightLineNum);
		memset(m_ppd[num].stLaneObjData.pstLaneData[i].pstRigthLaneData, 0, sizeof(RigthLaneData) * RightLineNum);

		for (int j = 0; j < RightLineNum; j++)
		{
			fread(&m_ppd[num].stLaneObjData.pstLaneData[i].pstRigthLaneData[j], sizeof(RigthLaneData), 1, fp);
		}
	}
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

void CMapFile::setGridCnt(int cnt)
{
	m_nGridCnt = cnt;
	m_ppd = (MapData*)malloc(sizeof(MapData) * m_nGridCnt);
}

MapData* CMapFile::getMapData()
{
	return m_ppd;
}


void CMapFile::GridIdListDel()
{
	m_vGridID.clear();
	std::vector<CString>().swap(m_vGridID);
}

void CMapFile::GridInfoDel()
{
	m_vGridInfo.clear();
	std::vector<GridInfo>().swap(m_vGridInfo);
}