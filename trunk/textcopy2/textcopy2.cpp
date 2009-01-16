#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "csaori.h"

/*---------------------------------------------------------
	������
---------------------------------------------------------*/
bool CSAORI::load(){
	return true;
}

/*---------------------------------------------------------
	���
---------------------------------------------------------*/
bool CSAORI::unload(){
	return true;
}

/*---------------------------------------------------------
	���s
---------------------------------------------------------*/
void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	out.result_code = SAORIRESULT_BAD_REQUEST;
	if ( in.args.size() < 1 ) { return; }
	if ( in.args[0].size() == 0 ) { return; }

	if ( ! ::OpenClipboard(NULL) ) {
		return;
	}

	//textcopy�d�l�ɒ����Ɂc
	out.result_code = SAORIRESULT_NO_CONTENT;
	if ( in.args.size() >= 2 ) {
		if ( wcstoul(in.args[1].c_str(),NULL,10) ) {
			out.result = in.args[0];
			out.result_code = SAORIRESULT_OK;
		}
	}

	::EmptyClipboard();

	//NT�ȏ�Ȃ�Unicode�Ή��A�łȂ����ACP�ɕϊ�
	OSVERSIONINFOEX osVer;
	ZeroMemory(&osVer,sizeof(osVer));
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx((OSVERSIONINFO*)&osVer);

	if ( osVer.dwPlatformId == VER_PLATFORM_WIN32_NT ) {
		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,(in.args[0].size() + 1) * sizeof(WORD));
		WORD *clData = (WORD*)::GlobalLock(hMem);
		wcscpy(clData,in.args[0].c_str());

		::SetClipboardData(CF_UNICODETEXT,hMem); //hMem�͊J�����Ȃ��Ă悢
	}
	else {
		std::string astr = SAORI_FUNC::UnicodeToMultiByte(in.args[0]);

		HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,astr.size() + 1);
		char *clData = (char*)::GlobalLock(hMem);
		strcpy(clData,astr.c_str());

		::SetClipboardData(CF_TEXT,hMem); //hMem�͊J�����Ȃ��Ă悢
	}

	::CloseClipboard();
}

