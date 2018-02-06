//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Item.h"
#include  "Task_Enemy.h"
#include  "Task_Effect_Manager.h"

namespace  ObjectItem
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "ItemMesh";

		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/Item.SOB");
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
		this->pos.x = (rm.x * 100) + 50;
		this->pos.y = 150;
		this->pos.z = (rm.y * 100) + 50;
		this->scale = ML::Vec3(50.0f, 50.0f, 50.0f);
		this->hitBase = ML::Box3D(-50, 0, -50, 100, 100, 100);
		this->render3D_Priority[0] = 0.8f;

		//���^�X�N�̐���
		//�����G�t�F�N�g
		auto EfM = ge->GetTask_One_G<EffectManager::Object>("�G�t�F�N�g");
		EfM->Add_Effect(this->pos, ML::Vec3(0, 0, 0), BEffect::effType::CreateItem);
			

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
		//auto p = ge->GetTask_One_G<Player::Object>("�v���C��");
		auto enemys = ge->GetTask_Group_GN<ObjectEnemy::Object>("�I�u�W�F�N�g", "�G�l�~�[");

		if (ge->pl == nullptr) { return; }
		ML::Box3D playerhitbox = ge->pl->hitBase.OffsetCopy(ge->pl->pos);
		
		//�v���C���Ƃ̂����蔻��
		if (this->Hit_Check_toPlayer(playerhitbox,this->pos))
		{			
			ge->pl->tailsize++;
			ge->pl->speed += 0.3f;
			ge->pl->incresedTail = true;

			//�폜�G�t�F�N�g����
			auto EfM = ge->GetTask_One_G<EffectManager::Object>("�G�t�F�N�g");
			EfM->Add_Effect(this->pos, ML::Vec3(0, 0, 0), BEffect::effType::DestroyItem);

			this->Kill();
			return;
		}

		//�����G�l�~�[�Əd�Ȃ��Ă��܂����玩�����E��
		for (auto it = enemys->begin(); it != enemys->end(); it++)
		{
			ML::Box3D you, me;
			you = (*it)->hitBase.OffsetCopy((*it)->pos);
			me = this->hitBase.OffsetCopy(this->pos);
			if (you.Hit(me))
			{
				this->Kill();
				return;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT,matS;
		matT.Translation(this->pos);
		matS.Scaling(this->scale);

		ML::Mat4x4 matW = matS * matT;

		DG::EffectState().param.matWorld = matW;
		DG::Mesh_Draw(this->res->meshName);
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