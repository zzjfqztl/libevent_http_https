#include "http_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <evhttp.h>
#include <event.h>
#include <string.h>
#include "event2/http.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "event2/bufferevent_compat.h"
#include "event2/http_struct.h"
#include "event2/http_compat.h"
#include "event2/util.h"
#include "event2/listener.h"
#include <iostream>
#include "QMap"
#include "QString"
#include "QVariant"
#include "QJsonDocument"
#include "QJsonObject"
#include "QUuid"
#include "QFile"
#include "QDir"
#include <vector>
#include <fstream>
#include "utils.h"
#define BUF_MAX 1024*16
#define FILE_BUF_MAX (1024*16)
#define HEADER_END_LEN 4


static QMap<QString, QString> userMap_ = { {"user1","1243"},{"aaa","a123456"},{"zhangsan","z123456"},{"test1","admin"},{"admin","admin123"}};
static QMap<QString,QString> tokenMap_;
static QString downloadPath_ = QString("D:/testData/download");
QString GenerateJsonString(const QMap<QString, QVariant>& dataMap)
{
	if (dataMap.isEmpty())
	{
		return QString();
	}
	QJsonDocument document;
	QJsonObject json;
	for (auto itr = dataMap.begin(); itr != dataMap.end(); itr++)
	{
		if (itr.key().isEmpty())
		{
			continue;
		}
		json.insert(itr.key(), itr.value().toJsonValue());

	}
	document.setObject(json);

	return document.toJson(QJsonDocument::Compact);
}

/**
 * @brief 解析post请求数据
 * @param buf
 * @param req
*/
void get_post_message(char* buf, struct evhttp_request* req)
{
	size_t post_size = 0;

	post_size = evbuffer_get_length(req->input_buffer);//获取数据长度
	printf("====line:%d,post len:%d\n", __LINE__, post_size);
	if (post_size <= 0)
	{
		printf("====line:%d,post msg is empty!\n", __LINE__);
		return;
	}
	else
	{
		size_t copy_len = post_size > BUF_MAX ? BUF_MAX : post_size;
		printf("====line:%d,post len:%d, copy_len:%d\n", __LINE__, post_size, copy_len);
		memcpy(buf, evbuffer_pullup(req->input_buffer, -1), copy_len);
		buf[copy_len] = '\0';
		printf("====line:%d,post msg:%s\n", __LINE__, buf);
	}
}

/**
 * @brief 解析http头，主要用于get请求时解析uri和请求参数
 * @param req
 * @param params
 * @param query_char
 * @return
*/
char* find_http_header(struct evhttp_request* req, struct evkeyvalq* params, const char* query_char)
{
	if (req == NULL || params == NULL || query_char == NULL)
	{
		printf("====line:%d,%s\n", __LINE__, "input params is null.");
		return NULL;
	}

	struct evhttp_uri* decoded = NULL;
	char* query = NULL;
	char* query_result = NULL;
	const char* path;
	const char* uri = evhttp_request_get_uri(req);//获取请求uri

	if (uri == NULL)
	{
		printf("====line:%d,evhttp_request_get_uri return null\n", __LINE__);
		return NULL;
	}
	else
	{
		printf("====line:%d,Got a GET request for <%s>\n", __LINE__, uri);
	}

	//解码uri
	decoded = evhttp_uri_parse(uri);
	if (!decoded)
	{
		printf("====line:%d,It's not a good URI. Sending BADREQUEST\n", __LINE__);
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return NULL;
	}

	//获取uri中的path部分
	path = evhttp_uri_get_path(decoded);
	if (path == NULL)
	{
		path = "/";
	}
	else
	{
		printf("====line:%d,path is:%s\n", __LINE__, path);
	}

	//获取uri中的参数部分
	query = (char*)evhttp_uri_get_query(decoded);
	if (query == NULL)
	{
		printf("====line:%d,evhttp_uri_get_query return null\n", __LINE__);
		return NULL;
	}

	//查询指定参数的值
	evhttp_parse_query_str(query, params);
	query_result = (char*)evhttp_find_header(params, query_char);

	return query_result;
}

/**
 * @brief 向客户端发送信息
 * @param req evhttp_request指针
 * @param QMap<QString,QVariant> 组合成json的列表 
*/
void reply_http(struct evhttp_request* req, const QMap<QString,QVariant>& retMap)
{
	//回响应
	struct evbuffer* retbuff = NULL;
	retbuff = evbuffer_new();
	if (retbuff == NULL)
	{
		printf("====line:%d,%s\n", __LINE__, "retbuff is null.");
		return;
	}
	//std::string strReponse= "{\"result\":\"success\",\"code\":\"0\",\"id\":\"" + std::string(sign) + "\"," + "\"psd\":\"" + std::string(data) + "\"}";
	QString strRespone = GenerateJsonString(retMap);
	evbuffer_add_printf(retbuff, strRespone.toUtf8().constData());
	evhttp_send_reply(req, HTTP_OK, "Client", retbuff);
	evbuffer_free(retbuff);
}

