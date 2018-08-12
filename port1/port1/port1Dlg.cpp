
// port1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "port1.h"
#include "port1Dlg.h"
#include "afxdialogex.h"
#include "goble.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// Cport1Dlg 对话框



Cport1Dlg::Cport1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cport1Dlg::IDD, pParent)
	, m_strRecvData(_T(""))
	, m_strSendData(_T(""))
	, sendmsg(_T(""))
	, m_recvData_h(_T(""))
	, m_recvData_l(_T(""))
	, m_DataHall(_T(""))
	, m_dataMotor(_T(""))
	, m_dataAir(_T(""))
	, m_dataJSQ(_T(""))
	, m_slider(0)
	, m_nops(0)
	, m_systime(_T(""))
	, m_hour(_T(""))
	, m_min(_T(""))
	, m_temp_tem(_T(""))
	, m_temp_hum(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CString ser1;

void Cport1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RECV, m_strRecvData);
	DDX_Control(pDX, IDC_MSCOMM1, m_cComm);
	DDX_Text(pDX, IDC_SENDMSG, sendmsg);
	DDX_Control(pDX, IDC_COMBO_PORT, setport);
	DDX_Control(pDX, IDC_COMBO_BAUD, setbaud);
	DDX_Control(pDX, IDC_SLIDER1, m_slider_ctrl);//滑块
	if (ser1 == "") //ser1 没有赋值，则枚举串口
	{
		//访问注册表获取串口信息
		HKEY hKey;
		if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey))   //打开串口注册表对应的键值 
		{
			int seri = 0;
			TCHAR portName[256], commName[256];
			DWORD dwLong, dwSize;
			while (1)
			{
				dwLong = dwSize = sizeof(portName);
				if (::RegEnumValue(hKey,
					seri,
					portName,
					&dwLong,
					NULL,
					NULL,
					(PUCHAR)commName,
					&dwSize) == ERROR_NO_MORE_ITEMS)//   枚举串口  
					break;
				setport.AddString(commName);   // commName就是串口名字  
				ser1 = "1";
				seri++;
			}
			//高度信息
			CRect rc;
			setport.SetCurSel(0);
			setport.GetDroppedControlRect(&rc);
			setport.GetParent()->ScreenToClient(&rc);
			rc.bottom += 150;
			setport.MoveWindow(&rc);
			RegCloseKey(hKey);
		}
	}
	DDX_Control(pDX, IDC_EDIT_RECV, m_editctrl);
	DDX_Text(pDX, IDC_EDIT1, m_recvData_h);
	DDX_Text(pDX, IDC_EDIT2, m_recvData_l);
	DDX_Text(pDX, IDC_EDIT_HALL, m_DataHall);
	DDX_Text(pDX, IDC_EDIT_MOTOR, m_dataMotor);
	DDX_Text(pDX, IDC_EDIT3, m_dataAir);
	DDX_Text(pDX, IDC_EDIT6, m_dataJSQ);
	DDX_Slider(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_SLIDER1, m_slider_ctrl);
	DDX_Text(pDX, IDC_EDIT7, m_nops);
	DDX_Text(pDX, IDC_EDIT8, m_systime);

	DDX_Text(pDX, IDC_EDIT9, m_hour);
	DDX_Text(pDX, IDC_EDIT10, m_min);

	DDX_Text(pDX, IDC_EDIT14, m_temp_tem);
	DDX_Text(pDX, IDC_EDIT15, m_temp_hum);
}

BEGIN_MESSAGE_MAP(Cport1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_RECV, &Cport1Dlg::OnEnChangeEditRecv)
	ON_BN_CLICKED(IDC_BUTTON_RECV, &Cport1Dlg::OnButtonRecv)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &Cport1Dlg::OnBn_Open)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Cport1Dlg::OnBnClickedSend)

	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &Cport1Dlg::OnBn_Close)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_KT, &Cport1Dlg::OnBnClickedKt)
	ON_BN_CLICKED(IDC_BUTTON_KT2, &Cport1Dlg::OnBnClickedKt2)
	ON_BN_CLICKED(IDC_BUTTON_JSQ, &Cport1Dlg::OnBnClickedJsq)
	ON_BN_CLICKED(IDC_BUTTON_JSQ2, &Cport1Dlg::OnBnClickedJsq2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &Cport1Dlg::OnNMCustomdrawSlider1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON6, &Cport1Dlg::OnBnClickedNAOZHONG)
	ON_BN_CLICKED(IDC_BUTTON7, &Cport1Dlg::OnClose_port)
