
// port1Dlg.cpp : ʵ���ļ�
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



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Cport1Dlg �Ի���



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
	DDX_Control(pDX, IDC_SLIDER1, m_slider_ctrl);//����
	if (ser1 == "") //ser1 û�и�ֵ����ö�ٴ���
	{
		//����ע����ȡ������Ϣ
		HKEY hKey;
		if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey))   //�򿪴���ע����Ӧ�ļ�ֵ 
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
					&dwSize) == ERROR_NO_MORE_ITEMS)//   ö�ٴ���  
					break;
				setport.AddString(commName);   // commName���Ǵ�������  
				ser1 = "1";
				seri++;
			}
			//�߶���Ϣ
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


// Cport1Dlg ��Ϣ�������

BOOL Cport1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SkinH_Attach();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������


	m_slider_ctrl.SetRange(1,9);//���û�����Χ
	m_slider_ctrl.SetTicFreq(1);//ÿ1����λ��һ�̶�

	setbaud.SetCurSel(3);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cport1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cport1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(Cport1Dlg, CDialogEx)
	ON_EVENT(Cport1Dlg, IDC_MSCOMM1, 1, Cport1Dlg::OnComm, VTS_NONE)
END_EVENTSINK_MAP()


void Cport1Dlg::OnComm()
{
	// TODO:  �ڴ˴������Ϣ����������
	//�������
	SetTimer(2, 99, NULL);


}





void Cport1Dlg::OnEnChangeEditRecv()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void Cport1Dlg::OnButtonRecv()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}

void Cport1Dlg::OnBn_Open()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT14)->GetWindowText(m_temp_tem);
	GetDlgItem(IDC_EDIT15)->GetWindowText(m_temp_hum);

	int ser2;
	if (ser1 == "")
	{
		MessageBox("δ�����豸�����������⣡", "����");
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

		if (m_cComm.get_PortOpen()) //������ִ��ڱ����Ǵ򿪵ģ���رմ���
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
		m_cComm.put_CommPort(ser2); //ѡ��COM1�˿�
		m_cComm.put_InputMode(1); //���뷽ʽΪ�����Ʒ�ʽ
		m_cComm.put_InBufferSize(1024); //�������뻺����
		m_cComm.put_OutBufferSize(512); //�������������
		m_cComm.put_Settings(baud);//�����ʣ���У�飬������λ����ֹͣλ
		m_cComm.put_RThreshold(4); //ÿ���յ�4���ַ�ʱ������OnComm�¼�
		m_cComm.put_PortOpen(true); //�򿪴���
	}	
}

//ģ��򿪴���//�����ת
void Cport1Dlg::OnBnClickedSend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CByteArray Array;

	//������Ϣ
	Array.Add(0x40);
	Array.Add(0x06);
	Array.Add(0x01);
	Array.Add(0x06);
	Array.Add(0x0a);
	Array.Add(0x57);
	
	m_cComm.put_Output(COleVariant(Array));
	SetTimer(1, 5000, NULL);
	m_dataMotor.Format(_T("�����Ѵ�"));

}


//ģ��رմ���//�����ת
void Cport1Dlg::OnBn_Close()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CByteArray Array;

	//������Ϣ
	Array.Add(0x40);
	Array.Add(0x06);
	Array.Add(0x01);
	Array.Add(0x06);
	Array.Add(0x0b);
	Array.Add(0x58);

	m_cComm.put_Output(COleVariant(Array));
	SetTimer(1, 5000, NULL);
	m_dataMotor.Format(_T("�����ѹر�"));
}


