----------------------------------------------------------------------
���ugainer�v�F���݊J���Ă���E�C���h�E��񋓂���SAORI
��Written by CSaori Project / original by �����l�€
�@http://code.google.com/p/csaori/
----------------------------------------------------------------------

������͉���������̂�

�@���݊J���Ă���E�C���h�E��񋓂��邽�߂�SAORI�ł��B

�������

�EWin95/NT4�ȏ�

���g�p���@

[�ėp�R�}���h]
�K��1�p�����[�^�ȏ�K�v�ł��B
1�߂̃p�����[�^(Argument0)�́u�R�}���h�v�ł��B�ȍ~�̍��ږ���ݒ肵�ĉ������B

��search
�E�@�\
�@Gainer���ڑ����ꂽ�|�[�g�ԍ���Ԃ��܂��B

�E����
�@Result�F�񋓂ł����|�[�g�̑��� / 0�͌�����Ȃ�����
�@Value0�`�F���ԂɃ|�[�g�ԍ�

��set.hwnd
�E�@�\
�@Gainer����̌��ʂ�SSTP�Œʒm���邽�߂�hwnd�ł��i�{�^����ԂȂǁj�B
�@�s�v�ȏꍇ�͐ݒ肷��K�v�͂���܂���B

�E�p�����[�^
�@Argument1�FSSTP���󂯕t����E�C���h�E��HWND

�E����
�@Result�FOK

[Gainer�p�R�}���h�Q]
�K��2�p�����[�^�ȏ�K�v�ł��B
1�߂̃p�����[�^(Argument0)�́u�R�}���h�v�ł��B�ȍ~�̍��ږ���ݒ肵�ĉ������B
2�߂̃p�����[�^(Argument1)�́u�V���A���|�[�g�ԍ��v�ł��B
1����128�܂ł̐��l���w�肷�邩�A���邢��0���w�肷��Ə����Gainer��T�����܂��B

��set.config
�E�@�\
�@Gainer�̃��[�h��ݒ肵�܂�(KONFIGURATION_n����)
�@
�E�p�����[�^
�@Argument2�F���[�h�ԍ�(1�`8���T�|�[�g)

�E����
�@Result�FOK/NG

��get.version
�E�@�\
�@Gainer�̃t�@�[���E�F�A�̃o�[�W�������擾���܂��B
�@
�E����
�@Result�F�t�@�[���E�F�A�̃o�[�W�����ԍ�

��in.analog.all
�E�@�\
�@�A�i���O���̓s���̏�Ԃ����ׂĎ擾���܂�

�E����
�@Result�FOK/NG
�@Value0�`�F

��out.analog.single
�E�@�\
�@1�̃A�i���O�o�̓s���ւ̏o�͓d����ύX���܂��B

�E�p�����[�^
�@Argument2�F�s���ԍ�
�@Argument3�F�l(0�`255)

�E����
�@Result�FOK/NG

��in.digital.all
��in.digital.all.bit
�E�@�\
�@�f�W�^�����̓s���̏�Ԃ����ׂĎ擾���܂�

�E����
�@Result�FOK/NG
�@Value0�F�s��0���ŉ��ʃr�b�g�Ƃ��ĉ��߂����ꍇ�̐��l
�@Value0�`�F(.bit)�s��0���珇�Ԃ�0/1

��out.digital.all
��out.digital.all.bit
�E�@�\
�@�f�W�^���o�̓s���̏�Ԃ����ׂĕύX���܂�

�E�p�����[�^
�@Argument2�F�s��0���ŉ��ʃr�b�g�Ƃ��ĉ��߂����ꍇ�̐��l
�@Argument2�`�F(.bit)�s��0���珇�Ԃ�0/1

��out.digital.single
�E�@�\
�@1�̃f�W�^���o�̓s���ւ̏o�̓t���O��ύX���܂��B

�E�p�����[�^
�@Argument2�F�s���ԍ�
�@Argument3�F0/1

�E����
�@Result�FOK/NG

��out.servo.single
�E�@�\
�@1��RC�T�[�{����s���ւ̏o�͏���ύX���܂��B
�@[����] Gainer mini config=8 �݂̂̃T�|�[�g�ł��B

�E�p�����[�^
�@Argument2�F�s���ԍ�
�@Argument3�F0�`255

�E����
�@Result�FOK/NG

��out.led
�E�@�\
�@Gainer��Տ��LED�����点�܂�

�E�p�����[�^
�@Argument2�F1�œ_���A0�ŏ���

�E����
�@Result�FOK/NG

��set.pga.dgnd
�E�@�\
�@0v��œ��̓A�i���O�M���̔{����ݒ肵�܂��B

�E�p�����[�^
�@Argument2�F�i1.0�`48.0�j
�@1.00,1.14,1.33,1.46,1.60,1.78,2.00,2.27
�@2.67,3.20,4.00,5.33,8.00,16.0,24.0,48.0�@����I�����܂��B
�@���Ԃ̒l��ݒ肷��ƓK���ȋ߂��l��I�т܂��B

�E����
�@Result�FOK/NG

��set.pga.agnd
�E�@�\
�@2.5v��œ��̓A�i���O�M���̔{����ݒ肵�܂��B

�E�p�����[�^
�@Argument2�F�{���i1.0�`48.0�j
�@1.00,1.14,1.33,1.46,1.60,1.78,2.00,2.27
�@2.67,3.20,4.00,5.33,8.00,16.0,24.0,48.0�@����I�����܂��B
�@���Ԃ̒l��ݒ肷��ƓK���ȋ߂��l��I�т܂��B

�E����
�@Result�FOK/NG

���z�z������

license.txt�����Ă��������B

���X�V����

�E2008/8/16 Initial Release

