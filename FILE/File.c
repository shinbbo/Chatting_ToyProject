#define _CRT_SECURE_NO_WARNINGS
#include "File.h"
#include "exData.h"
#include <string.h>
#include <stdlib.h>

/*
int main()
{
	//FILE* fp = fopen("data.bin", "wb");
	FILE* fp = fopen("data1.txt", "wt");
	
	FileStruct stFileStruct;
	int count = 0;

	memcpy(stFileStruct.stFileHdr.FileVersion, "v20220913", sizeof(stFileStruct.stFileHdr.FileVersion));

	stFileStruct.pStFileData = (FileData*)malloc(sizeof(FileData) * 10);
	memset(stFileStruct.pStFileData, 0, sizeof(FileData) * 10);

	stFileStruct.stFileOffset = (FileOffset*)malloc(sizeof(FileOffset) * 10);
	memset(stFileStruct.stFileOffset, 0, sizeof(FileOffset) * 10);

	Data pData[10] = { {0, "±èÃ¶¼ö", "010-1111-1111", 20}, {1, "È«±æµ¿", "010-2222-2222", 41}, {2, "¼¼Á¾´ë¿Õ", "010-3333-3333", 32}, {3, "¿¡µð½¼", "010-4444-4444", 26}, {4, "±è¿µÈñ", "010-5555-5555", 53}, {5, "¼­ÅÂ¿õ", "010-6666-6666", 22}, {6, "½ÅÅÂÀÏ", "010-7777-7777", 47} , {7, "Àå¿µ½Ç", "010-8888-8888", 35}, {8, "°í±æµ¿", "010-9999-9999", 67}, {9, "µÑ¸®", "010-0000-0000", 88} };

	for (int i = 0; i < 10; i++)
	{
		stFileStruct.pStFileData[i].FileIdx = pData[i].FileIdx;
		memcpy(stFileStruct.pStFileData[i].name, pData[i].name, sizeof(stFileStruct.pStFileData[i].name));
		memcpy(stFileStruct.pStFileData[i].PhonNumber, pData[i].PhonNumber, sizeof(stFileStruct.pStFileData[i].PhonNumber));
		stFileStruct.pStFileData[i].Age = pData[i].Age;
		fwrite(&stFileStruct.pStFileData[i], sizeof(FileData), 1, fp);
	}

	for (int i = 0; i < 10; i++)
	{
		stFileStruct.stFileOffset[i].FileOffset = sizeof(FileHeader) + (sizeof(FileOffset) * i) + (sizeof(FileData) * i);
		stFileStruct.stFileOffset[i].FileSize = sizeof(FileData);
		fwrite(&stFileStruct.stFileOffset[i], sizeof(FileOffset), 1, fp);
		count++;
	}
	stFileStruct.stFileHdr.FileDataCnt = count;
	
	for (int j = 0; j < count; j++)
	{
		printf("Idx : %d, Name : %s, Phon : %s,  Age : %d\n", stFileStruct.pStFileData[j].FileIdx, stFileStruct.pStFileData[j].name, stFileStruct.pStFileData[j].PhonNumber, stFileStruct.pStFileData[j].Age);
		printf("Offset : %d, Size : %d\n", stFileStruct.stFileOffset[j].FileOffset, stFileStruct.stFileOffset[j].FileSize);
	}

	fwrite(&stFileStruct.stFileHdr, sizeof(FileHeader), 1, fp);
	//fwrite(&stFileStruct, sizeof(FileStruct), 1, fp);
	

	fclose(fp);
	free(stFileStruct.pStFileData);
	free(stFileStruct.stFileOffset);

	return 0;
}*/
int main()
{
	//FILE* fp = fopen("data1.txt", "wt");
	FILE* fp = fopen("data.bin", "wb");

	FileHeader stFileHeader;
	FileOffset* pstFileOffset;
	FileData* pstFileData;

	

	memcpy(stFileHeader.FileVersion, "v20220913", sizeof(stFileHeader.FileVersion));
	stFileHeader.FileDataCnt = SIZE;

	fwrite(&stFileHeader, sizeof(FileHeader), 1, fp);

	pstFileData = (FileData*)malloc(sizeof(FileData) * SIZE);
	memset(pstFileData, 0, sizeof(FileData) * SIZE);

	pstFileOffset = (FileOffset*)malloc(sizeof(FileOffset) * SIZE);
	memset(pstFileOffset, 0, sizeof(FileOffset) * SIZE);

	Data pData[10] = { {0, "±èÃ¶¼ö", "010-1111-1111", 20}, {1, "È«±æµ¿", "010-2222-2222", 41}, {2, "¼¼Á¾´ë¿Õ", "010-3333-3333", 32}, {3, "¿¡µð½¼", "010-4444-4444", 26}, {4, "±è¿µÈñ", "010-5555-5555", 53}, {5, "¼­ÅÂ¿õ", "010-6666-6666", 22}, {6, "½ÅÅÂÀÏ", "010-7777-7777", 47} , {7, "Àå¿µ½Ç", "010-8888-8888", 35}, {8, "°í±æµ¿", "010-9999-9999", 67}, {9, "µÑ¸®", "010-0000-0000", 88} };

	size_t	size;
	for (int i = 0; i < SIZE; i++)
	{
		pstFileData[i].FileIdx = pData[i].FileIdx;
		memcpy(pstFileData[i].name, pData[i].name, sizeof(pstFileData[i].name));
		memcpy(pstFileData[i].PhonNumber, pData[i].PhonNumber, sizeof(pstFileData[i].PhonNumber));
		pstFileData[i].Age = pData[i].Age;
		size = fwrite(&pstFileData[i], sizeof(FileData), 1, fp);
		if (size != 0)
		{
			printf("È®ÀÎ\n");
		}
	}


	for (int i = 0; i < SIZE; i++)
	{
		pstFileOffset[i].FileOffset = sizeof(FileHeader) + (sizeof(FileOffset) * (i + 1)) + (sizeof(FileData) * i);
		pstFileOffset[i].FileSize = sizeof(FileData);
		fwrite(&pstFileOffset[i], sizeof(FileOffset), 1, fp);
	}

	for (int j = 0; j < SIZE; j++)
	{
		printf("Idx : %d, Name : %s, Phon : %s,  Age : %d\n", pstFileData[j].FileIdx, pstFileData[j].name, pstFileData[j].PhonNumber, pstFileData[j].Age);
		printf("Offset : %d, Size : %d\n", pstFileOffset[j].FileOffset, pstFileOffset[j].FileSize);
	}


	fclose(fp);
	free(pstFileData);
	free(pstFileOffset);
	printf("¿Ï·á");

	return 0;
}
