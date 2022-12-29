#include "pch.h"
#include "File.h"
#include <string>
#include <iostream>
#include <sstream>


#define MAX_PATH 256



CFileRead::CFileRead()
{
	m_pstNmea = NULL;
}

CFileRead::~CFileRead()
{

}


CString CFileRead::FileDialog()
{
	
	TCHAR szFilter[] = _T("All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())
	{
		FilePathName = dlg.GetPathName();
	}

	return FilePathName;
}


CString CFileRead::FileSaveDialog()
{

	TCHAR szFilter[] = _T("All Files(*.*)|*.*||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter);

	if (IDOK == dlg.DoModal())
	{
		FilePathName = dlg.GetPathName();
	}

	return FilePathName;
}


char* CFileRead::FileOpen()
{
	char* Buffer = NULL;
	
	_wfopen_s(&fp, FilePathName, L"rb");

	if (NULL == fp)
	{
		MessageBox(NULL, _T("File Open Fail"), _T("ERROR"), MB_OK);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	Buffer = (char*)malloc(sizeof(char)*size + 1);
	memset(Buffer, 0, sizeof(char)*size + 1);

	if (fread(Buffer, size, 1, fp) < 1)
	{
		free(Buffer);
		fclose(fp);
		return NULL;
	}

	fclose(fp);
	

	return Buffer;
}


bool CFileRead::FileParse()
{
	int pos = 0;
	int count;
	std::string str;

	char* pDoc = NULL;
	pDoc = FileOpen();

	if (NULL == pDoc)
	{
		return false;
	}

	while (pos < size)
	{
		if ('$' == pDoc[pos])
		{
			pos++;
			str += pDoc[pos];

			while (pos < size)
			{
				pos++;
				if (',' == pDoc[pos])
				{
					break;
				}
				str += pDoc[pos];
			}
			
			if ("GNRMC" == str)
			{
				while ('\n' != pDoc[pos])
				{
					str += pDoc[pos];
					pos++;
				}
				m_vGNRMC.push_back(str);
			}
			else
			{
				str = "";
			}
		}
		pos++;
	}

	
	int vecSize = m_vGNRMC.size();

	m_pstNmea = (NMEA*)malloc(sizeof(NMEA) * vecSize);
	memset(m_pstNmea, 0, sizeof(NMEA) * vecSize);

	for (int i = 0; i < vecSize; i++)
	{
		std::string a = m_vGNRMC[i];
		std::string temp = "";
		count = 0;

		for (int j = 0; j < a.length(); j++)
		{
			temp += a[j];
			if (a[j] == ',')
			{
				count++;

				if (4 == count)
				{
					std::stringstream ssdouble(temp);
					ssdouble >> m_pstNmea[i].lon;
				}
				else if (6 == count)
				{
					std::stringstream ssdouble(temp);
					ssdouble >> m_pstNmea[i].lat;
				}

				temp = "";
			}
		}
	}

	return true;
}


std::vector<std::string> CFileRead::getGNRMC()
{
	return m_vGNRMC;
}


NMEA* CFileRead::getNmeaLonLat()
{
	return m_pstNmea;
}


bool CFileRead::FileWrite()
{
	int size = m_vGNRMC.size();

	_wfopen_s(&fp, FilePathName, L"w");

	if (NULL == fp)
	{
		MessageBox(NULL, _T("File Save Fail"), _T("ERROR"), MB_OK);
		return false;
	}

	//m_pstNmea = (NMEA*)malloc(sizeof(NMEA) * size);
	//memset(m_pstNmea, 0, sizeof(NMEA) * size);

	for (int i = 0; i < size; i++)
	{
		fwrite(&m_pstNmea[i].lon, sizeof(double), 1, fp);
		fwrite(&m_pstNmea[i].lat, sizeof(double), 1, fp);
	}

	fclose(fp);

	return true;
}



