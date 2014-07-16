#include "Cocos2dxView.h"
#include <QtWidgets\QtWidgets>
#include <QtWidgets\QScrollArea>

USING_NS_CC;

//for cocos2dx3.0
//#define GLFW_EXPOSE_NATIVE_WIN32
//#define GLFW_EXPOSE_NATIVE_WGL
//#include "glfw3\include\win32\glfw3native.h"

Cocos2dxView::~Cocos2dxView()
{
	m_timer.stop();
	CCDirector::sharedDirector()->end();
	CCDirector::sharedDirector()->mainLoop();
}

Cocos2dxView::Cocos2dxView(QWidget *parent) : QWidget(parent)
{
	ModeStateX->setPrimaryMode(ModeState::NoMode);
	initCocos2dx();
	m_mouseSprite = NULL;
	m_curPolyOper = NULL;
	return;
}

void Cocos2dxView::initCocos2dx()
{
	HWND hwnd = (HWND)this->winId();
	
	CCEGLView *eglView = CCEGLView::sharedOpenGLView(hwnd);
	eglView->setViewName("cocos2dx_view");
	eglView->setFrameSize(defaultSizeX, defaultSizeY);
	this->setGeometry(0, 0, defaultSizeX, defaultSizeY);

	CCApplication::sharedApplication()->run(false);

	cocos2d::CCScene *sc = CCDirector::sharedDirector()->getRunningScene();
	//������һ֡����֤��ǰ���г�����Ϊ��
	CCDirector::sharedDirector()->mainLoop();
	
	m_timer.setParent(this);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(renderCocos2dx()));
	m_timer.start();
}

void Cocos2dxView::setViewGeometry(const QRect &rect)
{
	setGeometry(rect);
	CCEGLView::sharedOpenGLView()->setFrameSize(rect.width(), rect.height());
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(rect.width(), rect.height(), kResolutionShowAll);
}

void Cocos2dxView::renderCocos2dx()
{
	CCApplication::sharedApplication()->renderWorld();
}

void Cocos2dxView::enterEvent(QEvent *event)
{
	//���ģʽ�º���
	if (ModeStateX->getPrimaryMode() == ModeState::BrowseMode)	return;
	if ((ModeStateX->getPrimaryMode() == ModeState::EditMode) && (ModeStateX->getSubMode() == ModeState::CommonEdit))
	{
		attachSpriteToMouse();
	}
	qDebug("enter");
	return;
	cocos2d::CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
	
	if (scene)
	{
		QRect r = this->geometry();
		m_mouseSprite = cocos2d::CCSprite::create("CloseNormal.png");
		QPoint	pos = mapFromGlobal(QCursor::pos());
		m_mouseSprite->setPosition(ccp(pos.rx(), r.height() - pos.ry()));
		scene->addChild(m_mouseSprite);
	
		m_mouseTimer.setParent(this);
		connect(&m_mouseTimer, SIGNAL(timeout()), this, SLOT(mouseMoveInView()));
		
		m_mouseTimer.start(20);
	}
}

void Cocos2dxView::attachSpriteToMouse()
{
	ListWidgetItem *item = (ListWidgetItem*)m_listwidget->currentItem();
	if (!item) return;
	EditorScene *scene = getEditorScene();
	if (scene)
	{
		QRect r = this->geometry();
		if (!m_mouseSprite)
		{
			m_mouseSprite = CCSprite::create(item->getAbsoluteFilePath().toLatin1().data());
			m_mouseSprite->setOpacity(128);
			scene->addChild(m_mouseSprite);
		}
		QPoint	pos = mapFromGlobal(QCursor::pos());
		m_mouseSprite->setPosition(ccp(pos.rx(), r.height() - pos.ry()));	//qt��ui y������ת��Ϊopengl��y������
//		CCLOG("add");

		m_mouseTimer.setParent(this);
		connect(&m_mouseTimer, SIGNAL(timeout()), this, SLOT(mouseMoveInView()));
		
		m_mouseTimer.start(20);
	}
}

void Cocos2dxView::leaveEvent(QEvent *event)
{
	if (ModeStateX->getPrimaryMode() == ModeState::BrowseMode)	return;
	qDebug("leave");
	m_mouseTimer.stop();
	if ((ModeStateX->getPrimaryMode() == ModeState::EditMode) && (ModeStateX->getSubMode() == ModeState::CommonEdit))
	{
		if (m_mouseSprite)
		{
//			CCLOG("remove");
			m_mouseSprite->removeFromParent();
			m_mouseSprite = NULL;
		}
	}
}

void Cocos2dxView::mouseMoveInView()
{
	//QRect r = this->geometry();
	//QPoint	pos = mapFromGlobal(QCursor::pos());
	//m_mouseSprite->setPosition(ccp(pos.rx(), r.height() - pos.ry()));
	m_mouseSprite->setPosition(convertToOpenglPoint(QCursor::pos()));
}

