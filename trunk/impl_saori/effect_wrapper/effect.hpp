//==============================================================================
// effect.hpp
// Copyright (C) SatisKia. All rights reserved.
//==============================================================================
#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <windows.h>
#include <string.h>

typedef BOOL (__cdecl *EFFECT_LOAD_PROC  )(HGLOBAL /*path*/, long /*path_len*/);
typedef BOOL (__cdecl *EFFECT_UNLOAD_PROC)(void);
typedef BOOL (__cdecl *EFFECT_RUN_PROC   )(HGLOBAL /*plugin*/, long /*plugin_len*/, HGLOBAL /*param*/, long /*param_num*/);

//==============================================================================
// �G�t�F�N�g���W���[���ɃA�N�Z�X����N���X
//==============================================================================
class effect {

private:

	/* ���W���[���̃n���h�� */
	HMODULE _mod;

	/* �G�N�X�|�[�g���ꂽ�֐��̃A�h���X */
	EFFECT_LOAD_PROC	__load;
	EFFECT_UNLOAD_PROC	__unload;
	EFFECT_RUN_PROC		__run;

private:

	HGLOBAL str_to_h(const char* str, long* len)
	{
		if ( str == NULL ) {
			*len = 0;
		} else {
			*len = strlen(str);
		}
		HGLOBAL h = NULL;
		if ( *len > 0 ) {
			h = ::GlobalAlloc(GMEM_FIXED, *len);
			memcpy((char*)h, (LPCTSTR)str, *len);
		}
		return h;
	}

public:

	effect(const char* path, const char* dllname)
	//==========================================================================
	//�y�@  �\�z�R���X�g���N�^
	//--------------------------------------------------------------------------
	//�y��  ���zpath     �G�t�F�N�g���W���[���t�@�C���ւ̃p�X
	//          dllname  �G�t�F�N�g���W���[���t�@�C����
	//==========================================================================
	{
		// �J�����g�f�B���N�g����ݒ肷��
		char saveDir[MAX_PATH + 1];
		::GetCurrentDirectory(MAX_PATH, saveDir);
		::SetCurrentDirectory(path);

		// ���W���[����ǂݍ���
		_mod = ::LoadLibrary(dllname);

		// �J�����g�f�B���N�g�������ɖ߂�
		::SetCurrentDirectory(saveDir);

		// �G�N�X�|�[�g���ꂽ�֐��̃A�h���X���擾����
		if ( _mod ) {
			__load   = (EFFECT_LOAD_PROC  )::GetProcAddress(_mod, "_load"  );
			__unload = (EFFECT_UNLOAD_PROC)::GetProcAddress(_mod, "_unload");
			__run    = (EFFECT_RUN_PROC   )::GetProcAddress(_mod, "_run"   );
		} else {
			__load   = NULL;
			__unload = NULL;
			__run    = NULL;
		}

		// ���[�h
		if ( __load ) {
			long path_len;
			HGLOBAL path_h = str_to_h(path, &path_len);
			__load(path_h, path_len);
		}
	}

	virtual ~effect()
	//==========================================================================
	//�y�@  �\�z�f�X�g���N�^
	//==========================================================================
	{
		// �A�����[�h
		if ( __unload ) {
			__unload();
		}

		// ���W���[�����������
		if ( _mod ) {
			::FreeLibrary(_mod);
		}
	}

	BOOL _run(const char* plugin, const long* param, long param_num)
	//==========================================================================
	//�y�@  �\�z�v���O�C�����s
	//--------------------------------------------------------------------------
	//�y��  ���zplugin     �v���O�C�� DLL ��
	//                     �g���q�h.dll�h�͕t���Ă��t���Ȃ��Ă��ǂ�
	//          param      �v���O�C���ɓn���p�����[�^�̔z��
	//          param_num  �p�����[�^�̐�
	//--------------------------------------------------------------------------
	//�y�߂�l�z������ TRUE�A���s�� FALSE
	//==========================================================================
	{
		if ( __run ) {
			long plugin_len;
			HGLOBAL plugin_h = str_to_h(plugin, &plugin_len);
			HGLOBAL param_h = ::GlobalAlloc(GMEM_FIXED, sizeof(long) * param_num);
			memcpy((long*)param_h, param, sizeof(long) * param_num);
			return __run(plugin_h, plugin_len, param_h, param_num);
		}
		return FALSE;
	}

};

#endif // !EFFECT_HPP
//==============================================================================
// End of effect.hpp
//==============================================================================
