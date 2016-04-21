
// SnapDlg.h : 头文件
//

#pragma once
#include "mscomm1.h"
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include "stdio.h"
#include "math.h"
#include <iostream> 
#include "afxwin.h"
using namespace std;
using namespace cv;

// CSnapDlg 对话框
class CSnapDlg : public CDialogEx
{
// 构造
public:
	CSnapDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SNAP_DIALOG };

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
	afx_msg void OnBnClickedButtonSend25();
	afx_msg void OnBnClickedButtonSend125();
	afx_msg void OnBnClickedButtonSend5();
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	afx_msg void OnBnClickedButtonSend15();
	afx_msg void OnBnClickedButtonSend75();
	afx_msg void OnBnClickedButtonSend175();
	afx_msg void OnBnClickedButtonSend10();
	afx_msg void OnBnClickedButtonSend20();
	afx_msg void OnBnClickedButtonStart();
	int otsu(IplImage* src);
	double ThresholdTrans(IplImage* src, IplImage* dest, int num_R, int num_G, int num_B);
	int minn(int a, int b, int c);
	int Rgb2Hsi(const IplImage* src, IplImage* dataH, IplImage* dataS, IplImage* dataI);
	void extractyellow();
	void Grab25();
	void Grab50();
	void Grab75();
	void Grab100();
	void Grab125();
	void Grab150();
	void Grab175();
	void Grab200();
	void sleep_own(int m);
	int cac_delay(int x);

	template<class T>class Image
	{
	private:
		IplImage*imgp;
	public:
		Image(IplImage*img = 0){ imgp = img; }
		~Image(){ imgp = 0; }
		void operator=(IplImage*img){ imgp = img; }
		inline T*operator[](const int rowIndx)
		{
			return ((T*)(imgp->imageData + rowIndx*imgp->widthStep));
		}             
	};
	typedef struct
	{
		unsigned char b, g, r;
	}RgbPixel;
	typedef struct
	{
		float b, g, r;
	}RgbPixelFloat;
	typedef Image<RgbPixel>        RgbImage;
	typedef Image<RgbPixelFloat>   RgbImageFloat;
	typedef Image<unsigned char>   BwImage;
	typedef Image<float>           BwImageFloat;
	CMscomm1 m_ctrlComm;
	CString m_strRXData;
	CString m_strTXData;
	CvPoint center;
	CString m_coordinate;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
