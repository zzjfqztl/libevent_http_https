#pragma once

struct evhttp_request;
struct evkeyvalq;
/**
 * @brief ������Ϣ�ӿڣ���Ҫʵ����̳�
*/
class IHttpHandler
{
public:
	virtual void httpHandlerCallBack(struct evhttp_request* req, void* arg) = 0;
};

/**
 * @brief ��¼����
*/
class LoginOperator : public IHttpHandler
{
public:
	/**
	 * @brief �ص���������evhttp_set_cb�ص�ʹ��
	 * @param req evhttp_requestָ��
	 * @param arg ����
	*/
	static void callback(struct evhttp_request* req, void* arg)
	{
		LoginOperator().httpHandlerCallBack(req, arg);
	}

	/**
	 * @brief ��Ϣ����ӿں���
	 * @param req evhttp_requestָ�룬����http��ص���Ϣ
	 * @param arg ����
	*/
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

/**
 * @brief �ļ������ӿ�
*/
class FilesOperator : public IHttpHandler
{
public:
	/**
	 * @brief �ص���������evhttp_set_cb�ص�ʹ��
	 * @param req evhttp_requestָ��
	 * @param arg ����
	*/
	static void callback(struct evhttp_request* req, void* arg)
	{
		FilesOperator().httpHandlerCallBack(req, arg);
	}

	/**
	 * @brief ��Ϣ����ӿں���
	 * @param req evhttp_requestָ�룬����http��ص���Ϣ
	 * @param arg ����
	*/
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



