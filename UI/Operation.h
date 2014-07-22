#ifndef _OPERATION_H_
#define _OPERATION_H_
#include "CommonObject.h"
#include "PolygonObject.h"
#include <QtCore/QFileInfo>

//��ʾ��������
enum class OperType
{
	COMMON_EDIT, POLYGON_EDIT, CIRCLE_EDIT
};

//��������
class Operation
{
public:
	virtual bool exec() = 0;	//ִ�в���
	virtual bool undo() = 0;	//��������
	virtual ~Operation() {}
	CC_SYNTHESIZE(OperType, m_type, OperType);
};

//��ͨ�༭����
class CommonEditOper : public Operation
{
public:
	//_type_name:�����������
	//node:Ҫ��ӵ��Ĳ�
	//point:�����λ��
	//path:ͼƬ�ļ���·��
	CommonEditOper(std::string _type_name, cocos2d::CCNode *node, cocos2d::CCPoint point, QFileInfo _file_info) :pos(point), layer(node), file_info(_file_info) 
	{ 
		setOperType(OperType::COMMON_EDIT); 
		type_name = _type_name;
	}
	bool exec()
	{
		sprite_object = CommonObject::create(CCSprite::create(file_info.absoluteFilePath().toLatin1().data()));
		sprite_object->setTypeName(type_name);
		sprite_object->setFileName(file_info.fileName().toLatin1().data());
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
	QFileInfo	file_info;			//�ļ���Ϣ
	std::string  type_name;			//��������
	cocos2d::CCNode *layer;			//��ӵ��Ĳ�
	cocos2d::CCPoint pos;			//���λ��
	CC_SYNTHESIZE_READONLY(CommonObject*, sprite_object, CommonObject);	//�����ľ������
};

//����α༭����
class PolygonEditOper : public Operation
{
public:
	PolygonEditOper(cocos2d::CCNode *node) :layer(node) { setOperType(OperType::POLYGON_EDIT); }
	bool exec()
	{
		polygon_object = PolygonObject::create(CCSprite::create());
		layer->addChild(polygon_object);
		return true;
	}

	bool undo()
	{
		polygon_object->removeFromParent();
		return true;
	}

	//����Ķ���������ɾ����
	bool pushPoint(cocos2d::CCPoint p) { return polygon_object->pushPoint(p); }
	bool popPoint()	{ return polygon_object->popPoint(); }
private:
	cocos2d::CCNode *layer;			//��ӵ��Ĳ�
	CC_SYNTHESIZE_READONLY(PolygonObject*, polygon_object, PolyObject);	//�����Ķ���ζ���
};

//Բ�α༭����
class CircleEditOper : public Operation
{
public:
	CircleEditOper(cocos2d::CCNode *node,cocos2d::CCPoint center, float radius = 10):layer(node), m_center(center), m_radius(radius) 
	{
		setOperType(OperType::CIRCLE_EDIT);
	}
	bool exec()
	{
		circle_object = CircleObject::create(CCSprite::create());
		circle_object->setCenterPoint(m_center);
		circle_object->setRadius(m_radius);
		layer->addChild(circle_object);
		return true;
	}

	bool undo()
	{
		circle_object->removeFromParent();
		return true;
	}

	void setRadius(float radius)
	{
		m_radius = radius;
		circle_object->setRadius(m_radius);
	}
private:
	cocos2d::CCNode *layer;			//��ӵ��Ĳ�
	float	m_radius;
	CC_SYNTHESIZE_READONLY(CircleObject*, circle_object, CircleObject);	//������Բ�ζ���
	CC_SYNTHESIZE_READONLY(cocos2d::CCPoint, m_center, CenterPoint); //Բ��
};

#endif