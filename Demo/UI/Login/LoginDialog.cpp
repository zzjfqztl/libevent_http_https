#include "LoginDialog.h"
#include "QBoxLayout"
#include "QFrame"
#include "QLabel"
#include <QObject>
#include "QPushButton"
#include <QLineEdit>
#include <QCheckBox>
#include "Common/FramelessWidget.h"
#include "qevent.h"
#include <QComboBox>
#include <QListView>
#include <QTextEdit>
#include "Dialog/MessageBoxDialog.h"
#include "Network/NetworkManager.h"
#include "Network/HttpClient.h"
#include <QDebug>
#include "Network/JsonManager.h"

#if _MSC_VER >= 1600   // 保证vs代码不乱码和显示不乱码
#pragma execution_character_set("utf-8")
#endif

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
	initUI();
}

LoginDialog::~LoginDialog()
{

}

void LoginDialog::initUI()
{
	setWindowFlags(Qt::FramelessWindowHint);
	setObjectName("loginDialog");
	setWindowIcon(QIcon(":/icon/Common/logo.png"));
	setWindowTitle("Login");
	mainLayout_ = new QVBoxLayout();
	mainLayout_->setSpacing(0);
	mainLayout_->setMargin(0);
	this->setLayout(mainLayout_);

	initTitleFrame();
	initCenterFrame();
	initConnect();
	this->setFixedHeight(200);
}

void LoginDialog::initTitleFrame()
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
	titleLabel_->setText(QObject::tr("RunYun CAD"));
	hLayout->addWidget(titleLabel_);
	hLayout->addStretch();

	btnClose_ = new QPushButton(titleFrame_);
	btnClose_->setObjectName("btnClose");
	btnClose_->setToolTip(QObject::tr("Close"));
	btnClose_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	hLayout->addWidget(btnClose_);
	hLayout->addSpacing(10);

	mainLayout_->addWidget(titleFrame_);
}

void LoginDialog::initConnect()
{
	connect(btnClose_, &QPushButton::clicked, this, &LoginDialog::onClose);
	connect(btnUpDown_, &QPushButton::clicked, this, &LoginDialog::onUpDown);
	connect(btnLogin_, &QPushButton::clicked, this, &LoginDialog::onLogin);
	connect(btnCancel_, &QPushButton::clicked, this, &LoginDialog::onClose);

	connect(ckbFirewall_, &QCheckBox::clicked, this, &LoginDialog::onCheckFirewall);
	connect(ckbProxy_, &QPushButton::clicked, this, &LoginDialog::onCheckProxy);
	connect(ckbOffline_, &QPushButton::clicked, this, &LoginDialog::onCheckOffline);
}

void LoginDialog::initCenterFrame()
{
	centerFrame_ = new QFrame(this);
	centerFrame_->setFrameShape(QFrame::Shape::NoFrame);
	centerFrame_->setObjectName("centerFrame");

	centerLayout_ = new QVBoxLayout();
	centerLayout_->setSpacing(10);
	centerLayout_->setMargin(0);
	centerLayout_->setContentsMargins(20, 20, 20, 20);
	centerFrame_->setLayout(centerLayout_);

	initLoginInfo();

	initExtensionFrame();
	centerLayout_->addStretch();
	initLoginBottom();
	mainLayout_->addWidget(centerFrame_);
}

