#pragma once
#pragma pack(push, 1)
#define UINT unsigned int

typedef struct
{
	UINT iStart;										//ù �׸��� ������ ������ġ
	UINT iSize;											//ù �׸��� ������ ũ��
}GridTable;

typedef struct
{
	char acVersion[8];							//�� ���� ���ڿ�
	UINT iOffsetMapData;						//�ʵ����� ���� ��ġ
	UINT iGridStartID;							//�ʿ� ���Ե� �»�� �׸���ID
	UINT iGridNumOfx;							//�ʿ� ���Ե� x�� �׸����� ����
	UINT iGridNumOfy;							//�ʿ� ���Ե� y�� �׸����� ����
	UINT iNumOfGrid;							//�ʿ� ���Ե� �׸��� ����(A): X�� �׸��� ���� * y�� �׸��� ����
	UINT iReserved;								//Reserved
}MapInfo;

typedef struct
{
	MapInfo stMapInfo;										//�� ����
	GridTable* pstGridTable;							//�׸��� ���̺�
}MapHeader;
#pragma pack(pop)