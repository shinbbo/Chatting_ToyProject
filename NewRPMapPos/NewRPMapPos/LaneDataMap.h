#pragma once
#include <wtypes.h>


#pragma pack(1)
//���� ������
typedef struct
{
	BYTE				nLaneNo;														//���� ��ȣ
	BYTE				nMapSpeed;													//���Ѽӵ�
	WORD			nVertexNum;													//��ǥ ���� (B)
	WORD			nDirection;													//������� (��/��ȸ��, ���� ���)
	FLOAT				fLength;															//���� ����
	DWORD64		nLaneID;															//���� ID
	DWORD64		nR_LaneID;													//���� ���� ID
	DWORD64		nL_LaneID;														//���� ���� ID
}LaneData;


//�� ���
typedef struct
{
	UINT			iOffsetMapData;											//�ʵ����� ���� ��ġ
	UINT			iNumOfGrid;													//�ʿ� ���Ե� ���� ����(A)
}MapHeader;

typedef struct
{
	MapHeader			stMapHeader;
	LaneData*				pstLaneData;
}LaneDataMap;

#pragma pack(pop)