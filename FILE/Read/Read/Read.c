#define _CRT_SECURE_NO_WARNINGS
#include "Read.h"
#include <string.h>
#include <stdlib.h>

int main()
{
	FILE* fp = fopen("D:\\FILE\\FILE\\data.bin", "rb");
	//FileStruct stFileStruct;
	FileHeader stFileHeader;
	FileOffset* pstFileOffset;
	FileData* pstFileData;

	int Datacnt = 0;

	if (fp == NULL)
	{
		printf("FILE Open Error.\n");
		return 0;
	}

	printf("%d\n", sizeof(FileHeader));
	fread(&stFileHeader, sizeof(FileHeader), 1, fp);
	printf("Version : %s, Data Cnt : %d\n", stFileHeader.FileVersion, stFileHeader.FileDataCnt);

	Datacnt = stFileHeader.FileDataCnt;
	pstFileOffset = (FileOffset*)malloc(sizeof(FileOffset) * Datacnt);
	memset(pstFileOffset, 0, sizeof(FileOffset) * Datacnt);

	pstFileData = (FileData*)malloc(sizeof(FileData) * Datacnt);
	memset(pstFileData, 0, sizeof(FileData) * Datacnt);

	for (int i = 0; i < Datacnt; i++)
	{
		fread(&pstFileOffset[i + 1], sizeof(FileOffset) , 1, fp);
		fread(&pstFileData[i], sizeof(FileData), 1, fp);
		printf("Offset : %d, Size : %d\n", pstFileOffset[i].FileOffset, pstFileOffset[i].FileSize);
		printf("Idx : %d, Name : %s, Phon : %s, Age : %d\n", pstFileData[i].FileIdx, pstFileData[i].name, pstFileData[i].PhonNumber, pstFileData[i].Age);
	}

	/*
	for (int i = 0; i < Datacnt; i++)
	{
		fread(&pstFileData[i], sizeof(FileData), 1, fp);
		
	}
	*/
	fclose(fp);

	return 0;
}