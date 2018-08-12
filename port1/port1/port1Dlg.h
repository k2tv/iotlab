
// port1Dlg.h : 头文件
//

#pragma once
#include "mscomm1.h"
#include "afxwin.h"
#include "afxcmn.h"


// Cport1Dlg 对话框
class Cport1Dlg : public CDialogEx
{
// 构造
public:
	Cport1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PORT1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strSendData;
	CString m_recvData_h;
	CString m_recvData_l;
	CMscomm1 m_cComm;
	DECLARE_EVENTSINK_MAP()
	void OnComm();
	afx_msg void OnButtonSend();
	afx_msg void OnEnChangeEditRecv();
	afx_msg void OnBnClickedButtonRecv();
	afx_msg void OnButtonRecv();

	CListCtrl	m_list;
	CString	m_add_name;
	CString	m_add_num;
	CString	m_add_phone;
	CString	m_add_sex;
	CString	m_edit_name;
	CString	m_edit_num;



	afx_msg void OnBn_Open();
	afx_msg void OnBnClickedSend();
	CString sendmsg;


	CComboBox setport;
	CComboBox setbaud;
	CEdit m_editctrl;
	CString m_strRecvData;
	CString m_DataHall;
	afx_msg void OnClickedShaok();
	CString m_dataMotor;
	afx_msg void OnBn_Close();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedKt();
	afx_msg void OnBnClickedKt2();
	CString m_dataAir;
	afx_msg void OnBnClickedJsq();
	afx_msg void OnBnClickedJsq2();
	CString m_dataJSQ;
	int m_slider;
	CSliderCtrl m_slider_ctrl;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_nops;

	CString m_systime;
	CString m_hour;
	CString m_min;
	afx_msg void OnBnClickedNAOZHONG();

	CString m_temp_tem;
	CString m_temp_hum;
	afx_msg void OnClose_port();
};
