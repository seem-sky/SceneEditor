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

	//�Ե�����⴦��
	bool containsInDot(cocos2d::CCPoint world_point);
	
	//��ֱ�ߵ����⴦��
	bool containsInLine(cocos2d::CCPoint world_point);

	bool containsTouchLocation(cocos2d::CCPoint world_point) override;

	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	//��Ӷ���ζ���
	bool pushPoint(cocos2d::CCPoint p);
	//ɾ�������ӵĶ���
	bool popPoint();
	//���ƶ����
	void drawPolygon(bool solid = true);

	//����εĶ�������
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(std::vector<cocos2d::CCPoint>, m_points, PolyPoints);

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
