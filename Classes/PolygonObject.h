#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "BaseObject.h"
#include <iostream>
#include <vector>
USING_NS_CC;
class PolygonObject : public BaseObject
{
public:
	CREATE_OBJECT(PolygonObject);

	bool init() override;
	/*
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCLOG("ssss");
		CCPoint p = pTouch->getLocation();
		CCLOG("%f %f", p.x, p.y);
		return true;
	}
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCPoint p = pTouch->getLocation();
		CCLOG("%f %f", p.x, p.y);
	}
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{

	}
    
	void registerWithTouchDispatcher()
	{
		CCNode::registerWithTouchDispatcher();
		cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	}*/


	//��Ӷ���ζ���
	bool pushPoint(cocos2d::CCPoint p);
	//ɾ�������ӵĶ���
	bool popPoint();
	//���ƶ����
	void drawPolygon();

	//����εĶ�������
	std::vector<cocos2d::CCPoint> m_points;

	//���ƶ���εĽڵ�
	cocos2d::CCDrawNode *m_drawnode;
	//����εı߿�ɫ
	CC_SYNTHESIZE(cocos2d::ccColor4F, m_borderColor, BorderColor);
	//����ε����ɫ
	CC_SYNTHESIZE(cocos2d::ccColor4F, m_fillColor, FillColor);
	//����ζ�����ɫ
	CC_SYNTHESIZE(cocos2d::ccColor4F, m_vertexColor, VertexColor);
};

#endif
