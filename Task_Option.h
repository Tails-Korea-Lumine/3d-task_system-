#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//������
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "BObject.h"

namespace Tail
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�I�v�V����");	//�O���[�v��
	const  string  defName("������");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����
		string meshName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BObject
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_, ML::Vec3 vec , int m);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(ML::Vec3 vec, int m);
		bool  B_Finalize();
		bool  Initialize(ML::Vec3 vec, int m);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		int cnt;//1�t���[����ɏ��ł��邽�߂̃J�E���^
		int tailnum;
	};
}