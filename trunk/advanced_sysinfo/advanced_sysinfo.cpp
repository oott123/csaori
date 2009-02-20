#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include "csaori.h"
#include "SMARTInfo.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

/*=========================================================
	SAORI CORE
=========================================================*/

/*---------------------------------------------------------
	������
---------------------------------------------------------*/
CSMARTInfo *g_pSMARTInfo = NULL;

bool CSAORI::load(){
	return true;
}

/*---------------------------------------------------------
	���
---------------------------------------------------------*/
bool CSAORI::unload(){
	if ( g_pSMARTInfo ) {
		delete g_pSMARTInfo;
		g_pSMARTInfo = NULL;
	}
	return true;
}

/*---------------------------------------------------------
	���s
---------------------------------------------------------*/
static bool GetSpecialFolderPath(const std::wstring &nFolder, std::wstring &path );
static bool GetDiskSMARTInfo(int diskID,std::wstring &result,std::vector<std::wstring> &values);

void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	out.result_code = SAORIRESULT_BAD_REQUEST;
	if ( in.args.size() < 1 ) { return; }
	if ( in.args[0].size() == 0 ) { return; }

	if ( wcsicmp(in.args[0].c_str(),L"get_special_folder_path") == 0 ) {
		std::wstring path;

		if ( GetSpecialFolderPath(in.args[1],path) ) {
			out.result = path;
			out.result_code = SAORIRESULT_OK;
		}
		else {
			out.result_code = SAORIRESULT_NO_CONTENT;
		}
	}
	else if ( wcsicmp(in.args[0].c_str(),L"get_disk_smart_info") == 0 ) {
		if ( in.args.size() < 2 ) { return; }

		int id = _wtoi(in.args[0].c_str());
		if ( GetDiskSMARTInfo(id,out.result,out.values) ) {
			out.result_code = SAORIRESULT_OK;
		}
		else {
			out.result_code = SAORIRESULT_NO_CONTENT;
		}
	}
}

/*=========================================================
	�����֐��Q
=========================================================*/

/*---------------------------------------------------------
	get_special_folder_path
---------------------------------------------------------*/

static bool GetSpecialFolderPath(const std::wstring &nFolder, std::wstring &path )
{
	IMalloc    *pMalloc;
	ITEMIDLIST *pidl;
	
	char path_buffer[MAX_PATH*2+1];

	static const struct { int id; wchar_t *nFolder; } csidl_list[] = {
		{CSIDL_APPDATA,L"ApplicationData"},
		{CSIDL_COMMON_APPDATA,L"CommonApplicationData"},
		{CSIDL_LOCAL_APPDATA,L"LocalApplicationData"},
		{CSIDL_COOKIES,L"Cookies"},
		{CSIDL_DESKTOPDIRECTORY,L"Desktop"}, //CSIDL_DESKTOP�ł͂Ȃ��I
		{CSIDL_FAVORITES,L"Favorites"},
		{CSIDL_HISTORY,L"History"},
		{CSIDL_INTERNET_CACHE,L"InternetCache"},
		{CSIDL_PROGRAMS,L"Programs"},
		//{,L"MyComputer"},
		{CSIDL_MYMUSIC,L"MyMusic"},
		{CSIDL_MYPICTURES,L"MyPictures"},
		{CSIDL_RECENT,L"Recent"},
		{CSIDL_SENDTO,L"SendTo"},
		{CSIDL_STARTMENU,L"StartMenu"},
		{CSIDL_STARTUP,L"Startup"},
		{CSIDL_SYSTEM,L"System"},
		{CSIDL_WINDOWS,L"Windows"},
		{CSIDL_TEMPLATES,L"Templates"},
		{CSIDL_DESKTOPDIRECTORY,L"DesktopDirectory"},
		{CSIDL_PERSONAL,L"Personal"},
		{CSIDL_PERSONAL,L"MyDocuments"},
		{CSIDL_PROGRAM_FILES,L"ProgramFiles"},
		{CSIDL_PROGRAM_FILES_COMMON,L"CommonProgramFiles"},
	};

	int id = 0;

	for ( unsigned int i = 0 ; i < (sizeof(csidl_list)/sizeof(csidl_list[0])) ; ++i ) {
		if ( wcsicmp(nFolder.c_str(),csidl_list[i].nFolder) == 0 ) {
			id = csidl_list[i].id;
			break;
		}
	}

	if ( id == 0 ) {
		return false;
	}
	
	//��ɂ��낢��t�B���^
	if ( id == CSIDL_WINDOWS ) {
		::GetWindowsDirectory(path_buffer,sizeof(path_buffer)-1);
		path = SAORI_FUNC::MultiByteToUnicode(path_buffer);
		return true;
	}
	if ( id == CSIDL_SYSTEM ) {
		::GetSystemDirectory(path_buffer,sizeof(path_buffer)-1);
		path = SAORI_FUNC::MultiByteToUnicode(path_buffer);
		return true;
	}

	//���C��
	if ( NOERROR == ::SHGetMalloc( &pMalloc ) ) {
		if ( SUCCEEDED(::SHGetSpecialFolderLocation(NULL, id, &pidl )) ) {
			if ( ::SHGetPathFromIDList( pidl, path_buffer ) ) {
				path = SAORI_FUNC::MultiByteToUnicode(path_buffer);
				pMalloc->Free( pidl );
				pMalloc->Release();
				return true;
			}
			pMalloc->Free( pidl );
		}
		pMalloc->Release();
	}

	//���ЂƂ�
	{
		typedef HRESULT (WINAPI *P_SHGetFolderPathA)(HWND hwndOwner,int nFolder,HANDLE hToken,DWORD dwFlags,LPTSTR pszPath);
		
		HMODULE hmFolder = ::LoadLibrary("shfolder");
		if ( hmFolder ) {
			P_SHGetFolderPathA getFldrPath = (P_SHGetFolderPathA)::GetProcAddress(hmFolder,"SHGetFolderPathA");

			if ( getFldrPath ) {
				if ( SUCCEEDED(getFldrPath(NULL,id,NULL,0,path_buffer)) ) { //0=SHGFP_TYPE_CURRENT
					path = SAORI_FUNC::MultiByteToUnicode(path_buffer);
					return true;
				}
			}
			::FreeLibrary(hmFolder);
		}
	}

	return false;
}

