
// port1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include <winsock.h>

#pragma comment(lib,"libmySQL.lib") 

// Cport1App: 
// �йش����ʵ�֣������ port1.cpp
//

class Cport1App : public CWinApp
{
public:
	Cport1App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cport1App theApp;