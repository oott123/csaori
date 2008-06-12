
#pragma warning( disable : 4786 )

#include "csaori.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

//global CSAORI object
CSAORI* pSaori;

//------------------------------------------------------------------------------
//�����֐�
//------------------------------------------------------------------------------
namespace SAORI_FUNC{
	std::string UnicodeToMultiByte(const std::wstring& Source, UINT CodePage, DWORD Flags)
	{
	  if (int Len = ::WideCharToMultiByte(CodePage, Flags, Source.c_str(), static_cast<int>(Source.size()), NULL, 0, NULL, NULL)) {
		std::vector<char> Dest(Len);
		if (Len = ::WideCharToMultiByte(CodePage, Flags, Source.c_str(), static_cast<int>(Source.size()), &Dest[0], static_cast<int>(Dest.size()), NULL, NULL)) {
		  return std::string(Dest.begin(), Dest.begin() + Len);
		}
	  }
	  return "";
	}


	std::wstring MultiByteToUnicode(const std::string& Source, UINT CodePage, DWORD Flags)
	{
	  if (int Len = ::MultiByteToWideChar(CodePage, Flags, Source.c_str(), static_cast<int>(Source.size()), NULL, 0)) {
		std::vector<wchar_t> Dest(Len);
		if (Len = ::MultiByteToWideChar(CodePage, 0, Source.c_str(), static_cast<int>(Source.size()), &Dest[0], static_cast<int>(Dest.size()))) {
		  return std::wstring(Dest.begin(), Dest.begin() + Len);
		}
	  }
	  return L"";
	}

	UINT CHARSETtoCodePage(CHARSET cset){
		switch(cset){
			case CHARSET_Shift_JIS:
				return 932;
			case CHARSET_ISO_2022_JP:
				return 50220;
			case CHARSET_EUC_JP:
				return 20932;
			case CHARSET_UTF_8:
				return CP_UTF8;
		}
		return CP_ACP;
	}

	std::wstring CHARSETtoString(CHARSET cset){
		switch(cset){
			case CHARSET_Shift_JIS:
				return L"Shift_JIS";
			case CHARSET_ISO_2022_JP:
				return L"ISO-2022-JP";
			case CHARSET_EUC_JP:
				return L"EUC-JP";
			case CHARSET_UTF_8:
				return L"UTF-8";
		}
		return L"unknown charset";
	}

	string_t getResultString(int rc)
	{
		char_t	*p;
		
		switch(rc) {
			case 200:
				p = L"OK"; break;
			case 204:
				p = L"No Content"; break;
			case 210:
				p = L"Break"; break;		// SSTP Break (SSTP session aborted)
			case 300:
				p = L"Communicate"; break; // (obsolete)
			case 311:
				p = L"Not Enough"; break; // SHIORI/2.4 TEACH...need more info.
			case 312:
				p = L"Advice"; break; // SHIORI/2.4 TEACH...drop recent reference
			case 400:
				p = L"Bad Request"; break;
			case 401:
				p = L"Unauthorized"; break;
			case 403:
				p = L"Forbidden"; break;
			case 404:
				p = L"Not Found"; break;
			case 405:
				p = L"Method Not Allowed"; break;
			case 406:
				p = L"Not Acceptable"; break;
			case 408:
				p = L"Request Timeout"; break;
			case 409:
				p = L"Conflict"; break;
			case 420:
				p = L"Refuse"; break;		// SSTP refused by ghost
			case 500:
				p = L"Internal Server Error"; break;
			case 501:
				p = L"Not Implemented"; break;
			case 503:
				p = L"Service Unavailable"; break;
			case 510:
				p = L"Not Local IP"; break;	// SSTP from Non-Local IP
			case 511:
				p = L"In Black List"; break;	// SSTP from "Black-Listed" IP
			case 512:
				p = L"Invisible"; break;	// SSTP inivisible (not proceed)
			default:
				p = L"Unknown Error";
		}
		
		return string_t(p);
	}

