
// PVZCheaterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PVZCheater.h"
#include "PVZCheaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 监控线程的刷新时间
#define MONITOR_REFRESH_TIME 1000
// 植物大战僵尸的进程句柄
static HANDLE g_process_handle;
// 用来监控植物大战僵尸的线程句柄
static HANDLE g_monitor_thread_handle;
// 辅助窗口
static CPVZCheaterDlg* g_dlg;

// 将某个值写入植物大战僵尸内存（后面的可变参数是地址链，要以-1结尾）
void WriteMemory(void* value, DWORD valueSize, ...) {
	if (value == NULL || valueSize == 0 || g_process_handle == NULL) return;

	DWORD tempValue = 0;

	va_list addresses;
	va_start(addresses, valueSize);
	DWORD offset = 0;
	DWORD lastAddress = 0;
	while ((offset = va_arg(addresses, DWORD)) != -1) {
		lastAddress = tempValue + offset;
		ReadProcessMemory(g_process_handle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
	}
	va_end(addresses);

	WriteProcessMemory(g_process_handle, (LPVOID)lastAddress, value, valueSize, NULL);
}

// 将某个值写入植物大战僵尸内存
void WriteMemory(void* value, DWORD valueSize, DWORD address) {
	WriteMemory(value, valueSize, address, -1);
}

// 用来监控植物大战僵尸的线程
DWORD WINAPI MonitoringThreadProc(LPVOID lpParam) {
	while (1) {
		// 找窗口
		HWND hwnd = ::FindWindow(TEXT("MainWindow"), TEXT("植物大战僵尸中文版"));
		if (hwnd == NULL) {
			CloseHandle(g_process_handle);
			g_process_handle = NULL;

			// 游戏没有运行，禁止以下按钮点击
			g_dlg->m_sun.SetCheck(FALSE);
			g_dlg->m_sun.EnableWindow(FALSE);
			
			g_dlg->m_kill.SetCheck(FALSE);
			g_dlg->m_kill.EnableWindow(FALSE);
			g_dlg->OnBnClickedKill();
			
			g_dlg->m_cd.SetCheck(FALSE);
			g_dlg->m_cd.EnableWindow(FALSE);
			g_dlg->OnBnClickedCd();
			
			g_dlg->m_plant.SetCheck(FALSE);
			g_dlg->m_plant.EnableWindow(FALSE);
			g_dlg->OnBnClickedPlant();
			
			g_dlg->m_zombie.SetCheck(FALSE);
			g_dlg->m_zombie.EnableWindow(FALSE);
			g_dlg->OnBnClickedZombie();
			
			g_dlg->m_run.SetCheck(FALSE);
			g_dlg->m_run.EnableWindow(FALSE);
			g_dlg->OnBnClickedRun();
			
			g_dlg->m_coin.EnableWindow(FALSE);
			
			g_dlg->m_cancel.EnableWindow(FALSE);

		} else if (g_process_handle == NULL) {
			// 找进程ID
			DWORD pid = NULL;
			GetWindowThreadProcessId(hwnd, &pid);

			// 找进程句柄
			g_process_handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

			// 游戏正在运行，恢复以下按钮点击
			g_dlg->m_sun.EnableWindow(TRUE);
			g_dlg->m_kill.EnableWindow(TRUE);
			g_dlg->m_cd.EnableWindow(TRUE);
			g_dlg->m_plant.EnableWindow(TRUE);
			g_dlg->m_zombie.EnableWindow(TRUE);
			g_dlg->m_run.EnableWindow(TRUE);
			g_dlg->m_coin.EnableWindow(TRUE);
			g_dlg->m_cancel.EnableWindow(TRUE);
		}

		if (g_dlg->m_sun.GetCheck()) { // 如果需要无限阳光
			DWORD value = 9950;
			WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x320, 0x8, 0x0, 0x8, 0x144, 0x2c, 0x5560, -1);
		}

		// 间隔1秒
		Sleep(MONITOR_REFRESH_TIME);
	}
	return 0;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPVZCheaterDlg 对话框



CPVZCheaterDlg::CPVZCheaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PVZCHEATER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPVZCheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUN, m_sun);
	DDX_Control(pDX, IDC_KILL, m_kill);
	DDX_Control(pDX, IDC_CD, m_cd);
	DDX_Control(pDX, IDC_PLANT, m_plant);
	DDX_Control(pDX, IDC_ZOMBIE, m_zombie);
	DDX_Control(pDX, IDC_RUN, m_run);
	DDX_Control(pDX, IDC_COIN, m_coin);
	DDX_Control(pDX, IDC_CANCEL, m_cancel);
}

BEGIN_MESSAGE_MAP(CPVZCheaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BLOG, &CPVZCheaterDlg::OnBnClickedBlog)
	ON_BN_CLICKED(IDC_KILL, &CPVZCheaterDlg::OnBnClickedKill)
	ON_BN_CLICKED(IDC_CD, &CPVZCheaterDlg::OnBnClickedCd)
	ON_BN_CLICKED(IDC_PLANT, &CPVZCheaterDlg::OnBnClickedPlant)
	ON_BN_CLICKED(IDC_ZOMBIE, &CPVZCheaterDlg::OnBnClickedZombie)
	ON_BN_CLICKED(IDC_RUN, &CPVZCheaterDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDC_COIN, &CPVZCheaterDlg::OnBnClickedCoin)
	ON_BN_CLICKED(IDC_CANCEL, &CPVZCheaterDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPVZCheaterDlg 消息处理程序

BOOL CPVZCheaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码

	// 启动监控线程
	g_monitor_thread_handle = ::CreateThread(NULL, 0, MonitoringThreadProc, NULL, 0, NULL);
	
	// 保存对话框
	g_dlg = this;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPVZCheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPVZCheaterDlg::OnPaint()
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
HCURSOR CPVZCheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 访问我的博客
void CPVZCheaterDlg::OnBnClickedBlog() {
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, CString("open"), 
		CString("https://blog.laohai.xyz"), 
		NULL, NULL, SW_SHOWNORMAL);
}

