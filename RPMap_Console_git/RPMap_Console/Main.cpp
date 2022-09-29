#include <iostream>
#include <stdio.h>
#include "MapFile.h"

int main()
{

	/*
	FILE* fp = fopen("D:\\RPMap_Ex\\RP_GridFile\\417437.bin", "rb");
	if (NULL == fp)
	{
		printf("File Open Fail\n");
		return;
	}
	*/

	
	CMapFile* MapFile = new CMapFile;
	MapFile->FileOpen();
	

	return 0;
}