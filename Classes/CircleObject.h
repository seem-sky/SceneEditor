#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "BaseObject.h"

class CircleObject : public BaseObject
{
public:
	CREATE_OBJECT(CircleObject);

	bool init() override;

	//���ƶ����
	void drawCircle(bool solid = true);

	//���ƶ���εĽڵ�
	cocos2d::CCDrawNode *m_drawnode;

	//Բ��
	CC_SYNTHESIZE(cocos2d::CCPoint, m_center, CenterPoint);
	//�뾶
	float m_radius;
	void setRadius(float radius);
	float getRadius() { return m_radius; }

	//�߿�ɫ
	CC_SYNTHESIZE(cocos2d::ccColor4F, m_borderColor, BorderColor);
	//���ɫ
	CC_SYNTHESIZE(cocos2d::ccColor4F, m_fillColor, FillColor);


	//Բ�εĴ����ж�
	bool containsTouchLocation(cocos2d::CCTouch *touch) override;
	//��д���ִ�����ק����
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) override;

	void draw() override;
};

#endif
