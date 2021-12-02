#pragma once

struct evhttp_request;
struct evkeyvalq;
class IHttpHandler
{
public:
	virtual void httpHandlerCallBack(struct evhttp_request* req, void* arg) = 0;
};

class LoginOperator : public IHttpHandler
{
public:
	static void callback(struct evhttp_request* req, void* arg)
	{
		LoginOperator().httpHandlerCallBack(req, arg);
	}

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

class DownloadOperator : public IHttpHandler
{
public:
	static void callback(struct evhttp_request* req, void* arg)
	{
		DownloadOperator().httpHandlerCallBack(req, arg);
	}

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



