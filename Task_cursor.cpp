//-------------------------------------------------------------------
//^CgζΚ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_cursor.h"
#include "Task_Title.h"

namespace  Cursor
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//\[XΜϊ»
	bool  Resource::Initialize()
	{
		this->imageName = "Cursor";

		DG::Image_Create(this->imageName, "./data/image/cursor.png");
		return true;
	}
	//-------------------------------------------------------------------
	//\[XΜπϊ
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->pos = ML::Vec2(300, -1000);
		this->select_pos = -1;

		//^XNΜΆ¬

		return  true;
	}
	//-------------------------------------------------------------------
	//uIΉv^XNΑΕΙPρΎ―s€
	bool  Object::Finalize()
	{
		//f[^^XNπϊ

		auto title = ge->GetTask_One_G <Title::Object>("^Cg");
		if (title == nullptr)
		{
			return false;
		}
		title->I_Select(this->select_pos);


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//ψ«p¬^XNΜΆ¬
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//uXVvPt[Ιs€
	void  Object::UpDate()
	{
		this->pos.y = (float)this->Move_Cursor(this->select_pos);

		auto in1 = DI::GPad_GetState("P1");
		if (in1.B3.down || in1.ST.down)
		{
			this->Kill();
		}
		
	}
	//-------------------------------------------------------------------
	//uQc`ζvPt[Ιs€
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-16, -16, 32, 32);
		ML::Box2D src(0, 0, 100, 100);

		draw.Offset(this->pos);
		DG::Image_Draw(this->res->imageName, draw, src);
	}

	void  Object::Render3D_L0()
	{
		
	}

	//------------------------------------------------------------------------------------
	//ΗΑ\bh
	int Object::Move_Cursor(int& select)
	{
		auto in1 = DI::GPad_GetState("P1");

		if (in1.LStick.U.down || in1.HU.down)
		{
			select -= 2;
		}
		if (in1.LStick.D.down || in1.HD.down)
		{
			select += 2;
		}

		if (select < -1)
		{
			select = -1;
		}
		else if (this->select_pos > 1)
		{
			select = 1;
		}

		return 500 + (select * 45);
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