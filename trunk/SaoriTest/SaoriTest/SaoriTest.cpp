// SaoriTest.cpp : DLL �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "csaori.h"

void CSAORI::exec(const CSAORIInput& in,CSAORIOutput& out){
	out.result_code=SAORIRESULT_OK;
	out.result=module_path+in.args[0]+in.args[1];
}

bool CSAORI::unload(){
	return true;
}

bool CSAORI::load(){
	return true;
}
