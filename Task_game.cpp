//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include  "Task_game.h"
#include  "Task_Title.h"
#include  "Task_CameraMan.h"
#include "Task_Effect_Manager.h"
#include "Task_Result.h"
#include "Task_Board.h"
#include  "MyPG.h"


namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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

		this->getReady_Cnt = 140;
		ge->pl = nullptr;
		this->gameScore = 0;

		//�J�����̐ݒ�
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(1000.0f, 0.0f, 1000.0f),				//	�^�[�Q�b�g�ʒu
			ML::Vec3(1000.0f, 1400.0f, -1000.0f),			//	�J�����ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	�J�����̏�����x�N�g��
			ML::ToRadian(35), 10.0f, 4000.0f,	//	����p�E���싗��
			(float)ge->screenWidth / (float)ge->screenHeight);		//	��ʔ䗦		
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//���C�e�B���O�̐ݒ�
		//���C�e�B���O�L����
		DG::EffectState().param.lightsEnable = true;
		//�����̋�����ݒ肷��
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
		//���s�����̐ݒ�
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//�����̎��
		DG::EffectState().param.light[0].direction = ML::Vec3(1, -1, 1).Normalize();//�Ǝ˕���
		DG::EffectState().param.light[0].color = ML::Color(1, 1, 1, 1);//�F�Ƌ���

		//���^�X�N�̐���
		auto map = Map3d::Object::Create(true);
		auto CM = CameraMan::Object::Create(true);
		auto EfM = EffectManager::Object::Create(true);
		auto ObM = ObjectManager::Object::Create(true);
		auto OpM = OptionManager::Object::Create(true);
		auto bd = Board::Object::Create(true);

		//�t���O������
		ge->gameoverFlag = false;
		ge->getReadyFlag = true;
		
		
		//�����̓G�ƃA�C�e���̔z�u
		ML::Vec2 i1pos (200000, -30000);
		ML::Vec2 e1pos(100000, -10000000);
		auto i1 = ObjectItem::Object::Create(true, i1pos);
		auto e1 = ObjectEnemy::Object::Create(true, e1pos);
		
		
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		auto bd = ge->GetTask_One_G<Board::Object>("�{�[�h");
		this->gameScore = bd->score;
		ge->KillAll_G("�I�u�W�F�N�g");
		ge->KillAll_G("�}�b�v");
		ge->KillAll_G("�v���C��");
		ge->KillAll_G("�I�v�V����");
		ge->KillAll_G("�J�����}��");
		ge->KillAll_G("�G�t�F�N�g");
		ge->KillAll_G("�{�[�h");
		
		
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���			
			auto result = Result::Object::Create(true,this->gameScore);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{		
		if (ge->getReadyFlag)
		{
			this->GetReady();
		}

		auto in = DI::GPad_GetState("P1");
		
		if (in.ST.down && ge->gameoverFlag == true)
		{
			this->Kill();
		}
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{

	}

	//------------------------------------------------------------------------------
	//�ǉ����\�b�h
	//Get Ready
	void Object::GetReady()
	{		
		if (this->getReady_Cnt == 20)
		{
			auto pl = Player::Object::Create(true);
			ge->pl = pl;
		}
		else if (this->getReady_Cnt == 140)
		{
			//�L�����N�^�����G�t�F�N�g�Đ�
			auto Efm = ge->GetTask_One_G<EffectManager::Object>("�G�t�F�N�g");
			Efm->Add_Effect(ML::Vec3(600, 100, 600), ML::Vec3(0, 0, 0), BEffect::effType::CreateCharactor);
		}		
		if (this->getReady_Cnt < 0)
		{
			ge->getReadyFlag = false;
		}
		this->getReady_Cnt--;
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