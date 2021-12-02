#include "BaseDialog.h"
#include "QBoxLayout"
#include "DialogTitleFrame.h"
#include "qevent.h"

/// <summary>
/// ���캯��
/// </summary>
/// <param name="title">�����ı�</param>
/// <param name="parent">������ָ��</param>
BaseDialog::BaseDialog(const QString& title, QWidget *parent) : QDialog(parent),titleContent_(title)
{
	InitUI();
}

void BaseDialog::onClose()
{
	reject();
}
/// <summary>
/// �¼�����������Ҫ�����϶��������ƶ�����
/// </summary>
/// <param name="watched"></param>
/// <param name="event"></param>
/// <returns></returns>
bool BaseDialog::eventFilter(QObject* watched, QEvent* event)
{
	static QPoint mousePoint;
	static bool mousePressed = false;

	QMouseEvent* mouseEvent = reinterpret_cast<QMouseEvent*>(event);
	if (mouseEvent->type() == QEvent::MouseButtonPress) {
		if (mouseEvent->button() == Qt::LeftButton) {
			mousePressed = true;
			mousePoint = mouseEvent->globalPos() - this->pos();
		}
	}
	else if (mouseEvent->type() == QEvent::MouseButtonRelease) {
		mousePressed = false;
	}
	else if (mouseEvent->type() == QEvent::MouseMove) {
		if (mousePressed && (mouseEvent->buttons() && Qt::LeftButton)) {
			//if (this->property("canMove").toBool()) 
			{
				this->move(mouseEvent->globalPos() - mousePoint);
			}
		}
	}

	return __super::eventFilter(watched, event);
}

/// <summary>
/// ��ʼ��������Ϣ
/// </summary>
void BaseDialog::InitUI()
{
	setWindowFlags(Qt::FramelessWindowHint);
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->setSpacing(0);
	mainLayout->setMargin(0);
	this->setLayout(mainLayout);

	titleFrame_ = new DialogTitleFrame(titleContent_,this);
	titleFrame_->setFrameShape(QFrame::Shape::NoFrame);
	titleFrame_->setObjectName("titleFrame");
	connect(titleFrame_, &DialogTitleFrame::closeSgn, this, &BaseDialog::onClose);
	mainLayout->addWidget(titleFrame_);
	titleFrame_->installEventFilter(this);
}
