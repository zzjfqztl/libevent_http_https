/**
 * 自定义数据集合
 */

#ifndef DATASTRUCTDEF_H
#define DATASTRUCTDEF_H

#include <QString>

/**
 * @brief 登录用户数据
*/
struct UserData
{
	UserData()
	{
		clear();
	}
	void clear()
	{
		id_.clear();
		userName_.clear();
		userPsd_.clear();
		email_.clear();
	}
	bool operator==(const UserData& data)
	{
		return id_ == data.id_ && !id_.isEmpty();
	}
	QString id_;
	QString userName_;
	QString userPsd_;
	QString email_;
};

#endif // DATASTRUCTDEF_H
