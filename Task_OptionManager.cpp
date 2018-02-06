//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_OptionManager.h"
#include "Task_Effect_Manager.h"

namespace  OptionManager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
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
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Mesh_Erase(this->meshName[0]);
		DG::Mesh_Erase(this->meshName[1]);
		DG::Mesh_Erase(this->meshName[2]);

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
		this->angle[0] = ML::Vec3(0, 0, 0);
		this->angle[1] = ML::Vec3(0, 0, 0);
		this->scale = ML::Vec3(70.0f, 70.0f, 70.0f);
		//リストのゼロクリア
		this->tailList.clear();
		
		this->hitBase = ML::Box3D(-40, 0, -40, 80, 150, 80);
		this->render3D_Priority[0] = 0.8f;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		if (ge->gameoverFlag)
		{
			auto Efm = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
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
			//★引き継ぎタスクの生成			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (ge->pl == nullptr || ge->gameoverFlag) { return; }

		//list個数制御
		int count = 0;

		this->tailList.push_back(ge->pl->pos);

		if (this->tailList.size() >= (ge->pl->tailsize * 10)-8)
		{
			this->tailList.pop_front();
		}

		//生成エフェクト再生
		if (ge->pl->incresedTail)
		{
			auto Efm = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
			Efm->Add_Effect((*this->tailList.begin()), ML::Vec3(0, 0, 0), BEffect::effType::CreateTail);
			ge->pl->incresedTail = false;
		}
		//プレイヤとのあたり判定
		ML::Box3D playerhitbox = ge->pl->hitBase.OffsetCopy(ge->pl->pos);

		for (auto l = this->tailList.end(); l != this->tailList.begin(); l--)
		{
			if (count % 10 == 0 && count != 0 && count >10)
			{
				if (this->Hit_Check_toPlayer(playerhitbox,(*l)))
				{
					auto Efm = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
					Efm->Add_Effect((*l), ML::Vec3(0, 0, 0), BEffect::effType::Hit_to_Tail);					
					ge->pl->timeCnt = 0;
					ge->gameoverFlag = true;
					
					return;
				}
			}
			count++;
		}

		//リング回転
		this->angle[0].y += ML::ToRadian(2);
		this->angle[1].y -= ML::ToRadian(2);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
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

				//コアー表示
				DG::EffectState().param.matWorld = matS * matT;
				DG::Mesh_Draw(this->res->meshName[0]);
				//evenring 表示
				DG::EffectState().param.matWorld = matS * matR_ringeven * matT;
				DG::Mesh_Draw(this->res->meshName[1]);

				//oddring　表示
				DG::EffectState().param.matWorld = matS * matR_ringodd * matT;
				DG::Mesh_Draw(this->res->meshName[2]);
			}
			count++;
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