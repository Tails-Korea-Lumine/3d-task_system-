//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Item.h"
#include  "Task_Enemy.h"
#include  "Task_Effect_Manager.h"

namespace  ObjectItem
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "ItemMesh";

		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/Item.SOB");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize(ML::Vec2 rm)
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->pos.x = (rm.x * 100) + 50;
		this->pos.y = 150;
		this->pos.z = (rm.y * 100) + 50;
		this->scale = ML::Vec3(50.0f, 50.0f, 50.0f);
		this->hitBase = ML::Box3D(-50, 0, -50, 100, 100, 100);
		this->render3D_Priority[0] = 0.8f;

		//★タスクの生成
		//生成エフェクト
		auto EfM = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
		EfM->Add_Effect(this->pos, ML::Vec3(0, 0, 0), BEffect::effType::CreateItem);
			

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
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//auto p = ge->GetTask_One_G<Player::Object>("プレイヤ");
		auto enemys = ge->GetTask_Group_GN<ObjectEnemy::Object>("オブジェクト", "エネミー");

		if (ge->pl == nullptr) { return; }
		ML::Box3D playerhitbox = ge->pl->hitBase.OffsetCopy(ge->pl->pos);
		
		//プレイヤとのあたり判定
		if (this->Hit_Check_toPlayer(playerhitbox,this->pos))
		{			
			ge->pl->tailsize++;
			ge->pl->speed += 0.3f;
			ge->pl->incresedTail = true;

			//削除エフェクト生成
			auto EfM = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
			EfM->Add_Effect(this->pos, ML::Vec3(0, 0, 0), BEffect::effType::DestroyItem);

			this->Kill();
			return;
		}

		//もしエネミーと重なってしまったら自分を殺す
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
	//「２Ｄ描画」１フレーム毎に行う処理
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
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, ML::Vec2 rm)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize(rm)) {
				ob->Kill();//イニシャライズに失敗したらKill
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