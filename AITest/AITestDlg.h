#include "mnist_Reader.h"
#include "afxcmn.h"
#include "CvtWb2V.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// AITestDlg.h : ͷ�ļ�
//

#pragma once

typedef struct SthData
{
	vector<vector<char>>* images;
	vector<char>* labels;
} *pSthData;

UINT MyLoadData(LPVOID pParam);

// CAITestDlg �Ի���
class CAITestDlg : public CDialogEx
{
// ����
public:
	CAITestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AITEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAuto();

private:
	UINT32 curPicID = 0;
	vector<vector<char>> AllImages;
	vector<char> AllLabels;
	bool timerRun;
	cv::Mat mdl_w, mdl_b;
	int total_predict;
	int correct_predict;
	bool predict_rst;

public:
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPre();
	bool updateDlg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CProgressCtrl m_progressBar;
	void draw_9pixel(CImage &bmp, int x, int y);
	float* softmax(cv::Mat z);
	int arg_max(float* z);
	BYTE get_predict();
	afx_msg void OnClickedButtonLoadmodel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
