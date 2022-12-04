// FileAdd.cpp: 구현 파일
//

#include "pch.h"
#include "RPDataConverter.h"
#include "FileAdd.h"
#include "afxdialogex.h"


// CFileAdd 대화 상자

IMPLEMENT_DYNAMIC(CFileMerge, CDialogEx)

CFileMerge::CFileMerge(CMapFile* pMapData, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD, pParent)
{
	m_pMapFile = pMapData;
}

CFileMerge::~CFileMerge()
{
}

void CFileMerge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SAVEPATH, m_Edit);
}


BEGIN_MESSAGE_MAP(CFileMerge, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFileMerge::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CFileAdd 메시지 처리기
BOOL CFileMerge::FileMerge(TCHAR* szBuffer)
{
	std::vector<CString> vGridID;
	vGridID = m_pMapFile->getGridIdList();

	fp = m_pMapFile->getFilePoint();
	CString filepath = szBuffer;

	//CString str;
	//str.Format(_T("%s"), szBuffer);
	//m_Edit.SetWindowTextW(str);

	_wfopen_s(&fp, filepath + L"\\merge.bin", L"wb");
	if (NULL == fp)
	{
		return FALSE;
	}

	memcpy(m_MapHeader.stMapInfo.acVersion, "ver1.0", sizeof(m_MapHeader.stMapInfo.acVersion));
	fwrite(&m_MapHeader.stMapInfo.acVersion, sizeof(m_MapHeader.stMapInfo.acVersion), 1, fp);

	printf("Version = %s\n", m_MapHeader.stMapInfo.acVersion);
	int Cur = ftell(fp);

	fseek(fp, Cur + sizeof(UINT), SEEK_SET);
	m_MapHeader.stMapInfo.iGridStartID = 20180805;
	fwrite(&m_MapHeader.stMapInfo.iGridStartID, sizeof(UINT), 1, fp);

	m_MapHeader.stMapInfo.iGridNumOfx = 0;
	fwrite(&m_MapHeader.stMapInfo.iGridNumOfx, sizeof(UINT), 1, fp);

	m_MapHeader.stMapInfo.iGridNumOfy = 0;
	fwrite(&m_MapHeader.stMapInfo.iGridNumOfy, sizeof(UINT), 1, fp);

	m_MapHeader.stMapInfo.iNumOfGrid = 0;
	fwrite(&m_MapHeader.stMapInfo.iNumOfGrid, sizeof(UINT), 1, fp);

	m_MapHeader.stMapInfo.iReserved = 0;
	fwrite(&m_MapHeader.stMapInfo.iReserved, sizeof(UINT), 1, fp);


	m_pData = (MapData*)malloc(sizeof(MapData) * vGridID.size());
	m_pData = m_pMapFile->getMapData();

	int GridDataCur = ftell(fp);
	m_MapHeader.pstGridTable = (GridTable*)malloc(sizeof(GridTable) * vGridID.size());

	int GridTableSize = sizeof(GridTable);
	int FileCnt = vGridID.size();
	fseek(fp, GridTableSize * FileCnt, SEEK_CUR);

	for (int num = 0; num < vGridID.size(); num++)
	{
		
		m_MapHeader.pstGridTable[num].iStart = ftell(fp);

		fwrite(&m_pData[num].stGridInfo, sizeof(GridInfo), 1, fp);

		//객체 종류 테이블 read
		fwrite(&m_pData[num].stObjectTable, sizeof(ObjectTable), 1, fp);

		//객체 종류 정보 read
		fwrite(&m_pData[num].stRoadObjData.stObjTypeInfo, sizeof(ObjTypeInfo), 1, fp);

		//도로객체의 수
		UINT NumOfRoad = m_pData[num].stGridInfo.iNumOfRoad;

		for (int i = 0; i < NumOfRoad; i++)
		{
			//도로 옵셋 테이블 write
			fwrite(&m_pData[num].stRoadObjData.pstRoadOffsetTable[i], sizeof(RoadOffsetTable), 1, fp);
		}


		for (int i = 0; i < NumOfRoad; i++)
		{
			//도로 데이터 read & 도로 데이터 vector 저장
			fwrite(&m_pData[num].stRoadObjData.pstRoadData[i].stRoadDataInfo, sizeof(RoadDataInfo), 1, fp);

			//진입 도로 개수
			int8_t PrevRoadNum = m_pData[num].stRoadObjData.pstRoadData[i].stRoadDataInfo.nPrevRoadNum;


			for (int j = 0; j < PrevRoadNum; j++)
			{
				//진입 도로 연결성 데이터 write
				fwrite(&m_pData[num].stRoadObjData.pstRoadData[i].pstPrevConnectData[j], sizeof(PrevConnectData), 1, fp);
			}

			//진출 도로 개수
			int8_t NextRoadNum = m_pData[num].stRoadObjData.pstRoadData[i].stRoadDataInfo.nNextRoadNum;


			for (int j = 0; j < NextRoadNum; j++)
			{
				//진출 도로 연결성 데이터 write
				fwrite(&m_pData[num].stRoadObjData.pstRoadData[i].pstNextConnectData[j], sizeof(NextConnectData), 1, fp);
			}

			//도로 내 차로 개수
			int8_t LaneNum = m_pData[num].stRoadObjData.pstRoadData[i].stRoadDataInfo.nLaneNum;


			for (int j = 0; j < LaneNum; j++)
			{
				//도로 내 차로 ID리스트 write
				fwrite(&m_pData[num].stRoadObjData.pstRoadData[i].pstRoadIDList[j], sizeof(RoadIDList), 1, fp);
			}
		}


		//차로 객체 종류 정보write
		fwrite(&m_pData[num].stLaneObjData.stLaneObjTypeInfo, sizeof(LaneObjTypeInfo), 1, fp);

		//차로 옵셋 테이블 개수
		UINT NumOfLane = m_pData[num].stGridInfo.iNumOfLane;


		for (int i = 0; i < NumOfLane; i++)
		{
			//차로 옵셋 테이블 write
			fwrite(&m_pData[num].stLaneObjData.pstLaneOffsetTable[i], sizeof(LaneOffsetTable), 1, fp);
		}


		for (int i = 0; i < NumOfLane; i++)
		{
			//차로 데이터 write 
			//int size = ftell(fp);
			fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo, sizeof(LaneDataInfo), 1, fp);

			//진입 차로 연결성 데이터 개수
			int8_t PrevLaneNum = m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nPrevLaneNum;


			for (int j = 0; j < PrevLaneNum; j++)
			{
				//size = ftell(fp);
				fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].pstPrevLaneConnectData[j], sizeof(PrevLaneConnectData), 1, fp);
			}

			//진출 차로 연결성 데이터 개수
			int8_t NextLaneNum = m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nNextLaneNum;


			for (int j = 0; j < NextLaneNum; j++)
			{
				//size = ftell(fp);
				fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].pstNextLaneConnectData[j], sizeof(NextLaneConnectData), 1, fp);
			}

			//좌측 차선ID 개수
			int8_t LeftLineNum = m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nLeftLineNum;


			for (int j = 0; j < LeftLineNum; j++)
			{
				//size = ftell(fp);
				fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].pstLeftLaneData[j], sizeof(LeftLaneData), 1, fp);
			}


			//우측 차선ID 개수
			int8_t RightLineNum = m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nRightLineNum;


			for (int j = 0; j < RightLineNum; j++)
			{
				//size = ftell(fp);
				fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].pstRigthLaneData[j], sizeof(RigthLaneData), 1, fp);
			}
		}
		int ofsCur = ftell(fp);
		m_MapHeader.pstGridTable[num].iSize = ofsCur - m_MapHeader.pstGridTable[num].iStart;
		
	}


	fseek(fp, GridDataCur, SEEK_SET);
	for (int i = 0; i < FileCnt; i++)
	{
		fwrite(&m_MapHeader.pstGridTable[i].iStart, sizeof(UINT), 1, fp);
		fwrite(&m_MapHeader.pstGridTable[i].iSize, sizeof(UINT), 1, fp);
	}
	

	m_MapHeader.stMapInfo.iOffsetMapData = ftell(fp);
	fseek(fp, Cur, SEEK_SET);
	fwrite(&m_MapHeader.stMapInfo.iOffsetMapData, sizeof(UINT), 1, fp);

	fclose(fp);
	return TRUE;
}

void CFileMerge::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);				// 파일경로 읽어오기

	CString str;
	str.Format(_T("%s"), szBuffer);
	m_Edit.SetWindowTextW(str);

	if (TRUE == FileMerge(szBuffer))
	{
		AfxMessageBox(_T("저장 완료"));
	}
}




