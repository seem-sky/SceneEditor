#ifndef COCOS2DXVIEW_H
#define COCOS2DXVIEW_H
#include "GL\glew.h"

#include "../Classes/AppDelegate.h"
#include <QtWidgets\QMainWindow>
#include <QtWidgets\QScrollArea>
#include <QtGui\QMouseEvent>

USING_NS_CC;

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

public:
	//��qt��Ϣ�ٷ�װ��window��Ϣ���͸�cocos2dx
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	QTimer *m_timer;

	AppDelegate cocos2dx_app;

	//����Ĭ�Ϸֱ���
	const int defaultSizeX = 480;
	const int defaultSizeY = 320;
};

#endif
