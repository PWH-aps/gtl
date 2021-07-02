// ProgressDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "gtl/gtl.h"
#include "gtl/win_util/ProgressDlg.h"
#include "gtl/win_util/TMDialog.h"

// CProgressDlg dialog

namespace gtl::win_util {

	IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

	CProgressDlg::CProgressDlg(CWnd* pParent /*=nullptr*/)
		: CDialogEx(strIDD.c_str(), pParent) {

		m_strMessage = _T("Working...");
		m_calback = [&](int iPercent, bool bDone, bool bOK)->bool{
			m_iPercent = iPercent;
			m_bDone = bDone;

			return !m_bCancel;
		};
	}

	CProgressDlg::~CProgressDlg() {
	}

	void CProgressDlg::DoDataExchange(CDataExchange* pDX) {
		CDialogEx::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_PROGRESS, m_progress);
		DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
	}

	BOOL CProgressDlg::OnInitDialog() {
		CDialogEx::OnInitDialog();

		m_tStart  = std::chrono::system_clock::now();
		SetTimer(T_UPDATE_UI, 100, nullptr);

		m_progress.SetRange(0, 100);

		return TRUE;  // return TRUE unless you set the focus to a control
					  // EXCEPTION: OCX Property Pages should return FALSE
	}

	BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
		ON_WM_TIMER()
		ON_WM_CREATE()
	END_MESSAGE_MAP()


	// CProgressDlg message handlers

	//bool CProgressDlg::Create(CWnd* pWndParent) {
	//	return base_t::Create(strIDD.c_str(), pWndParent) != false;
	//}

	int CProgressDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) {
		if (CDialogEx::OnCreate(lpCreateStruct) == -1)
			return -1;

		return 0;
	}

	void CProgressDlg::OnTimer(UINT_PTR nIDEvent) {
		switch (nIDEvent) {
		case T_UPDATE_UI :
			if (m_progress) {
				int iPos = m_progress.GetPos();
				if (m_iPercent != iPos)
					m_progress.SetPos(m_iPercent);
				auto ts = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - m_tStart);
				auto str = std::format(_T("{:.3f} ��"), ts.count());
				CheckAndSetDlgItemText(this, IDC_STATE, str.c_str());
			}
			if (m_bDone) {
				EndDialog(m_bCancel ? IDCANCEL : IDOK);
				return;
			}
			//if (m_rThread and m_rThread->) {
			//	EndDialog(IDOK);
			//}
			return;
		}

		CDialogEx::OnTimer(nIDEvent);
	}


	void CProgressDlg::OnOK() {
		return;

		CDialogEx::OnOK();
	}


	void CProgressDlg::OnCancel() {
		m_bCancel = true;
		CDialogEx::OnCancel();
	}

}	// namespace gtl::win_util;
