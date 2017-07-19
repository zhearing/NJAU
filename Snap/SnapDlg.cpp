
// SnapDlg.cpp : 实现文件
#include "stdafx.h"
#include "Snap.h"
#include "SnapDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 通过串口传输给控制板的抓取指令，分别代表每个舵机转动的角度
const CString grabCommand[8][7] =
{
	{ "#1P900#2P1100#3P544#4P1144#5P2144#6P21100T500\r\n",
	"#1P900#3P500#4P744#5P1967T500\r\n",
	"#1P1456T500\r\n",
	"#4P1144#5P2211T500\r\n",
	"#3P1056#4P1700#5P2300T500\r\n",
	"#1P900T500\r\n",
	"#3P544#4P1144#5P2144T500\r\n" },
	{ "#1P900#2P1100#3P544#4P1144#5P2144#6P21100T500\r\n",
	"#3P544#4P744#5P1878T500\r\n",
	"#1P1456#2P1100#3P544#4P744#5P1878T500\r\n",
	"#4P1478T500\r\n",
	"#3P1433#4P1567#5P21100T500\r\n",
	"#1P900#3P1256#4P1611#5P2233T500\r\n",
	"#3P544#4P1144#5P2144T500\r\n" },
	{ "#1P900#2P1100#3P544#4P1144#5P2144#6P21100T500\r\n",
	"#1P900#3P500#4P1100#5P1611T500\r\n",
	"#1P1456T500\r\n",
	"#4P1611#5P2033T500\r\n",
	"#3P1078#4P1656#5P2322T500\r\n",
	"#1P900T500\r\n",
	"#3P544#4P1144#5P2144T500\r\n" },
	{ "#1P900#2P1100#3P544#4P1144#5P2144#6P21100T500\r\n",
	"#1P900#3P522#4P1211#5P1500T500\r\n",
	"#1P1456#4P1211#5P1500T500\r\n",
	"#4P1678#5P1900T500\r\n",
	"#3P1122#4P1700#5P2300T500\r\n",
	"#1P900T500\r\n",
	"#3P544#4P1144#5P2144T500\r\n" },
	{ "#1P900#2P1100#3P544#4P1144#5P2144#6P21100T500\r\n",
	"#1P900#2P1100#3P678#4P1167#5P1544T500\r\n",
	"#1P900#3P656#4P1122#5P1522T500\r\n",
	"#1P1456#3P856T500\r\n",
	"#4P1633#5P1789T500\r\n",
	"#1P900#3P1056#4P1744#5P2367T500\r\n",
	"#3P544#4P1144#5P2144T500\r\n" },
	{ "#1P900#2P1100#3P544#4P1144#5P2144#6P21100T500\r\n",
	"#3P522#4P1456#5P1322T500\r\n",
	"#1P1456T500\r\n",
	"#4P1811#5P1878T500\r\n",
	"#3P1167#4P1744#5P2344T500\r\n",
	"#1P900T500\r\n",
	"#3P544#4P1144#5P2144T500\r\n" },
	{ "#1P900#2P1100#3P544#4P1144#5P2144#6P21100T500\r\n",
	"#3P967#5P1411T500\r\n",
	"#1P1456T500\r\n",
	"#4P1678#5P1633T500\r\n",
	"#3P1011#4P1767#5P2389T500\r\n",
	"#1P900T500\r\n",
	"#3P544#4P1144#5P2144T500\r\n" },
	{ "#1P900#2P1100#3P544#4P1144#5P2144#6P21100T500\r\n",
	"#3P1033#4P1256#5P1344T500\r\n",
	"#1P1456T500\r\n",
	"#3P1433#4P1411#5P1767T500\r\n",
	"#3P967#4P1722#5P2278T500\r\n",
	"#1P900T500\r\n",
	"#3P544#4P1144#5P2144T500\r\n" }
};
Mat frame;

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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSnapDlg 对话框



CSnapDlg::CSnapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSnapDlg::IDD, pParent)
	, m_strRXData(_T(""))
	, m_strTXData(_T(""))
	, m_coordinate(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm);
	DDX_Text(pDX, IDC_EDIT_RXDATA, m_strRXData);
	DDX_Text(pDX, IDC_EDIT_TXDATA, m_strTXData);
	//  DDX_Control(pDX, IDC_EDIT_COORDINATE, CCoordinate);
	DDX_Text(pDX, IDC_EDIT_COORDINATE, m_coordinate);
}