void Cocos2dxView::mousePressEvent(QMouseEvent *event) 
{
	CCLOG("press");
	if (event->button() == Qt::LeftButton)
	{
		if (ModeStateX->getPrimaryMode() == ModeState::BrowseMode)
		{
			mousePressInBrowse(event);
		}
		else if (ModeStateX->getPrimaryMode() == ModeState::EditMode)
		{
			switch (ModeStateX->getSubMode())
			{
			case ModeState::CircleEdit: mousePressInCircleEdit(event); break;
			case ModeState::PolygonEdit:mousePressInPolygonEdit(event); break;
			case ModeState::CommonEdit: mousePressInCommonEdit(event); break;
			case ModeState::ChoiceEdit: mousePressInChoiceEdit(event); break;
			default: assert(false); break;
			}
		}
	}
	return;

//	CCLOG("begin");
	QPointF	pos = event->localPos();
//	CCLOG("%lf %lf", pos.rx(), pos.ry());
	//��װwindows��Ϣ
	UINT message  = WM_LBUTTONDOWN;
	WPARAM wparam = MK_LBUTTON;
	LPARAM lparam = MAKELPARAM(pos.rx(), pos.ry());
	CCEGLView::sharedOpenGLView()->WindowProc(message, wparam, lparam);
}

void Cocos2dxView::mouseMoveEvent(QMouseEvent *event) 
{
//	CCLOG("move");
	if (event->buttons() & Qt::LeftButton)
	{
		if (ModeStateX->getPrimaryMode() == ModeState::BrowseMode)
		{
			mouseMoveInBrowse(event);
		}
		else if (ModeStateX->getPrimaryMode() == ModeState::EditMode)
		{
			switch (ModeStateX->getSubMode())
			{
			case ModeState::CircleEdit: mouseMoveInCircleEdit(event); break;
			case ModeState::PolygonEdit:mouseMoveInPolygonEdit(event); break;
			case ModeState::CommonEdit: mouseMoveInCommonEdit(event); break;
			case ModeState::ChoiceEdit: mouseMoveInChoiceEdit(event); break;
			default: assert(false); break;
			}
		}
	}
	return;

//	CCLOG("move");
	QPointF	pos = event->localPos();
//	CCLOG("qt %lf %lf", pos.rx(), pos.ry());
	//��װwindows��Ϣ
	UINT  message = WM_MOUSEMOVE;
	WPARAM wparam = MK_LBUTTON;
	LPARAM lparam = MAKELPARAM(pos.rx(), pos.ry());
	CCEGLView::sharedOpenGLView()->WindowProc(message, wparam, lparam);
}

void Cocos2dxView::mouseReleaseEvent(QMouseEvent *event) 
{
	CCLOG("release");
	if (event->button() == Qt::LeftButton)
	{
		if (ModeStateX->getPrimaryMode() == ModeState::BrowseMode)
		{
			mouseReleaseInBrowse(event);
		}
		else if (ModeStateX->getPrimaryMode() == ModeState::EditMode)
		{
			switch (ModeStateX->getSubMode())
			{
			case ModeState::CircleEdit: mouseReleaseInCircleEdit(event); break;
			case ModeState::PolygonEdit:mouseReleaseInPolygonEdit(event); break;
			case ModeState::CommonEdit: mouseReleaseInCommonEdit(event); break;
			case ModeState::ChoiceEdit: mouseReleaseInChoiceEdit(event); break;
			default: assert(false); break;
			}
		}
	}
	return;

//	CCLOG("end");
	QPointF	pos = event->localPos();
	//CCLOG("%lf %lf", pos.rx(), pos.ry());
	//��װwindows��Ϣ
	UINT message  = WM_LBUTTONUP;
	WPARAM wparam = MK_LBUTTON;
	LPARAM lparam = MAKELPARAM(pos.rx(), pos.ry());
	CCEGLView::sharedOpenGLView()->WindowProc(message, wparam, lparam);
}

void Cocos2dxView::setBackground(QString filename)
{
	getEditorScene()->getBackground()->removeFromParent();
	cocos2d::CCSprite *spr = cocos2d::CCSprite::create(filename.toLatin1().data());
	getEditorScene()->setBackground(spr);
	getEditorScene()->addChild(spr,-1);
//	getEditorScene()->getBackground()->setTexture(CCTextureCache::sharedTextureCache()->addImage(filename.toLatin1().data()));
}

//���ģʽ
void Cocos2dxView::mousePressInBrowse(QMouseEvent *event)
{
	qDebug("press browse");
}
void Cocos2dxView::mouseMoveInBrowse(QMouseEvent *event)
{
	qDebug("move browse");
}
void Cocos2dxView::mouseReleaseInBrowse(QMouseEvent *event)
{
	qDebug("release browse");
}