void LoginDialog::initLoginInfo()
{
	QFrame* LoginInfoFrame = new QFrame(centerFrame_);
	LoginInfoFrame->setFrameShape(QFrame::Shape::NoFrame);
	LoginInfoFrame->setObjectName("LoginInfoFrame");

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setSpacing(2);
	vLayout->setMargin(0);
	vLayout->setContentsMargins(10, 0, 10, 0);
	LoginInfoFrame->setLayout(vLayout);

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setMargin(0);

	QLabel* lb = new QLabel(LoginInfoFrame);
	lb->setObjectName("idLabel");
	lb->setText(QObject::tr("ID"));

	hLayout->addWidget(lb);

	idLineEdit_ = new QLineEdit(LoginInfoFrame);
	idLineEdit_->setObjectName("idLineEdit");
	idLineEdit_->setPlaceholderText(QObject::tr("user ID"));
	idLineEdit_->clear();

	hLayout->addWidget(idLineEdit_);

	ckbSaveId_ = new QCheckBox(LoginInfoFrame);
	ckbSaveId_->setObjectName("ckbSaveId");
	ckbSaveId_->setText(QObject::tr("Save ID"));
	hLayout->addWidget(ckbSaveId_);

	vLayout->addLayout(hLayout);

	hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setMargin(0);

	lb = new QLabel(LoginInfoFrame);
	lb->setObjectName("pwdLabel");
	lb->setText(QObject::tr("Password"));

	hLayout->addWidget(lb);

	pwdLineEdit_ = new QLineEdit(LoginInfoFrame);
	pwdLineEdit_->setObjectName("pwdLineEdit");
	pwdLineEdit_->setPlaceholderText(QObject::tr("user Password"));
	pwdLineEdit_->setEchoMode(QLineEdit::Password);
	pwdLineEdit_->clear();

	hLayout->addWidget(pwdLineEdit_);

	ckbSavePwd_ = new QCheckBox(LoginInfoFrame);
	ckbSavePwd_->setObjectName("ckbSavePwd");
	ckbSavePwd_->setText(QObject::tr("Save PW"));
	hLayout->addWidget(ckbSavePwd_);

	vLayout->addLayout(hLayout);

	hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setMargin(0);

	lb = new QLabel(LoginInfoFrame);
	lb->setObjectName("noLabel");
	lb->setText(QObject::tr(""));

	hLayout->addWidget(lb);

	ckbAutoDeactivation_ = new QCheckBox(LoginInfoFrame);
	ckbAutoDeactivation_->setObjectName("ckbAutoDeactivation");
	ckbAutoDeactivation_->setText(QObject::tr("Auto Deactivation"));
	hLayout->addWidget(ckbAutoDeactivation_);
	hLayout->addStretch();

	vLayout->addLayout(hLayout);

	hLayout = new QHBoxLayout();
	hLayout->setSpacing(10);
	hLayout->setMargin(0);

	hLayout->addStretch();
	vLayout->addStretch();
	btnUpDown_ = new QPushButton(LoginInfoFrame);
	btnUpDown_->setObjectName("btnUpDown");
	btnUpDown_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	btnUpDown_->setProperty("state", "down");
	bShowExtension_ = false;
	hLayout->addWidget(btnUpDown_);
	vLayout->addLayout(hLayout);

	centerLayout_->addWidget(LoginInfoFrame);
}

void LoginDialog::initLoginBottom()
{

	QFrame* bottomFrame = new QFrame(centerFrame_);
	bottomFrame->setFrameShape(QFrame::Shape::NoFrame);
	bottomFrame->setObjectName("bottomFrame");

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setSpacing(0);
	hLayout->setMargin(0);
	bottomFrame->setLayout(hLayout);


	btnLogin_ = new QPushButton(bottomFrame);
	btnLogin_->setObjectName("btnLogin");
	btnLogin_->setText(QObject::tr("LOG IN"));
	btnLogin_->setFocus();    //设置默认焦点
	btnLogin_->setShortcut(QKeySequence::InsertParagraphSeparator);  //设置快捷键为键盘的“回车”键
	btnLogin_->setShortcut(Qt::Key_Enter);  //设置快捷键为enter键
	btnLogin_->setShortcut(Qt::Key_Return); //设置快捷键为小键盘上的enter键

	hLayout->addStretch();
	hLayout->addWidget(btnLogin_);
	hLayout->addSpacing(20);

	btnCancel_ = new QPushButton(bottomFrame);
	btnCancel_->setObjectName("btnCancel");
	btnCancel_->setText(QObject::tr("CANCEL"));
	btnCancel_->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	hLayout->addWidget(btnCancel_);
	hLayout->addStretch();

	centerLayout_->addWidget(bottomFrame);
}