/**
 * @brief token 值校验
 * @param req evhttp_request 指针
 * @return 存在 返回true， 不存在false
*/
bool token_http_check(struct evhttp_request* req)
{
	if (req == nullptr)
	{
		return false;
	}
	char* token = NULL;
	struct evkeyvalq token_params = { 0 };
	token = find_http_header(req, &token_params, "token");//获取get请求uri中的sign参数
	if (token == NULL)
	{
		printf("====line:%d,%s\n", __LINE__, "request uri no param token.");
		return false;
	}
	else
	{
		if (!tokenMap_.values().contains(token))
		{
			printf("====line:%d,%s\n", __LINE__, "param token isn't right.");
			return false;
		}
		return true;
	}
}

void LoginOperator::httpHandlerCallBack(struct evhttp_request* req, void* arg)
{
	switch (evhttp_request_get_command(req))
	{
	case EVHTTP_REQ_GET: 
	{
		getHandler(req, arg);
	}
	break;
	case EVHTTP_REQ_POST:
	{
		postHandler(req, arg);
	}
	break;
	case EVHTTP_REQ_PUT: 
	{
		putHandler(req, arg);
	}
	break;
	case EVHTTP_REQ_DELETE:
	{
		deleteHandler(req, arg);
	}
	break;
	default:  break;
	}

}

void LoginOperator::getHandler(struct evhttp_request* req, void* arg)
{
	if (req == NULL)
	{
		printf("====line:%d,%s\n", __LINE__, "input param req is null.");
		return;
	}
	QMap<QString, QVariant> retMap;
	char* sign = NULL;
	char* data = NULL;
	struct evkeyvalq sign_params = { 0 };
	retMap["result"] = "success";
	retMap["code"] = "0";
	sign = find_http_header(req, &sign_params, "id");//获取get请求uri中的sign参数
	if (sign == NULL)
	{
		retMap["result"] = "failed";
		retMap["code"] = "1";
		printf("====line:%d,%s\n", __LINE__, "request uri no param sign.");
	}
	else
	{
		//retMap["id"] = sign;
		printf("====line:%d,get request param: id=[%s]\n", __LINE__, sign);
	}

	data = find_http_header(req, &sign_params, "psd");//获取get请求uri中的data参数
	if (data == NULL)
	{
		retMap["result"] = "failed";
		retMap["code"] = "2";
		printf("====line:%d,%s\n", __LINE__, "request uri no param data.");
	}
	else
	{
		//retMap["psd"] = data;
		printf("====line:%d,get request param: psd=[%s]\n", __LINE__, data);
	}

	printf("\n");
	QString strid = sign;

	auto itrFind = userMap_.find(strid);
	if (itrFind != userMap_.end())
	{
		QString strpwd = data;
		if (strpwd != itrFind.value())
		{
			retMap["result"] = "failed";
			retMap["code"] = "-3";
		}
		else
		{
			QUuid uuid = QUuid::createUuid();
			QString struuid = uuid.toString(QUuid::Id128);
			tokenMap_[strid] = struuid;
			retMap["token"] = struuid;
		}
	}
	else
	{
		retMap["result"] = "failed";
		retMap["code"] = "-2";
	}
	reply_http(req, retMap);

}

void LoginOperator::postHandler(struct evhttp_request* req, void* arg)
{
	if (req == NULL)
	{
		printf("====line:%d,%s\n", __LINE__, "input param req is null.");
		return;
	}
	QMap<QString, QVariant> retMap;
	retMap["result"] = "success";
	retMap["code"] = "0";
	char buf[BUF_MAX] = { 0 };
	get_post_message(buf, req);//获取请求数据，一般是json格式的数据
	if (buf == NULL)
	{
		retMap["result"] = "failed";
		retMap["code"] = "-1";
		printf("====line:%d,%s\n", __LINE__, "get_post_message return null.");
	}
	else
	{
		//可以使用json库解析需要的数据
		printf("====line:%d,request data:%s", __LINE__, buf);
		QJsonParseError json_error;
		QJsonDocument jsonDoc(QJsonDocument::fromJson(buf, &json_error));
		//json 数据校验
		if (json_error.error != QJsonParseError::NoError)
		{
			retMap["result"] = "failed";
			retMap["code"] = json_error.error;
		}
		else
		{
			QJsonObject rootObj = jsonDoc.object();
			QString strid = rootObj.value("id").toString();

			auto itrFind = userMap_.find(strid);
			if (itrFind != userMap_.end())
			{
				QString strpwd = rootObj.value("psd").toString();
				if (strpwd != itrFind.value())
				{
					retMap["result"] = "failed";
					retMap["code"] = "-3";
				}
				else
				{
					QUuid uuid = QUuid::createUuid();
					QString struuid = uuid.toString(QUuid::Id128);
					tokenMap_[strid] = struuid;
					retMap["token"] = struuid;
				}
			}
			else
			{
				retMap["result"] = "failed";
				retMap["code"] = "-2";
			}

		}
		
	}

	reply_http(req, retMap);
}

