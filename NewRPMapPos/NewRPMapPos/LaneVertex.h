#pragma once
#include <wtypes.h>
#pragma pack(1)


typedef struct
{
	DOUBLE		dX;															//x��ǥ
	DOUBLE		dY;															//y��ǥ
	DOUBLE		dZ;															//z��ǥ
}LaneDataVertex;


typedef struct
{
	UINT							nIndex;													//���� �ε���
	LaneDataVertex*		psLaneDataVertex;								//���� ������
}LanePosData;


typedef struct
{
	UINT			iOffsetMapData;									//�ʵ����� ���� ��ġ
	UINT			iNumOfGrid;											//�ʿ� ���Ե� ���� ����(A)
}VertexHeader;

typedef struct
{
	VertexHeader				stVertexHeader;
	LanePosData*				pstLanePosData;
}LaneVertex;

#pragma pack(pop)