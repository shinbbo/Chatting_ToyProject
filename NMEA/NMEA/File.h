#pragma once
#include "nmeaStruct.h"
#include <atlstr.h>
#include <vector>


class CFileRead
{
public:
	CFileRead();
	~CFileRead();


public:
	char* FileOpen();
	CString FileDialog();
	CString FileSaveDialog();
	bool FileParse();
	std::vector<std::string> getGNRMC();
	NMEA* getNmeaLonLat();
	bool FileWrite();
	

private:
	FILE* fp;
	CString FilePathName;
	int size;
	std::vector<std::string> m_vGNRMC;
	std::vector<NMEA> m_vstNmea;
	NMEA* m_pstNmea;
};