//-------------------------------------------------------------------
//�����̕\����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Board.h"

namespace  Board
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->ready_R_ImgName = "Ready_R";
		this->ready_E_ImgName = "Ready_E";
		this->ready_A_ImgName = "Ready_A";
		this->ready_D_ImgName = "Ready_D";
		this->ready_Y_ImgName = "Ready_Y";
		this->ui_Score_ImgName = "UI_Score";
		this->ui_speed_ImgName = "UI_Speed";
		this->ui_Tail_ImgName = "UI_Tail";
		this->ui_Time_ImgName = "UI_Time";
		this->number_ImgName = "Number";
		this->mps_ImgName = "MPS";
		this->gameOver_Img = "GOI";

		DG::Image_Create(this->ready_R_ImgName, "./data/image/R.png");
		DG::Image_Create(this->ready_E_ImgName, "./data/image/E.png");
		DG::Image_Create(this->ready_A_ImgName, "./data/image/A.png");
		DG::Image_Create(this->ready_D_ImgName, "./data/image/D.png");
		DG::Image_Create(this->ready_Y_ImgName, "./data/image/Y.png");
		DG::Image_Create(this->ui_Score_ImgName, "./data/image/UI_Score.png");
		DG::Image_Create(this->ui_speed_ImgName, "./data/image/UI_Speed.png");
		DG::Image_Create(this->ui_Tail_ImgName, "./data/image/UI_Tail.png");
		DG::Image_Create(this->ui_Time_ImgName, "./data/image/UI_Time.png");
		DG::Image_Create(this->number_ImgName, "./data/image/Number.png");
		DG::Image_Create(this->mps_ImgName, "./data/image/mpers.png");
		DG::Image_Create(this->gameOver_Img, "./data/image/gameover.png");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->ready_A_ImgName);
		DG::Image_Erase(this->ready_D_ImgName);
		DG::Image_Erase(this->ready_E_ImgName);
		DG::Image_Erase(this->ready_R_ImgName);
		DG::Image_Erase(this->ready_Y_ImgName);
		DG::Image_Erase(this->ui_Score_ImgName);
		DG::Image_Erase(this->ui_speed_ImgName);
		DG::Image_Erase(this->ui_Tail_ImgName);
		DG::Image_Erase(this->ui_Time_ImgName);
		DG::Image_Erase(this->number_ImgName);
		DG::Image_Erase(this->mps_ImgName);
		DG::Image_Erase(this->gameOver_Img);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������	
		this->timeCnt = 0;
		this->score = 0;
		this->show_GameOver = false;
		//Y���W��easingfunction�œ������̂ŉ�ʂ̊O�ɏ�����
		for (int i = 0; i < 5; i++)
		{
			this->ready[i] = ML::Vec2(280.0f+(128.0f*i), -1000.0f);
		}		
		this->ui_Score = ML::Vec2(880.0f, -1000.0f);
		this->ui_speed = ML::Vec2(0, -1000.0f);
		this->ui_Tail = ML::Vec2(980, -1000.0f);
		this->ui_Time = ML::Vec2(0, -1000.0f);
		this->ui_GameOver = ML::Vec2(160.0f, -1000.0f);
		//�X�R�A�[�{�[�h�̕\����
		easing::Set("Down_to_Up", easing::SINEOUT, 800, 640, 110);
		easing::Set("Up_to_Down", easing::SINEOUT, -50, 0, 110);
		easing::Start("Down_to_Up");
		easing::Start("Up_to_Down");
		//ready�̕\����
		easing::Set("Appear_R", easing::CUBICOUT, 800, 310, 54);
		easing::Set("Appear_E", easing::CUBICOUT, 800, 310, 58);
		easing::Set("Appear_A", easing::CUBICOUT, 800, 310, 62);
		easing::Set("Appear_D", easing::CUBICOUT, 800, 310, 66);
		easing::Set("Appear_Y", easing::CUBICOUT, 800, 310, 70);
		easing::Start("Appear_R");
		easing::Start("Appear_E");
		easing::Start("Appear_A");
		easing::Start("Appear_D");
		easing::Start("Appear_Y");
		easing::Set("Disappear_R", easing::BACKIN, 310, -160, 54);
		easing::Set("Disappear_E", easing::BACKIN, 310, -160, 58);
		easing::Set("Disappear_A", easing::BACKIN, 310, -160, 62);
		easing::Set("Disappear_D", easing::BACKIN, 310, -160, 66);
		easing::Set("Disappear_Y", easing::BACKIN, 310, -160, 70);
		//game over�̕\����
		easing::Set("GameOver_Y", easing::BOUNCEOUT, -400, 100, 400);		

		//�����̓ǂݍ��݃`�b�v��������
		for (int i = 0; i < 11; i++)
		{
			this->src_Number[i].x = (140 * i);
			this->src_Number[i].y = 0;
			this->src_Number[i].w = 140;
			this->src_Number[i].h = 140;
		}

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		easing::UpDate();
		if (ge->getReadyFlag)
		{
			this->Appear_Score_Board();
			this->UpDate_Ready();
		}
		if (!ge->gameoverFlag && ge->pl != nullptr)
		{
			//�X�R�A�[�̌v�Z,���Ԃ̌o�߂Ƃ����ۂ̐��Ō��߂���
			int score_Tail = (ge->pl->tailsize - 1) * 10;
			int score_Min = (this->timeCnt / 60 / 60) * 100;
			int score_Sec = (this->timeCnt / 60 % 60 / 10);

			this->score = score_Tail + score_Min + score_Sec;
		}
		if (ge->gameoverFlag && !this->show_GameOver)
		{
			easing::Start("GameOver_Y");
			this->show_GameOver = true;
		}


		this->UpDate_GameOver();
		
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{		
		this->Draw_Score_Board();
		this->Draw_Ready();

		this->Draw_Tail_Size();
		this->Draw_speed();
		this->Draw_Time();
		this->Draw_Score(this->score);
		this->Draw_GameOver();
	}

	void  Object::Render3D_L0()
	{
		
	}

	//--------------------------------------------------------------------------------
	//�ǉ����\�b�h
	//�X�R�A�[�{�[�h��\����
	void Object::Appear_Score_Board()
	{
		this->ui_speed.y = easing::GetPos("Down_to_Up");
		this->ui_Tail.y = easing::GetPos("Down_to_Up");
		this->ui_Score.y = easing::GetPos("Up_to_Down");
		this->ui_Time.y = easing::GetPos("Up_to_Down");
	}
	//-------------------------------------------------------------------
	//�X�R�A�[�{�[�h�̕`��
	void Object::Draw_Score_Board()
	{
		ML::Box2D draw_Time(0, 0, 300, 64);
		ML::Box2D draw_Score(0, 0, 300, 64);
		ML::Box2D draw_Speed(0, 0, 300, 64);
		ML::Box2D draw_Tail(0, 0, 300, 64);
		ML::Box2D src(0, 0, 700, 140);		

		draw_Time.Offset(this->ui_Time);
		draw_Score.Offset(this->ui_Score);
		draw_Speed.Offset(this->ui_speed);
		draw_Tail.Offset(this->ui_Tail);

		DG::Image_Draw(this->res->ui_Time_ImgName, draw_Time, src);
		DG::Image_Draw(this->res->ui_Score_ImgName, draw_Score, src);
		DG::Image_Draw(this->res->ui_speed_ImgName, draw_Speed, src);
		DG::Image_Draw(this->res->ui_Tail_ImgName, draw_Tail, src);
	}
	//---------------------------------------------------------------------------
	//ready��\����
	void Object::UpDate_Ready()
	{
		if (this->timeCnt < 70)
		{
			this->ready[0].y = easing::GetPos("Appear_R");
			this->ready[1].y = easing::GetPos("Appear_E");
			this->ready[2].y = easing::GetPos("Appear_A");
			this->ready[3].y = easing::GetPos("Appear_D");
			this->ready[4].y = easing::GetPos("Appear_Y");
		}
		else if (this->timeCnt == 70)
		{
			easing::Start("Disappear_R");
			easing::Start("Disappear_E");
			easing::Start("Disappear_A");
			easing::Start("Disappear_D");
			easing::Start("Disappear_Y");
		}
		else if(this->timeCnt > 70)
		{
			this->ready[0].y = easing::GetPos("Disappear_R");
			this->ready[1].y = easing::GetPos("Disappear_E");
			this->ready[2].y = easing::GetPos("Disappear_A");
			this->ready[3].y = easing::GetPos("Disappear_D");
			this->ready[4].y = easing::GetPos("Disappear_Y");
		}
		
		if (this->timeCnt > 140)
		{
			this->timeCnt = 0;
		}
	}

	//----------------------------------------------------------------------------------
	//ready�̕`��
	void Object::Draw_Ready()
	{
		ML::Box2D draw_R(0, 0, 140, 140);
		ML::Box2D draw_E(0, 0, 140, 140);
		ML::Box2D draw_A(0, 0, 140, 140);
		ML::Box2D draw_D(0, 0, 140, 140);
		ML::Box2D draw_Y(0, 0, 140, 140);
		ML::Box2D src(0, 0, 140, 140);

		draw_R.Offset(this->ready[0]);
		draw_E.Offset(this->ready[1]);
		draw_A.Offset(this->ready[2]);
		draw_D.Offset(this->ready[3]);
		draw_Y.Offset(this->ready[4]);

		DG::Image_Draw(this->res->ready_R_ImgName, draw_R, src);
		DG::Image_Draw(this->res->ready_E_ImgName, draw_E, src);
		DG::Image_Draw(this->res->ready_A_ImgName, draw_A, src);
		DG::Image_Draw(this->res->ready_D_ImgName, draw_D, src);
		DG::Image_Draw(this->res->ready_Y_ImgName, draw_Y, src);
	}

	//---------------------------------------------------------------------------------------------------------
	//�����ۂ̐��\��
	void Object::Draw_Tail_Size()
	{
		if (ge->pl == nullptr)
		{
			return;
		}
		
		//�e�ʂ̐��l��ʁX�ɂ��炤
		int tail10, tail1;
		tail10 = ge->pl->tailsize / 10;
		tail1 = (ge->pl->tailsize % 10) - 1;

		if (ge->pl->tailsize % 10 == 0)
		{			
			tail10 = ge->pl->tailsize / 10 - 1;
			tail1 = 9;
		}		
		
		ML::Box2D draw10(1150, 640, 64, 64);
		ML::Box2D draw1(1200, 640, 64, 64);
		
		DG::Image_Draw(this->res->number_ImgName, draw10, this->src_Number[tail10]);		
		DG::Image_Draw(this->res->number_ImgName, draw1, this->src_Number[tail1]);
	}

	//------------------------------------------------------------------------------------------------------
	//���x�̕\��
	void Object::Draw_speed()
	{
		if (ge->pl == nullptr)
		{
			return;
		}
		int speed_per_Sec;
		int speed1000, speed100, speed10, speed1;
		//1�b�ɒl���鑬�x(cm/s)
		speed_per_Sec = int(ge->pl->speed * 60);
		//�e�ʂ̐��l��ʁX�ɂ��炤
		speed1000 = speed_per_Sec / 1000;
		speed100 = (speed_per_Sec % 1000) / 100;
		speed10 = (speed_per_Sec % 100) / 10;
		speed1 = speed_per_Sec % 10;

		ML::Box2D draw1000(200, 640, 64, 64);
		ML::Box2D draw100(250, 640, 64, 64);
		ML::Box2D draw10(300, 640, 64, 64);
		ML::Box2D draw1(350, 640, 64, 64);
		ML::Box2D draw_MPS(400, 640, 128, 64);
		ML::Box2D src_MPS(0, 0, 300, 140);

		//�`��
		DG::Image_Draw(this->res->number_ImgName, draw1000, this->src_Number[speed1000]);
		DG::Image_Draw(this->res->number_ImgName, draw100, this->src_Number[speed100]);
		DG::Image_Draw(this->res->number_ImgName, draw10, this->src_Number[speed10]);
		DG::Image_Draw(this->res->number_ImgName, draw1, this->src_Number[speed1]);
		DG::Image_Draw(this->res->mps_ImgName, draw_MPS, src_MPS);

	}

	//------------------------------------------------------------------------------------
	//���Ԃ̕\��
	void Object::Draw_Time()
	{
		if (ge->pl == nullptr)
		{
			return;
		}
		int min10,min1, sec10,sec1;
		//�e�ʂ̐��l��ʁX�ɂ��炤
		min10 = this->timeCnt / 60 / 60/10;
		min1 = this->timeCnt / 60 / 60 % 10;
		sec10 = this->timeCnt / 60 % 60 / 10;
		sec1 = this->timeCnt / 60 % 60 % 10;

		ML::Box2D draw_Min10(200, 0, 64, 64);
		ML::Box2D draw_Min1(250, 0, 64, 64);
		ML::Box2D draw_Sec10(310, 0, 64, 64);
		ML::Box2D draw_Sec1(360, 0, 64, 64);
		ML::Box2D draw_colon(280, 0, 50, 64);

		//�`��
		DG::Image_Draw(this->res->number_ImgName, draw_Min10, this->src_Number[min10]);
		DG::Image_Draw(this->res->number_ImgName, draw_Min1, this->src_Number[min1]);
		DG::Image_Draw(this->res->number_ImgName, draw_Sec10, this->src_Number[sec10]);
		DG::Image_Draw(this->res->number_ImgName, draw_Sec1, this->src_Number[sec1]);
		DG::Image_Draw(this->res->number_ImgName, draw_colon, this->src_Number[10]);
		
	}

	//-----------------------------------------------------------------------------------------
	//�_���̕\��
	void Object::Draw_Score(int score)
	{
		if (ge->getReadyFlag)
		{
			return;
		}
		//�e�ʂ̐��l��ʁX�ɂ��炤
		int score1000, score100, score10, score1;		
		score1000 = score / 1000;
		score100 = (score / 100) % 10;
		score10 = (score / 10) % 10;
		score1 = score % 10;

		ML::Box2D draw1000(1090, 0, 50, 64);
		ML::Box2D draw100(1130,0, 50, 64);
		ML::Box2D draw10(1170, 0, 50, 64);
		ML::Box2D draw1(1210, 0, 50, 64);

		//�`��
		DG::Image_Draw(this->res->number_ImgName, draw1000, this->src_Number[score1000]);
		DG::Image_Draw(this->res->number_ImgName, draw100, this->src_Number[score100]);
		DG::Image_Draw(this->res->number_ImgName, draw10, this->src_Number[score10]);
		DG::Image_Draw(this->res->number_ImgName, draw1, this->src_Number[score1]);
	}

	//-------------------------------------------------------------------------------------
	//Game Over�̍��W�X�V
	void Object::UpDate_GameOver()
	{
		this->ui_GameOver.y = easing::GetPos("GameOver_Y");
	}

	//--------------------------------------------------------------------------------------
	//Game Over�̕`��
	void Object::Draw_GameOver()
	{
		if(!ge->gameoverFlag)
		{
			return;
		}
		ML::Box2D draw(0, 0, 960, 540);
		ML::Box2D src(0, 0, 960, 540);

		draw.Offset(this->ui_GameOver);

		DG::Image_Draw(this->res->gameOver_Img, draw, src);
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}