
#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#include "csaori.h"
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <time.h>

#include "tinyxml.h"
#include "xpath_processor.h"

//////////WINDOWS DEFINE///////////////////////////
//include�̂��Ƃɂ����ĂˁI
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
///////////////////////////////////////////////////

class XMLDocInfo
{
private:
	XMLDocInfo() { }
public:
	TiXmlDocument *xml;
	unsigned int id;
	string_t filepath;
	unsigned int cp;

	XMLDocInfo(TiXmlDocument *x,unsigned int i,string_t f,unsigned int c) :
		xml(x) , id(i) , filepath(f) , cp(c) { }
	~XMLDocInfo() {
		delete xml;
	}
};

typedef std::vector<XMLDocInfo *> XMLPtrVector;

static XMLPtrVector g_xml;

/*---------------------------------------------------------
	������
---------------------------------------------------------*/
bool CSAORI::load()
{
	srand(time(NULL));
	return true;
}

/*---------------------------------------------------------
	���
---------------------------------------------------------*/
bool CSAORI::unload()
{
	for ( XMLPtrVector::iterator it = g_xml.begin() ; it != g_xml.end() ; ++it ) {
		delete *it;
	}
	g_xml.clear();
	return true;
}

/*---------------------------------------------------------
	���s
---------------------------------------------------------*/
void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out)
{
	//�W���F�o�b�h���N�G�X�g
	//����ɂ����邱�Ƃ��킩�������_��200/204�ɏ���������
	out.result_code = SAORIRESULT_BAD_REQUEST;
	if ( ! in.args.size() ) {
		return;
	}

	if ( wcsicmp(in.args[0].c_str(),L"open") == 0 ) {
		//���́Fopen,�t�@�C����
		//�o�́F�t�@�C��ID

		if ( in.args.size() < 2 ) {
			return;
		}

		//�����̃t�@�C�����J���Ă���ė��p
		for ( XMLPtrVector::iterator it = g_xml.begin() ; it != g_xml.end() ; ++it ) {
			if ( (**it).filepath == in.args[1] ) {
				std::wstringstream outstr(out.result);
				outstr << (**it).id;

				out.result_code = SAORIRESULT_OK;
				return;
			}
		}

		//�ӂ����邨�[�ۂ�I
		std::string fname = SAORI_FUNC::UnicodeToMultiByte(in.args[1]);

		TiXmlDocument *doc = new TiXmlDocument;
		if ( ! doc->LoadFile(fname.c_str()) ) {
			delete doc;
			return;
		}

		//�ŏ��̎q�����邩�m�F
		TiXmlNode *pNodeDec = doc->FirstChild();
		if ( ! pNodeDec ) {
			delete doc;
			return;
		}

		//��`
		unsigned int cp;
		TiXmlDeclaration *pDec = pNodeDec->ToDeclaration();
		if ( pDec && strlen(pDec->Encoding()) ) {
			cp = SAORI_FUNC::StringtoCodePage(pDec->Encoding());
		}
		else {
			cp = CP_UTF8;
		}

		//�Ǘ��\���̂ɂ������200���^�[��
		unsigned int id = rand();

		XMLDocInfo *pInfo = new XMLDocInfo(doc,id,in.args[1],cp);
		g_xml.push_back(pInfo);

		std::wstringstream outstr;
		outstr << id;

		out.result = outstr.str();
		out.result_code = SAORIRESULT_OK;
	}
	else if ( wcsicmp(in.args[0].c_str(),L"close") == 0 ) {
		//���́Fclose,�t�@�C��ID

		if ( in.args.size() < 2 ) {
			return;
		}

		//�P���ɒT���Č���������{�b�V���[�g
		//�����Ђǂ����Ƃ��N������ID�����Ԃ�ꍇ�ɔ����đS���Ȃ߂�
		unsigned int id = wcstoul(in.args[1].c_str(),NULL,10);
		bool found = false;

		for ( XMLPtrVector::iterator it = g_xml.begin() ; it != g_xml.end() ; ++it ) {
			if ( (**it).id == id ) {
				delete *it;
				it = g_xml.erase(it);
				found = true;

				if ( it == g_xml.end() ) { break; }
			}
		}

		if ( found ) {
			out.result_code = SAORIRESULT_NO_CONTENT;
		}
	}
	else if ( wcsicmp(in.args[0].c_str(),L"get") == 0 ) {
		//���́Fget,�t�@�C��ID,XPath

		if ( in.args.size() < 3 ) {
			return;
		}

		//�P���ɒT���Č����邾��
		unsigned int id = wcstoul(in.args[1].c_str(),NULL,10);
		bool found = false;
		XMLPtrVector::iterator it = g_xml.begin();

		for ( ; it != g_xml.end() ; ++it ) {
			if ( (**it).id == id ) {
				break;
			}
		}

		if ( it == g_xml.end() ) {
			return;
		}

		std::string xpath = SAORI_FUNC::UnicodeToMultiByte(in.args[2],(**it).cp);
		try {
			TinyXPath::xpath_processor xpath((**it).xml->FirstChild(),xpath.c_str());
			TiXmlString str = xpath.S_compute_xpath();
			out.result = SAORI_FUNC::MultiByteToUnicode(str.c_str(),(**it).cp);
		}
		catch(...) {
			out.result_code = SAORIRESULT_INTERNAL_SERVER_ERROR;
		}

		out.result_code = SAORIRESULT_OK;
	}
}

