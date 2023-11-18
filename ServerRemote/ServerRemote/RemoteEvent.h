#pragma once
#include <atlimage.h>

enum class eMouseClickEvent : UINT
{
	None = 0,
	LButtonDown,
	RButtonDown,
	LButtonDouble,
	WheelDown,
	WheelUp,
	LButtonUp
};

#pragma pack(push, 1)
typedef struct
{
	CPoint pos;
	eMouseClickEvent MouseClickEvnet;
	BOOL bFlag;
}MouseEvent;

typedef struct
{
	UINT iKey[2];
	BOOL bFlag;
}KeyboardEvent;

typedef struct
{
	MouseEvent stMouseEvent;
	KeyboardEvent stKeyboardEvent;
	char szBuf[32];
}RemoteEvent;

#pragma pack(pop)