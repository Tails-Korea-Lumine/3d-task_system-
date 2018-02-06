#include"BObject.h"

//----------------------------------------------------------
//オブジェックトスーパークラス
//----------------------------------------------------------
//地面に着地しているかどうかを判定
bool BObject::Object_Check_Bother()
{
	auto map = ge->GetTask_One_G<Map3d::Object>("マップ");
	//判定用のBOXを作る
	ML::Box3D hit(this->hitBase.x, this->hitBase.y - 1, this->hitBase.z, this->hitBase.w, 1, this->hitBase.d);

	hit.Offset(this->pos);
	return map->Map_Check_Hit(hit);
}

//プレイヤとのあたり判定(引数はoffsetさせて送ること)
bool BObject::Hit_Check_toPlayer(ML::Box3D playerHItbox, ML::Vec3 pos)
{		
	ML::Box3D you = playerHItbox;
	ML::Box3D me = this->hitBase.OffsetCopy(pos);

	if (you.Hit(me))
	{
		return true;
	}
	
	return false;
}

//マス単位の位置を返す関数(XはそのままX、Zは戻り値のYになる)
ML::Vec2 BObject::Object_Position_Mass()
{
	ML::Vec2 m;
	m.x = this->pos.x/100.0f;
	m.y = this->pos.z/100.0f;

	return m;
}