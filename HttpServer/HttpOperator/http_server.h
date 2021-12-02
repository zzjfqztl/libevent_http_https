#pragma once

struct evhttp_request;
struct evkeyvalq;
/**
 * @brief 处理消息接口，需要实现类继承
*/
class IHttpHandler
{
public:
	virtual void httpHandlerCallBack(struct evhttp_request* req, void* arg) = 0;
};

/**
 * @brief 登录处理
*/
class LoginOperator : public IHttpHandler
{
public:
	/**
	 * @brief 回调函数，供evhttp_set_cb回调使用
	 * @param req evhttp_request指针
	 * @param arg 参数
	*/
	static void callback(struct evhttp_request* req, void* arg)
	{
		LoginOperator().httpHandlerCallBack(req, arg);
	}

	/**
	 * @brief 消息处理接口函数
	 * @param req evhttp_request指针，包含http相关的信息
	 * @param arg 参数
	*/
	virtual void httpHandlerCallBack(struct evhttp_request* req, void* arg);

private:
	

	/**
	 * @brief 登录get 请求处理
	 * @param req 
	 * @param arg 
	*/
	void getHandler(struct evhttp_request* req, void* arg);

	/**
	 * @brief 登录Post请求处理
	 * @param req 
	 * @param arg 
	*/
	void postHandler(struct evhttp_request* req, void* arg);

	/**
	 * @brief 登录Put请求处理
	 * @param req
	 * @param arg
	*/
	void putHandler(struct evhttp_request* req, void* arg);

	/**
	 * @brief 登录Delete请求处理
	 * @param req
	 * @param arg
	*/
	void deleteHandler(struct evhttp_request* req, void* arg);

};

/**
 * @brief 文件操作接口
*/
class FilesOperator : public IHttpHandler
{
public:
	/**
	 * @brief 回调函数，供evhttp_set_cb回调使用
	 * @param req evhttp_request指针
	 * @param arg 参数
	*/
	static void callback(struct evhttp_request* req, void* arg)
	{
		FilesOperator().httpHandlerCallBack(req, arg);
	}

	/**
	 * @brief 消息处理接口函数
	 * @param req evhttp_request指针，包含http相关的信息
	 * @param arg 参数
	*/
	virtual void httpHandlerCallBack(struct evhttp_request* req, void* arg);

private:
	/**
	 * @brief 客户端请求下载文件
	 * @param req
	 * @param arg
	*/
	void getHandler(struct evhttp_request* req, void* arg);

	/**
	 * @brief 客户端上传文件
	 * @param req
	 * @param arg
	*/
	void postHandler(struct evhttp_request* req, void* arg);

};



