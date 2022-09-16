#pragma once
#include <stdio.h>

#pragma pack(push, 1)
typedef struct _FileData
{
	unsigned int FileIdx;
	char name[12];
	char PhonNumber[32];
	unsigned char Age;
}FileData;

typedef struct _FileOffset
{
	unsigned int FileOffset;
	unsigned short FileSize;
}FileOffset;

typedef struct _FileHeader
{
	char FileVersion[32];
	unsigned int FileDataCnt;
}FileHeader;

typedef struct _FileStruct
{
	FileHeader stFileHdr;
	FileOffset* stFileOffset;
	FileData* pStFileData;
}FileStruct;
#pragma pack(pop)
