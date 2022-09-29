#pragma once
#pragma pack(push, 1)
#include <stdint.h>


#define UINT unsigned int


//우측 차선 데이터(J)
typedef struct
{
	int64_t nID;									//우측 차선ID
}RigthLaneData;

//좌측 차선 데이터(I)
typedef struct
{
	int64_t nID;									//좌측 차선ID
}LeftLaneData;

//진출 차로 연결성 데이터(H)
typedef struct
{
	int8_t nIndex;								//도로 인덱스
	int16_t nDirection;						//진행방향 (좌/우회전, 직진 등등)
	int64_t nID;									//도로 ID
}NextLaneConnectData;

//진입 차로 연결성 데이터(G)
typedef struct
{
	int8_t nIndex;								//도로 인덱스
	int16_t nDirection;						//진행방향 (좌/우회전, 직진 등등)
	int64_t nID;									//도로 ID
}PrevLaneConnectData;

typedef struct
{
	int8_t nLaneIndex;											//가공된 차로 번호
	int8_t nLaneNo;												//차로 번호(원도 기준)
	int8_t nPrevLaneNum;										//진입차로 개수(G)
	int8_t nNextLaneNum;										//진출차로 개수(H)
	int8_t nLeftLineNum;										//좌측차선 개수(I)
	int8_t nRightLineNum;									//우측차선 개수(J)
	int8_t nMaxSpeed;											//최고제한속도
	int16_t nDirection;											//진행방향 (좌/우회전, 직진 등등)
	float fLength;														//차로 길이
	double dStartX;												//차로 시작 X좌표
	double dStartY;													//차로 시작 Y좌표
	double dEndX;													//차로 끝 X좌표
	double dEndY;													//차로 끝 Y좌표
	int64_t nLaneID;												//차로ID
	int64_t nR_LanelD;											//우측 차로ID
	int64_t nL_LanelD;											//좌측 차로ID
	char acReserved[3];										//Reserved
}LaneDataInfo;

typedef struct
{
	LaneDataInfo stLaneDataInfo;														//차로 데이터
	PrevLaneConnectData* pstPrevLaneConnectData;					//진입 차로 연결성 데이터
	NextLaneConnectData* pstNextLaneConnectData;					//진출 차로 연결성 데이터
	LeftLaneData* pstLeftLaneData;													//좌측 차선 ID
	RigthLaneData* pstRigthLaneData;												//우측 차선 ID
}LaneData;
//---------------------------------------------------------------------------------------------------------------------차로 데이터
typedef struct
{
	UINT iStart;											//차로 데이터 시작위치
	UINT iSize;												//차로 데이터 크기
}LaneOffsetTable;

//객체 종류 정보
typedef struct
{
	UINT iTypeObject;															//객체종류 타입 번호
	UINT iNumOfObject;														//같은 타입의 객체 개수
}LaneObjTypeInfo;

typedef struct
{
	LaneObjTypeInfo stLaneObjTypeInfo;							//객체 종류 정보
	LaneOffsetTable* pstLaneOffsetTable;							//차로 옵셋 테이블(C)
	LaneData* pstLaneData;													//차로 객체 데이터
}LaneObjData;
//---------------------------------------------------------------------------------------------------------------------차로 객체 데이터
//도로 내 차로 ID리스트
typedef struct
{
	int64_t LaneID;								//도로 내 포홤된 차로
}RoadIDList;

//진출 도로 연결성 데이터
typedef struct
{
	int8_t nIndex;								//도로 인덱스
	int16_t nDirection;						//진행방향 (좌/우회전, 직진 등등)
	int64_t nID;									//도로 ID
}NextConnectData;

//진입 도로 연결성 데이터
typedef struct
{
	int8_t nIndex;								//도로 인덱스
	int16_t nDirection;						//진행방향 (좌/우회전, 직진 등등)
	int64_t nID;									//도로 ID
}PrevConnectData;

//도로 데이터
typedef struct
{
	int8_t						nMaxSpeed;												//최고 제한 속도
	int8_t						nPrevRoadNum;										//진입 도로 개수
	int8_t						nNextRoadNum;										//진출 도로 개수
	int8_t						nLaneNum;												//도로 내 차로 개수
	int16_t					nDirection;												//진행방향 (좌/우회전, 직진 등등)
	float						fLength;														//도로 길이
	double					dStartX;														//도로 시작 X좌표
	double					dStartY;														//도로 시작 Y좌표
	double					dEndX;														//도로 끝 X좌표
	double					dEndY;														//도로 끝 Y좌표
	int64_t					nRoadID;													//도로 ID
	char						acReserved[6];											//Reserved
}RoadDataInfo;

//도로 데이터
typedef struct
{
	RoadDataInfo stRoadDataInfo;											//도로 데이터
	PrevConnectData* pstPrevConnectData;							//진입 도로 연결성 데이터
	NextConnectData* pstNextConnectData;							//진출 도로 연결성 데이터
	RoadIDList* pstRoadIDList;													//도로 내 차로 ID 리스트
}RoadData;
//---------------------------------------------------------------------------------------------------------------------
//도로 옵셋 테이블
typedef struct
{
	UINT iStart;																				//도로 데이터 시작 위치
	UINT iSize;																					//도로 데이터 크기
}RoadOffsetTable;

//객체 종류 정보
typedef struct
{
	UINT iTypeObject;																	//객체종류 타입 번호
	UINT iNumOfObject;																//같은 타입의 객체 개수
}ObjTypeInfo;

//도로 객체 데이터
typedef struct
{
	ObjTypeInfo stObjTypeInfo;													//객체 종류 정보
	RoadOffsetTable* pstRoadOffsetTable;								//도로 옵셋 테이블
	RoadData* pstRoadData;														//도로 데이터
}RoadObjData;
//---------------------------------------------------------------------------------------------------------------------
//객체종류 테이블
typedef struct
{
	UINT iRoadDataStart;																//도로 객체 데이터 시작 위치
	UINT iRoadDataSize;																//도로 객체 데이터 크기
	UINT iLaneDataStart;																//차로 객체 데이터 시작 위치
	UINT iLaneDataSize;																//차로 객체 데이터 크기
}ObjectTable;
//---------------------------------------------------------------------------------------------------------------------
//그리드 정보
typedef struct
{
	UINT iGridID;																			//그리드 ID
	UINT iNumOfRoad;																	//그리드에 포홤되어 있는 도로객체의 수(B)
	UINT iNumOfLane;																	//그리드에 포홤되어 있는 차로객체의 수(C)
}GridInfo;
//---------------------------------------------------------------------------------------------------------------------


typedef struct
{
	GridInfo stGridInfo;																//그리드 정보
	ObjectTable stObjectTable;													//객체종류 테이블
	RoadObjData stRoadObjData;
	LaneObjData stLaneObjData;
}MapData;



#pragma pack(pop)
