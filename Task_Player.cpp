//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Effect_Manager.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->meshName = "PlayerMesh";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/char_Stand.SOB");
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
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化	
		this->pos = ML::Vec3(600, 100, 600);
		this->tailsize = 1;
		this->playerAngle = Right;
		this->hitBase = ML::Box3D(-30, 0, -30, 60, 160, 60);
		this->angle = ML::Vec3(0, 0, 0);
		this->speed = 8.0f;
		this->incresedTail = false;
		this->deadsence_Move = 0.0f;

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
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (ge->gameoverFlag)
		{			
			if (this->timeCnt == 0)
			{
				//オプションをkill
				ge->KillAll_G("オプション");

				//死亡エフェクト生成
				auto Efm = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
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
		//方向転換
		this->Change_Angle(this->playerAngle,true);		
		

		//自動移動
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);
		est.x += this->speed;
		est = matR.TransformCoord(est);
		
		this->Chara_CheckMove(est);

		
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
		//回転行列
		ML::Mat4x4 matR;
		matR.RotationY(this->angle.y);
		//モデルの表示
		ML::Mat4x4 matW = matR*matT;
		DG::EffectState().param.matWorld = matW;
		DG::Mesh_Draw(this->res->meshName);
	}
	//-----------------------------------------------------------------------
	//追加メソッド
	
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