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
	
	bool containsTouchLocation(cocos2d::CCPoint world_point) override;
	/*
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCPoint p = pTouch->getLocation();
		CCLOG("%f %f", p.x, p.y);
		return true;
	}*/
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	//��Ӷ���ζ���
	bool pushPoint(cocos2d::CCPoint p);
	//ɾ�������ӵĶ���
	bool popPoint();
	//���ƶ����
	void drawPolygon(bool solid = true);

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
