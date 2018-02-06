#include"BObject.h"

//----------------------------------------------------------
//�I�u�W�F�b�N�g�X�[�p�[�N���X
//----------------------------------------------------------
//�n�ʂɒ��n���Ă��邩�ǂ����𔻒�
bool BObject::Object_Check_Bother()
{
	auto map = ge->GetTask_One_G<Map3d::Object>("�}�b�v");
	//����p��BOX�����
	ML::Box3D hit(this->hitBase.x, this->hitBase.y - 1, this->hitBase.z, this->hitBase.w, 1, this->hitBase.d);

	hit.Offset(this->pos);
	return map->Map_Check_Hit(hit);
}

//�v���C���Ƃ̂����蔻��(������offset�����đ��邱��)
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

//�}�X�P�ʂ̈ʒu��Ԃ��֐�(X�͂��̂܂�X�AZ�͖߂�l��Y�ɂȂ�)
ML::Vec2 BObject::Object_Position_Mass()
{
	ML::Vec2 m;
	m.x = this->pos.x/100.0f;
	m.y = this->pos.z/100.0f;

	return m;
}