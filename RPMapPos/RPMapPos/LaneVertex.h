#pragma once
#include <wtypes.h>
#pragma pack(1)


typedef struct
{
	DOUBLE		dX;															//x좌표
	DOUBLE		dY;															//y좌표
	DOUBLE		dZ;															//z좌표
}LaneDataVertex;


typedef struct
{
	UINT							nIndex;													//차로 인덱스
	LaneDataVertex*		psLaneDataVertex;								//차로 데이터
}LanePosData;


typedef struct
{
	UINT			iOffsetMapData;									//맵데이터 시작 위치
	UINT			iNumOfGrid;											//맵에 포함된 차로 개수(A)
}VertexHeader;

typedef struct
{
	VertexHeader				stVertexHeader;
	LanePosData*				pstLanePosData;
}LaneVertex;

#pragma pack(pop)