	//
	// string �� tpos �ʒu���炩��P�s�i���s�������͏I�[�܂Łj���o�� 
	// �߂�l ... ���s�J�n�ʒu�i���s���Ȃ��Ƃ��� string::npos�j 
	//
	string_t::size_type getLine(string_t &sl, const string_t &src, string_t::size_type tpos)
	{
		string_t::size_type len = src.size();
		if (tpos == string_t::npos || tpos >= len) {
			sl = L"";
			return string_t::npos;
		}
		string_t::size_type  ppos = src.find_first_of(L"\r\n", tpos);
		if (ppos == string_t::npos) {
			sl = src.substr(tpos);
			return ppos;
		}
		sl = src.substr(tpos, ppos - tpos);
		ppos++;
		if (ppos >= len) return string_t::npos;
		if (src[ppos - 1] == '\r' && src[ppos] == '\n') {
			ppos++;
			if (ppos >= len) return string_t::npos;
		}
		
		return ppos;
	}


	//
	// numToString(num, base, sign)
	// �����Ȃ����l�������� 
	// num ���l 
	// base ��i�f�t�H���g 10�j 
	// sign ���ʕ�����̐擪�ɂ��镶���B\0 �̏ꍇ�͉������Ȃ� 
	//
	// intToString(num, base)
	// ���l�������� 
	// numToString �Ƃ������������B�����̂Ƃ��� - ��擪�ɂ��� 
	//
	string_t numToString(unsigned num, unsigned int base, char_t sign)
	{
		if (num == 0) return L"0";
		
		unsigned m;
		char_t	c, s[96];
		int		pos = sizeof(s);
		
		s[--pos] = '\0';
		while(pos > 0) {
			m = num % base;
			if (m < 10) c = L'0' + m;
			else c = L'a' - 10 + m;
			s[--pos] = c;
			num /= base;
			if (num == 0) break;
		}
		if (sign) s[--pos] = sign;
		
		return string_t(s + pos);
	}

	string_t intToString(int num, unsigned int base)
	{
		char_t c = L'\0';
		if (num < 0) { num = -num; c = '-'; }
		return SAORI_FUNC::numToString(num, base, c);
	}
}

//------------------------------------------------------------------------------
//CSAORIInput
//------------------------------------------------------------------------------
bool CSAORIInput::parseString(const string_t src)
{
	const char_t atag[] = L"Argument";
	const string_t::size_type catag = 8;//"Argument"�̒���
	string_t::size_type pos = 0, nextpos;
	string_t::size_type ts;
	vector<string_t> _arg;
	map<string_t, string_t> _opt;
	string_t _cmd;
	
	string_t sl;
	
	pos = SAORI_FUNC::getLine(sl, src, pos);
	ts = sl.find(L" SAORI/1.");
	if (ts == string_t::npos) return false;
	_cmd = sl.substr(0, ts);
	
	string_t k, v;
	int argc = 0;
	
	while(1) {
		sl = L"";
		nextpos = SAORI_FUNC::getLine(sl, src, pos);
		ts = sl.find(L": ");
		if (ts != string_t::npos) {
			k = sl.substr(0, ts);
			v = sl.substr(ts + 2);
			const char_t *pk = k.c_str();
			// _argument[n] �� vector (_args) �ɐς݁A����ȊO�̗v�f�� 
			// map (_opt) �ɐς� 
			if (k.size() > catag && _memicmp(pk, atag, catag) == 0) {
				int ord = _wtoi(pk + catag);
				if (ord > 0 || (ord == 0 && k[catag] == L'0')) {
					if (argc <= ord) {
						// _argument[n] �����Ԃɓ����ĂȂ��ꍇ�� 
						// ���Ԃ��ɖ��߂Ă��� 
						for(int i=(int)(_arg.size()); i<=ord; i++) {
							_arg.push_back(L"");
						}
					}
					//_arg.push_back(v);
					_arg[ord]=v;
					argc = (int)(_arg.size());
				}
			}
			else {
				if (ts > 0) {
					map<string_t,string_t>::iterator i;
					i = _opt.find(k);
					if (i == _opt.end()) {
						_opt.insert(pair<string_t,string_t>(k, v));
					}
					else _opt[k] = v;
				}
			}
		}
		if (nextpos == string_t::npos) break;
		pos = nextpos;
	}
	cmd=_cmd;
	args=_arg;
	opts=_opt;
	return true;
}