void Cport1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
		byte rxdata[512]; //����BYTE����
		unsigned char temp[400];

		if (m_cComm.get_CommEvent() == 2) //ֵΪ��ʾ���ջ����������ַ�
		{
			//SetTimer(2, 99, NULL);
			variant_inp = m_cComm.get_Input(); //����������Ϣ
			safearray_inp = variant_inp; ///����ת��
			len = safearray_inp.GetOneDimSize(); //�õ���Ч�����ݳ���
			for (k = 0; k < len; k++)
				safearray_inp.GetElement(&k, rxdata + k);
			for (k = 0; k < len; k++) //������ת��ΪCString�ͱ���
			{
				unsigned char bt = *(unsigned char*)(rxdata + k);//�ַ���
				temp[k] = bt;
			}
			if (temp[0] == (byte)0x40)
			{
				//������Ϣ
				if (temp[3] == (byte)0x00){
					sendmsg.Format(_T("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x "), temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8], temp[9], temp[10], temp[11], temp[12], temp[13], temp[14], temp[15]); //���ַ�������ʱ����strtemp���
				}
				//�𶯴�����
				if ((temp[3] == (byte)0x03) && (temp[4] == (byte)0x01)){
					sendmsg.Format(_T("%02x %02x %02x %02x %02x %02x %02x "), temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6]); //���ַ�������ʱ����strtemp���
					if (temp[5] == (byte)0x01){
						m_DataHall.Format(_T("��⵽��"));
					}
					if (temp[5] == (byte)0x00){
						m_DataHall.Format(_T("δ��⵽��"));
					}
				}
				//��ʪ�ȡ����մ�����

				if ((temp[3] == (byte)0x2) && (temp[4] == (byte)0x1)){
					sendmsg.Format(_T("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x "), temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8], temp[9], temp[10], temp[11]); //���ַ�������ʱ����strtemp���

					int tem = temp[5] * 256 + temp[6];
					int hum = temp[7] * 256 + temp[8];
					long light = (long)((temp[9] * 256 + temp[10])*3012.9 / (32768 * 4));
					m_strRecvData.Format(_T("%d ��"), tem);
					m_recvData_h.Format(_T("%d %%"), hum);
					m_recvData_l.Format(_T("%ld LX"), light);

					//ģ�������ʪ���Զ�����

					int tem_temp, hum_temp;
					char * temp1 = (char*)(LPCTSTR)m_temp_tem;
					char * temp2 = (char*)(LPCTSTR)m_temp_hum;
					tem_temp = atoi(temp1);
					hum_temp = atoi(temp2);
					//�¶�
					if (((tem < tem_temp - 3) || (tem >tem_temp + 3)) && (m_temp_tem != "")){
						OnBnClickedKt();  //��
					}

					if (((tem > tem_temp - 3) && (tem < tem_temp + 3)) && (m_temp_tem != "")){
						OnBnClickedKt2();  //�ر�
						//sendmsg.Format(_T("ʵ���¶ȣ�%d �趨�¶ȣ�%d"), tem, tem_temp);
					}


					//ʪ��
					if ((hum < hum_temp) && (m_temp_hum != "")){
						OnBnClickedJsq(); //��
					}
					if ((tem > hum_temp) && (m_temp_hum != "")){
						OnBnClickedJsq2();  //�ر�
					}
				}

			}
		}

		//�������
		CString tm, tmNew;
		CTime t = CTime::GetCurrentTime();
		tm = t.Format("%H%M");
		tmNew.Format(_T("%s%s"), m_hour, m_min);
		m_systime = t.Format("��ǰ��%H ʱ %M ��");
		UpdateData(FALSE); //���±༭������

		if (tm == tmNew){
			CByteArray Array;
			Array.Add(0x40);
			Array.Add(0x06);
			Array.Add(0x01);
			Array.Add(0x09);
			Array.Add(0x09);
			Array.Add(0x59);
			m_cComm.put_Output(COleVariant(Array));
			//���Դ����������¼�
		}
	}
		break;
	default:
		break;
	}
		//������Ϣ
		

	KillTimer(nIDEvent);
}


//�̵���ģ��  ģ��յ���1·���ͼ�ʪ����2·��
//�̵���ģ��  ģ��յ���1·�� //��
void Cport1Dlg::OnBnClickedKt()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CByteArray Array;
	//������Ϣ
	Array.Add(0x40);
	Array.Add(0x07);
	Array.Add(0x01);
	Array.Add(0x0A);
	Array.Add(0x01);
	Array.Add(0x01);
	Array.Add(0x54);
	m_cComm.put_Output(COleVariant(Array));
	m_dataAir.Format(_T("�յ��Ѵ�"));

}

//�ر�
void Cport1Dlg::OnBnClickedKt2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CByteArray Array;
	//������Ϣ
	Array.Add(0x40);
	Array.Add(0x07);
	Array.Add(0x01);
	Array.Add(0x0A);
	Array.Add(0x01);
	Array.Add(0x00);
	Array.Add(0x53);
	m_cComm.put_Output(COleVariant(Array));
	m_dataAir.Format(_T("�յ��ѹر�"));


}

//�̵���ģ��  ģ���ʪ����2·�� //��
void Cport1Dlg::OnBnClickedJsq()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CByteArray Array;
	//������Ϣ
	Array.Add(0x40);
	Array.Add(0x07);
	Array.Add(0x01);
	Array.Add(0x0A);
	Array.Add(0x02);
	Array.Add(0x02);
	Array.Add(0x56);
	m_cComm.put_Output(COleVariant(Array));
	m_dataJSQ.Format(_T("��ʪ���Ѵ�"));
}

//�ر�
void Cport1Dlg::OnBnClickedJsq2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CByteArray Array;
	Array.Add(0x40);
	Array.Add(0x07);
	Array.Add(0x01);
	Array.Add(0x0A);
	Array.Add(0x02);
	Array.Add(0x00);
	Array.Add(0x54);
	m_cComm.put_Output(COleVariant(Array));
	m_dataJSQ.Format(_T("��ʪ���ѹر�"));
}

//�ƹ����
void Cport1Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}


void Cport1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CString hou,mini;
	
	GetDlgItem(IDC_EDIT9)->GetWindowText(m_hour);
	GetDlgItem(IDC_EDIT10)->GetWindowText(m_min);
	if ((m_hour != "") && (m_min != "")){
		MessageBox("�������ӳɹ�");
	}
	else
	{
		MessageBox("��������ʧ��");
	}


}


void Cport1Dlg::OnClose_port()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_cComm.get_PortOpen()) //������ִ��ڱ����Ǵ򿪵ģ���رմ���
	{
		m_cComm.put_PortOpen(FALSE);
	}
}
