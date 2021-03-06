//-------------------------------------------------------------------
//ΗφJ}
//-------------------------------------------------------------------
#include  "MyPG.h"
#include "Task_CameraMan.h"
#include "Task_Player.h"

namespace  CameraMan
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{		
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//uϊ»v^XNΆ¬ΙPρΎ―s€
	bool  Object::Initialize()
	{
		//X[p[NXϊ»
		__super::Initialize(defGroupName, defName, true);
		//\[XNXΆ¬or\[X€L
		this->res = Resource::Create();

		//f[^ϊ»
		this->dist = ML::Vec3(-600, +300, 0);
		//JΜέθ
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(600,300,600),				//	^[QbgΚu
			ML::Vec3(0,400,600),			//	JΚu
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	JΜγϋόxNg
			ML::ToRadian(35), 10.0f, 4000.0f,	//	μpEμ£
			(float)ge->screenWidth / (float)ge->screenHeight);		//	ζΚδ¦		
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//ψ«p¬^XNΜΆ¬
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		auto in1 = DI::GPad_GetState("P1");
		auto p = ge->GetTask_One_G<Player::Object>("vC");

		if (p == nullptr || ge->gameoverFlag) { return; }

		this->angle.y = in1.LStick.axis.x * ML::ToRadian(4);
		if (in1.HL.on)
		{
			this->angle.y -= ML::ToRadian(4);			
		}
		if (in1.HR.on)
		{
			this->angle.y += ML::ToRadian(4);			
		}

		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);
		this->dist = matR.TransformCoord(this->dist);

		ge->camera[0]->target = p->pos + ML::Vec3(0, 200, 0);;
		ge->camera[0]->pos = p->pos + this->dist;

	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{

	}
	//
	//ΘΊΝξ{IΙΟXsvΘ\bh
	//
	//-------------------------------------------------------------------
	//^XNΆ¬ϋ
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//Q[GWΙo^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//CjVCYΙΈs΅½ηKill
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
	//\[XNXΜΆ¬
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