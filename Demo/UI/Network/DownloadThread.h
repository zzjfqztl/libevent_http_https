#pragma once
#ifndef DOWNLOAD_THREAD_H
#define  DOWNLOAD_THREAD_H

#include <QThread>
#include <QtCore>
#include "Downloadmanager.h"

/**
 * @brief 下载文件线程
*/
class DownloadThread : public QThread
{
	Q_OBJECT

public:
	/**
	 * @brief 构造函数
	*/
	DownloadThread();

	/**
	 * 析构函数
	 */
	~DownloadThread();

private:
	/**
	 * @brief 移除文件
	 * @param fileName 删除指定文件
	*/
	void removeFile(QString fileName);

private slots :
	/**
	 * @brief 执行函数
	*/
	void run();

	/**
	 * @brief 接收源下载地址事件
	 * @param strSourceAddress 下载地址
	*/
	void reciveDownloadAddress(const QString& strSourceAddress);

	/**
	 * @brief 接收目标下载地址事件
	 * @param strTargetAddress 目标地址
	*/
	void reciveTargetAddress(const QString& strTargetAddress);

	/**
	 * @brief 暂停下载
	*/
	void recivePauseDownload();

	/**
	 * @brief 停止下载
	*/
	void reciveCloseDownload();

signals:
	/**
	 * @brief 下载文件进度
	 * @param bytesReceived 已下载文件总字节数
	 * @param bytesTotal 文件总字节数
	*/
	void signalDownloadProcess(qint64, qint64);

	/**
	 * @brief 请求返回信号
	 * @param  
	*/
	void signalReplyFinished(int);

private:
	QString m_strTargetAddress;//下载目标地址

	DownLoadManager* m_downloadManager;//下载管理

	QString m_strSourceAddress;//服务器源下载地址
};


#endif // !downloadThread_H
