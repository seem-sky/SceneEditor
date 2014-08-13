#include "Cocos2dxView.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Classes/JsonX.h"
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
	m_choicedObj = NULL;
	m_bkFileName = "HelloWorld.png";
	m_isPretty = true;
	m_isInView = false;

	QTimer *time = new QTimer(this);
	connect(time, SIGNAL(timeout()), this, SLOT(mouseMovePosition()));
	time->start();

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
	m_isInView = true;
	//���ģʽ�º���
	if (ModeStateX->getPrimaryMode() == ModeState::BrowseMode)	return;
	if ((ModeStateX->getPrimaryMode() == ModeState::EditMode) && (ModeStateX->getSubMode() == ModeState::CommonEdit))
	{
		attachSpriteToMouse();
	}
	qDebug("enter");
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

		m_mouseTimer.setParent(this);
		connect(&m_mouseTimer, SIGNAL(timeout()), this, SLOT(mouseMoveInView()));
		
		m_mouseTimer.start(20);
	}
}

void Cocos2dxView::leaveEvent(QEvent *event)
{
	m_isInView = false;
	if (ModeStateX->getPrimaryMode() == ModeState::BrowseMode)	return;
	qDebug("leave");
	m_mouseTimer.stop();
	if ((ModeStateX->getPrimaryMode() == ModeState::EditMode) && (ModeStateX->getSubMode() == ModeState::CommonEdit))
	{
		if (m_mouseSprite)
		{
			m_mouseSprite->removeFromParent();
			m_mouseSprite = NULL;
		}
	}
}

void Cocos2dxView::mouseMoveInView()
{
	m_mouseSprite->setPosition(convertToOpenglPoint(QCursor::pos()));
}

