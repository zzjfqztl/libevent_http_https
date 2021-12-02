#ifndef JSONMANAGER_H
#define JSONMANAGER_H

#include <QObject>
#include "QMap"
#include "QVariant"
/**
 * @brief Json 数据解析与组装
*/
class JsonManager : public QObject
{
    Q_OBJECT
public:
    explicit JsonManager(QObject *parent = nullptr);

    /**
     * @brief 传入map 生成json字符串
     * @param dataMap 生成接送需要的数据
     * @return QString json 字符串
    */
    static QString GenerateJsonString(const QMap<QString, QVariant>& dataMap);

    /**
     * @brief 根据服务端返回的信息校验是否登录成功
     * @param strReponse 服务端返回的信息
     * @param nCode 服务端返回的错误码; 大于0 服务器返回的错误码；小于0json 错误码对应的负值
     * @param token 校验成功后服务端返回的token
     * @return true 成功，false 失败
    */
    static bool CheckUserLogin(const QString& strReponse, int& nCode, QString& token);
signals:

};

#endif // JSONMANAGER_H
