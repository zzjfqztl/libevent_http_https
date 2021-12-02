#include "DialogTitleFrame.h"
#include "QBoxLayout"
#include "QLabel"
#include "QPushButton"

/// <summary>
/// ���캯��
/// </summary>
/// <param name="title">��������</param>
/// <param name="parent">������ָ��</param>
/// <param name="align">�����Ƿ���б�ʾ</param>
DialogTitleFrame::DialogTitleFrame(const QString& title, QWidget* parent, Qt::Alignment align) : QFrame(parent),titleContent_(title), titleAlign_(align)
{
	initUI();
}

/// <summary>
/// ���ñ�������
/// </summary>
/// <param name="title"></param>
void DialogTitleFrame::setTitle(const QString& title)
{
	titleContent_ = title;
	titleLabel_->setText(titleContent_);
}

/// <summary>
/// �������ɲ���
/// </summary>
void DialogTitleFrame::initUI()
{
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setSpacing(0);
	hLayout->setMargin(0);

	setLayout(hLayout);
	if (titleAlign_ == Qt::AlignCenter)
	{
		hLayout->addStretch();
	}

	titleLabel_ = new QLabel(this);
	titleLabel_->setObjectName("titleLabel");
	titleLabel_->setText(titleContent_);
	hLayout->addWidget(titleLabel_);
	hLayout->addStretch();

	btnClose_ = new QPushButton(this);
	btnClose_->setObjectName("btnClose");
	btnClose_->setToolTip(QObject::tr("Close"));
	btnClose_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	connect(btnClose_, &QPushButton::clicked, this,&DialogTitleFrame::closeSgn);
	hLayout->addWidget(btnClose_);
	hLayout->addSpacing(10);
}