//------------------------------------------------------------------------------
//CSAORIOutput
//------------------------------------------------------------------------------
string_t CSAORIOutput::toString()
{
	string_t rcstr = SAORI_FUNC::getResultString(result_code);
	
	string_t dest;
	dest = SAORI_VERSIONSTRING L" ";
	dest += SAORI_FUNC::numToString(result_code) +L" "+ rcstr + L"\r\n";

	dest +=L"Charset: "+SAORI_FUNC::CHARSETtoString(charset)+L"\r\n";
	
	if (!result.empty()) {
		dest += L"Result: ";
		dest += result;
		dest += L"\r\n";
	}
	if (!values.empty()) {
		int i, n = (int)(values.size());
		for(i=0; i<n; i++) {
			dest += ( L"Value" + SAORI_FUNC::numToString(i) + L": " );
			dest += values[i];
			dest += L"\r\n";
		}
	}
	if (!opts.empty()) {
		map<string_t,string_t>::iterator i;
		for(i=opts.begin(); i != opts.end(); i++) {
			dest += (i->first + L": " + i->second + L"\r\n");
		}
	}
	
	dest += L"\r\n";
	return dest;
}

//------------------------------------------------------------------------------
//CSAORI
//------------------------------------------------------------------------------
string CSAORI::request(string rq_tmp){
	string cmd;
	CSAORIInput* pIn;
	CSAORIOutput* pOut;

	//�����HRequest���������ɕϊ�
	string tmp=rq_tmp;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);

	//Charset����
	CHARSET cset=CHARSET_Shift_JIS;
	if(tmp.find("\ncharset: shift_jis")!=string::npos){
		cset=CHARSET_Shift_JIS;
	}else if(tmp.find("\ncharset: iso-2022-jp")!=string::npos){
		cset=CHARSET_ISO_2022_JP;
	}else if(tmp.find("\ncharset: euc-jp")!=string::npos){
		cset=CHARSET_EUC_JP;
	}else if(tmp.find("\ncharset: utf-8")!=string::npos){
		cset=CHARSET_UTF_8;
	}

	//�ϊ�
	string_t rq=SAORI_FUNC::MultiByteToUnicode(rq_tmp,SAORI_FUNC::CHARSETtoCodePage(cset));

	//��͏����J�n
	pIn=new CSAORIInput();
	pIn->charset=cset;
	pIn->opts[L"SecurityLevel"] = L"Local";
	bool result=pIn->parseString(rq);

	pOut=new CSAORIOutput();
	pOut->charset=pIn->charset;
	if(result==false){
		pOut->result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;
	}else{
		if (pIn->cmd == L"GET Version") {
			pOut->result_code=SAORIRESULT_OK;
		}else if (pIn->cmd == L"EXECUTE") {
			string_t sec = pIn->opts[L"SecurityLevel"];
			if (sec==L"Local" || sec==L"local") {
				exec(*pIn,*pOut);
			}
			else {
				pOut->result_code=SAORIRESULT_INTERNAL_SERVER_ERROR;
			}
		}else {
			pOut->result_code=SAORIRESULT_BAD_REQUEST;
		}
	}
	string_t res_wstr=pOut->toString();
	string res_str=SAORI_FUNC::UnicodeToMultiByte(res_wstr,SAORI_FUNC::CHARSETtoCodePage(pOut->charset));
	delete pIn;
	delete pOut;
	return res_str;
}

void CSAORI::setModulePath(string str){
	module_path=SAORI_FUNC::MultiByteToUnicode(str);
}


//------------------------------------------------------------------------------
//SAORI INTERFACES
//------------------------------------------------------------------------------
BOOL APIENTRY DllMain(
	HANDLE hModule,
	DWORD  ul_reason_for_call, 
	LPVOID lpReserved
){
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		unload();
		break;
	}
    return TRUE;
}

SAORIAPI
HGLOBAL
SAORICDECL 
request(HGLOBAL h, long *len)
{
	string rq((char *)h, *len);
	GlobalFree(h);
	
	string re=pSaori->request(rq);

	*len = (long)(re.size());
	h = GlobalAlloc(GMEM_FIXED, *len+1);
	if (h) {
		memcpy(h,re.c_str(),*len+1);
	}
	
	return h;
}

SAORIAPI
BOOL
SAORICDECL
load(HGLOBAL h, long len)
{
	if(pSaori!=NULL){
		unload();
	}
	pSaori=new CSAORI();
	if (h) {
		string mpath;
		mpath.assign((char*)h,len);
		GlobalFree(h);
		pSaori->setModulePath(mpath);
	}
	return pSaori->load();
}

SAORIAPI
BOOL
SAORICDECL
unload()
{
	if(pSaori==NULL){
		return TRUE;
	}
	BOOL re=pSaori->unload();
	delete pSaori;
	pSaori=NULL;
	return re;
}
