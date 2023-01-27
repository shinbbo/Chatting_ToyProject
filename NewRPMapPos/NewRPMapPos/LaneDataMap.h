#pragma once
#include <wtypes.h>


#pragma pack(1)
//차로 데이터
typedef struct
{
	BYTE				nLaneNo;														//차로 번호
	BYTE				nMapSpeed;													//제한속도
	WORD			nVertexNum;													//좌표 개수 (B)
	WORD			nDirection;													//진행방향 (좌/우회전, 직진 등등)
	FLOAT				fLength;															//차로 길이
	DWORD64		nLaneID;															//차로 ID
	DWORD64		nR_LaneID;													//우측 차로 ID
	DWORD64		nL_LaneID;														//좌측 차로 ID
}LaneData;


//맵 헤더
typedef struct
{
	UINT			iOffsetMapData;											//맵데이터 시작 우치
	UINT			iNumOfGrid;													//맵에 포함된 차로 개수(A)
}MapHeader;

typedef struct
{
	MapHeader			stMapHeader;
	LaneData*				pstLaneData;
}LaneDataMap;

#pragma pack(pop)