END_MESSAGE_MAP()


// Cport1Dlg 消息处理程序

BOOL Cport1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SkinH_Attach();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码


	m_slider_ctrl.SetRange(1,9);//设置滑动范围
	m_slider_ctrl.SetTicFreq(1);//每1个单位画一刻度

	setbaud.SetCurSel(3);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cport1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cport1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cport1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(Cport1Dlg, CDialogEx)
	ON_EVENT(Cport1Dlg, IDC_MSCOMM1, 1, Cport1Dlg::OnComm, VTS_NONE)
END_EVENTSINK_MAP()


void Cport1Dlg::OnComm()
{
	// TODO:  在此处添加消息处理程序代码
	//串口相关
	SetTimer(2, 99, NULL);


}





void Cport1Dlg::OnEnChangeEditRecv()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Cport1Dlg::OnButtonRecv()
{
	// TODO:  在此添加控件通知处理程序代码
	
}

void Cport1Dlg::OnBn_Open()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT14)->GetWindowText(m_temp_tem);
	GetDlgItem(IDC_EDIT15)->GetWindowText(m_temp_hum);

	int ser2;
	if (ser1 == "")
	{
		MessageBox("未连接设备或驱动有问题！", "错误");
	}
	else
	{
		int selectedPos1 = setport.GetCurSel();
		setport.GetLBText(selectedPos1, ser1);
		for (int i = 1; i < 20; i++)
		{
			CString temp;
			temp.Format("COM%d", i);
			if (ser1 == temp)
			{
				ser2 = i;
				break;
			}
		}

		if (m_cComm.get_PortOpen()) //如果发现串口本来是打开的，则关闭串口
		{
			m_cComm.put_PortOpen(FALSE);
		}
		//GetDlgItem(IDC_EDIT_PORT)->GetWindowText(SetPort);
		//GetDlgItem(IDC_EDIT_BAUD)->GetWindowText(SetBaud);
		CString baud;
		CString baud2;
		int selectedPos2 = setbaud.GetCurSel();
		setbaud.GetLBText(selectedPos2, baud2);
		//short port = _ttoi(SetPort);
		baud.Format(_T("%s,n,8,1"), baud2);
		//MessageBox(SetPort, SetBaud, MB_OK);
		m_cComm.put_CommPort(ser2); //选择COM1端口
		m_cComm.put_InputMode(1); //输入方式为二进制方式
		m_cComm.put_InBufferSize(1024); //设置输入缓冲区
		m_cComm.put_OutBufferSize(512); //设置输出缓冲区
		m_cComm.put_Settings(baud);//波特率，无校验，个数据位，个停止位
		m_cComm.put_RThreshold(4); //每接收到4个字符时，触发OnComm事件
		m_cComm.put_PortOpen(true); //打开串口
	}	
}

//模拟打开窗帘//电机正转
void Cport1Dlg::OnBnClickedSend()
{
	// TODO:  在此添加控件通知处理程序代码

	CByteArray Array;

	//控制消息
	Array.Add(0x40);
	Array.Add(0x06);
	Array.Add(0x01);
	Array.Add(0x06);
	Array.Add(0x0a);
	Array.Add(0x57);
	
	m_cComm.put_Output(COleVariant(Array));
	SetTimer(1, 5000, NULL);
	m_dataMotor.Format(_T("窗帘已打开"));

}


//模拟关闭窗帘//电机反转
void Cport1Dlg::OnBn_Close()
{
	// TODO:  在此添加控件通知处理程序代码
	CByteArray Array;

	//控制消息
	Array.Add(0x40);
	Array.Add(0x06);
	Array.Add(0x01);
	Array.Add(0x06);
	Array.Add(0x0b);
	Array.Add(0x58);

	m_cComm.put_Output(COleVariant(Array));
	SetTimer(1, 5000, NULL);
	m_dataMotor.Format(_T("窗帘已关闭"));
}


