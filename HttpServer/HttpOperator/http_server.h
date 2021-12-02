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
	 * @brief ��¼get ������
	 * @param req 
	 * @param arg 
	*/
	void getHandler(struct evhttp_request* req, void* arg);

	/**
	 * @brief ��¼Post������
	 * @param req 
	 * @param arg 
	*/
	void postHandler(struct evhttp_request* req, void* arg);

	/**
	 * @brief ��¼Put������
	 * @param req
	 * @param arg
	*/
	void putHandler(struct evhttp_request* req, void* arg);

	/**
	 * @brief ��¼Delete������
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
	 * @brief �ͻ������������ļ�
	 * @param req
	 * @param arg
	*/
	void getHandler(struct evhttp_request* req, void* arg);
	/**
	 * @brief �ͻ����ϴ��ļ�
	 * @param req
	 * @param arg
	*/
	void postHandler(struct evhttp_request* req, void* arg);

};



