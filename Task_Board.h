#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�����̕\����
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "easing.h"

namespace Board
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{�[�h");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
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
		string ready_R_ImgName;
		string ready_E_ImgName;
		string ready_A_ImgName;
		string ready_D_ImgName;
		string ready_Y_ImgName;
		string ui_Score_ImgName;
		string ui_Time_ImgName;
		string ui_speed_ImgName;
		string ui_Tail_ImgName;
		string number_ImgName;
		string mps_ImgName;
		string gameOver_Img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�ϐ�
		//�G�ŕ\�����镶��
		ML::Vec2 ready[5];
		ML::Vec2 ui_Score;
		ML::Vec2 ui_Time;
		ML::Vec2 ui_speed;
		ML::Vec2 ui_Tail;
		ML::Vec2 ui_GameOver;
		ML::Box2D src_Number[11];

		int timeCnt;
		int score;
		bool show_GameOver;
		//���\�b�h
		void Appear_Score_Board();
		void Draw_Score_Board();
		void UpDate_Ready();
		void Draw_Ready();
		void Draw_Tail_Size();
		void Draw_Time();
		void Draw_Score(int score);
		void Draw_speed();
		void Draw_GameOver();
		void UpDate_GameOver();
	};	
}