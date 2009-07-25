#include "CInetHelper.h"
#include "mlang2unicode.h"

using namespace std;
// *** Public ***

int CInetHelper::getUrlContent(const char* url, wstring& out) {
	string oResult;

	HINTERNET hInternet = CInetHelper::getInternet();
	if ( hInternet==NULL ) {
		out = L"InternetOpen failed";
		return CIH_FAIL;
	}

	/* remote_folder�UǦ������ */
	HINTERNET hFile = InternetOpenUrl(
		hInternet,
		url,
		NULL,
		0,
		INTERNET_FLAG_RELOAD,
		0);
	if ( hFile==NULL ) {
		InternetCloseHandle(hInternet);
		out = L"InternetOpenUrl failed";
		return CIH_FAIL;
	}

		/* Ǧ���������Fremote_folder���p����ǻ�y(8192���~���A�K)���f�D�g */
	vector<char>	theData;
	for(;;) {
		DWORD ReadSize=0;
		static char Buf[8192];
		BOOL bResult = InternetReadFile(
			hFile,
			Buf,
			8192,
			&ReadSize);

		/* ���M���f�D�z�G�p�������y�����r */
		if(bResult && (ReadSize == 0))
			break;

		int cur = theData.size();
		theData.resize( cur + ReadSize );
		memcpy( &theData[0] + cur, Buf, ReadSize );
	}

	/* ���z */
	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	// ��r�C�׺ݡCUnicode�U���X�y�����M4byte�C
	for (int i=0 ; i<4 ; ++i)
		theData.push_back(0);

	oResult.assign(&theData[0], theData.size());

	wstring nResult;
	mlangToUnicode(0, oResult, nResult);

	out = nResult;
	return CIH_OK;
}

bool CInetHelper::checkInternet() {
	HINTERNET hInternet = CInetHelper::getInternet();
	if ( hInternet==NULL ) {
		return false;
	} else {
		InternetCloseHandle(hInternet);
		return true;
	}
}

// *** Private ***

HINTERNET CInetHelper::getInternet() {
	HINTERNET hInternet = InternetOpen(
		"chttpcSaori/1.0",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);
	return hInternet;
}
