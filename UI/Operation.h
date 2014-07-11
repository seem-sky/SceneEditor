#ifndef _OPERATION_H_
#define _OPERATION_H_
#include "CommonObject.h"

//��������
class Operation
{
public:
	virtual bool exec() = 0;	//ִ�в���
	virtual bool undo() = 0;	//��������
	virtual ~Operation() {}
};

//��ͨ�༭����
class CommonEditOper : public Operation
{
public:
	CommonEditOper(cocos2d::CCNode *node, cocos2d::CCPoint point, std::string path):pos(point),layer(node),file_path(path) {}
	bool exec()
	{
		sprite_object = CommonObject::create(CCSprite::create(file_path.c_str()));
		sprite_object->setPosition(pos);
		layer->addChild(sprite_object);
		return true;
	}

	bool undo()
	{
		sprite_object->removeFromParent();
		return true;
	}
private:
	std::string  file_path;			
	cocos2d::CCNode *layer;			//��ӵ��Ĳ�
	cocos2d::CCPoint pos;			//���λ��
	CommonObject *sprite_object;	//�����ľ������

	CommonEditOper(const CommonEditOper&);
};

#endif