BEGIN_MESSAGE_MAP(CSnapDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND2_5, &CSnapDlg::OnBnClickedButtonSend25)
	ON_BN_CLICKED(IDC_BUTTON_SEND5, &CSnapDlg::OnBnClickedButtonSend5)
	ON_BN_CLICKED(IDC_BUTTON_SEND7_5, &CSnapDlg::OnBnClickedButtonSend75)
	ON_BN_CLICKED(IDC_BUTTON_SEND10, &CSnapDlg::OnBnClickedButtonSend10)
	ON_BN_CLICKED(IDC_BUTTON_SEND12_5, &CSnapDlg::OnBnClickedButtonSend125)
	ON_BN_CLICKED(IDC_BUTTON_SEND15, &CSnapDlg::OnBnClickedButtonSend15)
	ON_BN_CLICKED(IDC_BUTTON_SEND17_5, &CSnapDlg::OnBnClickedButtonSend175)
	ON_BN_CLICKED(IDC_BUTTON_SEND20, &CSnapDlg::OnBnClickedButtonSend20)
	ON_BN_CLICKED(IDC_BUTTON_START, &CSnapDlg::OnBnClickedButtonStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSnapDlg 消息处理程序

BOOL CSnapDlg::OnInitDialog()
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

	//搜索串口
	//HANDLE m_hCom;
	//CString com[6] = { "COM1", "COM2", "COM3", "COM4", "COM5", "COM6" };
	//CString str = "";
	//int cnt = 0;
	//for (int i = 0; i < 6; i++)
	//{
	//	m_hCom = CreateFile(com[i], GENERIC_READ | GENERIC_WRITE, 0, NULL,
	//		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//	// 这里的CreateFile函数起了很大的作用，可以用来创建系统设备文件，
	//	//如果该设备不存在或者被占用，则会返回一个错误，
	//	//即下面的 INVALID_HANDLE_VALUE ，据此可以判断可使用性。详细参见MSDN中的介绍。
	//	if (m_hCom == INVALID_HANDLE_VALUE)	// 如果没有该设备，或者被其他应用程序在用   
	//	{
	//		str += com[i];// 记录下该串口名称，以备后面提示用
	//		str += " ";
	//	}
	//	else
	//		cnt = i + 1;// 如果存在，则记录下来。
	//	//这里只记录了一个，也可以采用一个数组来记录所有存在串口；
	//	CloseHandle(m_hCom);// 关闭文件句柄，后面我们采用控件，不用API
	//}
	//if (cnt)// 如果串口存在，则执行相应的初始化（采用控件）
	//{
	//	if (m_ctrlComm.get_PortOpen())// m_ctrlMscom是控件的一个实例
	//		m_ctrlComm.put_PortOpen(FALSE);
	//	m_ctrlComm.put_CommPort(cnt);
	//	if (!m_ctrlComm.get_PortOpen())
	//		m_ctrlComm.put_PortOpen(TRUE);
	//	m_ctrlComm.put_RThreshold(2);//收到两个字节引发OnComm事件	
	//	m_ctrlComm.put_Settings(_T("115200,n,8,1"));//设置串口参数，波特率，无奇偶校验，位停止位，位数据位	
	//	m_ctrlComm.put_InputMode(1);  // 以二进制方式检取数据 
	//	m_ctrlComm.put_RThreshold(1); //参数1表示每当串口接收缓冲区中有多于或等于1个字符时
	//	//将引发一个接收数据的OnComm事件 
	//	m_ctrlComm.put_InputLen(0); //设置当前接收区数据长度为0 
	//	m_ctrlComm.get_Input();//先预读缓冲区以清除残留数据 
	//}
	//else	   // 如果不存在，则显示错误信息，而不进行串口操作，防止系统异常造成界面的初始化不完全
	//	AfxMessageBox(str + "doesn't exist or is being used by other program", MB_OK);

	SetTimer(1, 1000, NULL); //ID为1，定时1000ms
	SetTimer(2, 10, NULL); //ID为2，定时1000ms

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSnapDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSnapDlg::OnPaint()
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
HCURSOR CSnapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




BEGIN_EVENTSINK_MAP(CSnapDlg, CDialogEx)
	ON_EVENT(CSnapDlg, IDC_MSCOMM1, 1, CSnapDlg::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()

//serial port communication
void CSnapDlg::OnCommMscomm1()
{
	//VARIANT variant_inp;
	//COleSafeArray safearray_inp;
	//LONG len, k;
	//BYTE rxdata[2048];
	//CString strtemp;
	//if (m_ctrlComm.get_CommEvent() == 2) //事件值为2表示接收缓冲区内有字符     
	//{
	//	variant_inp = m_ctrlComm.get_Input(); //读缓冲区  
	//	safearray_inp = variant_inp; //VARIANT型变量转换为ColeSafeArray型变量
	//	len = safearray_inp.GetOneDimSize(); //得到有效数据长度        
	//	for (k = 0; k < len; k++)
	//		safearray_inp.GetElement(&k, rxdata + k);//转换为BYTE型数组         
	//	for (k = 0; k < len; k++)                    //将数组转换为Cstring型变量    
	//	{
	//		BYTE bt = *(char*)(rxdata + k);//字符型      
	//		strtemp.Format("%c", bt); //将字符送入临时变量strtemp存放   
	//		//此处编译有错误，可在项目属性->配置属性->常规里将“字符集”改为使用多字节字符集即可
	//		m_strRXData += strtemp; //加入接收编辑框对应字符串      
	//	}
	//}
	//m_strRXData = m_strRXData + "\r\n";
	//UpdateData(FALSE); //更新编辑框内容 
}

int CSnapDlg::otsu(IplImage* src)
{
	int i = 0, j = 0;
	int hei = src->height;
	int wid = src->width;
	long N = hei * wid;
	int h[256];
	double p[256], u[256], w[256];
	int nongdu = 0;
	int k = 0;
	for (i = 0; i < 256; i++)
	{
		h[i] = 0;
		p[i] = 0;
		u[i] = 0;
		w[i] = 0;
	}
	for (i = 0; i < hei; i++)
	{
		for (j = 0; j < wid; j++)
		{
			k = ((uchar*)(src->imageData + src->widthStep*i))[j];
			h[k]++;
		}
	}
	for (i = 0; i < 256; i++)
		p[i] = h[i] / double(N);
	int T = 0;
	double uT, thegma2fang;
	double thegma2fang_max = -10000;
	for (k = 0; k < 256; k++)
	{
		uT = 0;
		for (int i = 0; i <= k; i++)
		{
			u[k] += i*p[i];
			w[k] += p[i];
		}
		for (i = 0; i < 256; i++)
			uT += i*p[i];

		thegma2fang = (uT*w[k] - u[k])*(uT*w[k] - u[k]) / (w[k] * (1 - w[k]));
		if (thegma2fang > thegma2fang_max)
		{
			thegma2fang_max = thegma2fang;
			T = k;
		}
	}
	return T;
}

double CSnapDlg::ThresholdTrans(IplImage* src, IplImage* dest, int num_R, int num_G, int num_B)
{
	int nongdu = 0;
	int hei = src->height;
	int wid = src->width;
	long N = hei * wid;
	IplImage* pImgthresh;
	pImgthresh = cvCreateImage(cvGetSize(src), src->depth, 3);
	for (int i = 0; i < hei; i++)
	{
		for (int j = 0; j < wid; j++)
		{
			CvScalar s;
			int B1 = (int)cvGet2D(src, i, j).val[0];
			int G1 = (int)cvGet2D(src, i, j).val[1];
			int R1 = (int)cvGet2D(src, i, j).val[2];
			if (B1 > num_B)
				s.val[0] = 255;
			else
				s.val[0] = 0;
			if (G1 > num_G)
				s.val[1] = 255;
			else
				s.val[1] = 0;
			if (R1 > num_R)
				s.val[2] = 255;
			else
				s.val[2] = 0;
			cvSet2D(pImgthresh, i, j, s);
			B1 = (int)cvGet2D(pImgthresh, i, j).val[0];
			G1 = (int)cvGet2D(pImgthresh, i, j).val[1];
			R1 = (int)cvGet2D(pImgthresh, i, j).val[2];
			if (B1 == 0 && G1 == 255 && R1 == 0)
			{
				s.val[0] = 255;
				s.val[1] = 255;
				s.val[2] = 255;
				cvSet2D(dest, i, j, s);
			}
			else
			{
				s.val[0] = 0;
				s.val[1] = 0;
				s.val[2] = 0;
				cvSet2D(dest, i, j, s);
				nongdu++;
			}
		}
	}
	cvReleaseImage(&pImgthresh);
	return (double)nongdu / N * 100;
}

void CSnapDlg::Rgb2Hsi(IplImage* src, IplImage* dataH, IplImage* dataS, IplImage* dataI)
{
	//输入为彩色图像src, 输出dataH,dataS,dataI,分别为HSI模型的H、S、I分量
	int y, x, offsetSrc, offsetHsi;
	int step, channels, step1;
	int r, g, b, minRgb;
	double cosThita, num, den, S, H;
	uchar *dataImg;
	uchar *dataS1;
	uchar *dataI1;
	uchar *dataH1;
	channels = src->nChannels;
	dataImg = (uchar *)src->imageData;
	dataI1 = (uchar *)dataI->imageData;
	dataH1 = (uchar *)dataH->imageData;
	dataS1 = (uchar *)dataS->imageData;
	step = src->widthStep;
	step1 = dataI->widthStep;
	for (y = 0; y < src->height; y++)
	{
		for (x = 0; x < src->width; x++)
		{
			offsetSrc = y*step + x*channels;
			offsetHsi = y*step1 + x;

			b = dataImg[offsetSrc];
			g = dataImg[offsetSrc + 1];
			r = dataImg[offsetSrc + 2];

			dataI1[offsetHsi] = (int)((r + g + b) / 3 + 0.5);
			num = (2 * r - g - b) / 2;
			den = sqrt(1.0*((r - g)*(r - g) + (r - b)*(g - b)));
			if (den == 0)
				den = 0.01;
			cosThita = acos(num / den);
			minRgb = minn(r, g, b);

			den = r + g + b;
			if (den == 0)
				den = 0.01;
			S = 1 - 3 * minRgb / den;

			dataS1[offsetHsi] = (int)(S * 255 + 0.5);//将S分量和H分量都扩充到[0,255]区间以便于显示
			//一般H分量在[0,2pi]之间，S在[0,1]之间
			if (b <= g)
				H = cosThita / (2 * 3.14);
			else
				H = (2 * 3.14 - cosThita) / (2 * 3.14);
			dataH1[offsetHsi] = (int)(H * 255 + 0.5);
			if (S == 0)
				dataH1[offsetHsi] = 0;
		}
	}
}

void CSnapDlg::grabYellow()
{
	IplImage *src;//定义源图像指针
	IplImage *dstContours;//定义轮廓图像指针
	IplConvKernel *element = 0;//定义形态学结构指针
	int numObject = 0; //定义目标对象数量
	IplImage *img;
	img = &IplImage(frame);
	//IplImage *img = cvLoadImage("veg.bmp", -1);//flags=-1表示默认读入的图像为原通道图
	IplImage *img1;
	img1 = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	cvCopy(img, img1, 0);
	IplImage *ea_tar;
	ea_tar = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	IplImage *ea_tar1;
	ea_tar1 = cvCreateImage(cvGetSize(ea_tar), ea_tar->depth, ea_tar->nChannels);

	IplImage * HImg = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage * SImg = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage * IImg = cvCreateImage(cvGetSize(img), img->depth, 1);
	
	//Lab模型转换
	IplImage *img_LAB = cvCreateImage(cvGetSize(img), img->depth, 3);
	IplImage *channel_L = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage *channel_A = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage *channel_B = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage *m_dest = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage *m_destRGB = cvCreateImage(cvGetSize(img), img->depth, 3);
	cvCvtColor(img, img_LAB, CV_BGR2Lab);
	cvSplit(img_LAB, channel_L, channel_A, channel_B, NULL);
	int threshold_L = 0, threshold_A = 0, threshold_B = 0;
	threshold_B = otsu(channel_B);
	threshold_L = otsu(channel_L);
	threshold_A = otsu(channel_A);
	RgbImage Nimg_LAB(img_LAB);
	RgbImage Nimg(img);
	RgbImage Nm_destRGB(m_destRGB);
	BwImage Nm_dest(m_dest);
	int i = 0, j = 0;
	int hei = img->height;
	int wid = img->width;
	for (i = 0; i < hei; i++)
	{
		for (j = 0; j<wid; j++)
		{
			int tempL = Nimg_LAB[i][j].b;
			int tempA = Nimg_LAB[i][j].g;
			int tempB = Nimg_LAB[i][j].r;
			if ((tempL<threshold_L) || (tempB>threshold_B) || (tempA>threshold_A))
			{
				Nm_dest[i][j] = 0;
				Nm_destRGB[i][j].r = Nimg[i][j].r;
				Nm_destRGB[i][j].g = Nimg[i][j].g;
				Nm_destRGB[i][j].b = Nimg[i][j].b;
			}
			else
			{
				Nm_dest[i][j] = 255;
				Nm_destRGB[i][j].r = 255;
				Nm_destRGB[i][j].g = 255;
				Nm_destRGB[i][j].b = 255;
			}
		}
	}
	IplImage *thre_L = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage *thre_A = cvCreateImage(cvGetSize(img), img->depth, 1);
	IplImage *thre_B = cvCreateImage(cvGetSize(img), img->depth, 1);
	BwImage Nthre_L(thre_L);
	BwImage Nthre_A(thre_A);
	BwImage Nthre_B(thre_B);
	for (i = 0; i < hei; i++)
	{
		for (j = 0; j < wid; j++)
		{
			int tempL = Nimg_LAB[i][j].b;
			int tempA = Nimg_LAB[i][j].g;
			int tempB = Nimg_LAB[i][j].r;
			if (tempL < threshold_L)
				Nthre_L[i][j] = 255;
			else
				Nthre_L[i][j] = 0;
			if (tempA > threshold_A)
				Nthre_A[i][j] = 255;
			else
				Nthre_A[i][j] = 0;
			if (tempB > threshold_B)
				Nthre_B[i][j] = 255;
			else
				Nthre_B[i][j] = 0;
		}
	}
	//读取和显示图像
	src = thre_B;
	dstContours = src;
	IplImage *dstr1;
	dstr1 = cvCreateImage(cvGetSize(img), 8, 1);      //生成一个单通道二值图像
	CvMemStorage* storage = cvCreateMemStorage(0);     //检索轮廓
	CvSeq* first_contour = NULL;
	int Nc = cvFindContours(dstContours, storage, &first_contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	cvZero(dstContours);   //初始化图像空间
	int l = 0, la = 300;   //当周长小于500时进行剔除
	CvSeq* c = 0;
	for (c = first_contour; c != NULL; c = c->h_next)
	{
		l = cvContourPerimeter(c);
		if (l > la&& l < 700)
		{
			cvZero(dstContours);		//单独将每个轮廓画出
			cvDrawContours(dstContours, c, cvScalarAll(255), cvScalarAll(0), 0, -1, 8);   //CV_FILLED即负数表示画轮廓时同时填充内部
			//根据每个轮廓提取出每一个目标,背景为黑色
			cvZero(ea_tar);
			cvCopy(img1, ea_tar, dstContours);    //掩膜
			Rgb2Hsi(ea_tar, HImg, SImg, IImg);
			cvThreshold(HImg, HImg, 100, 255, CV_THRESH_BINARY_INV);
			cvZero(ea_tar1);
			cvCopy(ea_tar, ea_tar1, HImg);    //掩膜
			cvSplit(ea_tar1, NULL, NULL, dstr1, NULL);
			CvScalar cs;
			cs = cvSum(ea_tar1);
			int n = cvCountNonZero(dstr1);   //针对单通道图像计算
			double co_ratio = 0;
			if ((cs.val[1] + cs.val[2]) != 0)
			{
				co_ratio = (fabs(200 * (cs.val[1] - cs.val[2])) / (cs.val[1] + cs.val[2]));
			}
			double m00, m10, m01;
			CvMoments moment;
			//cout << "percent" << co_ratio << endl;
			if (co_ratio < 15.00&&co_ratio > 0.00)
			{
				cvMoments(dstr1, &moment, 1);    //处理对象是单通道二值图
				m00 = cvGetSpatialMoment(&moment, 0, 0);
				if (m00 != 0)     //m00表示的是二值图像上目标对象的面积
				{
					m10 = cvGetSpatialMoment(&moment, 1, 0);
					m01 = cvGetSpatialMoment(&moment, 0, 1);
					center.x = (int)(m10 / m00);
					center.x = (640 - center.x) * 40 / 640;
					center.y = (int)(m01 / m00);
					center.y = (480 - center.y) * 30 / 480;
					//cout << "横坐标：" << center.x << endl;
					//cout << "纵坐标：" << center.y << endl;
				}
			}
			//cvNamedWindow("dst_contours", CV_WINDOW_AUTOSIZE);
			//cvShowImage("dst_contours", ea_tar);
			//cvSaveImage("ea_tar.jpg", ea_tar);
		}
	}
	cvWaitKey(-1); //等待退出
	cvReleaseMemStorage(&storage);
}

void CSnapDlg::grab(int distance)
{
	for (int i = 0; i < 7; i++)
	{
		m_strTXData = grabCommand[distance][i];
		m_ctrlComm.put_Output(COleVariant(m_strTXData)); //发送数据
		Sleep(50);
	}
}

void CSnapDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
	{
		VideoCapture capture(0);//选取摄像头
		capture >> frame;  //读取当前帧
	}

	break;
	case 2:
	{
		grabYellow();
		char buf[10];
		sprintf(buf, "%d", center.y);
		m_coordinate = buf;
		UpdateData(FALSE);
		if (center.y != 0)
		{
			if (center.y <= 7)
			{
				calculateSleep(calculateDelay(center.x));
				grab(0);
				center.y = 0;
			}
			else if (center.y <= 8 && center.y > 7)
			{
				calculateSleep(calculateDelay(center.x));
				grab(1);
				center.y = 0;
			}
			else if (center.y <= 14 && center.y >= 9)
			{
				calculateSleep(calculateDelay(center.x));
				grab(2);
				center.y = 0;
			}
			else if (center.y <= 16 && center.y >= 15)
			{
				calculateSleep(calculateDelay(center.x));
				grab(3);
				center.y = 0;
			}
			else if (center.y < 18 && center.y >= 17)
			{
				calculateSleep(calculateDelay(center.x));
				grab(4);
				center.y = 0;
			}
			else if (center.y <= 20 && center.y >= 18)
			{
				calculateSleep(calculateDelay(center.x));
				grab(5);
				center.y = 0;
			}
			else if (center.y < 22 && center.y >= 21)
			{
				calculateSleep(calculateDelay(center.x));
				grab(6);
				center.y = 0;
			}
			else if (center.y <= 24 && center.y >= 22)
			{
				calculateSleep(calculateDelay(center.x));
				grab(7);
				center.y = 0;
			}
			else;
			center.y = 0;
		}
	}
	break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

int CSnapDlg::minn(int a, int b, int c)    //比较a, b, c三个数的大小
{
	int m = a;
	if (m > b)
		m = b;
	if (m > c)
		m = c;
	return m;
}

void CSnapDlg::calculateSleep(int sleep)
{
	int i;
	int j = sleep / 1000;
	int k = sleep % 1000;
	for (i = 1; i <= j; i++)
	{
		Sleep(1000);
	}
	Sleep(k);
}


int CSnapDlg::calculateDelay(int centerX)
{
	int delay = 0;
	int err = 700;
	delay = int((-0.1327 * centerX + 7.8179) * 1000) + err;
	return delay;
}

/*************************************************
Description: // 函数功能、性能等的描述
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated: // 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input: // 输入参数说明，包括每个参数的作
// 用、取值说明及参数间关系。
Output: // 对输出参数的说明。
Return: void
Others: 
*************************************************/
/*************************************************
//Method: onMouse
//Description: do the actions after onMouse event is called
// Author: Zeyu Zhong
// Date: 2017/07/13
//Returns: void
//Parameter: event
//Parameter: x Mouse's coordinate
//Parameter: y
//History:
*************************************************/

void CSnapDlg::OnBnClickedButtonSend25()
{
	grab(0);
}

void CSnapDlg::OnBnClickedButtonSend5()
{
	grab(1);
}

void CSnapDlg::OnBnClickedButtonSend75()
{
	grab(2);
}

void CSnapDlg::OnBnClickedButtonSend10()
{
	grab(3);
}

void CSnapDlg::OnBnClickedButtonSend125()
{
	grab(4);
}

void CSnapDlg::OnBnClickedButtonSend15()
{
	grab(5);
}

void CSnapDlg::OnBnClickedButtonSend175()
{
	grab(6);
}

void CSnapDlg::OnBnClickedButtonSend20()
{
	grab(7);
}

//this button just for test
void CSnapDlg::OnBnClickedButtonStart()
{
	grabYellow();
	char buf[10];
	sprintf(buf, "%d", center.y);
	m_coordinate = buf;
	UpdateData(FALSE);
	if (center.y != 0)
	{
		//Sleep(2300);
		grab(0);
		center.y = 0;
		UpdateData(FALSE);
	}
}
