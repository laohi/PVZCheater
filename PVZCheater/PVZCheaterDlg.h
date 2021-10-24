
// PVZCheaterDlg.h: 头文件
//

#pragma once


// CPVZCheaterDlg 对话框
class CPVZCheaterDlg : public CDialogEx
{
// 构造
public:
	CPVZCheaterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PVZCHEATER_DIALOG };
#endif

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
private:
	friend DWORD WINAPI MonitoringThreadProc(LPVOID lpParam);
	CButton m_sun;
	CButton m_kill;
	CButton m_cd;
	CButton m_plant;
	CButton m_zombie;
	CButton m_run;
	CButton m_coin;
	CButton m_cancel;
	afx_msg void OnBnClickedBlog();
	afx_msg void OnBnClickedKill();
	afx_msg void OnBnClickedCd();
	afx_msg void OnBnClickedPlant();
	afx_msg void OnBnClickedZombie();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedCoin();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