/*---------------------------------------------------------
	get_disk_smart_info
---------------------------------------------------------*/
static std::wstring SMARTStringToWString(const char *pBuffer,size_t n)
{
	size_t i;
	for ( i = 0 ; i < n ; ++i ) {
		if ( pBuffer[i] != L' ' ) { break; }
	}
	if ( i == n ) { return L""; }

	wchar_t buffer[128];
	size_t j = 0;
	for ( ; i < n ; ++i,++j ) {
		buffer[j] = pBuffer[i];
	}
	buffer[j] = 0; //�[���I�[

	while ( j > 0 ) {
		if ( buffer[j-1] != L' ' ) { break; }
		buffer[j-1] = 0;
		--j;
	}
	return buffer;
}

static bool GetDiskSMARTInfo(int diskID,std::wstring &result,std::vector<std::wstring> &values)
{
	if ( ! g_pSMARTInfo ) {
		g_pSMARTInfo = new CSMARTInfo;
	}

	static const struct { unsigned int id; wchar_t *text; wchar_t *textj; } smart_description[] = {
		{1  ,L"Raw Read Error Rate",L"�ǂݎ��G���[��"},
		{2  ,L"Throughput Performance",L"�S�̐��\"},
		{3  ,L"Spin Up Time",L"��]�J�n����"},
		{4  ,L"Start/Stop Count",L"�N����"},
		{5  ,L"Reallocated Sectors Count",L"��֏����Z�N�^��"},
		{7  ,L"Seek Error Rate",L"�V�[�N�G���[��"},
		{8  ,L"Seek Time Performance",L"�V�[�N�^�C�����\"},
		{9  ,L"Power-On Hours",L"�d����������"},
		{10 ,L"Spin Retry Count",L"�ĉ�]���s��"},
		{11 ,L"Recalibration Retries",L"�Ē����Ď��s��"},
		{12 ,L"Device Power Cycle Count",L"�d��������"},
		{13 ,L"Soft Read Error Rate",L"�I�t�g���b�N��"},
		{193,L"Load/Unload Cycle Count",L"�ޔ���"},
		{194,L"Temperature",L"���x"},
		{195,L"Hardware ECC recovered",L"ECC�C����"},
		{196,L"Reallocation Event Count",L"��֏�����"},
		{197,L"Current Pending Sector Count",L"��֏����҂��Z�N�^��"},
		{198,L"Off-Line Scan Uncorrectable Sector Count",L"�I�t���C���X�L���������C���s�Z�N�^��"},
		{199,L"UltraDMA CRC Error Count",L"UltraDMA CRC�G���[��"},
		{200,L"Write Error Rate",L"�������݃G���[��"},
		{201,L"Soft Read Error Rate",L"�\�t�g�ǂݎ��G���[��"},
		{202,L"Data Address Mark Error",L"DAM�G���[��"},
		{203,L"Run Out Cancel",L"ECC�G���[��"},
		{204,L"Soft ECC Correction",L"ECC�C����"},
		{205,L"Thermal Asperity Rate",L"�T�[�}���E�A�X�y���e�B��"},
		{206,L"Flying Height",L"�w�b�h��s��"},
		{207,L"Spin High Current",L"��]�J�n�����ߓd����"},
		{208,L"Spin Buzz",L"�w�b�h���ˏグ��"},
		{209,L"Offline Seek Performance",L"�I�t���C���X�L�����V�[�N���\"},
		{210,L"Vibration During Write",L"�������ݎ��̐U����"},
		{211,L"Vibration During Read",L"�ǂݎ�莞�̐U����"},
		{212,L"Shock During Write",L"�������ݎ��̃V���b�N��"},
		{220,L"Disk Shift",L"�v���b�^�����"},
		{221,L"G-Sense Error Rate",L"�Ռ����m�G���[��"},
		{222,L"Loaded Hours",L"�w�b�h���׏�"},
		{223,L"Load/Unload Retry Count",L"���[�h/�A�����[�h�Ď��s��"},
		{224,L"Load Friction",L"�w�b�h���C���׏�"},
		{226,L"Load-in Time",L"�w�b�h���׏�"},
		{227,L"Torque Amplification Count",L"��]���ב����"},
		{228,L"Power-Off Retract Count",L"�d���ؒf�ً}�ޔ���"},
		{230,L"GMR Head Amplitude",L"�w�b�h�U��"},
		{240,L"Head Flying Hours",L"�w�b�h��s����"},
		{250,L"Read Error Retry Rate",L"�ǂݎ��Ď��s��"},
	};

	wchar_t status[64] = L"Normal";

	if ( g_pSMARTInfo->Init() ) {
		CDriveSmartInfo *pDiskInfo = g_pSMARTInfo->GetInfo(diskID);
		if ( pDiskInfo ) {
			std::wstring tmpstr;
			wchar_t buffer[512];

			tmpstr = L"ModelNumber\1";
			tmpstr += SMARTStringToWString(pDiskInfo->m_sector.sModelNumber,sizeof(pDiskInfo->m_sector.sModelNumber));
			values.push_back(tmpstr);
		
			tmpstr = L"SerialNumber\1";
			tmpstr += SMARTStringToWString(pDiskInfo->m_sector.sSerialNumber,sizeof(pDiskInfo->m_sector.sSerialNumber));
			values.push_back(tmpstr);
		
			tmpstr = L"FirmwareRev\1";
			tmpstr += SMARTStringToWString(pDiskInfo->m_sector.sFirmwareRev,sizeof(pDiskInfo->m_sector.sFirmwareRev));
			values.push_back(tmpstr);

			tmpstr = L"SectorCount\1";
			swprintf(buffer,L"%u",pDiskInfo->m_sector.ulTotalAddressableSectors);
			tmpstr += buffer;
			values.push_back(tmpstr);

			size_t n = pDiskInfo->m_smartParams.size();
			unsigned int raw;
			unsigned int attr;
			wchar_t *desc;
			wchar_t *descj;

			for ( size_t i = 0 ; i < n ; ++i ) {
				raw = pDiskInfo->m_smartParams[i].attr.bRawValue[3];
				raw = raw << 8;
				raw = pDiskInfo->m_smartParams[i].attr.bRawValue[2];
				raw = raw << 8;
				raw = pDiskInfo->m_smartParams[i].attr.bRawValue[1];
				raw = raw << 8;
				raw = pDiskInfo->m_smartParams[i].attr.bRawValue[0];

				attr = pDiskInfo->m_smartParams[i].attr.bAttrID;
				desc = L"";
				descj = L"";
				for ( size_t j = 0 ; j < (sizeof(smart_description)/sizeof(smart_description[0])) ; ++j ) {
					if ( attr == smart_description[j].id ) {
						desc = smart_description[j].text;
						descj = smart_description[j].textj;
						break;
					}
				}

				swprintf(buffer,L"SMARTValue\1%u,%s,%s,%u,%u,%u,%u",
					attr,desc,descj,
					pDiskInfo->m_smartParams[i].attr.bAttrValue,
					pDiskInfo->m_smartParams[i].attr.bWorstValue,
					pDiskInfo->m_smartParams[i].thresh.bWarrantyThreshold,
					raw);
				values.push_back(buffer);

				if ( pDiskInfo->m_smartParams[i].attr.bAttrValue < pDiskInfo->m_smartParams[i].thresh.bWarrantyThreshold ) {
					swprintf(status,L"Emergency,%u",pDiskInfo->m_smartParams[i].attr.bAttrID);
				}
				else if ( pDiskInfo->m_smartParams[i].attr.bWorstValue < pDiskInfo->m_smartParams[i].thresh.bWarrantyThreshold ) {
					swprintf(status,L"Warning,%u",pDiskInfo->m_smartParams[i].attr.bAttrID);
				}
			}
		}
	}
	
	result = status;
	return false;
}
