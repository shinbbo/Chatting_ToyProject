#include <iostream>
#include "MapFile.h"


CMapFile::CMapFile()
{
	 
}
CMapFile::~CMapFile()
{

}

void CMapFile::FileOpen()
{
	//fp = fopen("D:\\RPMap_Ex\\Map����\\TC_RP_Yeongjongdo.bin", "rb");
	fp = fopen("D:\\RPMap_Ex\\RP_GridFile\\417438.bin", "rb");
	if (NULL == fp)
	{
		printf("File Open Fail\n");
		return;
	}

	MapDataRead(fp);

	//MapHeaderRead(fp);
	fclose(fp);
}

void CMapFile::MapHeaderRead(FILE* fp)
{
	fread(&m_MapHeader.stMapInfo, sizeof(MapInfo), 1, fp);
	printf("�� ���� ���ڿ� : %s\n", m_MapHeader.stMapInfo.acVersion);

	UINT GirdCnt = m_MapHeader.stMapInfo.iNumOfGrid;
	m_MapHeader.pstGridTable = (GridTable*)malloc(sizeof(GridTable) * GirdCnt);
	memset(m_MapHeader.pstGridTable, 0, sizeof(GridTable) * GirdCnt);

	for (int i = 0; i < GirdCnt; i++)
	{
		fread(&m_MapHeader.pstGridTable[i], sizeof(GridTable), 1, fp);

	}

}



