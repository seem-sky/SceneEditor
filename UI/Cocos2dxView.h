#ifndef COCOS2DXVIEW_H
#define COCOS2DXVIEW_H
#include "GL\glew.h"

#include "../Classes/AppDelegate.h"
#include "../Classes/EditorScene.h"
#include "../Classes/CommonObject.h"
#include <QtWidgets\QMainWindow>
#include <QtWidgets\QScrollArea>
#include <QtGui\QMouseEvent>
#include <QtCore\QTimer>
#include "../proj.win32/ModeState.h"
#include "ListWidget.h"

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
	//������ͼ����
	void setBackground(QString filename);
	//��ȡ��ͼ��ָ��
	EditorScene* getEditorScene() {return (EditorScene*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(view_tag);}

private slots:
	//��Ⱦcocos2dx
	void renderCocos2dx();

	//��������ϵľ���λ��
	void mouseMoveInView();

public:
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	//���ű�ѡ�еľ��鵽�����
	void attachSpriteToMouse();

	//�������ͼ�ƶ��Ķ�ʱ��
	QTimer m_mouseTimer;

	//��Ⱦ��ʱ��
	QTimer m_timer;

	//��qt��Ϣ�ٷ�װ��window��Ϣ���͸�cocos2dx
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	//����ģʽ�¶����������Ӧ����
	//���ģʽ
	void mousePressInBrowse(QMouseEvent *event);
	void mouseMoveInBrowse(QMouseEvent *event);
	void mouseReleaseInBrowse(QMouseEvent *event);
	//Բ�α༭ģʽ
	void mousePressInCircleEdit(QMouseEvent *event);
	void mouseMoveInCircleEdit(QMouseEvent *event);
	void mouseReleaseInCircleEdit(QMouseEvent *event);
	//����α༭ģʽ
	void mousePressInPolygonEdit(QMouseEvent *event);
	void mouseMoveInPolygonEdit(QMouseEvent *event);
	void mouseReleaseInPolygonEdit(QMouseEvent *event);
	//��ͨ�༭ģʽ
	void mousePressInCommonEdit(QMouseEvent *event);
	void mouseMoveInCommonEdit(QMouseEvent *event);
	void mouseReleaseInCommonEdit(QMouseEvent *event);
	QPointF	m_commonPos;
	//ѡȡ�༭ģʽ
	void mousePressInChoiceEdit(QMouseEvent *event);
	void mouseMoveInChoiceEdit(QMouseEvent *event);
	void mouseReleaseInChoiceEdit(QMouseEvent *event);

	cocos2d::CCPoint convertToOpenglPoint(QPoint point)
	{
		QRect r = this->geometry();
		QPoint	pos = mapFromGlobal(point);
		return ccp(pos.rx(), r.height() - pos.ry());
	}

	AppDelegate cocos2dx_app;
	cocos2d::CCSprite *m_mouseSprite;

	//����ͼƬ�б�
	CC_SYNTHESIZE(ListWidget*, m_listwidget, ListWidget);

	//����Ĭ�Ϸֱ���
	const int defaultSizeX = 480;
	const int defaultSizeY = 320;
};

#endif
