//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Object_manager.h"

namespace  ObjectManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化		
		this->timeCnt = 0;
		srand((unsigned int)time(NULL));

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (ge->gameoverFlag)
		{
			this->Kill();
		}
		this->timeCnt++;		

		if (this->timeCnt % 120 ==0)//アイテム生成
		{
			this->Create_Object(true);
			return;
		}
		else if (this->timeCnt % 300 == 0)//エネミー生成
		{
			this->Create_Object(false);
			return;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{

	}

	//オブジェクト生成
	void Object::Create_Object(bool isitItem)
	{
		auto enemys = ge->GetTask_Group_GN<ObjectEnemy::Object>("オブジェクト", "エネミー");
		auto Items = ge->GetTask_Group_GN<ObjectItem::Object>("オブジェクト", "アイテム");
		auto map = ge->GetTask_One_G<Map3d::Object>("マップ");	
		
		ML::Vec2 rm; //randommass
		rm.x = float(rand() % (map->sizeX - 2)) + 1;
		rm.y = float(rand() % (map->sizeZ - 2)) + 1;
		
		//重ならないオブジェクト生成
		if (isitItem)
		{
			for (auto i = Items->begin(); i != Items->end(); i++)
			{	
				if ((*i)->Object_Position_Mass() == rm)
				{
					rm.x = float(rand() % (map->sizeX - 2)) + 1;
					rm.y = float(rand() % (map->sizeZ - 2)) + 1;
					i = Items->begin();
				}
			}			
			ObjectItem::Object::Create(true, rm);
		}
		else
		{			
			for (auto i = enemys->begin(); i != enemys->end(); i++)
			{
				if ((*i)->Object_Position_Mass() == rm)
				{
					rm.x = float(rand() % (map->sizeX - 2)) + 1;
					rm.y = float(rand() % (map->sizeZ - 2)) + 1;
					i = enemys->begin();
				}
			}
			ObjectEnemy::Object::Create(true, rm);
		}
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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