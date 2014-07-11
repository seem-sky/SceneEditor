#ifndef _EDITMODE_H_
#define _EDITMODE_H_

#include "Singleton.h"

//�༭����ģʽ����
class ModeState : public Singleton<ModeState>
{
public:
	//0xffffffff��ʾδ����
	//���༭ģʽ
	enum PrimaryMode
	{
		NoMode		= 0xffffffff,
		BrowseMode	= 0x1,	//���ģʽ
		EditMode	= 0x2	//�༭ģʽ
	};
	//��ģʽ
	enum SubMode
	{
		NoSubMode	= 0xffffffff,
		//���ģʽ����ģʽ

		//�༭��ģʽ
		CircleEdit	= 0x1,	//Բ�α༭
		PolygonEdit	= 0x2,	//����α༭
		CommonEdit	= 0x3,	//��ͨ�༭
		ChoiceEdit	= 0x4	//ѡȡ�༭
	};

	PrimaryMode getPrimaryMode()	{ return primary_mode; }
	SubMode		getSubMode()	{ return sub_mod; }
	void setPrimaryMode(PrimaryMode mode) { primary_mode = mode; }
	void setSubMode(SubMode smode)	{ sub_mod = smode; }

private:
	PrimaryMode	primary_mode;	//��ģʽ
	SubMode		sub_mod;		//��ģʽ
};

#define ModeStateX (ModeState::getInstance())

#endif