void LoginDialog::initProxyFrame()
{
	proxyFrame_ = new QFrame(extensionFrame_);
	proxyFrame_->setFrameShape(QFrame::Shape::NoFrame);
	proxyFrame_->setObjectName("proxyFrame");

	QVBoxLayout* vProxyLayout = new QVBoxLayout();
	vProxyLayout->setSpacing(6);
	vProxyLayout->setMargin(0);
	vProxyLayout->setContentsMargins(20, 10, 10, 10);
	proxyFrame_->setLayout(vProxyLayout);

	QHBoxLayout* hlayout = new QHBoxLayout();
	hlayout->setSpacing(10);
	hlayout->setMargin(0);

	QLabel* lb = new QLabel(proxyFrame_);
	lb->setObjectName("proxyLabel");
	lb->setText(QObject::tr("Host Name"));

	hlayout->addWidget(lb);
	hostNameLineEdit_ = new QLineEdit(proxyFrame_);
	hostNameLineEdit_->setObjectName("hostNameLineEdit");
	hlayout->addWidget(hostNameLineEdit_);

	vProxyLayout->addLayout(hlayout);

	hlayout = new QHBoxLayout();
	hlayout->setSpacing(10);
	hlayout->setMargin(0);

	lb = new QLabel(proxyFrame_);
	lb->setObjectName("proxyLabel");
	lb->setText(QObject::tr("Proxy Type"));

	hlayout->addWidget(lb);
	proxyTypeComBobox_ = new QComboBox(proxyFrame_);
	proxyTypeComBobox_->setObjectName("proxyTypeComBobox");
	proxyTypeComBobox_->addItems(QStringList() << QObject::tr("HTTP") << QObject::tr("HTTP 1.0") << QObject::tr("SOCKS4") << QObject::tr("SOCKS5")
		<< QObject::tr("SOCKS4A") << QObject::tr("SOCKS5_HOSTNAME"));
	proxyTypeComBobox_->setView(new QListView());
	hlayout->addWidget(proxyTypeComBobox_);
	vProxyLayout->addLayout(hlayout);

	hlayout = new QHBoxLayout();
	hlayout->setSpacing(10);
	hlayout->setMargin(0);

	lb = new QLabel(proxyFrame_);
	lb->setObjectName("proxyLabel");
	lb->setText(QObject::tr("Port"));

	hlayout->addWidget(lb);
	portLineEdit_ = new QLineEdit(proxyFrame_);
	portLineEdit_->setObjectName("portLineEdit");
	hlayout->addWidget(portLineEdit_);

	vProxyLayout->addLayout(hlayout);

	hlayout = new QHBoxLayout();
	hlayout->setSpacing(10);
	hlayout->setMargin(0);

	lb = new QLabel(proxyFrame_);
	lb->setObjectName("proxyLabel");
	lb->setText(QObject::tr("ID"));

	hlayout->addWidget(lb);
	proxyIdLineEdit_ = new QLineEdit(proxyFrame_);
	proxyIdLineEdit_->setObjectName("proxyIdLineEdit");
	hlayout->addWidget(proxyIdLineEdit_);

	vProxyLayout->addLayout(hlayout);

	hlayout = new QHBoxLayout();
	hlayout->setSpacing(10);
	hlayout->setMargin(0);

	lb = new QLabel(proxyFrame_);
	lb->setObjectName("proxyLabel");
	lb->setText(QObject::tr("Password"));

	hlayout->addWidget(lb);
	proxyPwdLineEdit_ = new QLineEdit(proxyFrame_);
	proxyPwdLineEdit_->setObjectName("proxyPwdLineEdit");
	proxyPwdLineEdit_->setEchoMode(QLineEdit::Password);
	proxyPwdLineEdit_->clear();
	hlayout->addWidget(proxyPwdLineEdit_);

	vProxyLayout->addLayout(hlayout);
}