void CMapFile::MapDataRead(FILE* fp)
{
	fread(&m_MapData.stGridInfo, sizeof(GridInfo), 1, fp);
	fread(&m_MapData.stObjectTable, sizeof(ObjectTable), 1, fp);
	fread(&m_MapData.stRoadObjData.stObjTypeInfo, sizeof(ObjTypeInfo), 1, fp);
	printf("%d", sizeof(LaneDataInfo));

	//���ΰ�ü�� ��
	UINT NumOfRoad = m_MapData.stGridInfo.iNumOfRoad;

	//���� �ɼ� ���̺� ���� �Ҵ�
	m_MapData.stRoadObjData.pstRoadOffsetTable = (RoadOffsetTable*)malloc(sizeof(RoadOffsetTable) * NumOfRoad);
	memset(m_MapData.stRoadObjData.pstRoadOffsetTable, 0, sizeof(RoadOffsetTable) * NumOfRoad);

	for (int i = 0; i < NumOfRoad; i++)
	{
		//���� �ɼ� ���̺� read
		fread(&m_MapData.stRoadObjData.pstRoadOffsetTable[i], sizeof(RoadOffsetTable), 1, fp);
	}

	m_MapData.stRoadObjData.pstRoadData = (RoadData*)malloc(sizeof(RoadData) * NumOfRoad);

	for (int i = 0; i < NumOfRoad; i++)
	{
		//���� ������ read
		fread(&m_MapData.stRoadObjData.pstRoadData[i].stRoadDataInfo, sizeof(RoadDataInfo), 1, fp);

		//���� ���� ����
		int8_t PrevRoadNum = m_MapData.stRoadObjData.pstRoadData[i].stRoadDataInfo.nPrevRoadNum;

		//���� ���� ���Ἲ ������ �����Ҵ�
		m_MapData.stRoadObjData.pstRoadData[i].pstPrevConnectData = (PrevConnectData*)malloc(sizeof(PrevConnectData) * PrevRoadNum);
		memset(m_MapData.stRoadObjData.pstRoadData[i].pstPrevConnectData, 0, sizeof(PrevConnectData) * PrevRoadNum);

		for (int j = 0; j < PrevRoadNum; j++)
		{
			//���� ���� ���Ἲ ������ read
			fread(&m_MapData.stRoadObjData.pstRoadData[i].pstPrevConnectData[j], sizeof(PrevConnectData), 1, fp);
		}

		//���� ���� ����
		int8_t NextRoadNum = m_MapData.stRoadObjData.pstRoadData[i].stRoadDataInfo.nNextRoadNum;

		//���� ���� ���Ἲ ������ �����Ҵ�
		m_MapData.stRoadObjData.pstRoadData[i].pstNextConnectData = (NextConnectData*)malloc(sizeof(NextConnectData) * NextRoadNum);
		memset(m_MapData.stRoadObjData.pstRoadData[i].pstNextConnectData, 0, sizeof(NextConnectData) * NextRoadNum);

		for (int j = 0; j < NextRoadNum; j++)
		{
			//���� ���� ���Ἲ ������ read
			fread(&m_MapData.stRoadObjData.pstRoadData[i].pstNextConnectData[j], sizeof(NextConnectData), 1, fp);
		}

		//���� �� ���� ����
		int8_t LaneNum = m_MapData.stRoadObjData.pstRoadData[i].stRoadDataInfo.nLaneNum;

		//���� �� ���� ID ����Ʈ �����Ҵ�
		m_MapData.stRoadObjData.pstRoadData[i].pstRoadIDList = (RoadIDList*)malloc(sizeof(RoadIDList) * LaneNum);
		memset(m_MapData.stRoadObjData.pstRoadData[i].pstRoadIDList, 0, sizeof(RoadIDList) * LaneNum);

		for (int j = 0; j < LaneNum; j++)
		{
			//���� �� ���� ID����Ʈ read
			fread(&m_MapData.stRoadObjData.pstRoadData[i].pstRoadIDList[j], sizeof(RoadIDList), 1, fp);
		}
	}





	//���� ��ü ���� ����read
	fread(&m_MapData.stLaneObjData.stLaneObjTypeInfo, sizeof(LaneObjTypeInfo), 1, fp);

	//���� �ɼ� ���̺� ����
	UINT NumOfLane = m_MapData.stGridInfo.iNumOfLane;
	m_MapData.stLaneObjData.pstLaneOffsetTable = (LaneOffsetTable*)malloc(sizeof(LaneOffsetTable) * NumOfLane);
	memset(m_MapData.stLaneObjData.pstLaneOffsetTable, 0, sizeof(LaneOffsetTable) * NumOfLane);

	for (int i = 0; i < NumOfLane; i++)
	{
		//���� �ɼ� ���̺� read
		fread(&m_MapData.stLaneObjData.pstLaneOffsetTable[i], sizeof(LaneOffsetTable), 1, fp);
	}

	m_MapData.stLaneObjData.pstLaneData = (LaneData*)malloc(sizeof(LaneData) * NumOfLane);
	memset(m_MapData.stLaneObjData.pstLaneData, 0, sizeof(LaneData) * NumOfLane);

	for (int i = 0; i < NumOfLane; i++)
	{
		//���� ������ read
		//fseek(fp, m_MapData.stLaneObjData.pstLaneOffsetTable[i].iStart, SEEK_SET);
		fread(&m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo, sizeof(LaneDataInfo), 1, fp);

		//���� ���� ���Ἲ ������ ����
		int8_t PrevLaneNum = m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo.nPrevLaneNum;

		//���� ���� ���Ἲ ������ �����Ҵ�
		m_MapData.stLaneObjData.pstLaneData[i].pstPrevLaneConnectData = (PrevLaneConnectData*)malloc(sizeof(PrevLaneConnectData) * PrevLaneNum);
		memset(m_MapData.stLaneObjData.pstLaneData[i].pstPrevLaneConnectData, 0, sizeof(PrevLaneConnectData) * PrevLaneNum);

		for (int j = 0; j < PrevLaneNum; j++)
		{
			fread(&m_MapData.stLaneObjData.pstLaneData[i].pstPrevLaneConnectData[j], sizeof(PrevLaneConnectData), 1, fp);
		}

		//���� ���� ���Ἲ ������ ����
		int8_t NextLaneNum = m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo.nNextLaneNum;

		//���� ���� ���Ἲ ������ �����Ҵ�
		m_MapData.stLaneObjData.pstLaneData[i].pstNextLaneConnectData = (NextLaneConnectData*)malloc(sizeof(NextLaneConnectData) * NextLaneNum);
		memset(m_MapData.stLaneObjData.pstLaneData[i].pstNextLaneConnectData, 0, sizeof(NextLaneConnectData) * PrevLaneNum);

		for (int j = 0; j < PrevLaneNum; j++)
		{
			fread(&m_MapData.stLaneObjData.pstLaneData[i].pstNextLaneConnectData[j], sizeof(NextLaneConnectData), 1, fp);
		}

		//���� ����ID ����
		int8_t LeftLineNum = m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo.nLeftLineNum;

		//���� ����ID �����Ҵ�
		m_MapData.stLaneObjData.pstLaneData[i].pstLeftLaneData = (LeftLaneData*)malloc(sizeof(LeftLaneData) * LeftLineNum);
		memset(m_MapData.stLaneObjData.pstLaneData[i].pstLeftLaneData, 0, sizeof(LeftLaneData) * LeftLineNum);

		for (int j = 0; j < LeftLineNum; j++)
		{
			fread(&m_MapData.stLaneObjData.pstLaneData[i].pstLeftLaneData[j], sizeof(LeftLaneData), 1, fp);
		}


		//���� ����ID ����
		int8_t RightLineNum = m_MapData.stLaneObjData.pstLaneData[i].stLaneDataInfo.nRightLineNum;

		//���� ����ID �����Ҵ�
		m_MapData.stLaneObjData.pstLaneData[i].pstRigthLaneData = (RigthLaneData*)malloc(sizeof(RigthLaneData) * RightLineNum);
		memset(m_MapData.stLaneObjData.pstLaneData[i].pstRigthLaneData, 0, sizeof(RigthLaneData) * RightLineNum);

		for (int j = 0; j < LeftLineNum; j++)
		{
			fread(&m_MapData.stLaneObjData.pstLaneData[i].pstRigthLaneData[j], sizeof(RigthLaneData), 1, fp);
		}
	}
	

	
}