void Cport1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnTimer(nIDEvent);
	CByteArray Array;
	switch (nIDEvent)
	{
	case 1:{
		Array.Add(0x40);
		Array.Add(0x06);
		Array.Add(0x01);
		Array.Add(0x06);
		Array.Add(0x0c);
		Array.Add(0x59);
		m_cComm.put_Output(COleVariant(Array));
	}
		break;
	case 2:{

		VARIANT variant_inp;
		COleSafeArray safearray_inp;
		long len, k;
		byte rxdata[512]; //设置BYTE数组
		unsigned char temp[400];

		if (m_cComm.get_CommEvent() == 2) //值为表示接收缓冲区内有字符
		{
			//SetTimer(2, 99, NULL);
			variant_inp = m_cComm.get_Input(); //读缓冲区消息
			safearray_inp = variant_inp; ///变量转换
			len = safearray_inp.GetOneDimSize(); //得到有效的数据长度
			for (k = 0; k < len; k++)
				safearray_inp.GetElement(&k, rxdata + k);
			for (k = 0; k < len; k++) //将数组转换为CString型变量
			{
				unsigned char bt = *(unsigned char*)(rxdata + k);//字符型
				temp[k] = bt;
			}
			if (temp[0] == (byte)0x40)
			{
				//心跳消息
				if (temp[3] == (byte)0x00){
					sendmsg.Format(_T("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x "), temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8], temp[9], temp[10], temp[11], temp[12], temp[13], temp[14], temp[15]); //将字符送入临时变量strtemp存放
				}
				//震动传感器
				if ((temp[3] == (byte)0x03) && (temp[4] == (byte)0x01)){
					sendmsg.Format(_T("%02x %02x %02x %02x %02x %02x %02x "), temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6]); //将字符送入临时变量strtemp存放
					if (temp[5] == (byte)0x01){
						m_DataHall.Format(_T("检测到震动"));
					}
					if (temp[5] == (byte)0x00){
						m_DataHall.Format(_T("未检测到震动"));
					}
				}
				//温湿度、光照传感器

				if ((temp[3] == (byte)0x2) && (temp[4] == (byte)0x1)){
					sendmsg.Format(_T("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x "), temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8], temp[9], temp[10], temp[11]); //将字符送入临时变量strtemp存放

					int tem = temp[5] * 256 + temp[6];
					int hum = temp[7] * 256 + temp[8];
					long light = (long)((temp[9] * 256 + temp[10])*3012.9 / (32768 * 4));
					m_strRecvData.Format(_T("%d ℃"), tem);
					m_recvData_h.Format(_T("%d %%"), hum);
					m_recvData_l.Format(_T("%ld LX"), light);

					//模拟根据温湿度自动控制

					int tem_temp, hum_temp;
					char * temp1 = (char*)(LPCTSTR)m_temp_tem;
					char * temp2 = (char*)(LPCTSTR)m_temp_hum;
					tem_temp = atoi(temp1);
					hum_temp = atoi(temp2);
					//温度
					if (((tem < tem_temp - 3) || (tem >tem_temp + 3)) && (m_temp_tem != "")){
						OnBnClickedKt();  //打开
					}

					if (((tem > tem_temp - 3) && (tem < tem_temp + 3)) && (m_temp_tem != "")){
						OnBnClickedKt2();  //关闭
						//sendmsg.Format(_T("实际温度：%d 设定温度：%d"), tem, tem_temp);
					}


					//湿度
					if ((hum < hum_temp) && (m_temp_hum != "")){
						OnBnClickedJsq(); //打开
					}
					if ((tem > hum_temp) && (m_temp_hum != "")){
						OnBnClickedJsq2();  //关闭
					}
				}

			}
		}

		//闹钟相关
		CString tm, tmNew;
		CTime t = CTime::GetCurrentTime();
		tm = t.Format("%H%M");
		tmNew.Format(_T("%s%s"), m_hour, m_min);
		m_systime = t.Format("当前：%H 时 %M 分");
		UpdateData(FALSE); //更新编辑框内容

		if (tm == tmNew){
			CByteArray Array;
			Array.Add(0x40);
			Array.Add(0x06);
			Array.Add(0x01);
			Array.Add(0x09);
			Array.Add(0x09);
			Array.Add(0x59);
			m_cComm.put_Output(COleVariant(Array));
			//可以触发其他的事件
		}
	}
		break;
	default:
		break;
	}
		//控制消息
		

	KillTimer(nIDEvent);
}