void LoginDialog::initOfflineFrame()
{
	offlineFrame_ = new QFrame(extensionFrame_);
	offlineFrame_->setObjectName("offlineFrame");
	offlineFrame_->setFrameShape(QFrame::Shape::NoFrame);

	QVBoxLayout* offlineLayout = new QVBoxLayout();
	offlineLayout->setSpacing(0);
	offlineLayout->setMargin(0);
	offlineLayout->setContentsMargins(20, 10, 20, 10);
	offlineFrame_->setLayout(offlineLayout);
	textEdit_ = new QTextEdit(offlineFrame_);
	textEdit_->setObjectName("textEdit");
	textEdit_->append(QObject::tr("If online authentication fails due to nework issues, the program can run with offline authentication. Please follow the steps below to obtain offline authentication;"));
	textEdit_->setReadOnly(true);
	offlineLayout->addWidget(textEdit_);
	offlineLayout->addSpacing(9);
	textEdit1_ = new QTextEdit(offlineFrame_);
	textEdit1_->setObjectName("textEdit");
	textEdit1_->append(QObject::tr("1. Click on the Create HW File button below to create a HW file. After saving the file, please send the file as an attachment to CLO Support Team."));
	textEdit1_->setReadOnly(true);
	offlineLayout->addWidget(textEdit1_);
	offlineLayout->addSpacing(9);
	textEdit2_ = new QTextEdit(offlineFrame_);
	textEdit2_->setObjectName("textEdit2");
	textEdit2_->append(QObject::tr("※ Please include your user ID in the file name and email."));
	textEdit2_->setReadOnly(true);
	offlineLayout->addWidget(textEdit2_);
	offlineLayout->addSpacing(9);

	QHBoxLayout* hhLayout = new QHBoxLayout();
	hhLayout->setSpacing(0);
	hhLayout->setMargin(0);

	hhLayout->addStretch();
	btnCreateHW_ = new QPushButton(offlineFrame_);
	btnCreateHW_->setObjectName("btnCreateHW");
	btnCreateHW_->setText(QObject::tr("Create HW File"));
	btnCreateHW_->setFocusPolicy(Qt::NoFocus);
	connect(btnCreateHW_, &QPushButton::clicked, this, &LoginDialog::onbtnCreateHWClick);
	hhLayout->addWidget(btnCreateHW_);
	hhLayout->addStretch();

	offlineLayout->addLayout(hhLayout);
	offlineLayout->addSpacing(9);
	textEdit3_ = new QTextEdit(offlineFrame_);
	textEdit3_->setObjectName("textEdit");
	textEdit3_->setReadOnly(true);
	textEdit3_->append(QObject::tr("2. After receiving the LCN file from the CLO Team, register the LCN file by using the button below, then click on LOG IN."));
	offlineLayout->addWidget(textEdit3_);
	offlineLayout->addSpacing(9);
	hhLayout = new QHBoxLayout();
	hhLayout->setSpacing(0);
	hhLayout->setMargin(0);

	hhLayout->addStretch();
	btnRegisterLCN_ = new QPushButton(offlineFrame_);
	btnRegisterLCN_->setObjectName("btnRegisterLCN");
	btnRegisterLCN_->setText(QObject::tr("Register LCN File"));
	btnRegisterLCN_->setFocusPolicy(Qt::NoFocus);
	connect(btnRegisterLCN_, &QPushButton::clicked, this, &LoginDialog::onbtnRegisterLCN);
	hhLayout->addWidget(btnRegisterLCN_);
	hhLayout->addStretch();
	offlineLayout->addLayout(hhLayout);
}

void LoginDialog::initExtensionFrame()
{
	extensionFrame_ = new QFrame(centerFrame_);
	extensionFrame_->setFrameShape(QFrame::Shape::Box);

	extensionFrame_->setObjectName("extensionFrame");


	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setSpacing(6);
	vLayout->setMargin(0);
	vLayout->setContentsMargins(10, 10, 10, 10);
	extensionFrame_->setLayout(vLayout);

	ckbFirewall_ = new QCheckBox(extensionFrame_);
	ckbFirewall_->setObjectName("ckbFirewall");
	ckbFirewall_->setText(QObject::tr("If using a Firewall,mark this checkbox"));
	ckbFirewall_->blockSignals(true);
	ckbFirewall_->setChecked(false);
	ckbFirewall_->blockSignals(false);
	vLayout->addWidget(ckbFirewall_);

	ckbProxy_ = new QCheckBox(extensionFrame_);
	ckbProxy_->setObjectName("ckbProxy");
	ckbProxy_->setText(QObject::tr("Use Proxy Server"));
	ckbProxy_->blockSignals(true);
	ckbProxy_->setChecked(false);
	ckbProxy_->blockSignals(false);
	vLayout->addWidget(ckbProxy_);

	initProxyFrame();
	vLayout->addWidget(proxyFrame_);


	ckbOffline_ = new QCheckBox(extensionFrame_);
	ckbOffline_->setObjectName("ckbOffline");
	ckbOffline_->setText(QObject::tr("Offline Authentication"));
	ckbOffline_->blockSignals(true);
	ckbOffline_->setChecked(false);
	ckbOffline_->blockSignals(false);
	vLayout->addWidget(ckbOffline_);

	initOfflineFrame();

	vLayout->addWidget(offlineFrame_);
	centerLayout_->addWidget(extensionFrame_);
	proxyFrame_->hide();
	offlineFrame_->hide();
	extensionFrame_->setVisible(bShowExtension_);

}

