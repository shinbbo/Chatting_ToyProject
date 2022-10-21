#pragma once
#pragma pack(push, 1)
#include <stdint.h>


#define UINT unsigned int


//���� ���� ������(J)
typedef struct
{
	int64_t nID;									//���� ����ID
}RigthLaneData;

//���� ���� ������(I)
typedef struct
{
	int64_t nID;									//���� ����ID
}LeftLaneData;

//���� ���� ���Ἲ ������(H)
typedef struct
{
	int8_t nIndex;								//���� �ε���
	int16_t nDirection;						//������� (��/��ȸ��, ���� ���)
	int64_t nID;									//���� ID
}NextLaneConnectData;

//���� ���� ���Ἲ ������(G)
typedef struct
{
	int8_t nIndex;								//���� �ε���
	int16_t nDirection;						//������� (��/��ȸ��, ���� ���)
	int64_t nID;									//���� ID
}PrevLaneConnectData;

typedef struct
{
	int8_t nLaneIndex;											//������ ���� ��ȣ
	int8_t nLaneNo;												//���� ��ȣ(���� ����)
	int8_t nPrevLaneNum;										//�������� ����(G)
	int8_t nNextLaneNum;										//�������� ����(H)
	int8_t nLeftLineNum;										//�������� ����(I)
	int8_t nRightLineNum;									//�������� ����(J)
	int8_t nMaxSpeed;											//�ְ����Ѽӵ�
	int16_t nDirection;											//������� (��/��ȸ��, ���� ���)
	float fLength;														//���� ����
	double dStartX;												//���� ���� X��ǥ
	double dStartY;													//���� ���� Y��ǥ
	double dEndX;													//���� �� X��ǥ
	double dEndY;													//���� �� Y��ǥ
	int64_t nLaneID;												//����ID
	int64_t nR_LanelD;											//���� ����ID
	int64_t nL_LanelD;											//���� ����ID
	char acReserved[3];										//Reserved
}LaneDataInfo;

typedef struct
{
	LaneDataInfo stLaneDataInfo;														//���� ������
	PrevLaneConnectData* pstPrevLaneConnectData;					//���� ���� ���Ἲ ������
	NextLaneConnectData* pstNextLaneConnectData;					//���� ���� ���Ἲ ������
	LeftLaneData* pstLeftLaneData;													//���� ���� ID
	RigthLaneData* pstRigthLaneData;												//���� ���� ID
}LaneData;
//---------------------------------------------------------------------------------------------------------------------���� ������
typedef struct
{
	UINT iStart;											//���� ������ ������ġ
	UINT iSize;												//���� ������ ũ��
}LaneOffsetTable;

//��ü ���� ����
typedef struct
{
	UINT iTypeObject;															//��ü���� Ÿ�� ��ȣ
	UINT iNumOfObject;														//���� Ÿ���� ��ü ����
}LaneObjTypeInfo;

typedef struct
{
	LaneObjTypeInfo stLaneObjTypeInfo;							//��ü ���� ����
	LaneOffsetTable* pstLaneOffsetTable;							//���� �ɼ� ���̺�(C)
	LaneData* pstLaneData;													//���� ��ü ������
}LaneObjData;
//---------------------------------------------------------------------------------------------------------------------���� ��ü ������
//���� �� ���� ID����Ʈ
typedef struct
{
	int64_t LaneID;								//���� �� ���c�� ����
}RoadIDList;

//���� ���� ���Ἲ ������
typedef struct
{
	int8_t nIndex;								//���� �ε���
	int16_t nDirection;						//������� (��/��ȸ��, ���� ���)
	int64_t nID;									//���� ID
}NextConnectData;

//���� ���� ���Ἲ ������
typedef struct
{
	int8_t nIndex;								//���� �ε���
	int16_t nDirection;						//������� (��/��ȸ��, ���� ���)
	int64_t nID;									//���� ID
}PrevConnectData;

//���� ������
typedef struct
{
	int8_t						nMaxSpeed;												//�ְ� ���� �ӵ�
	int8_t						nPrevRoadNum;										//���� ���� ����
	int8_t						nNextRoadNum;										//���� ���� ����
	int8_t						nLaneNum;												//���� �� ���� ����
	int16_t					nDirection;												//������� (��/��ȸ��, ���� ���)
	float						fLength;														//���� ����
	double					dStartX;														//���� ���� X��ǥ
	double					dStartY;														//���� ���� Y��ǥ
	double					dEndX;														//���� �� X��ǥ
	double					dEndY;														//���� �� Y��ǥ
	int64_t					nRoadID;													//���� ID
	char						acReserved[6];											//Reserved
}RoadDataInfo;

//���� ������
typedef struct
{
	RoadDataInfo stRoadDataInfo;											//���� ������
	PrevConnectData* pstPrevConnectData;							//���� ���� ���Ἲ ������
	NextConnectData* pstNextConnectData;							//���� ���� ���Ἲ ������
	RoadIDList* pstRoadIDList;													//���� �� ���� ID ����Ʈ
}RoadData;
//---------------------------------------------------------------------------------------------------------------------
//���� �ɼ� ���̺�
typedef struct
{
	UINT iStart;																				//���� ������ ���� ��ġ
	UINT iSize;																					//���� ������ ũ��
}RoadOffsetTable;

//��ü ���� ����
typedef struct
{
	UINT iTypeObject;																	//��ü���� Ÿ�� ��ȣ
	UINT iNumOfObject;																//���� Ÿ���� ��ü ����
}ObjTypeInfo;

//���� ��ü ������
typedef struct
{
	ObjTypeInfo stObjTypeInfo;													//��ü ���� ����
	RoadOffsetTable* pstRoadOffsetTable;								//���� �ɼ� ���̺�
	RoadData* pstRoadData;														//���� ������
}RoadObjData;
//---------------------------------------------------------------------------------------------------------------------
//��ü���� ���̺�
typedef struct
{
	UINT iRoadDataStart;																//���� ��ü ������ ���� ��ġ
	UINT iRoadDataSize;																//���� ��ü ������ ũ��
	UINT iLaneDataStart;																//���� ��ü ������ ���� ��ġ
	UINT iLaneDataSize;																//���� ��ü ������ ũ��
}ObjectTable;
//---------------------------------------------------------------------------------------------------------------------
//�׸��� ����
typedef struct
{
	UINT iGridID;																			//�׸��� ID
	UINT iNumOfRoad;																	//�׸��忡 ���c�Ǿ� �ִ� ���ΰ�ü�� ��(B)
	UINT iNumOfLane;																	//�׸��忡 ���c�Ǿ� �ִ� ���ΰ�ü�� ��(C)
}GridInfo;
//---------------------------------------------------------------------------------------------------------------------


typedef struct
{
	GridInfo stGridInfo;																//�׸��� ����
	ObjectTable stObjectTable;													//��ü���� ���̺�
	RoadObjData stRoadObjData;
	LaneObjData stLaneObjData;
}MapData;



#pragma pack(pop)
