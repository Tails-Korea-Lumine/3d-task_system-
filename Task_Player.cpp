//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Effect_Manager.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "PlayerMesh";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/char_Stand.SOB");
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
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������	
		this->pos = ML::Vec3(600, 100, 600);
		this->tailsize = 1;
		this->playerAngle = Right;
		this->hitBase = ML::Box3D(-30, 0, -30, 60, 160, 60);
		this->angle = ML::Vec3(0, 0, 0);
		this->speed = 8.0f;
		this->incresedTail = false;
		this->deadsence_Move = 0.0f;

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
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (ge->gameoverFlag)
		{			
			if (this->timeCnt == 0)
			{
				//�I�v�V������kill
				ge->KillAll_G("�I�v�V����");

				//���S�G�t�F�N�g����
				auto Efm = ge->GetTask_One_G<EffectManager::Object>("�G�t�F�N�g");
				Efm->Add_Effect(this->pos, ML::Vec3(0, 0, 0), BEffect::effType::DestroyCharactor);			
			}
			if (this->timeCnt > 270)
			{
				this->Kill();
				ge->pl = nullptr;
			}
			this->timeCnt++;
			return;
		}
		ML::Vec3 est;
		//�����]��
		this->Change_Angle(this->playerAngle,true);		
		

		//�����ړ�
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);
		est.x += this->speed;
		est = matR.TransformCoord(est);
		
		this->Chara_CheckMove(est);

		
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
		//��]�s��
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);
		//���f���̕\��
		ML::Mat4x4 matW = matR*matT;
		DG::EffectState().param.matWorld = matW;
		DG::Mesh_Draw(this->res->meshName);
	}
	//-----------------------------------------------------------------------
	//�ǉ����\�b�h
	
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