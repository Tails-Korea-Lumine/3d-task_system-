//-------------------------------------------------------------------
//キャラクタのスーパークラス
//-----------------------------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Effect_Manager.h"
#include "Task_Map3d.h"

//方向転換
void BChara::Change_Angle(Angle4& playerAngle, bool Rotation_is_true_Angle4_is_false)
{
	auto in1 = DI::GPad_GetState("P1");
	//4方向転換
	if (!Rotation_is_true_Angle4_is_false)
	{
		switch (playerAngle)
		{
		case Up:
		case Down:
			if (in1.LStick.L.on)
			{
				playerAngle = Left;
				this->angle.y = ML::ToRadian((float)Left);
			}
			if (in1.LStick.R.on)
			{
				playerAngle = Right;
				this->angle.y = ML::ToRadian((float)Right);
			}
			break;
		case Left:
		case Right:
			if (in1.LStick.U.on)
			{
				playerAngle = Up;
				this->angle.y = ML::ToRadian((float)Up);
			}
			if (in1.LStick.D.on)
			{
				playerAngle = Down;
				this->angle.y = ML::ToRadian((float)Down);
			}
			break;
		}
	}
	else
	{
		//左回転右回転
		if (in1.LStick.volume > 0)
		{			
			this->angle.y += in1.LStick.axis.x * ML::ToRadian(4);
			ML::Mat4x4 matR;
			matR.RotationY(this->angle.y);
		}
		if (in1.HL.on)
		{
			this->angle.y -= ML::ToRadian(4);
			ML::Mat4x4 matR;
			matR.RotationY(this->angle.y);
		}
		if (in1.HR.on)
		{
			this->angle.y += ML::ToRadian(4);
			ML::Mat4x4 matR;
			matR.RotationY(this->angle.y);
		}
	}
}
//めり込まない処理
void BChara::Chara_CheckMove(ML::Vec3& est_)
{
	if (this->timeCnt > 80)
	{
		this->playing_Effect_Flag = false;
		this->timeCnt = 0;
	}
	auto map = ge->GetTask_One_G<Map3d::Object>("マップ");
	//XZ平面に対する移動
	while (est_.x != 0.0f)//X移動
	{
		float preX = this->pos.x;//移動前の位置を確報
		if (est_.x >= 1.0f)
		{
			this->pos.x += 1.0f;
			est_.x -= 1.0f;
		}
		else if (est_.x <= -1.0f)
		{
			this->pos.x -= 1.0f;
			est_.x += 1.0f;
		}
		else
		{
			this->pos.x += est_.x;
			est_.x = 0.0f;
		}

		//接触判定を試してみる
		ML::Box3D hit = this->hitBase;
		hit.Offset((int)this->pos.x, (int)this->pos.y, (int)this->pos.z);
		if (map->Map_Check_Hit(hit))
		{
			this->playing_Effect_Flag = true;
			this->pos.x = preX;
			break;
		}
	}

	while (est_.z != 0.0f)//Z移動
	{
		float preZ = this->pos.z;

		if (est_.z >= 1.0f)
		{
			this->pos.z += 1.0f;
			est_.z -= 1.0f;
		}
		else if (est_.z <= -1.0f)
		{
			this->pos.z -= 1.0f;
			est_.z += 1.0f;
		}
		else
		{
			this->pos.z += est_.z;
			est_.z = 0.0f;
		}
		//接触判定を試してみる
		ML::Box3D hit = this->hitBase;
		hit.Offset((int)this->pos.x, (int)this->pos.y, (int)this->pos.z);
		if (map->Map_Check_Hit(hit))
		{
			this->playing_Effect_Flag = true;
			this->pos.z = preZ;
			break;
		}
	}

	//縦軸に対する移動
	while (est_.y != 0.0f)
	{
		float preY = this->pos.y;
		if (est_.y >= 1.0f)
		{
			this->pos.y += 1.0f;
			est_.y -= 1.0f;
		}
		else if (est_.y <= -1.0f)
		{
			this->pos.y -= 1.0f;
			est_.y += 1.0f;
		}
		else
		{
			this->pos.y += est_.y;
			est_.y = 0.0f;
		}
		//接触判定を試してみる
		ML::Box3D hit = this->hitBase;
		hit.Offset((int)this->pos.x, (int)this->pos.y, (int)this->pos.z);
		if (map->Map_Check_Hit(hit))
		{
			this->playing_Effect_Flag = true;
			this->pos.y = preY;
			break;
		}
	}
	if (this->playing_Effect_Flag)
	{
		if (this->timeCnt != 0)
		{
			this->timeCnt++;
			return;
		}
		//壁にぶつかった瞬間だけエフェクト生成
		auto Efm = ge->GetTask_One_G<EffectManager::Object>("エフェクト");
		Efm->Add_Effect(this->pos + ML::Vec3(0, 110, 0), ML::Vec3(0, 0, 0), BEffect::effType::Hit_to_Wall);
		this->timeCnt++;
	}	
}