void Cocos2dxView::mouseMovePosition()
{
	if (m_isInView)
	{
		cocos2d::CCPoint p = convertToOpenglPoint(QCursor::pos());
		((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->lineEditPosition->setText(
			QString().sprintf("x:%.0f,y:%.0f", p.x, p.y));
	}
}

void Cocos2dxView::mousePressEvent(QMouseEvent *event) 
{
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
}

void Cocos2dxView::mouseMoveEvent(QMouseEvent *event) 
{
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
}

void Cocos2dxView::mouseReleaseEvent(QMouseEvent *event) 
{
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
}

void Cocos2dxView::setBackground(QString filename)
{
	getEditorScene()->getBackground()->removeFromParent();
	cocos2d::CCSprite *spr = cocos2d::CCSprite::create(filename.toLatin1().data());
	getEditorScene()->setBackground(spr);
	getEditorScene()->addChild(spr,-1);

	QFileInfo file(filename);
	setBackgroundFileName(std::string(file.fileName().toLatin1().data()));
}

//���ģʽ
void Cocos2dxView::mousePressInBrowse(QMouseEvent *event)
{
	qDebug("press browse");
	m_prePosition = event->localPos();
}
void Cocos2dxView::mouseMoveInBrowse(QMouseEvent *event)
{
	qDebug("move browse");
	QPointF curPos = event->localPos();
	QPointF diff = curPos - m_prePosition;
	qDebug("diff: %f %f", diff.x(), diff.y());
	QScrollArea *scroll = (QScrollArea*)this->parent()->parent();

	scroll->horizontalScrollBar()->setValue(scroll->horizontalScrollBar()->value() - diff.x());
	scroll->verticalScrollBar()->setValue(scroll->verticalScrollBar()->value() - diff.y());

	m_prePosition = curPos;
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
	std::string tpname = std::string(item->text().toLatin1().data());
	std::string filename = std::string(item->getAbsoluteFilePath().toLatin1().data());
	OperationManageX->exec(new CommonEditOper(tpname, scene->getObjectLayer(), 
									convertToOpenglPoint(QCursor::pos()), item->file_info));

}

void Cocos2dxView::stopAllBlink()
{
	CCObject *obj;
	CCARRAY_FOREACH(getEditorScene()->getObjectLayer()->getChildren(), obj)
		((BaseObject*)obj)->setBlink(false);
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

	stopAllBlink();
	m_choicedObj = getEditorScene()->getObjectInPoint(convertToOpenglPoint(QCursor::pos()));
	if (!m_choicedObj) return;
	m_choicedObj->setBlink(true);

	//qDebug("test:%s ", this->parent()->parent()->parent()->parent()->objectName().toLatin1().data());
	//��qtdesigner�������ĵ��۵ĸ��ӹ�ϵ
	//���ö���λ��
	bool disable = true;
	CCPoint point= ccp(0,0);
	if (m_choicedObj->getObjectType() == ObjectType::COMMON_OBJECT)
	{
		point = m_choicedObj->getPosition();
		disable = false;
	}
	((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->posxSpinBox->setDisabled(disable);
	((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->posySpinBox->setDisabled(disable);
	((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->posxSpinBox->setValue(point.x);
	((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->posySpinBox->setValue(point.y);

	//������ת�Ƕȿؼ�ֵ
	((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->rotateSpinBox->setValue(m_choicedObj->getRotation());
	//�������Ŵ�С�ؼ�ֵ
	((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->scaleSpinBox->setValue(m_choicedObj->getScale());
	//���ö���������
	((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->typeName->setText(m_choicedObj->getTypeName().c_str());
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
	if (m_choicedObj && m_choicedObj->getObjectType()==ObjectType::COMMON_OBJECT)
	{
		CCPoint point = m_choicedObj->getPosition();
		((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->posxSpinBox->setValue(point.x);
		((MainWindow*)(this->parent()->parent()->parent()->parent()))->ui->posySpinBox->setValue(point.y);
	}

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

	QAction *setting = new QAction(QIcon(":/images/setting.png"), QStringLiteral("�����ض���������"), this);
    connect(setting, SIGNAL(triggered()), this, SLOT(setObjectAttribute()));
    popMenu->addAction(setting);

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

void Cocos2dxView::setObjectAttribute()
{
    Dialog *d = new Dialog(&m_choicedObj->getObjectAttribute(), this);
    d->exec();
}

void Cocos2dxView::saveObjectData(JsonX &data, BaseObject *object)
{
	std::string tmp = object->getTypeName();
	const char *type_name = tmp.c_str();
	ObjectType type = object->getObjectType();

	if (!data.has(type_name))
		data.insertArray(type_name);

	//�洢ͬһ���Ͷ����ͨ�����Ե��������
	rapidjson::Value &arr = data[type_name];
	if (type == ObjectType::COMMON_OBJECT)
	{
		if (!getObjectAttr(type_name, object->getFileName())) return;

		rapidjson::StringBuffer buf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
		getObjectAttr(type_name, object->getFileName())->getDocument().Accept(writer);
		rapidjson::Document attr(&data.getAllocator());
		attr.Parse<0>(buf.GetString());

		rapidjson::Value v(object->getFileName().c_str(), data.getAllocator());
		attr.AddMember("filename", v, data.getAllocator());

		arr.PushBack((rapidjson::Value&)attr, data.getAllocator());
	}
	else if (type == ObjectType::CIRCLE_OBJECT)
	{
		CircleObject *circle = (CircleObject *)object;
		rapidjson::Document attr(&data.getAllocator()); 
		attr.SetObject();
		rapidjson::Value point(rapidjson::kObjectType);
		point.AddMember("x", circle->getCenterPoint().x, data.getAllocator());
		point.AddMember("y", circle->getCenterPoint().y, data.getAllocator());
		attr.AddMember("center", point, data.getAllocator());
		attr.AddMember("radius", circle->getRadius(), data.getAllocator());

		arr.PushBack((rapidjson::Value&)attr, data.getAllocator());
	}
	else if (type == ObjectType::POLYGON_OBJECT)
	{
		PolygonObject *polygon = (PolygonObject*)object;
		rapidjson::Document attr(&data.getAllocator());
		attr.SetObject();

		//�洢��״�㼯
		rapidjson::Value shape_arr(rapidjson::kArrayType);//shape_arr.SetArray()
		for (auto pos : polygon->getPolyPoints())
		{
			rapidjson::Value point(rapidjson::kObjectType);
			point.AddMember("x", pos.x, data.getAllocator());
			point.AddMember("y", pos.y, data.getAllocator());
			shape_arr.PushBack(point, data.getAllocator());
		}
		attr.AddMember("shape", shape_arr, data.getAllocator());

		arr.PushBack((rapidjson::Value&)attr, data.getAllocator());
	}

	//���������ض�����
	rapidjson::Value &attr = (rapidjson::Value&)object->getObjectAttribute();
	for (auto iter = attr.MemberonBegin(); iter != attr.MemberonEnd(); iter++)
	{
		rapidjson::Value k(iter->name.GetString(), data.getAllocator());
		rapidjson::Value v(iter->value.GetString(), data.getAllocator());
		arr[arr.Size() - 1].AddMember(k, v, data.getAllocator());
	}

	//������󳣹�����
	//rapidjson::Value &arr = data[type_name];
	rapidjson::Value v(rapidjson::kObjectType);
	v.AddMember("x", object->getPositionX(), data.getAllocator());
	v.AddMember("y", object->getPositionY(), data.getAllocator());
	arr[arr.Size() - 1].AddMember("position", v, data.getAllocator());

	rapidjson::Value sv(rapidjson::kObjectType);
	sv.AddMember("width", object->getSprite()->getContentSize().width, data.getAllocator());
	sv.AddMember("height", object->getSprite()->getContentSize().height, data.getAllocator());
	arr[arr.Size() - 1].AddMember("size", sv, data.getAllocator());

	arr[arr.Size() - 1].AddMember("rotate", object->getRotation(), data.getAllocator());
	arr[arr.Size() - 1].AddMember("scale", object->getScale(), data.getAllocator());
}

bool Cocos2dxView::saveDataToFile(QString filepath)
{
	JsonX data;
	CCArray *object_arr = getEditorScene()->getObjectLayer()->getChildren();
	CCObject *_obj;
	CCARRAY_FOREACH(object_arr, _obj)
	{
		BaseObject *object = (BaseObject*)_obj;
		saveObjectData(data, object);
	}
	data.insert("background_filename", m_bkFileName.c_str());
	data.saveToFile(filepath.toLatin1().data(), getIsPretty());
	return true;
}

JsonX* Cocos2dxView::getObjectAttr(std::string type_name, std::string filename)
{
	//����֧����ͬ�ľ������ͣ�������Ҫ�þ���ľ����ļ�������λָ�������Խṹ
	QList<QListWidgetItem *> res = m_listwidget->findItems(type_name.c_str(),Qt::MatchCaseSensitive);
	if (res.length() <= 0) return NULL;
	for (QListWidgetItem *iter : res)
	{
		ListWidgetItem *item = (ListWidgetItem*)iter;//res.at(0);

		std::string str = std::string(item->getFileName().toLatin1().data());
		if (str == filename)
			return &(item->m_attr);
	}
	return NULL;
}

