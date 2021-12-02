#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
class QNetworkRequest;
class HttpClient : public QObject
{
    Q_OBJECT
signals:
    
    /**
     * @brief 下载文件进度
	 * @param bytesReceived 已下载文件总字节数
	 * @param bytesTotal 文件总字节数
    */
    void downloadProgressSgn(qint64 bytesReceived, qint64 bytesTotal);

    /**
     * @brief 上传文件进度
     * @param bytesSent 已上传文件总字节数
     * @param bytesTotal 文件总字节数
    */
    void uploadProgressSgn(qint64 bytesSent, qint64 bytesTotal);
public:
    explicit HttpClient(QObject *parent = nullptr);

    /**
     * @brief 向服务端请求数据，HTTP GET请求
     * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
     * @param strParamers 输入参数,使用如下格式name=val1&pwd=val2&…
     * @param strReponse 输出参数,返回的内容，请求到的数据
     * @param isAsync 是否异步操作
     * @return int 0 成功 非0 QNetworkReply的错误类型
    */
    int GetRequest(const QString& strUrl, const QString& strParamers, QString& strReponse, bool isAsync = true);

    /**
     * @brief HTTP POST 请求
     * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
     * @param strParamers 输入参数,使用如下格式name=val1&pwd=val2&…
     * @param strJson 输入参数,body 主题部分，json 数据格式
     * @param strReponse 输出参数,返回的内容，请求到的数据
     * @param bJson 是否采用json 格式
     * @param isAsync 是否异步操作
     * @return 0 成功 -1 json 数据格式不正确 大于0 QNetworkReply的错误类型
    */
    int PostRequest(const QString& strUrl, const QString& strParamers, const QString& strJson, QString& strReponse, bool bJson = true, bool isAsync = true);

    /**
     * @brief HTTP PUT 请求
     * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
     * @param strParamers 输入参数,使用如下格式name=val1&pwd=val2&…
     * @param strJson 输入参数,body 主题部分，json 数据格式
     * @param strReponse 输出参数,返回的内容，请求到的数据
     * @param bJson 是否采用json 格式
     * @param isAsync 是否异步操作
     * @return 0 成功 -1 json 数据格式不正确 大于0 QNetworkReply的错误类型
    */
    int PutRequest(const QString& strUrl, const QString& strParamers, const QString& strJson, QString& strReponse, bool bJson = true, bool isAsync = true);

    /**
        * @brief 向服务端请求数据，HTTP DELETE请求
        * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
        * @param strParamers 输入参数,使用如下格式name=val1&pwd=val2&…
        * @param strReponse 输出参数,返回的内容，请求到的数据
        * @param isAsync 是否异步操作
        * @return int 0 成功 非0 QNetworkReply的错误类型
       */
    int DeleteRequest(const QString& strUrl, const QString& strParamers, QString& strReponse, bool isAsync = true);

    /**
     * @brief 从服务端下载文件
     * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
     * @param strParamers 输入参数,使用如下格式name=val1&pwd=val2&…
     * @param savePath 保存下载文件的路径
     * @param isAsync 是否异步操作,true 异步，false 同步，默认异步
     * @return 0 成功，其他返回QNetworkReply的错误类型
    */
    int DownloadFileRequest(const QString& strUrl, const QString& strParamers, const QString& savePath, bool isAsync = true);

    /**
     * @brief 上传文件到服务器
     * @param strUrl strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
     * @param strParamers strParamers 输入参数,使用如下格式name=val1&pwd=val2&…
     * @param uploadfile 待上传的文件
     * @param strReponse 输出参数,返回的内容，请求到的数据
     * @param isAsync 是否异步操作,true 异步，false 同步，默认异步
     * @return 0 成功，其他返回QNetworkReply的错误类型
    */
    int uploadFileRequest(const QString& strUrl, const QString& strParamers, const QString& uploadfile, QString& strReponse, bool isAsync = true);
signals:

private:
    /**
     * @brief ssl设置，适用于https
    */
    void setSSlConfig(QNetworkRequest& request);
};

#endif // HTTPCLIENT_H