void LoginDialog::onClose()
{
	//reject();

	//QString strUrl = QString("http://127.0.0.1:8081/login");
	//QString strParameter;// = QString("id=%1&psd=%2").arg(idLineEdit_->text()).arg(pwdLineEdit_->text());
	//HttpClient httpClient;
	//QString strReponse;
	//QString strJson;
	//QMap<QString, QVariant> map;
	//map["id"] = idLineEdit_->text();
	//map["psd"] = pwdLineEdit_->text();
	//strJson = JsonManager::GenerateJsonString(map);
	//int nCode = httpClient.PostRequest(strUrl, strParameter,strJson, strReponse);
	//qDebug() << "nCode=" << nCode << " reponse=" << strReponse;
	//QString strtoken;
	//JsonManager::CheckUserLogin(strReponse, nCode, strtoken);
	//if (!strtoken.isEmpty())
	//{
	//	token_ = strtoken;
	//}

	QString strUrl = QString("http://127.0.0.1:8081/download");
	QString strReponse;
	QString strParameter = QString("token=%1&fileid=12.mp4").arg(token_);
	QString strSavePath = QString("D:/testData/12.mp4");
	HttpClient httpClient;
	//int nCode = httpClient.DownloadFileRequest(strUrl, strParameter, strSavePath, true);
	int nCode = httpClient.uploadFileRequest(strUrl, strParameter, strSavePath, strReponse, true);
	qDebug() << "nCode=" << nCode << " reponse=" << strReponse;
}

void LoginDialog::onUpDown()
{
	bShowExtension_ = !bShowExtension_;

	if (extensionFrame_)
	{
		extensionFrame_->setVisible(bShowExtension_);
	}
	if (bShowExtension_)
	{
		btnUpDown_->setProperty("state", "up");
		if (!ckbProxy_->isChecked() && !ckbOffline_->isChecked())
		{
			this->setFixedHeight(280);
		}
	    if (ckbProxy_->isChecked())
	    {
			this->setFixedHeight(446);
	    }
		else if (ckbOffline_->isChecked())
		{
			this->setFixedHeight(566);
		}
	}
	else
	{
		btnUpDown_->setProperty("state", "down");
		this->setFixedHeight(200);
	}
	style()->unpolish(btnUpDown_);
	style()->polish(btnUpDown_);
}

void LoginDialog::onLogin()
{
	/*if (idLineEdit_->text().isEmpty())
	{
		MessageBoxDialog* messageBox = new MessageBoxDialog(QObject::tr("Information"), QObject::tr("Please enter the user ID"), MessageBoxDialog::MessageButtonType::MB_OK);
		messageBox->exec();
		delete messageBox;
		messageBox = nullptr;
		return;
	}*/
	if (idLineEdit_->text().isEmpty() || pwdLineEdit_->text().isEmpty())
	{
		MessageBoxDialog* messageBox = new MessageBoxDialog(QObject::tr("Information"), QObject::tr("Please enter the user ID or user password"), MessageBoxDialog::MessageButtonType::MB_OK);
		messageBox->exec();
		delete messageBox;
		messageBox = nullptr;
		return;
	}
	/*QString strUrl = QString("http://www.baidu.com");
	QString token;
	if (0 != NetworkManager::CheckUserLogin(strUrl, idLineEdit_->text(), pwdLineEdit_->text(), token))
	{
		MessageBoxDialog* messageBox = new MessageBoxDialog(QObject::tr("Information"), QObject::tr("Login failed"), MessageBoxDialog::MessageButtonType::MB_OK);
		messageBox->exec();
		delete messageBox;
		messageBox = nullptr;
		return;
	}
	accept();*/
	/*QString strUrl = QString("http://127.0.0.1:8081/login");
	QString strParameter = QString("id=%1&psd=%2").arg(idLineEdit_->text()).arg(pwdLineEdit_->text());
	HttpClient httpClient;
	QString strReponse;
	int nCode = httpClient.GetRequest(strUrl, strParameter, strReponse);
	qDebug() << "nCode=" << nCode << " reponse=" << strReponse;
	QString strtoken;
	JsonManager::CheckUserLogin(strReponse, nCode, strtoken);
	if (!strtoken.isEmpty())
	{
		token_ = strtoken;
	}*/


	QString strUrl = QString("http://127.0.0.1:8081/login");
	QString strParameter;// = QString("id=%1&psd=%2").arg(idLineEdit_->text()).arg(pwdLineEdit_->text());
	HttpClient httpClient;
	QString strReponse;
	QString strJson;
	QMap<QString, QVariant> map;
	map["id"] = idLineEdit_->text();
	map["psd"] = pwdLineEdit_->text();
	strJson = JsonManager::GenerateJsonString(map);
	int nCode = httpClient.PostRequest(strUrl, strParameter, strJson, strReponse);
	qDebug() << "nCode=" << nCode << " reponse=" << strReponse;
	QString strtoken;
	JsonManager::CheckUserLogin(strReponse, nCode, strtoken);
	if (!strtoken.isEmpty())
	{
		token_ = strtoken;
	}
}