// 秒杀僵尸
void CPVZCheaterDlg::OnBnClickedKill() {
	// TODO: 在此添加控件通知处理程序代码
	DWORD address1 = 0x53130F;
	DWORD address2 = 0x531066;
	DWORD address3 = 0x530CB1;
	if (this->m_kill.GetCheck()) {
		// 选中秒杀僵尸，去除僵尸不死
		g_dlg->m_zombie.SetCheck(FALSE);
		g_dlg->OnBnClickedZombie();

		BYTE data1[] = { 0x2B, 0xFF, 0x90, 0x90 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3);
	} else {
		// 取消秒杀僵尸
		BYTE data1[] = { 0x2B, 0x7C, 0x24, 0x20 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x75, 0x11 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x75, 0x17 };
		WriteMemory(data3, sizeof(data3), address3);
	}
}

// 无CD
void CPVZCheaterDlg::OnBnClickedCd() {
	// TODO: 在此添加控件通知处理程序代码
	DWORD address = 0x487296;
	if (this->m_cd.GetCheck()) {
		// 选中无CD
		BYTE data[] = { 0x90, 0x90 };
		WriteMemory(data, sizeof(data), address);
	} else {
		// 取消无CD
		BYTE data[] = { 0x7E, 0x14 };
		WriteMemory(data, sizeof(data), address);
	}
}

// 植物不死
void CPVZCheaterDlg::OnBnClickedPlant() {
	// TODO: 在此添加控件通知处理程序代码
	DWORD address1 = 0x52FCF0;
	DWORD address2 = 0x46D7A6;
	DWORD address3 = 0x45EC63;
	DWORD address4 = 0x46CFEB;
	if (this->m_plant.GetCheck()) {
		// 选中植物不死
		BYTE data1[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x90, 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3);

		BYTE data4[] = { 0x90, 0x90, 0x90 };
		WriteMemory(data4, sizeof(data4), address4);
	} else {
		// 取消植物不死
		BYTE data1[] = { 0x83, 0x46, 0x40, 0xFC };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x29, 0x4E, 0X40 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x83, 0x46, 0x40, 0xCE };
		WriteMemory(data3, sizeof(data3), address3);

		BYTE data4[] = { 0x29, 0x50, 0x40 };
		WriteMemory(data4, sizeof(data4), address4);
	}
}

// 僵尸不死
void CPVZCheaterDlg::OnBnClickedZombie() {
	// TODO: 在此添加控件通知处理程序代码
	DWORD address1 = 0x53130F;
	DWORD address2 = 0x531042;
	DWORD address3 = 0x530C9F;
	if (this->m_zombie.GetCheck()) {
		// 选中僵尸不死，去除秒杀僵尸
		g_dlg->m_kill.SetCheck(FALSE);
		g_dlg->OnBnClickedKill();

		BYTE data1[] = { 0x83, 0xEF, 0x00, 0x90 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3, -1);
	} else {
		// 取消僵尸不死
		BYTE data1[] = { 0x2B, 0x7C, 0x24, 0x20 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x2B, 0xF0, 0x2B, 0xC8 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x2B, 0xD0, 0x29, 0x86, 0xDC, 0x00, 0x00, 0x00 };
		WriteMemory(data3, sizeof(data3), address3);
	}
}

// 后台运行
void CPVZCheaterDlg::OnBnClickedRun() {
	// TODO: 在此添加控件通知处理程序代码
	DWORD address = 0x54E1C2;
	if (this->m_run.GetCheck()) {
		// 选中后台运行
		BYTE data[] = { 0x90, 0x90, 0x90 };
		WriteMemory(data, sizeof(data), address);
	} else {
		// 取消后台运行
		BYTE data[] = { 0x0F, 0x95, 0xC0 };
		WriteMemory(data, sizeof(data), address);
	}
}

// 无限金币
void CPVZCheaterDlg::OnBnClickedCoin() {
	// TODO: 在此添加控件通知处理程序代码
	DWORD value = 88888;
	WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x82C, 0x28, -1);
}

// 一键取消
void CPVZCheaterDlg::OnBnClickedCancel() {
	// TODO: 在此添加控件通知处理程序代码
	this->m_sun.SetCheck(FALSE);

	this->m_kill.SetCheck(FALSE);
	this->OnBnClickedKill();

	this->m_cd.SetCheck(FALSE);
	this->OnBnClickedCd();

	this->m_plant.SetCheck(FALSE);
	this->OnBnClickedPlant();

	this->m_zombie.SetCheck(FALSE);
	this->OnBnClickedZombie();

	this->m_run.SetCheck(FALSE);
	this->OnBnClickedRun();
}

// 关闭窗口
void CPVZCheaterDlg::OnClose() {
	CDialogEx::OnClose();

	// 去掉外挂效果
	this->OnBnClickedCancel();

	// 杀死线程
	TerminateThread(g_monitor_thread_handle, 0);
	CloseHandle(g_monitor_thread_handle);

	// 关闭句柄
	CloseHandle(g_process_handle);
}