void LoginOperator::putHandler(struct evhttp_request* req, void* arg)
{
	if (req == NULL)
	{
		printf("====line:%d,%s\n", __LINE__, "input param req is null.");
		return;
	}
	QMap<QString, QVariant> retMap;
	retMap["result"] = "success";
	retMap["code"] = "0";
	if (!token_http_check(req))
	{
		retMap["result"] = "failed";
		retMap["code"] = "-4";
	}
	else
	{
		char buf[BUF_MAX] = { 0 };
		get_post_message(buf, req);//获取请求数据，一般是json格式的数据
		if (buf == NULL)
		{
			retMap["result"] = "failed";
			retMap["code"] = "-1";
			printf("====line:%d,%s\n", __LINE__, "get_post_message return null.");
		}
		else
		{
			//可以使用json库解析需要的数据
			printf("====line:%d,request data:%s", __LINE__, buf);
			QJsonParseError json_error;
			QJsonDocument jsonDoc(QJsonDocument::fromJson(buf, &json_error));
			//json 数据校验
			if (json_error.error != QJsonParseError::NoError)
			{
				retMap["result"] = "failed";
				retMap["code"] = json_error.error;
			}
			else
			{
				QJsonObject rootObj = jsonDoc.object();
				QString strid = rootObj.value("id").toString();

				auto itrFind = userMap_.find(strid);
				if (itrFind != userMap_.end())
				{
					QString strpwd = rootObj.value("psd").toString();
					userMap_[strid] = strpwd;
				}
				else
				{
					retMap["result"] = "failed";
					retMap["code"] = "-2";
				}

			}

		}
	}
	reply_http(req, retMap);
}

void LoginOperator::deleteHandler(struct evhttp_request* req, void* arg)
{
	if (req == NULL)
	{
		printf("====line:%d,%s\n", __LINE__, "input param req is null.");
		return;
	}
	QMap<QString, QVariant> retMap;
	retMap["result"] = "success";
	retMap["code"] = "0";
	if (!token_http_check(req))
	{
		retMap["result"] = "failed";
		retMap["code"] = "-4";
	}
	else
	{
		char* strid = NULL;
		struct evkeyvalq id_params = { 0 };
		strid = find_http_header(req, &id_params, "id");//获取get请求uri中的id参数
		if (strid == NULL)
		{
			retMap["result"] = "failed";
			retMap["code"] = "1";
			printf("====line:%d,%s\n", __LINE__, "request uri no param id.");
		}
		else
		{
			printf("====line:%d,delete request param: id=[%s]\n", __LINE__, strid);
			userMap_.remove(strid);
			tokenMap_.remove(strid);
		}

	}
	reply_http(req, retMap);
}

void DownloadOperator::httpHandlerCallBack(struct evhttp_request* req, void* arg)
{
	switch (evhttp_request_get_command(req))
	{
	case EVHTTP_REQ_GET:
	{
		getHandler(req, arg);
	}
	break;
	case EVHTTP_REQ_POST:
	{
		postHandler(req, arg);
	}
	break;

	default:  break;
	}

}

void DownloadOperator::getHandler(struct evhttp_request* req, void* arg)
{
	if (req == NULL)
	{
		printf("====line:%d,%s\n", __LINE__, "input param req is null.");
		return;
	}
	QMap<QString, QVariant> retMap;
	if (!token_http_check(req))
	{
		retMap["result"] = "failed";
		retMap["code"] = "-4";
	}
	else
	{
		char* sign = NULL;
		struct evkeyvalq sign_params = { 0 };
		retMap["result"] = "success";
		retMap["code"] = "0";
		sign = find_http_header(req, &sign_params, "fileid");//获取get请求uri中的sign参数
		if (sign == NULL)
		{
			retMap["result"] = "failed";
			retMap["code"] = "1";
			printf("====line:%d,%s\n", __LINE__, "request uri no param fileid.");
		}
		else
		{
			std::string fileName = Utf8ToGbk(sign);
			//retMap["id"] = sign;
			printf("====line:%d,get request param: id=[%s]\n", __LINE__, fileName.c_str());

			fileName = downloadPath_.toStdString()+"/"+ fileName;
			FILE* fp = fopen(fileName.c_str(), "rb");
			if (!fp)
			{
				retMap["result"] = "failed";
				retMap["code"] = "2";
				reply_http(req, retMap);
				return;
			}

			//2 回复浏览器
			//状态行， 消息报头， 响应正文
			evbuffer* outbuf = evhttp_request_get_output_buffer(req);
			char buf[BUF_MAX] = { 0 };
			for (;;)
			{
				int len = fread(buf, 1, sizeof(buf), fp);
				if (len <= 0) break;
				evbuffer_add(outbuf, buf, len);
			}
			fclose(fp);
			evhttp_send_reply(req, HTTP_OK, "", outbuf);
			return;
		
		}


		printf("\n");
		
	}
	reply_http(req, retMap);
}

