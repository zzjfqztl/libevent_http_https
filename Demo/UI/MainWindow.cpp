#include "MainWindow.h"
#include "QBoxLayout"
#include "QPushButton"
#include "Common/CustomControls.h"
#include "QCheckBox"
#include <QTextEdit>
#include <QMessageBox>

#include <thread>
#include "Common/MaskWidget.h"
#include "Network/HttpClient.h"
#include "QDateTime"
#include "Network/JsonManager.h"
#include "QLabel"
#include "QLineEdit"
#include "QFileInfo"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	InitUI();
	showMaximized();
}

void MainWindow::onGetClick()
{
	if (fileSelectFrame_->GetFileList().count() <= 0)
	{
		return;
	}
	QString strUrl = fileSelectFrame_->GetFileList()[0];
	QString strParameter = QString("id=%1&psd=%2").arg(idLineEdit_->text()).arg(psdLineEdit_->text());
	HttpClient httpClient;
	QString strReponse;
	int nCode = httpClient.GetRequest(strUrl, strParameter, strReponse);
	textEdit_->append(QString("code =%1").arg(nCode));
	textEdit_->append(QString("Response:"));
	textEdit_->append(strReponse);
	QString strtoken;
	JsonManager::CheckUserLogin(strReponse, nCode, strtoken);
	if (!strtoken.isEmpty())
	{
		token_ = strtoken;
	}
	textEdit_->append(QString("json code =%1").arg(nCode));
	textEdit_->append(QString("token= %1").arg(token_));

}

void MainWindow::onPostClick()
{
	if (fileSelectFrame_->GetFileList().count() <= 0)
	{
		return;
	}
	QString strUrl = fileSelectFrame_->GetFileList()[0];
	QString strParameter;
	HttpClient httpClient;
	QString strReponse;
	QString strJson;
	QMap<QString, QVariant> map;
	map["id"] = idLineEdit_->text();
	map["psd"] = psdLineEdit_->text();
	strJson = JsonManager::GenerateJsonString(map);
	int nCode = httpClient.PostRequest(strUrl, strParameter, strJson, strReponse);
	textEdit_->append(QString("code =%1").arg(nCode));
	textEdit_->append(QString("Response:"));
	textEdit_->append(strReponse);
	QString strtoken;
	JsonManager::CheckUserLogin(strReponse, nCode, strtoken);
	if (!strtoken.isEmpty())
	{
		token_ = strtoken;
	}
	textEdit_->append(QString("json code =%1").arg(nCode));
	textEdit_->append(QString("token= %1").arg(token_));
}

void MainWindow::onPutClick()
{
	//验证http put功能 debug
	if (fileSelectFrame_->GetFileList().count() <= 0)
	{
		return;
	}
	QString strUrl = fileSelectFrame_->GetFileList()[0];
	QString strParameter = QString("token=%1").arg(token_);
	HttpClient httpClient;
	QString strReponse;
	QString strJson;
	QMap<QString, QVariant> map;
	map["id"] = idLineEdit_->text();
	map["psd"] = psdLineEdit_->text();
	strJson = JsonManager::GenerateJsonString(map);
	int nCode = httpClient.PutRequest(strUrl, strParameter, strJson, strReponse);
	textEdit_->append(QString("code =%1").arg(nCode));
	textEdit_->append(QString("Response:"));
	textEdit_->append(strReponse);
}

void MainWindow::onDeleteClick()
{
	//验证http delete功能 debug
	if (fileSelectFrame_->GetFileList().count() <= 0)
	{
		return;
	}
	QString strUrl = fileSelectFrame_->GetFileList()[0];
	QString strParameter = QString("id=%1&token=%2").arg(idLineEdit_->text()).arg(token_);
	HttpClient httpClient;
	QString strReponse;
	int nCode = httpClient.DeleteRequest(strUrl, strParameter, strReponse);
	textEdit_->append(QString("code =%1").arg(nCode));
	textEdit_->append(QString("Response:"));
	textEdit_->append(strReponse);
}

void MainWindow::onUploadClick()
{
	if (fileSelectFrame_->GetFileList().count() <= 0)
	{
		return;
	}
	if (fileDirFrame_->GetFileList().count() <= 0)
	{
		return;
	}
	QString strUrl = fileSelectFrame_->GetFileList()[0];
	QString strReponse;
	QString strParameter = QString("token=%1").arg(token_);
	QString strSavePath = fileDirFrame_->GetFileList()[0];
	HttpClient httpClient;
	connect(&httpClient, &HttpClient::uploadProgressSgn, this, &MainWindow::onUploadProgress);
	int nCode = httpClient.uploadFileRequest(strUrl, strParameter, strSavePath, strReponse, true);
	textEdit_->append(QString("code =%1").arg(nCode));
	textEdit_->append(QString("Response:"));
	textEdit_->append(strReponse);
}

void MainWindow::onDownloadClick()
{
	if (fileSelectFrame_->GetFileList().count() <= 0)
	{
		return;
	}
	if (fileDirFrame_->GetFileList().count() <= 0)
	{
		return;
	}
	QString strUrl = fileSelectFrame_->GetFileList()[0];
	QString strReponse;
	QString strDownFile = fileDirFrame_->GetFileList()[0];
	QString strParameter = QString("token=%1&fileid=%2").arg(token_).arg(strDownFile);
	QString strSaveFile = QString("D:/testData/data/%1").arg(strDownFile);
	HttpClient httpClient;
	connect(&httpClient, &HttpClient::downloadProgressSgn, this, &MainWindow::onDownloadProgress);
	int nCode = httpClient.DownloadFileRequest(strUrl, strParameter, strSaveFile,true);
	textEdit_->append(QString::number(nCode));
	textEdit_->append(strReponse);
}

