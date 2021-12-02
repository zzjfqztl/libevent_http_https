#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

/**
 * @brief 网络管理模块
*/
class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

    /**
     * @brief 设置token值
     * @param token token 值，此值是从服务端获取的，是客户端连接服务端的唯一标识
    */
    static void SetToken(const QString& token);

    /**
     * @brief 获取当前客户端的token值
     * @return 当前客户端的token值
    */
    static QString GetToken();

    /**
     * @brief 检查登录用户是否登录成功
     * @param strUrl 客户端请求登录时的url
     * @param userId 用户登录唯一标识
     * @param userPsd 用户登录密码
     * @param token 登录成功后，返回的token值
     * @return 0 成功，其他返回登录错误码
    */
   static int CheckUserLogin(const QString& strUrl, const QString& userId, const QString& userPsd, QString& token);
signals:

private:
   static QString token_;
};

#endif // NETWORKMANAGER_H
