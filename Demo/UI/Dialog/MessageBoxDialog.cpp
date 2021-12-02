#include "MessageBoxDialog.h"
#include "QBoxLayout"
#include "QFrame"
#include "QLabel"
#include "QPushButton"
#include "QTextEdit"
#include "qevent.h"



MessageBoxDialog::MessageBoxDialog(const QString& title, const QString& text, MessageButtonType buttons /*= MB_NONE*/, QWidget* parent /*= nullptr*/)
	:titleContent(title),msgContent(text),messageButtonType_(buttons),QDialog(parent)
{
	initUI();
}

void MessageBoxDialog::initUI()
{
	setWindowFlags(Qt::FramelessWindowHint);
	setObjectName("messageBoxDialog");

	mainLayout_ = new QVBoxLayout();
	mainLayout_->setSpacing(0);
	mainLayout_->setMargin(0);
	this->setLayout(mainLayout_);

	initTitleFrame();
	initCenterFrame();
	initBottomFrame();
}

void MessageBoxDialog::initTitleFrame()
{
	titleFrame_ = new QFrame(this);
	titleFrame_->setFrameShape(QFrame::Shape::NoFrame);
	titleFrame_->setObjectName("titleFrame");

	titleFrame_->installEventFilter(this);

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setSpacing(0);
	hLayout->setMargin(0);

	titleFrame_->setLayout(hLayout);

	hLayout->addStretch();
	titleLabel_ = new QLabel(titleFrame_);
	titleLabel_->setObjectName("titleLabel");
	titleLabel_->setText(titleContent);
	hLayout->addWidget(titleLabel_);
	hLayout->addStretch();

	btnClose_ = new QPushButton(titleFrame_);
	btnClose_->setObjectName("btnClose");
	btnClose_->setToolTip(QObject::tr("Close"));
	btnClose_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	connect(btnClose_, &QPushButton::clicked, this, &MessageBoxDialog::onClose);
	hLayout->addWidget(btnClose_);
	hLayout->addSpacing(10);

	mainLayout_->addWidget(titleFrame_);
}


void MessageBoxDialog::initCenterFrame()
{
	centerFrame_ = new QFrame(this);
	centerFrame_->setFrameShape(QFrame::Shape::NoFrame);
	centerFrame_->setObjectName("centerFrame");

	centerLayout_ = new QVBoxLayout();
	centerLayout_->setSpacing(10);
	centerLayout_->setMargin(0);
	centerLayout_->setContentsMargins(6, 6, 6, 10);
	centerFrame_->setLayout(centerLayout_);

	messageInfoFrame_ = new QFrame(centerFrame_);
	messageInfoFrame_->setFrameShape(QFrame::Shape::NoFrame);
	messageInfoFrame_->setObjectName("messageInfoFrame");

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setSpacing(0);
	vLayout->setMargin(0);
	vLayout->setContentsMargins(20, 20, 20, 20);
	messageInfoFrame_->setLayout(vLayout);

	messageTextEdit_ = new QTextEdit(messageInfoFrame_);
	messageTextEdit_->setObjectName("messageTextEdit");
	messageTextEdit_->setReadOnly(true);
	messageTextEdit_->append(msgContent);
	vLayout->addWidget(messageTextEdit_);

	centerLayout_->addWidget(messageInfoFrame_);

	mainLayout_->addWidget(centerFrame_);
}

void MessageBoxDialog::initBottomFrame()
{
	bottomFrame_ = new QFrame(centerFrame_);
	bottomFrame_->setFrameShape(QFrame::Shape::NoFrame);
	bottomFrame_->setObjectName("bottomFrame");

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setMargin(0);
	bottomFrame_->setLayout(hLayout);
	hLayout->addStretch();
	if (messageButtonType_ == MB_OK || messageButtonType_ == MB_OKCANCEL)
	{
		btnOK_ = new QPushButton(bottomFrame_);
		btnOK_->setObjectName("btnOK");
		btnOK_->setText(QObject::tr("OK"));
		btnOK_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
		connect(btnOK_, &QPushButton::clicked, this, &MessageBoxDialog::onOK);
		hLayout->addWidget(btnOK_);
	}
	
	if (messageButtonType_ == MB_OKCANCEL)
	{
		btnCancel_ = new QPushButton(centerFrame_);
		btnCancel_->setObjectName("btnCancel");
		btnCancel_->setText(QObject::tr("Cancel"));
		btnCancel_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
		connect(btnCancel_, &QPushButton::clicked, this, &MessageBoxDialog::onClose);

		hLayout->addWidget(btnCancel_);
	}

	hLayout->addStretch();

	centerLayout_->addWidget(bottomFrame_);
}

void MessageBoxDialog::onOK()
{
	accept();
}

void MessageBoxDialog::onClose()
{
	reject();
}

bool MessageBoxDialog::eventFilter(QObject* watched, QEvent* event)
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
