#ifndef COCOS2DXVIEW_H
#define COCOS2DXVIEW_H
#include "GL\glew.h"

#include "../Classes/AppDelegate.h"
#include <QtWidgets\QMainWindow>
#include <QtWidgets\QScrollArea>
#include <QtGui\QMouseEvent>
#include <QtCore\QTimer>

USING_NS_CC;
//CloseNormal
class Cocos2dxView : public QWidget
{
	Q_OBJECT

public:
	Cocos2dxView(QWidget *parent = 0);

	virtual ~Cocos2dxView();

	//��ʼ��cocos2dx��ͼ��������Ⱦ
	void initCocos2dx();
	
	//��ȡcocos2dx gl��ͼ
	CCEGLView* getView() { return CCEGLView::sharedOpenGLView(); }

	//������ͼ�Ĵ�С
	void setViewGeometry(const QRect &rect);

private slots:
	//��Ⱦcocos2dx
	void renderCocos2dx();

	//��������ϵľ���λ��
	void mouseMoveInView();

public:
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;

	//��qt��Ϣ�ٷ�װ��window��Ϣ���͸�cocos2dx
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	//�������ͼ�ƶ��Ķ�ʱ��
	QTimer m_mouseTimer;

	//��Ⱦ��ʱ��
	QTimer m_timer;

	AppDelegate cocos2dx_app;
	cocos2d::CCSprite *m_mouseSprite;

	//����Ĭ�Ϸֱ���
	const int defaultSizeX = 480;
	const int defaultSizeY = 320;
};

#endif
