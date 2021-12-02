#pragma once
#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkConfigurationManager>
#include <QEventLoop>
#include <QTimer>
#include <QtCore/QtCore>

/**
 * @brief 下载文件管理器
*/
class DownLoadManager : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief 下载文件管理器构造函数
	*/
	DownLoadManager(QObject *parent = Q_NULLPTR);

	/**
	 * 析构函数
	 */
	~DownLoadManager();

public:
	/**
	 * @brief 开始下载文件
	 * @param url 指定的url地址
	 * @param fileName 保存的文件全路径
	*/
	void downloadFile(QString url, QString fileName);

	/**
	 * @brief 暂停下载
	*/
	void stopDownload();
	
	/**
	 * @brief 关闭下载
	*/
	void closeDownload();
	
	/**
	 * @brief 获取下载的源地址
	 * @return 
	*/
	QString getDownloadUrl();
	
	/**
	 * @brief 路径和数据重置
	*/
	void reset();

	/**
	 * @brief 暂停下载
	*/
	void setClickPause() { m_bClickPause = true; }
private:
	/**
	 * @brief 停止下载
	*/
	void stopWork();

	/**
	 * @brief 删除临时文件
	 * @param fileName 临时文件
	*/
	void removeFile(QString fileName);

	/**
	 * @brief 遍历删除除了要下载的当前版本缓存文件之外的Tmp文件
	 * @param fileName 当前版本缓存文件
	*/
	void removeFileterFiles(QString fileName);

	/**
	 * @brief 获取是否支持断点续传
	 * @param url 指定url
	 * @return 
	*/
	bool bSupportBreak(const QUrl &url);

signals:
	/**
	 * @brief 下载文件进度
	 * @param bytesReceived 已下载文件总字节数
	 * @param bytesTotal 文件总字节数
	*/
	void signalDownloadProcess(qint64 bytesReceived, qint64 bytesTotal);//发送下载过程信号
	
	/**
	 * @brief 发送下载完成信号
	 * @param  下载完成标记
	*/
	void signalReplyFinished(int);
	
	/**
	 * @brief 发送下载过程中出现错误信号
	*/
	void signalDownloadError();

private slots:
	/**
	 * @brief 下载文件进度
	 * @param bytesReceived 已下载文件总字节数
	 * @param bytesTotal 文件总字节数
	*/
	void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	
	/**
	 * @brief 下载过程中数据发生改变事件
	*/
	void onReadyRead();
	
	/**
	 * @brief 下载完成事件
	*/
	void onFinished();
	
	/**
	 * @brief 下载错误事件
	 * @param code 错误码
	*/
	void onError(QNetworkReply::NetworkError code);

	/**
	 * @brief 请求结束事件
	 * @param  
	*/
	void replyFinished(QNetworkReply*);

	/**
	 * @brief 定时器到期事件
	*/
	void handleTimeOut();

private:
	QNetworkAccessManager* m_networkManager;//下载管理对象

	QNetworkReply* m_reply;//请求返回对象

	QUrl m_url;//请求下载的url地址

	QString m_fileName;//目标下载地址

	qint64 m_bytesReceived;//本次接收的数据字节大小

	qint64 m_bytesTotal;//数据的总字节大小

	qint64 m_bytesCurrentReceived;//断点之前已经下载的字节大小

	bool m_isStop;//标识是否停止下载

	QEventLoop*   qEvent;//用于下载的事件循环

	QTimer        *_timeOut;//用于控制超时的定时器

	qint64 m_bytesDown;//断点之前已经下载的字节大小（用于判断一段时间内数据是否有变化）

	QString		m_qStrUrl;//源下载地址

	QString		m_qStrFilePath;//目标下载地址

	QTime m_timeElapsed;//用于超时30分钟的定时器对象

	bool m_bClickPause;
};

#endif // 
