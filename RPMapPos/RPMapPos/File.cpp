#include "pch.h"
#include "File.h"


CFileRead::CFileRead()
{

}

CFileRead::~CFileRead()
{

}


bool CFileRead::FileDialog()
{
	int count = 0;
	TCHAR szFilter[] = _T("All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, szFilter);

	CString FilePath;
	dlg.m_ofn.lpstrFile = FilePath.GetBuffer(4096);
	dlg.m_ofn.nMaxFile = 4096;

	if (IDOK == dlg.DoModal())
	{
		POSITION pos = dlg.GetStartPosition();
		while (NULL != pos)
		{
			CString FileName = dlg.GetNextPathName(pos);
			if (false == FileOpen(FileName, count))
			{
				return false;;
			}
			count++;
		}
	}

	return true;
}


bool CFileRead::FileOpen(CString path, int count)
{
	bool flag = false;

	if (0 == count)
	{
		_wfopen_s(&fp, path, L"rb");


		if (NULL == fp)
		{
			MessageBox(NULL, _T("File Open Fail"), _T("ERROR"), MB_OK);
			return false;
		}

		flag = true;
	}
	else if (1 == count)
	{
		_wfopen_s(&ffp, path, L"rb");

		if (NULL == ffp)
		{
			MessageBox(NULL, _T("File Open Fail"), _T("ERROR"), MB_OK);
			return false;
		}
	}
	
	if (true == flag)
	{
		LaneDataRead(fp);
	}
	else
	{
		LaneVertexRead(ffp);
	}

	return true;
}


//Lane Data MapFile Read fnc
void CFileRead::LaneDataRead(FILE* fp)
{
	//Map Header Read
	fread(&m_stLaneDataMap.stMapHeader, sizeof(MapHeader), 1, fp);

	//맵에 포함된 차로 개수
	UINT nLaneCnt = m_stLaneDataMap.stMapHeader.iNumOfGrid;

	//차로 데이터 할당
	LaneData* pLaneData = m_stLaneDataMap.pstLaneData;
	pLaneData = (LaneData*)malloc(sizeof(LaneData) * nLaneCnt);
	memset(pLaneData, 0, sizeof(LaneData) * nLaneCnt);


	for (int i = 0; i < nLaneCnt; i++)
	{
		//Lane Data Read
		fread(&pLaneData[i], sizeof(LaneData), 1, fp);

		//좌표 개수 vector 저장
		m_vVertexCnt.push_back(pLaneData[i].nVertexNum);
	}
}



//Lane Vertex File Read fnc
void CFileRead::LaneVertexRead(FILE* ffp)
{
	//Vertex Header Read
	fread(&m_stLaneVertex.stVertexHeader, sizeof(VertexHeader), 1, ffp);

	//맵에 포함된 차로 개수
	UINT nLaneCnt = m_stLaneVertex.stVertexHeader.iNumOfGrid;

	//좌표 데이터 할당
	m_stLaneVertex.pstLanePosData = (LanePosData*)malloc(sizeof(LanePosData) * nLaneCnt);
	memset(m_stLaneVertex.pstLanePosData, 0, sizeof(LanePosData) * nLaneCnt);

	for (int i = 0; i < nLaneCnt; i++)
	{
		//Lane Index Read
		fread(&m_stLaneVertex.pstLanePosData[i].nIndex, sizeof(UINT), 1, ffp);

		//차로 데이터 맵파일 -> 차로 데이터 -> 좌표개수
		int nVertexCnt = m_vVertexCnt[i];

		//차로데이터 할당
		LaneDataVertex* pLaneDataVertex = m_stLaneVertex.pstLanePosData[i].psLaneDataVertex;
		pLaneDataVertex = (LaneDataVertex*)malloc(sizeof(LaneDataVertex) * nLaneCnt);
		memset(pLaneDataVertex, 0, sizeof(LaneDataVertex) * nLaneCnt);

		for (int j = 0; j < nVertexCnt; j++)
		{
			//Lane Data Read
			fread(&pLaneDataVertex[j], sizeof(LaneDataVertex), 1, ffp);
		}
	}
}