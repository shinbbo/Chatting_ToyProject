#pragma once
#pragma pack(push, 1)
#define UINT unsigned int

typedef struct
{
	UINT iStart;										//첫 그리드 데이터 시작위치
	UINT iSize;											//첫 그리드 데이터 크기
}GridTable;

typedef struct
{
	char acVersion[8];							//맵 버전 문자열
	UINT iOffsetMapData;						//맵데이터 시작 위치
	UINT iGridStartID;							//맵에 포함된 좌상단 그리드ID
	UINT iGridNumOfx;							//맵에 포함된 x축 그리드의 개수
	UINT iGridNumOfy;							//맵에 포함된 y축 그리드의 개수
	UINT iNumOfGrid;							//맵에 포함된 그리드 개수(A): X축 그리드 개수 * y축 그리드 개수
	UINT iReserved;								//Reserved
}MapInfo;

typedef struct
{
	MapInfo stMapInfo;										//맵 정보
	GridTable* pstGridTable;							//그리드 테이블
}MapHeader;
#pragma pack(pop)