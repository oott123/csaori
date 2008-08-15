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
BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lp) {
	CSAORIOutput *pOut = (CSAORIOutput*)lp;
	
	DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
	if ( ::IsWindow(hwnd) && ::IsWindowVisible(hwnd) && (style&WS_CAPTION) && !(style&WS_POPUP) ) {
		char buf[65536];
		::GetWindowText(hwnd, buf, 65535);
		if ( buf[0] != '\0' ) {
			pOut->values.push_back(SAORI_FUNC::MultiByteToUnicode(buf));
		}
	}
	return TRUE;
}

void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	out.result_code = SAORIRESULT_OK;
	::EnumWindows(EnumWindowProc, (LPARAM)&out);
	out.result = SAORI_FUNC::intToString(out.values.size());
}

