// SaoriTester.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#include "SaoriTester.h"
#include "SaoriTesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaoriTesterApp

BEGIN_MESSAGE_MAP(CSaoriTesterApp, CWinApp)
	//{{AFX_MSG_MAP(CSaoriTesterApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaoriTesterApp �N���X�̍\�z

CSaoriTesterApp::CSaoriTesterApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSaoriTesterApp �I�u�W�F�N�g

CSaoriTesterApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSaoriTesterApp �N���X�̏�����

BOOL CSaoriTesterApp::InitInstance()
{
	m_pMainWnd = new CSaoriTesterDlg;

	return TRUE;
}
