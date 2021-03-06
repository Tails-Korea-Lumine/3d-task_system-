//-------------------------------------------------------------------
//^CgæÊ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Effect_Manager.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XÌú»
	bool  Resource::Initialize()
	{
		this->meshName = "PlayerMesh";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/char_Stand.SOB");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XÌðú
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
		return true;
	}
	//-------------------------------------------------------------------
	//uú»v^XN¶¬ÉPñ¾¯s¤
	bool  Object::Initialize()
	{
		//X[p[NXú»
		__super::Initialize(defGroupName, defName, true);
		//\[XNX¶¬or\[X¤L
		this->res = Resource::Create();

		//f[^ú»	
		this->pos = ML::Vec3(600, 100, 600);
		this->tailsize = 1;
		this->playerAngle = Right;
		this->hitBase = ML::Box3D(-30, 0, -30, 60, 160, 60);
		this->angle = ML::Vec3(0, 0, 0);
		this->speed = 8.0f;
		this->incresedTail = false;
		this->deadsence_Move = 0.0f;

		this->render3D_Priority[0] = 0.8f;
		//^XNÌ¶¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uI¹v^XNÁÅÉPñ¾¯s¤
	bool  Object::Finalize()
	{
		//f[^^XNðú		
		
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//ø«p¬^XNÌ¶¬			
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[És¤
	void  Object::UpDate()
	{
		if (ge->gameoverFlag)
		{			
			if (this->timeCnt == 0)
			{
				//IvVðkill
				ge->KillAll_G("IvV");

				//SGtFNg¶¬
				auto Efm = ge->GetTask_One_G<EffectManager::Object>("GtFNg");
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
		//ûü]·
		this->Change_Angle(this->playerAngle,true);		
		

		//©®Ú®
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);
		est.x += this->speed;
		est = matR.TransformCoord(est);
		
		this->Chara_CheckMove(est);

		
	}
	//-------------------------------------------------------------------
	//uQc`ævPt[És¤
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT;
		matT.Translation(this->pos);
		//ñ]sñ
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);
		//fÌ\¦
		ML::Mat4x4 matW = matR*matT;
		DG::EffectState().param.matWorld = matW;
		DG::Mesh_Draw(this->res->meshName);
	}
	//-----------------------------------------------------------------------
	//ÇÁ\bh
	
	//
	//ÈºÍî{IÉÏXsvÈ\bh
	//
	//-------------------------------------------------------------------
	//^XN¶¬û
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWÉo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYÉ¸sµ½çKill
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
	//\[XNXÌ¶¬
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