//Բ�α༭ģʽ
void Cocos2dxView::mousePressInCircleEdit(QMouseEvent *event)
{
	qDebug("press circle");
	m_curCircleOper = new CircleEditOper(getEditorScene()->getObjectLayer(), convertToOpenglPoint(QCursor::pos()));
	OperationManageX->exec(m_curCircleOper);
}
void Cocos2dxView::mouseMoveInCircleEdit(QMouseEvent *event)
{
	qDebug("move circle");
	CCPoint cur_pos = convertToOpenglPoint(QCursor::pos());
	float distance = cur_pos.getDistance(m_curCircleOper->getCenterPoint());
	m_curCircleOper->setRadius(distance + 10);
}
void Cocos2dxView::mouseReleaseInCircleEdit(QMouseEvent *event)
{
	qDebug("release circle");
	m_curCircleOper->getCircleObject()->drawCircle(false);
	m_curCircleOper = NULL;
}

//����α༭ģʽ
void Cocos2dxView::mousePressInPolygonEdit(QMouseEvent *event)
{
	qDebug("press polygon1");
	QPointF pos = event->localPos();

	EditorScene *scene = getEditorScene();
	//ִ�в���
	if (m_curPolyOper == NULL)
	{
		m_curPolyOper = new PolygonEditOper(scene->getObjectLayer());
		OperationManageX->exec(m_curPolyOper);
	}
	assert(m_curPolyOper);
	m_curPolyOper->pushPoint(convertToOpenglPoint(QCursor::pos()));
}
void Cocos2dxView::mouseMoveInPolygonEdit(QMouseEvent *event)
{
	qDebug("press polygon2");
	m_curPolyOper->popPoint();
	m_curPolyOper->pushPoint(convertToOpenglPoint(QCursor::pos()));
}
void Cocos2dxView::mouseReleaseInPolygonEdit(QMouseEvent *event)
{
	qDebug("press polygon3");
	//�����ƿ��ĵĶ����
	m_curPolyOper->getPolyObject()->drawPolygon(false);
}

//��ͨ�༭ģʽ
void Cocos2dxView::mousePressInCommonEdit(QMouseEvent *event)
{
	m_commonPos = event->localPos();
}
void Cocos2dxView::mouseMoveInCommonEdit(QMouseEvent *event)
{
}
void Cocos2dxView::mouseReleaseInCommonEdit(QMouseEvent *event)
{
	if (m_commonPos != event->localPos())
		return;

	ListWidgetItem *item = (ListWidgetItem*)m_listwidget->currentItem();
	if (!item) return;
	EditorScene *scene = getEditorScene();

	//ִ�в���
	std::string filename = std::string(item->getAbsoluteFilePath().toLatin1().data());
	OperationManageX->exec(new CommonEditOper(scene->getObjectLayer(), convertToOpenglPoint(QCursor::pos()), filename));

	//CommonObject *object = CommonObject::create(CCSprite::create(item->getAbsoluteFilePath().toLatin1().data()));
	//object->setPosition(convertToOpenglPoint(QCursor::pos()));
	//scene->addChild(object);
}

//ѡȡ�༭ģʽ
void Cocos2dxView::mousePressInChoiceEdit(QMouseEvent *event)
{
	QPointF	pos = event->localPos();
	//��װwindows��Ϣ
	UINT message  = WM_LBUTTONDOWN;
	WPARAM wparam = MK_LBUTTON;
	LPARAM lparam = MAKELPARAM(pos.rx(), pos.ry());
	CCEGLView::sharedOpenGLView()->WindowProc(message, wparam, lparam);
}

void Cocos2dxView::mouseMoveInChoiceEdit(QMouseEvent *event)
{
	QPointF	pos = event->localPos();
	//��װwindows��Ϣ
	UINT  message = WM_MOUSEMOVE;
	WPARAM wparam = MK_LBUTTON;
	LPARAM lparam = MAKELPARAM(pos.rx(), pos.ry());
	CCEGLView::sharedOpenGLView()->WindowProc(message, wparam, lparam);
}
void Cocos2dxView::mouseReleaseInChoiceEdit(QMouseEvent *event)
{
	QPointF	pos = event->localPos();
	//��װwindows��Ϣ
	UINT message  = WM_LBUTTONUP;
	WPARAM wparam = MK_LBUTTON;
	LPARAM lparam = MAKELPARAM(pos.rx(), pos.ry());
	CCEGLView::sharedOpenGLView()->WindowProc(message, wparam, lparam);
}

void Cocos2dxView::contextMenuEvent( QContextMenuEvent * event)
{
	if (ModeStateX->getPrimaryMode() != ModeState::EditMode || ModeStateX->getSubMode() != ModeState::ChoiceEdit)
		return;

	m_choicedObj = getEditorScene()->getObjectInPoint(convertToOpenglPoint(QCursor::pos()));
	if (!m_choicedObj) return;

    QMenu *popMenu = new QMenu(this);
	QAction *del = new QAction(QIcon(":/images/delete.png"), QStringLiteral("ɾ���˶���"), this);
	connect(del, SIGNAL(triggered()), this, SLOT(delChoiceObject()));
	popMenu->addAction(del);
    popMenu->exec(QCursor::pos()); // �˵����ֵ�λ��Ϊ��ǰ����λ��
}

void Cocos2dxView::delChoiceObject()
{
	OperationManageX->undo(m_choicedObj);
	m_choicedObj = NULL;
}
