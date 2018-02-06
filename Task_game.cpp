//-------------------------------------------------------------------
//ゲーム本編
//-------------------------------------------------------------------
#include  "Task_game.h"
#include  "Task_Title.h"
#include  "Task_CameraMan.h"
#include "Task_Effect_Manager.h"
#include "Task_Result.h"
#include "Task_Board.h"
#include  "MyPG.h"


namespace  Game
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

		this->getReady_Cnt = 140;
		ge->pl = nullptr;
		this->gameScore = 0;

		//カメラの設定
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(1000.0f, 0.0f, 1000.0f),				//	ターゲット位置
			ML::Vec3(1000.0f, 1400.0f, -1000.0f),			//	カメラ位置
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	カメラの上方向ベクトル
			ML::ToRadian(35), 10.0f, 4000.0f,	//	視野角・視野距離
			(float)ge->screenWidth / (float)ge->screenHeight);		//	画面比率		
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//ライティングの設定
		//ライティング有効化
		DG::EffectState().param.lightsEnable = true;
		//環境光の強さを設定する
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
		//平行光源の設定
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//光源の種類
		DG::EffectState().param.light[0].direction = ML::Vec3(1, -1, 1).Normalize();//照射方向
		DG::EffectState().param.light[0].color = ML::Color(1, 1, 1, 1);//色と強さ

		//★タスクの生成
		auto map = Map3d::Object::Create(true);
		auto CM = CameraMan::Object::Create(true);
		auto EfM = EffectManager::Object::Create(true);
		auto ObM = ObjectManager::Object::Create(true);
		auto OpM = OptionManager::Object::Create(true);
		auto bd = Board::Object::Create(true);

		//フラグ初期化
		ge->gameoverFlag = false;
		ge->getReadyFlag = true;
		
		
		//初期の敵とアイテムの配置
		ML::Vec2 i1pos (200000, -30000);
		ML::Vec2 e1pos(100000, -10000000);
		auto i1 = ObjectItem::Object::Create(true, i1pos);
		auto e1 = ObjectEnemy::Object::Create(true, e1pos);
		
		
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		auto bd = ge->GetTask_One_G<Board::Object>("ボード");
		this->gameScore = bd->score;
		ge->KillAll_G("オブジェクト");
		ge->KillAll_G("マップ");
		ge->KillAll_G("プレイヤ");
		ge->KillAll_G("オプション");
		ge->KillAll_G("カメラマン");
		ge->KillAll_G("エフェクト");
		ge->KillAll_G("ボード");
		
		
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成			
			auto result = Result::Object::Create(true,this->gameScore);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{		
		if (ge->getReadyFlag)
		{
			this->GetReady();
		}

		auto in = DI::GPad_GetState("P1");
		
		if (in.ST.down && ge->gameoverFlag == true)
		{
			this->Kill();
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

	//------------------------------------------------------------------------------
	//追加メソッド
	//Get Ready
	void Object::GetReady()
	{		
		if (this->getReady_Cnt == 20)
		{
			auto pl = Player::Object::Create(true);
			ge->pl = pl;
		}
		else if (this->getReady_Cnt == 140)
		{
			//キャラクタ生成エフェクト再生
			auto Efm = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
			Efm->Add_Effect(ML::Vec3(600, 100, 600), ML::Vec3(0, 0, 0), BEffect::effType::CreateCharactor);
		}		
		if (this->getReady_Cnt < 0)
		{
			ge->getReadyFlag = false;
		}
		this->getReady_Cnt--;
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