void LoginDialog::onCheckFirewall(bool bcheck)
{
	if (ckbFirewall_->isChecked())
	{
		ckbOffline_->setChecked(false);
		ckbOffline_->setEnabled(false);
		if (offlineFrame_)
		{
			offlineFrame_->setVisible(false);
		}
	}
	else
	{
		ckbOffline_->setEnabled(true);
	}
}

void LoginDialog::onCheckProxy(bool bcheck)
{
	if (ckbProxy_->isChecked())
	{
		if (proxyFrame_)
		{
			proxyFrame_->setVisible(true);
		}
		ckbOffline_->setChecked(false);
		ckbOffline_->setEnabled(false);
		if (offlineFrame_)
		{
			offlineFrame_->setVisible(false);
		}
	
	}
	else
	{
		ckbOffline_->setEnabled(true);
		if (proxyFrame_)
		{
			proxyFrame_->setVisible(false);
		}
	}
	if (bShowExtension_)
	{
		if (!ckbProxy_->isChecked() && !ckbOffline_->isChecked())
		{
			this->setFixedHeight(280);
		}
		if (ckbProxy_->isChecked())
		{
			this->setFixedHeight(446);
		}
		else if (ckbOffline_->isChecked())
		{
			this->setFixedHeight(566);
		}
	}
	
}

void LoginDialog::onCheckOffline(bool bcheck)
{
	if (ckbOffline_->isChecked())
	{
		ckbFirewall_->setEnabled(false);
		ckbProxy_->setEnabled(false);

		if (offlineFrame_)
		{
			offlineFrame_->setVisible(true);
		}
	}
	else
	{
		if (offlineFrame_)
		{
			offlineFrame_->setVisible(false);
		}
		ckbFirewall_->setEnabled(true);
		ckbProxy_->setEnabled(true);

	}
	if (bShowExtension_)
	{
		if (!ckbProxy_->isChecked() && !ckbOffline_->isChecked())
		{
			this->setFixedHeight(280);
		}
		if (ckbProxy_->isChecked())
		{
			this->setFixedHeight(446);
		}
		else if (ckbOffline_->isChecked())
		{
			this->setFixedHeight(566);
		}
	}
}

void LoginDialog::onbtnCreateHWClick()
{
	//验证http put功能 debug
	QString strUrl = QString("http://127.0.0.1:8081/login");
	QString strParameter = QString("token=%1").arg(token_);
	HttpClient httpClient;
	QString strReponse;
	QString strJson;
	QMap<QString, QVariant> map;
	map["id"] = idLineEdit_->text();
	map["psd"] = pwdLineEdit_->text();
	strJson = JsonManager::GenerateJsonString(map);
	int nCode = httpClient.PutRequest(strUrl, strParameter, strJson, strReponse);
	qDebug() << "nCode=" << nCode << " reponse=" << strReponse;
	
}

void LoginDialog::onbtnRegisterLCN()
{
	//验证http delete功能 debug
	QString strUrl = QString("http://127.0.0.1:8081/login");
	QString strParameter = QString("id=%1&token=%2").arg(idLineEdit_->text()).arg(token_);
	HttpClient httpClient;
	QString strReponse;
	int nCode = httpClient.DeleteRequest(strUrl, strParameter, strReponse);
	qDebug() << "nCode=" << nCode << " reponse=" << strReponse;
}

bool LoginDialog::eventFilter(QObject* watched, QEvent* event)
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
