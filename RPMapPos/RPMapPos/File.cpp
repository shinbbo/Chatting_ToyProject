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

	//�ʿ� ���Ե� ���� ����
	UINT nLaneCnt = m_stLaneDataMap.stMapHeader.iNumOfGrid;

	//���� ������ �Ҵ�
	LaneData* pLaneData = m_stLaneDataMap.pstLaneData;
	pLaneData = (LaneData*)malloc(sizeof(LaneData) * nLaneCnt);
	memset(pLaneData, 0, sizeof(LaneData) * nLaneCnt);


	for (int i = 0; i < nLaneCnt; i++)
	{
		//Lane Data Read
		fread(&pLaneData[i], sizeof(LaneData), 1, fp);

		//��ǥ ���� vector ����
		m_vVertexCnt.push_back(pLaneData[i].nVertexNum);
	}
}



//Lane Vertex File Read fnc
void CFileRead::LaneVertexRead(FILE* ffp)
{
	//Vertex Header Read
	fread(&m_stLaneVertex.stVertexHeader, sizeof(VertexHeader), 1, ffp);

	//�ʿ� ���Ե� ���� ����
	UINT nLaneCnt = m_stLaneVertex.stVertexHeader.iNumOfGrid;

	//��ǥ ������ �Ҵ�
	m_stLaneVertex.pstLanePosData = (LanePosData*)malloc(sizeof(LanePosData) * nLaneCnt);
	memset(m_stLaneVertex.pstLanePosData, 0, sizeof(LanePosData) * nLaneCnt);

	for (int i = 0; i < nLaneCnt; i++)
	{
		//Lane Index Read
		fread(&m_stLaneVertex.pstLanePosData[i].nIndex, sizeof(UINT), 1, ffp);

		//���� ������ ������ -> ���� ������ -> ��ǥ����
		int nVertexCnt = m_vVertexCnt[i];

		//���ε����� �Ҵ�
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