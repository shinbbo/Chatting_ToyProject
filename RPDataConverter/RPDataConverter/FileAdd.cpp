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
	CString a = szBuffer;
	_wfopen_s(&fp, a + L"\\merge1.bin", L"wb");
	if (NULL == fp)
	{
		return FALSE;
	}

	m_pData = (MapData*)malloc(sizeof(MapData) * vGridID.size());
	m_pData = m_pMapFile->getMapData();

	for (int num = 0; num < vGridID.size(); num++)
	{
		fwrite(&m_pData[num].stGridInfo, sizeof(GridInfo), 1, fp);

		//객체 종류 테이블 read
		fwrite(&m_pData[num].stObjectTable, sizeof(ObjectTable), 1, fp);

		//객체 종류 정보 read
		fwrite(&m_pData[num].stRoadObjData.stObjTypeInfo, sizeof(ObjTypeInfo), 1, fp);


		//도로객체의 수
		UINT NumOfRoad = m_pData[num].stGridInfo.iNumOfRoad;

		//도로 옵셋 테이블 동적 할당
		//m_pData[num].stRoadObjData.pstRoadOffsetTable = (RoadOffsetTable*)malloc(sizeof(RoadOffsetTable) * NumOfRoad);
		//memset(m_pData[num].stRoadObjData.pstRoadOffsetTable, 0, sizeof(RoadOffsetTable) * NumOfRoad);

		for (int i = 0; i < NumOfRoad; i++)
		{
			//도로 옵셋 테이블 write
			fwrite(&m_pData[num].stRoadObjData.pstRoadOffsetTable[i], sizeof(RoadOffsetTable), 1, fp);
		}

		//m_pData[num].stRoadObjData.pstRoadData = (RoadData*)malloc(sizeof(RoadData) * NumOfRoad);
		//memset(m_pData[num].stRoadObjData.pstRoadData, 0, sizeof(RoadData) * NumOfRoad);

		for (int i = 0; i < NumOfRoad; i++)
		{
			//도로 데이터 read & 도로 데이터 vector 저장
			fwrite(&m_pData[num].stRoadObjData.pstRoadData[i].stRoadDataInfo, sizeof(RoadDataInfo), 1, fp);

			//진입 도로 개수
			int8_t PrevRoadNum = m_pData[num].stRoadObjData.pstRoadData[i].stRoadDataInfo.nPrevRoadNum;

			//진입 도로 연결성 데이터 동적할당
			//m_pData[num].stRoadObjData.pstRoadData[i].pstPrevConnectData = (PrevConnectData*)malloc(sizeof(PrevConnectData) * PrevRoadNum);
			//memset(m_pData[num].stRoadObjData.pstRoadData[i].pstPrevConnectData, 0, sizeof(PrevConnectData) * PrevRoadNum);

			for (int j = 0; j < PrevRoadNum; j++)
			{
				//진입 도로 연결성 데이터 write
				fwrite(&m_pData[num].stRoadObjData.pstRoadData[i].pstPrevConnectData[j], sizeof(PrevConnectData), 1, fp);
			}

			//진출 도로 개수
			int8_t NextRoadNum = m_pData[num].stRoadObjData.pstRoadData[i].stRoadDataInfo.nNextRoadNum;

			//진출 도로 연결성 데이터 동적할당
			//m_pData[num].stRoadObjData.pstRoadData[i].pstNextConnectData = (NextConnectData*)malloc(sizeof(NextConnectData) * NextRoadNum);
			//memset(m_pData[num].stRoadObjData.pstRoadData[i].pstNextConnectData, 0, sizeof(NextConnectData) * NextRoadNum);

			for (int j = 0; j < NextRoadNum; j++)
			{
				//진출 도로 연결성 데이터 write
				fwrite(&m_pData[num].stRoadObjData.pstRoadData[i].pstNextConnectData[j], sizeof(NextConnectData), 1, fp);
			}

			//도로 내 차로 개수
			int8_t LaneNum = m_pData[num].stRoadObjData.pstRoadData[i].stRoadDataInfo.nLaneNum;

			//도로 내 차로 ID 리스트 동적할당
			//m_pData[num].stRoadObjData.pstRoadData[i].pstRoadIDList = (RoadIDList*)malloc(sizeof(RoadIDList) * LaneNum);
			//memset(m_pData[num].stRoadObjData.pstRoadData[i].pstRoadIDList, 0, sizeof(RoadIDList) * LaneNum);

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
		//m_pData[num].stLaneObjData.pstLaneOffsetTable = (LaneOffsetTable*)malloc(sizeof(LaneOffsetTable) * NumOfLane);
		//memset(m_pData[num].stLaneObjData.pstLaneOffsetTable, 0, sizeof(LaneOffsetTable) * NumOfLane);

		for (int i = 0; i < NumOfLane; i++)
		{
			//차로 옵셋 테이블 write
			fwrite(&m_pData[num].stLaneObjData.pstLaneOffsetTable[i], sizeof(LaneOffsetTable), 1, fp);
		}

		//m_pData[num].stLaneObjData.pstLaneData = (LaneData*)malloc(sizeof(LaneData) * NumOfLane);
		//memset(m_pData[num].stLaneObjData.pstLaneData, 0, sizeof(LaneData) * NumOfLane);

		for (int i = 0; i < NumOfLane; i++)
		{
			//차로 데이터 시작 위치로 fseek
			//fseek(fp, m_pData[num].stLaneObjData.pstLaneOffsetTable[i].iStart, SEEK_SET);

			//차로 데이터 write 
			int size = ftell(fp);
			fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo, sizeof(LaneDataInfo), 1, fp);

			//진입 차로 연결성 데이터 개수
			int8_t PrevLaneNum = m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nPrevLaneNum;

			//진입 차로 연결성 데이터 동적할당
			//m_pData[num].stLaneObjData.pstLaneData[i].pstPrevLaneConnectData = (PrevLaneConnectData*)malloc(sizeof(PrevLaneConnectData) * PrevLaneNum);
			//memset(m_pData[num].stLaneObjData.pstLaneData[i].pstPrevLaneConnectData, 0, sizeof(PrevLaneConnectData) * PrevLaneNum);

			for (int j = 0; j < PrevLaneNum; j++)
			{
				size = ftell(fp);
				fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].pstPrevLaneConnectData[j], sizeof(PrevLaneConnectData), 1, fp);
			}

			//진출 차로 연결성 데이터 개수
			int8_t NextLaneNum = m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nNextLaneNum;

			//진출 차로 연결성 데이터 동적할당
			//m_pData[num].stLaneObjData.pstLaneData[i].pstNextLaneConnectData = (NextLaneConnectData*)malloc(sizeof(NextLaneConnectData) * NextLaneNum);
			//memset(m_pData[num].stLaneObjData.pstLaneData[i].pstNextLaneConnectData, 0, sizeof(NextLaneConnectData) * NextLaneNum);

			for (int j = 0; j < NextLaneNum; j++)
			{
				size = ftell(fp);
				fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].pstNextLaneConnectData[j], sizeof(NextLaneConnectData), 1, fp);
			}

			//좌측 차선ID 개수
			int8_t LeftLineNum = m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nLeftLineNum;

			//좌측 차선ID 동적할당
			//m_pData[num].stLaneObjData.pstLaneData[i].pstLeftLaneData = (LeftLaneData*)malloc(sizeof(LeftLaneData) * LeftLineNum);
			//memset(m_pData[num].stLaneObjData.pstLaneData[i].pstLeftLaneData, 0, sizeof(LeftLaneData) * LeftLineNum);

			for (int j = 0; j < LeftLineNum; j++)
			{
				size = ftell(fp);
				fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].pstLeftLaneData[j], sizeof(LeftLaneData), 1, fp);
			}


			//우측 차선ID 개수
			int8_t RightLineNum = m_pData[num].stLaneObjData.pstLaneData[i].stLaneDataInfo.nRightLineNum;

			//우측 차선ID 동적할당
			//m_pData[num].stLaneObjData.pstLaneData[i].pstRigthLaneData = (RigthLaneData*)malloc(sizeof(RigthLaneData) * RightLineNum);
			//memset(m_pData[num].stLaneObjData.pstLaneData[i].pstRigthLaneData, 0, sizeof(RigthLaneData) * RightLineNum);

			for (int j = 0; j < RightLineNum; j++)
			{
				size = ftell(fp);
				fwrite(&m_pData[num].stLaneObjData.pstLaneData[i].pstRigthLaneData[j], sizeof(RigthLaneData), 1, fp);
			}
		}
	}

	fclose(fp);
	return true;
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




