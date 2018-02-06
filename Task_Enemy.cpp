//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Enemy.h"
#include  "Task_Effect_Manager.h"

namespace  ObjectEnemy
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "EnemyMesh";

		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/box3.sob");
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
		this->pos.x = (rm.x * 100)+50;
		this->pos.y = 700;
		this->pos.z = (rm.y * 100)+50;
		this->hitBase = ML::Box3D(-50, -50, -50, 100, 100, 100);
		this->fallspeed = -10;

		this->Load_Eff_Flag = false;
		this->efCnt = 0;

		this->render3D_Priority[0] = 0.8f;
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
		//地面につくまで落下
		if (!this->Object_Check_Bother())
		{
			this->pos.y += this->fallspeed;
		}
		else
		{
			//地面についたらエフェクト生成フラグを立てる
			this->Load_Eff_Flag = true;
		}
		if (this->Load_Eff_Flag && this->efCnt == 0)
		{

			auto EfM = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
			EfM->Add_Effect(this->pos, ML::Vec3(0, 0, 0), BEffect::effType::EnemyLanding);


			this->efCnt++;
		}
		this->fallspeed -= ML::Gravity(100);

		//auto p = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (ge->pl == nullptr || ge->gameoverFlag) { return; }		
		

		//プレイヤとのあたり判定
		ML::Box3D playerhitbox = ge->pl->hitBase.OffsetCopy(ge->pl->pos);
		if (this->Hit_Check_toPlayer(playerhitbox,this->pos))
		{
			auto EfM = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
			EfM->Add_Effect(this->pos, ML::Vec3(0, 0, 0), BEffect::effType::Hit_to_Enemy);
			ge->pl->timeCnt = 0;
			ge->gameoverFlag = true;
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
		ML::Mat4x4 matT;
		matT.Translation(this->pos);

		ML::Mat4x4 matW = matT;

		DG::EffectState().param.matWorld = matW;
		DG::EffectState().param.objectColor = ML::Color(1, 1, 0, 0);
		DG::Mesh_Draw(this->res->meshName);
		DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
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