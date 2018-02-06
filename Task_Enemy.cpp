//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy.h"
#include  "Task_Effect_Manager.h"

namespace  ObjectEnemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "EnemyMesh";

		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/box3.sob");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(ML::Vec2 rm)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->pos.x = (rm.x * 100)+50;
		this->pos.y = 700;
		this->pos.z = (rm.y * 100)+50;
		this->hitBase = ML::Box3D(-50, -50, -50, 100, 100, 100);
		this->fallspeed = -10;

		this->Load_Eff_Flag = false;
		this->efCnt = 0;

		this->render3D_Priority[0] = 0.8f;
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
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�n�ʂɂ��܂ŗ���
		if (!this->Object_Check_Bother())
		{
			this->pos.y += this->fallspeed;
		}
		else
		{
			//�n�ʂɂ�����G�t�F�N�g�����t���O�𗧂Ă�
			this->Load_Eff_Flag = true;
		}
		if (this->Load_Eff_Flag && this->efCnt == 0)
		{

			auto EfM = ge->GetTask_One_G<EffectManager::Object>("�G�t�F�N�g");
			EfM->Add_Effect(this->pos, ML::Vec3(0, 0, 0), BEffect::effType::EnemyLanding);


			this->efCnt++;
		}
		this->fallspeed -= ML::Gravity(100);

		//auto p = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (ge->pl == nullptr || ge->gameoverFlag) { return; }		
		

		//�v���C���Ƃ̂����蔻��
		ML::Box3D playerhitbox = ge->pl->hitBase.OffsetCopy(ge->pl->pos);
		if (this->Hit_Check_toPlayer(playerhitbox,this->pos))
		{
			auto EfM = ge->GetTask_One_G<EffectManager::Object>("�G�t�F�N�g");
			EfM->Add_Effect(this->pos, ML::Vec3(0, 0, 0), BEffect::effType::Hit_to_Enemy);
			ge->pl->timeCnt = 0;
			ge->gameoverFlag = true;
			return;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT;
		matT.Translation(this->pos);

		ML::Mat4x4 matW = matT;

		DG::EffectState().param.matWorld = matW;
		DG::EffectState().param.objectColor = ML::Color(1, 1, 0, 0);
		DG::Mesh_Draw(this->res->meshName);
		DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, ML::Vec2 rm)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(rm)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(ML::Vec2 rm)
	{
		return  this->Initialize(rm);
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