void DownloadOperator::postHandler(struct evhttp_request* req, void* arg)
{
	if (req == NULL)
	{
		printf("====line:%d,%s\n", __LINE__, "input param req is null.");
		return;
	}
	QMap<QString, QVariant> retMap;
	if (!token_http_check(req))
	{
		retMap["result"] = "failed";
		retMap["code"] = "-4";
	}
	else
	{
		std::ofstream outfile;
		struct evbuffer* buf;
		buf = evbuffer_new();
		const int	MSG_DATA_SIZE = 15000;
		char		body_data[MSG_DATA_SIZE] = { 0 };
		int			body_datalen = 0;
		//get data from post
		memset(body_data, 0, sizeof(body_data));

		size_t		len_of_file_to_recv = 0;
		bool		is_first = true;
		size_t		loc = -1;
		std::string body_str;

		while (len_of_file_to_recv < req->body_size)
		{
			body_datalen = evbuffer_remove(req->input_buffer, body_data, sizeof(body_data));
			if (body_datalen > MSG_DATA_SIZE)
			{
				buf = evbuffer_new();
				evbuffer_add_printf(buf, "%s", "HTTP_DATALENTOLANG");
				// output
				evhttp_send_reply(req, HTTP_BADREQUEST, "OK", buf);
				// mem release
				evbuffer_free(buf);
				return;
			}
			else if (is_first)
			{
				std::string buf_in = body_data;
				size_t		loc = buf_in.find("\r\n\r\n");
				size_t		loc_filename = -1;

				//get file description, and extract the file name
				//and cat precise time to the extracted file name to result in a unique file name
				char* filename = NULL;
				filename = new char[loc + 1];

				memcpy(filename, body_data, loc);

				filename[loc] = 0;
				buf_in = filename;
				memset(filename, 0, loc + 1);
				loc_filename = buf_in.find("filename");
				std::string file_name_front;
				if (-1 != loc_filename)
				{
					loc_filename += strlen("filename");
					while (loc_filename != loc)
					{
						if ('"' == buf_in[loc_filename])
						{
							int i = loc_filename + 1;
							int j = 0;
							while ('"' != buf_in[i] && i != loc)
							{
								filename[j++] = buf_in[i++];
							}
							filename[j] = 0;

							break;
						}
						loc_filename++;
					}
					file_name_front = Utf8ToGbk(filename);
					std::string strPath = downloadPath_.toStdString() + "/" + file_name_front.c_str();
					if (QFile::exists(strPath.c_str()))
					{
						//get the filetype
						buf_in = filename;
						size_t loc_dot = buf_in.find_first_of('.');
						file_name_front = buf_in.substr(0, loc_dot);
						std::string file_type = buf_in.substr(loc_dot, strlen(filename) - loc_dot);


						time_t t = time(0);
						char tmp[64];
						memset(tmp, 0, sizeof(tmp));
						strftime(tmp, sizeof(tmp), "-%Y%m%d%H%M%S", localtime(&t));

						file_name_front = file_name_front + tmp + file_type;  //complete filename
						file_name_front = Utf8ToGbk(file_name_front.c_str());
					}
				}
				else
				{
					file_name_front = "default.avi";
				}
				
				delete[]filename;

				char* buf_in_char = NULL;
				buf_in_char = (body_data + loc + HEADER_END_LEN); //offset = loc + HEADER_END_LEN
				is_first = false;
				if (!(outfile.is_open()))
				{
					std::string strPath = downloadPath_.toStdString() + "/"+file_name_front.c_str();
					outfile.open(strPath, std::ios::binary | std::ios::app);
				}

				outfile.write(buf_in_char, body_datalen - (loc + HEADER_END_LEN)); //minus size of file description

				len_of_file_to_recv += body_datalen;
			}
			else {
				//parse file type
				outfile.write(body_data, body_datalen);
				len_of_file_to_recv += body_datalen;
			}
		}
		if (outfile.is_open())
		{
			outfile.close();
		}
		evbuffer_free(buf);
	}
	reply_http(req, retMap);
}
