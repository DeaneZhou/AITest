
// AITestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AITest.h"
#include "AITestDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int model_magic_word = 13247547;
int total_imageCnt = 70000000;
bool load_success = false;

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


// CAITestDlg 对话框



CAITestDlg::CAITestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AITEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAITestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_LOAD, m_progressBar);
}

BEGIN_MESSAGE_MAP(CAITestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CAITestDlg::OnBnClickedButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CAITestDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CAITestDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PRE, &CAITestDlg::OnBnClickedButtonPre)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LOADMODEL, &CAITestDlg::OnClickedButtonLoadmodel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAITestDlg 消息处理程序

BOOL CAITestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_AUTO)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_MDL)->SetWindowTextW(_T("Please Load Model"));

	total_predict = 0;
	correct_predict = 0;
	predict_rst = false;
	timerRun = false;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAITestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAITestDlg::OnPaint()
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
HCURSOR CAITestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAITestDlg::OnBnClickedButtonAuto()
{
	// TODO: 在此添加控件通知处理程序代码

		
	if (!timerRun)
	{
		SetTimer(1, 300, NULL);
		timerRun = true;
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_AUTO)->SetWindowTextW(_T("Stop"));
	}
	else
	{
		KillTimer(1);
		timerRun = false;
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_AUTO)->SetWindowTextW(_T("Auto"));
	}

}


void CAITestDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	
	GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(false);

	SetTimer(2, 100, NULL);

	pSthData pDataValue = new SthData;
	pDataValue->images = &AllImages;
	pDataValue->labels = &AllLabels;

	CWinThread* pThread;
	pThread = AfxBeginThread(MyLoadData, pDataValue);

}


void CAITestDlg::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	if (curPicID == AllImages.size()-1)
	{
		curPicID = 0;
	}
	else
	{
		curPicID += 1;
	}
	updateDlg();
}


void CAITestDlg::OnBnClickedButtonPre()
{
	// TODO: 在此添加控件通知处理程序代码

	if (curPicID == 0)
	{
		curPicID = (UINT)AllImages.size() - 1;
	}
	else
	{
		curPicID -= 1;
	}
	updateDlg();
}

void CAITestDlg::draw_9pixel(CImage &bmp, int j, int i)
{
	
	for (int k = 0; k < 3; k++)
	{
		for (int l = 0; l < 3; l++)
		{
			bmp.SetPixelRGB(3*j+l, 3*i+k, AllImages[curPicID][i * 28 + j], AllImages[curPicID][i * 28 + j], AllImages[curPicID][i * 28 + j]);
		}
	}
}

bool CAITestDlg::updateDlg()
{
	CImage bmp;
	BYTE* pLbl;

	if (0 == bmp.Create(28*3, 28*3, 24))
	{
		MessageBox(_T("Create img failed!"));
		return false;
	};

	for (int i = 0; i < 28; i++)
	{
		for (int j = 0; j < 28; j++)
		{
			draw_9pixel(bmp, j, i);
		}
	}

	pLbl = (BYTE*)&AllLabels[curPicID];

	CStatic* pic = (CStatic*)GetDlgItem(IDC_STATIC_PIC);
	pic->SetBitmap(bmp);

	total_predict += 1;
	CString str;

	BYTE predit = get_predict();

	str.Format(_T("%d"), predit);
	GetDlgItem(IDC_EDIT_PREDICT)->SetWindowTextW(str);

	if (predit == *pLbl){
		correct_predict += 1;
		predict_rst = true;
		GetDlgItem(IDC_EDIT_RSLT)->SetWindowTextW(_T("Right"));
	}
	else
	{
		predict_rst = false;
		GetDlgItem(IDC_EDIT_RSLT)->SetWindowTextW(_T("Wrong"));
	}
	float accuracy = correct_predict / float(total_predict);

	str.Format(_T("%d"), *pLbl);
	GetDlgItem(IDC_EDIT_ACT)->SetWindowTextW(str);
	str.Format(_T("%.1f"), accuracy*100);
	GetDlgItem(IDC_EDIT_ACY)->SetWindowTextW(str+"%");
	
	return true;
}

void CAITestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent) 
	{
	case 1:
		OnBnClickedButtonNext();
		break;

	case 2:
		float pct;
		int pos;

		pct =  AllImages.size() / (float) total_imageCnt;
		pos = (int)(pct * 100);

		m_progressBar.SetPos(pos);

		if (load_success && pos == 100)
		{
			if (AllImages.size() != AllLabels.size())
			{
				KillTimer(2);
				MessageBox(_T("Pls check the sizeof images and labels"));
				AllImages.clear();
				AllLabels.clear();
				load_success = false;
				m_progressBar.SetPos(0);
				GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(true);

			}
			else
			{
				KillTimer(2);
				GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_AUTO)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(true);
			}
			
		}
		break;

	}
		

	CDialogEx::OnTimer(nIDEvent);
}


UINT MyLoadData(LPVOID pParam)
{

	SthData* stcSthDataData = (SthData *)pParam;
	vector<char>* labels = stcSthDataData->labels;
	vector<vector<char>>* images = stcSthDataData->images;
	
	bool b_imgLoad = false;
	bool b_lblLoad = false;

	b_lblLoad = read_Mnist_Label("..\\MNIST_DATA\\t10k-labels.idx1-ubyte", *labels);
	b_imgLoad = read_Mnist_Images("..\\MNIST_DATA\\t10k-images.idx3-ubyte", *images);

	if (b_lblLoad && b_imgLoad)
	{
		load_success = true;
		return 1;
	}
	else
	{
		return 0;
	}
}

float* CAITestDlg::softmax(cv::Mat z) { //pass a Mat will size 10

	float sum = 0.0;
	for (int i = 0; i < 10; i++) {
		sum = sum + exp(z.at<float>(0, i));
	}

	float* rst=new float[10];
	for (int i = 0; i < 10; i++) {
		rst[i] = exp(z.at<float>(0, i)) / sum;
	}

	return rst;
}

int CAITestDlg::arg_max(float* z)
{
	
	int arg_num = 0;

	for (int i = 1; i < 10; i++) {
		if (z[i] > z[arg_num]) arg_num = i;
	}
	
	delete[]z;
	
	return arg_num;
}

BYTE CAITestDlg::get_predict()
{
	vector<float> img_f;
	for (int i = 0; i < AllImages[curPicID].size(); i++)
	{
		unsigned char pix_Value = 0;
		pix_Value = (unsigned char)AllImages[curPicID][i]; //Very import
		float tmp = static_cast<float>(pix_Value) / float(255);
		img_f.push_back(tmp);
	}

	cv::Mat img = cv::Mat(img_f, true);

	CString str;
	cv::Mat z_out;
	img = img.reshape(0, 1);
	mdl_w = mdl_w.reshape(0, 784);
	z_out = img * mdl_w + mdl_b; //mdl_w: 784x10; img: 784x1

	BYTE rst = (BYTE)arg_max(softmax(z_out));
	return rst;
}

void CAITestDlg::OnClickedButtonLoadmodel()
{
	// TODO: 在此添加控件通知处理程序代码
	CvtWb2V model = CvtWb2V("example.model");
	if (model.get_magicWord() != model_magic_word)
	{
		GetDlgItem(IDC_EDIT_MDL)->SetWindowTextW(_T("Model is Wrong"));
		return;
	}
	vector<float>* vec = model.get_vector();

	mdl_w = cv::Mat(vec[0], true);
	mdl_b = cv::Mat(vec[1], true).reshape(0, 1);

	delete[] vec;
	GetDlgItem(IDC_BUTTON_LOADMODEL)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_MDL)->SetWindowTextW(_T("Model is Ready"));
}


HBRUSH CAITestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_RSLT)
	{
		if (predict_rst == false)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		else
		{
			pDC->SetTextColor(RGB(0, 255, 0));
		}
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