void MainWindow::onClearClick()
{
	textEdit_->clear();
	fileSelectFrame_->clearText();
}

void MainWindow::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	textEdit_->append(QStringLiteral("当前已下载：%1 byte\n 文件总大小：%2 byte").arg(bytesReceived).arg(bytesTotal));
}

void MainWindow::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
	textEdit_->append(QStringLiteral("当前已上传：%1 byte\n 文件总大小：%2 byte").arg(bytesSent).arg(bytesTotal));
}

void MainWindow::InitUI()
{
	QWidget* centerWidget_ = new QWidget(this);
	centerWidget_->setObjectName("centerWidget");
	setCentralWidget(centerWidget_);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setSpacing(10);
	vLayout->setMargin(0);
	vLayout->setContentsMargins(20, 10, 20, 10);
	vLayout->setAlignment(Qt::AlignTop);

	centerWidget_->setLayout(vLayout);

	QHBoxLayout* hlayout = new QHBoxLayout();
	hlayout->setSpacing(20);
	hlayout->setMargin(0);
	vLayout->addLayout(hlayout);

	QHBoxLayout* userLayout = new QHBoxLayout();
	userLayout->setSpacing(6);
	userLayout->setMargin(0);
	hlayout->addLayout(userLayout);

	QLabel* lbUser = new QLabel(this);
	lbUser->setObjectName("userLabel");
	lbUser->setText(QObject::tr("ID"));
	userLayout->addWidget(lbUser);

	idLineEdit_ = new QLineEdit(this);
	idLineEdit_->setObjectName("idLineEdit");
	idLineEdit_->clear();
	userLayout->addWidget(idLineEdit_);

	userLayout = new QHBoxLayout();
	userLayout->setSpacing(6);
	userLayout->setMargin(0);
	hlayout->addLayout(userLayout);

	QLabel* lbPsd = new QLabel(this);
	lbPsd->setObjectName("psdLabel");
	lbPsd->setText(QObject::tr("password"));
	userLayout->addWidget(lbPsd);

	psdLineEdit_ = new QLineEdit(this);
	psdLineEdit_->setObjectName("psdLineEdit");
	psdLineEdit_->clear();
	userLayout->addWidget(psdLineEdit_);


	//文件选择
	fileSelectFrame_ = new FileSelectFrame(QObject::tr("Select Files"), this);
	hlayout->addWidget(fileSelectFrame_);
	//解压后文件存储路径选择
	fileDirFrame_ = new FileSelectFrame(QObject::tr("Select Dir"), this, false);
	hlayout->addWidget(fileDirFrame_);

	hlayout = new QHBoxLayout();
	hlayout->setSpacing(20);
	hlayout->setMargin(0);
	vLayout->addLayout(hlayout);
	

	//Get按钮
	btnGet_ = new QPushButton(this);
	btnGet_->setObjectName("btnGet");
	btnGet_->setText(QObject::tr("Get"));
	btnGet_->setFocusPolicy(Qt::NoFocus);
	connect(btnGet_, &QPushButton::clicked, this, &MainWindow::onGetClick);
	hlayout->addWidget(btnGet_);

	//Post按钮
	btnPost_ = new QPushButton(this);
	btnPost_->setObjectName("btnPost");
	btnPost_->setText(QObject::tr("Post"));
	btnPost_->setFocusPolicy(Qt::NoFocus);
	connect(btnPost_, &QPushButton::clicked, this, &MainWindow::onPostClick);
	hlayout->addWidget(btnPost_);

	//Put按钮
	btnPut_ = new QPushButton(this);
	btnPut_->setObjectName("btnPut");
	btnPut_->setText(QObject::tr("Put"));
	btnPut_->setFocusPolicy(Qt::NoFocus);
	connect(btnPut_, &QPushButton::clicked, this, &MainWindow::onPutClick);
	hlayout->addWidget(btnPut_);

	//Delete按钮
	btnDelete_ = new QPushButton(this);
	btnDelete_->setObjectName("btnDelete");
	btnDelete_->setText(QObject::tr("Delete"));
	btnDelete_->setFocusPolicy(Qt::NoFocus);
	connect(btnDelete_, &QPushButton::clicked, this, &MainWindow::onDeleteClick);
	hlayout->addWidget(btnDelete_);


	//Upload按钮
	btnUpload_ = new QPushButton(this);
	btnUpload_->setObjectName("btnUpload");
	btnUpload_->setText(QObject::tr("Upload"));
	btnUpload_->setFocusPolicy(Qt::NoFocus);
	connect(btnUpload_, &QPushButton::clicked, this, &MainWindow::onUploadClick);
	hlayout->addWidget(btnUpload_);


	//Delete按钮
	btnDownload_ = new QPushButton(this);
	btnDownload_->setObjectName("btnDownload");
	btnDownload_->setText(QObject::tr("Download"));
	btnDownload_->setFocusPolicy(Qt::NoFocus);
	connect(btnDownload_, &QPushButton::clicked, this, &MainWindow::onDownloadClick);
	hlayout->addWidget(btnDownload_);

	btnClear_ = new QPushButton(this);
	btnClear_->setObjectName("btnClear");
	btnClear_->setText(QObject::tr("Clear"));
	btnClear_->setFocusPolicy(Qt::NoFocus);
	connect(btnClear_, &QPushButton::clicked, this, &MainWindow::onClearClick);
	hlayout->addWidget(btnClear_);
	hlayout->addStretch();

	textEdit_ = new QTextEdit(this);
	textEdit_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	textEdit_->setObjectName("textEdit");
	vLayout->addWidget(textEdit_);
}
