#pragma once
#include <stdio.h>
#include <atlstr.h>
#include <vector>
#include "LaneDataMap.h"
#include "LaneVertex.h"



class CFileRead
{
public:
	CFileRead();
	~CFileRead();



public:
	bool FileDialog();
	bool FileOpen(CString path, int count);
	void LaneDataRead(FILE* fp);
	void LaneVertexRead(FILE* ffp);

private:
	FILE* fp;
	FILE* ffp;

	CString FilePathName;

	LaneDataMap m_stLaneDataMap;
	LaneVertex m_stLaneVertex;

	std::vector< WORD> m_vVertexCnt;
};