//继电器模块  模拟空调（1路）和加湿器（2路）
//继电器模块  模拟空调（1路） //打开
void Cport1Dlg::OnBnClickedKt()
{
	// TODO:  在此添加控件通知处理程序代码
	CByteArray Array;
	//控制消息
	Array.Add(0x40);
	Array.Add(0x07);
	Array.Add(0x01);
	Array.Add(0x0A);
	Array.Add(0x01);
	Array.Add(0x01);
	Array.Add(0x54);
	m_cComm.put_Output(COleVariant(Array));
	m_dataAir.Format(_T("空调已打开"));

}

//关闭
void Cport1Dlg::OnBnClickedKt2()
{
	// TODO:  在此添加控件通知处理程序代码
	CByteArray Array;
	//控制消息
	Array.Add(0x40);
	Array.Add(0x07);
	Array.Add(0x01);
	Array.Add(0x0A);
	Array.Add(0x01);
	Array.Add(0x00);
	Array.Add(0x53);
	m_cComm.put_Output(COleVariant(Array));
	m_dataAir.Format(_T("空调已关闭"));


}

//继电器模块  模拟加湿器（2路） //打开
void Cport1Dlg::OnBnClickedJsq()
{
	// TODO:  在此添加控件通知处理程序代码
	CByteArray Array;
	//控制消息
	Array.Add(0x40);
	Array.Add(0x07);
	Array.Add(0x01);
	Array.Add(0x0A);
	Array.Add(0x02);
	Array.Add(0x02);
	Array.Add(0x56);
	m_cComm.put_Output(COleVariant(Array));
	m_dataJSQ.Format(_T("加湿器已打开"));
}

//关闭
void Cport1Dlg::OnBnClickedJsq2()
{
	// TODO:  在此添加控件通知处理程序代码
	CByteArray Array;
	Array.Add(0x40);
	Array.Add(0x07);
	Array.Add(0x01);
	Array.Add(0x0A);
	Array.Add(0x02);
	Array.Add(0x00);
	Array.Add(0x54);
	m_cComm.put_Output(COleVariant(Array));
	m_dataJSQ.Format(_T("加湿器已关闭"));
}

//灯光控制
void Cport1Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	*pResult = 0;
}


void Cport1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	UpdateData(TRUE);
	m_nops = m_slider_ctrl.GetPos();
	//m_recvData_l.Format(_T("%d"), m_nops);
	UpdateData(FALSE);

	CByteArray Array;
	Array.Add(0x40);
	Array.Add(0x06);
	Array.Add(0x01);
	Array.Add(0x09);
	Array.Add(m_nops);
	Array.Add(0x50 + m_nops);
	m_cComm.put_Output(COleVariant(Array));

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}




void Cport1Dlg::OnBnClickedNAOZHONG()
{
	// TODO:  在此添加控件通知处理程序代码
	//CString hou,mini;
	
	GetDlgItem(IDC_EDIT9)->GetWindowText(m_hour);
	GetDlgItem(IDC_EDIT10)->GetWindowText(m_min);
	if ((m_hour != "") && (m_min != "")){
		MessageBox("设置闹钟成功");
	}
	else
	{
		MessageBox("设置闹钟失败");
	}


}


void Cport1Dlg::OnClose_port()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_cComm.get_PortOpen()) //如果发现串口本来是打开的，则关闭串口
	{
		m_cComm.put_PortOpen(FALSE);
	}
}
