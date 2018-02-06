//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_OptionManager.h"
#include "Task_Effect_Manager.h"

namespace  OptionManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName[0] = "Tail_Core";
		this->meshName[1] = "Tail_EvenRing";
		this->meshName[2] = "Tail_OddRing";

		DG::Mesh_CreateFromSOBFile(this->meshName[0], "./data/mesh/tail_core.SOB");
		DG::Mesh_CreateFromSOBFile(this->meshName[1], "./data/mesh/tail_evenRing.SOB");
		DG::Mesh_CreateFromSOBFile(this->meshName[2], "./data/mesh/tail_oddRing.SOB");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName[0]);
		DG::Mesh_Erase(this->meshName[1]);
		DG::Mesh_Erase(this->meshName[2]);

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
		this->angle[0] = ML::Vec3(0, 0, 0);
		this->angle[1] = ML::Vec3(0, 0, 0);
		this->scale = ML::Vec3(70.0f, 70.0f, 70.0f);
		//���X�g�̃[���N���A
		this->tailList.clear();
		
		this->hitBase = ML::Box3D(-40, 0, -40, 80, 150, 80);
		this->render3D_Priority[0] = 0.8f;

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		if (ge->gameoverFlag)
		{
			auto Efm = ge->GetTask_One_G<EffectManager::Object>("�G�t�F�N�g");
			int count = 0;
			for (auto it = this->tailList.end(); it != this->tailList.begin(); it--)
			{
				if (count % 10 == 0 && count != 0)
				{
					Efm->Add_Effect((*it), ML::Vec3(0, 0, 0), BEffect::effType::DestroyTail);
				}
				count++;
			}			
		}
		this->tailList.clear();
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
		//auto pl = ge->GetTask_One_G<Player::Object>("�v���C��");
		if (ge->pl == nullptr || ge->gameoverFlag) { return; }

		//list������
		int count = 0;

		this->tailList.push_back(ge->pl->pos);

		if (this->tailList.size() >= (ge->pl->tailsize * 10)-8)
		{
			this->tailList.pop_front();
		}

		//�����G�t�F�N�g�Đ�
		if (ge->pl->incresedTail)
		{
			auto Efm = ge->GetTask_One_G<EffectManager::Object>("�G�t�F�N�g");
			Efm->Add_Effect((*this->tailList.begin()), ML::Vec3(0, 0, 0), BEffect::effType::CreateTail);
			ge->pl->incresedTail = false;
		}
		//�v���C���Ƃ̂����蔻��
		ML::Box3D playerhitbox = ge->pl->hitBase.OffsetCopy(ge->pl->pos);

		for (auto l = this->tailList.end(); l != this->tailList.begin(); l--)
		{
			if (count % 10 == 0 && count != 0 && count >10)
			{
				if (this->Hit_Check_toPlayer(playerhitbox,(*l)))
				{
					auto Efm = ge->GetTask_One_G<EffectManager::Object>("�G�t�F�N�g");
					Efm->Add_Effect((*l), ML::Vec3(0, 0, 0), BEffect::effType::Hit_to_Tail);					
					ge->pl->timeCnt = 0;
					ge->gameoverFlag = true;
					
					return;
				}
			}
			count++;
		}

		//�����O��]
		this->angle[0].y += ML::ToRadian(2);
		this->angle[1].y -= ML::ToRadian(2);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}

	void  Object::Render3D_L0()
	{
		int count = 0;
		for (auto l = this->tailList.end(); l != this->tailList.begin(); l--)
		{
			if (count % 10 == 0 && count != 0)
			{
				ML::Mat4x4 matS ,matT ,matR_ringodd ,matR_ringeven;
				matS.Scaling(this->scale);
				matT.Translation((*l) + ML::Vec3(0,40.0f,0));
				matR_ringeven.RotationY(this->angle[0].y);
				matR_ringodd.RotationY(this->angle[1].y);

				//�R�A�[�\��
				DG::EffectState().param.matWorld = matS * matT;
				DG::Mesh_Draw(this->res->meshName[0]);
				//evenring �\��
				DG::EffectState().param.matWorld = matS * matR_ringeven * matT;
				DG::Mesh_Draw(this->res->meshName[1]);

				//oddring�@�\��
				DG::EffectState().param.matWorld = matS * matR_ringodd * matT;
				DG::Mesh_Draw(this->res->meshName[2]);
			}
			count++;
		}
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