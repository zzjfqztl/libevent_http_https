
#include <stdio.h>
#include <stdlib.h>
#include <evhttp.h>
#include <event.h>
#include <string.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
#include "event2/http.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/http_struct.h"
#include "event2/http_compat.h"
#include "event2/util.h"
#include "event2/listener.h"

#include "HttpOperator/http_server.h"
#include "HttpOperator/https-common.h"
#include <string>
#include <fstream>

/*
* This callback is responsible for creating a new SSL connection
*and wrapping it in an OpenSSL bufferevent.This is the way
* we implement an https server instead of a plain old http server.
*/
static struct bufferevent* bevcb(struct event_base* base, void* arg)
{
	struct bufferevent* r;
	SSL_CTX* ctx = (SSL_CTX*)arg;

	r = bufferevent_openssl_socket_new(base,
		-1,
		SSL_new(ctx),
		BUFFEREVENT_SSL_ACCEPTING,
		BEV_OPT_CLOSE_ON_FREE);
	return r;
}

static void server_setup_certs(SSL_CTX * ctx,
	const char* certificate_chain,
	const char* private_key)
{
	info_report("Loading certificate chain from '%s'\n"
		"and private key from '%s'\n",
		certificate_chain, private_key);

	if (1 != SSL_CTX_use_certificate_chain_file(ctx, certificate_chain))
		die_most_horribly_from_openssl_error("SSL_CTX_use_certificate_chain_file");

	if (1 != SSL_CTX_use_PrivateKey_file(ctx, private_key, SSL_FILETYPE_PEM))
		die_most_horribly_from_openssl_error("SSL_CTX_use_PrivateKey_file");

	if (1 != SSL_CTX_check_private_key(ctx))
		die_most_horribly_from_openssl_error("SSL_CTX_check_private_key");
}
int main()
{
#ifdef _WIN32
	// 初始化windows自带的socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#else
	// linux
	// 忽略管道信号，发送数据给自己关闭的socket
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		return 1;
	}
#endif //
	/*OpenSSL 初始化 */
	common_setup();
	struct evhttp *https_server = NULL;
	struct evhttp* http_server = NULL;
	short http_port = 8081;
	char *http_addr = "0.0.0.0";
	short https_port = 443;
	
	//初始化
	event_init();
	
	//启动http服务端
	https_server = evhttp_start(http_addr, https_port);
	http_server = evhttp_start(http_addr, http_port);
	if(https_server == NULL || http_server == NULL)
	{
		printf("====line:%d,%s\n",__LINE__,"http server start failed.");
		return -1;
	}

	/* 创建SSL上下文环境 ，可以理解为 SSL句柄 */
	SSL_CTX* ctx = SSL_CTX_new(SSLv23_server_method());
	SSL_CTX_set_options(ctx,
		SSL_OP_SINGLE_DH_USE |
		SSL_OP_SINGLE_ECDH_USE |
		SSL_OP_NO_SSLv2);

	/* Cheesily pick an elliptic curve to use with elliptic curve ciphersuites.
	 * We just hardcode a single curve which is reasonably decent.
	 * See http://www.mail-archive.com/openssl-dev@openssl.org/msg30957.html */
	EC_KEY* ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	if (!ecdh)
		die_most_horribly_from_openssl_error("EC_KEY_new_by_curve_name");
	if (1 != SSL_CTX_set_tmp_ecdh(ctx, ecdh))
		die_most_horribly_from_openssl_error("SSL_CTX_set_tmp_ecdh");
	/* 选择服务器证书 和 服务器私钥. */
	const char* certificate_chain = "D:/workspace/Login_demo/LibeventHttps/server-certificate-chain.pem";
	const char* private_key = "D:/workspace/Login_demo/LibeventHttps/server-private-key.pem";
	/* 设置服务器证书 和 服务器私钥 到
	 OPENSSL ctx上下文句柄中 */
	server_setup_certs(ctx, certificate_chain, private_key);

	/*
		使我们创建好的evhttp句柄 支持 SSL加密
		实际上，加密的动作和解密的动作都已经帮
		我们自动完成，我们拿到的数据就已经解密之后的
	*/
	evhttp_set_bevcb(https_server, bevcb, ctx);

	//设置请求超时时间(s)
	evhttp_set_timeout(https_server,5);
	evhttp_set_timeout(http_server, 5);
	//设置事件处理函数，evhttp_set_cb针对每一个事件(请求)注册一个处理函数，
	//区别于evhttp_set_gencb函数，是对所有请求设置一个统一的处理函数

	evhttp_set_cb(https_server,"/login",LoginOperator::callback ,NULL);
	evhttp_set_cb(https_server, "/download", DownloadOperator::callback, NULL);
	evhttp_set_cb(http_server, "/login", LoginOperator::callback, NULL);
	evhttp_set_cb(http_server, "/download", DownloadOperator::callback, NULL);

	//循环监听
	event_dispatch();
	//实际上不会释放，代码不会运行到这一步
	evhttp_free(https_server);
	evhttp_free(http_server);
#ifdef _WIN32
	WSACleanup(); // 清理Windows端socket库
#endif //	
	return 0;
}

//int init_win_socket();
//void generic_handler(struct evhttp_request* req, void* arg);
//void http_handler(struct evhttp_request* req, void* arg);
//
//
//std::ofstream outfile;
//
//int main(int argc, char** argv)
//{
//
//#ifdef WIN32
//	init_win_socket();
//#endif
//	char* http_addr = "127.0.0.1";
//	const int	PORT = 9995;
//	//create a event_base
//	struct event_base* base = event_base_new();
//	if (!base)
//	{
//		fprintf(stderr, "Could not initialize libevent!\n");
//		return 1;
//	}
//
//	struct evhttp* http_server = evhttp_new(base);
//	if (!http_server)
//	{
//		return -1;
//	}
//
//	int ret = evhttp_bind_socket(http_server, http_addr, PORT);
//	if (ret != 0)
//	{
//		return -1;
//	}
//
//	evhttp_set_gencb(http_server, http_handler, NULL);
//	printf("http server start OK! \n");
//	event_base_dispatch(base);
//
//	evhttp_free(http_server);
//	event_base_free(base);
//	WSACleanup();
//	system("pause");
//	return 0;
//}
//
//
//int init_win_socket()
//{
//	WSADATA wsaData;
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//	{
//		return -1;
//	}
//	return 0;
//}
//
//void http_handler(struct evhttp_request* req, void* arg)
//{
//	struct evbuffer* buf;
//	buf = evbuffer_new();
//
//	// 分析请求
//	char* decode_uri = _strdup((char*)evhttp_request_get_uri(req));
//	struct evkeyvalq http_query;
//	evhttp_parse_query(decode_uri, &http_query);
//	free(decode_uri);
//
//	char* uri = _strdup((char*)evhttp_request_uri(req));
//	char		GetWayURL[100] = { 0 };
//	const int	MSG_DATA_SIZE = 15000;
//	char		body_data[MSG_DATA_SIZE] = { 0 };
//	int			body_datalen = 0;
//
//
//	// ?
//	if (memcmp(uri, GetWayURL, strlen(GetWayURL) != 0))
//	{
//		free(uri);
//		buf = evbuffer_new();
//		evbuffer_add_printf(buf, "%s", "HTTP_BADREQUEST");
//		evhttp_send_reply(req, HTTP_BADREQUEST, "OK", buf);
//
//		evbuffer_free(buf);
//		return;
//	}
//	free(uri);
//
//
//	//get data from post
//	memset(body_data, 0, sizeof(body_data));
//
//	size_t		len_of_file_to_recv = 0;
//	bool		is_first = true;
//	size_t		loc = -1;
//	std::string body_str;
//
//	while (len_of_file_to_recv < req->body_size)
//	{
//		body_datalen = evbuffer_remove(req->input_buffer, body_data, sizeof(body_data));
//		if (body_datalen > MSG_DATA_SIZE)
//		{
//			buf = evbuffer_new();
//			evbuffer_add_printf(buf, "%s", "HTTP_DATALENTOLANG");
//			// output
//			evhttp_send_reply(req, HTTP_BADREQUEST, "OK", buf);
//			// mem release
//			evbuffer_free(buf);
//			return;
//		}
//		else if (is_first)
//		{
//			std::string buf_in = body_data;
//			size_t		loc = buf_in.find("\r\n\r\n");
//			size_t		loc_filename = -1;
//
//			//get file description, and extract the file name
//			//and cat precise time to the extracted file name to result in a unique file name
//			char* filename = NULL;
//			filename = new char[loc + 1];
//
//			memcpy(filename, body_data, loc);
//
//			filename[loc] = 0;
//			buf_in = filename;
//			memset(filename, 0, loc + 1);
//			loc_filename = buf_in.find("filename");
//			std::string file_name_front;
//			if (-1 != loc_filename)
//			{
//				loc_filename += strlen("filename");
//				while (loc_filename != loc)
//				{
//					if ('"' == buf_in[loc_filename])
//					{
//						int i = loc_filename + 1;
//						int j = 0;
//						while ('"' != buf_in[i] && i != loc)
//						{
//							filename[j++] = buf_in[i++];
//						}
//						filename[j] = 0;
//
//						break;
//					}
//					loc_filename++;
//				}
//
//				//get the filetype
//				buf_in = filename;
//				size_t loc_dot = buf_in.find_first_of('.');
//				file_name_front = buf_in.substr(0, loc_dot);
//				std::string file_type = buf_in.substr(loc_dot, strlen(filename) - loc_dot);
//
//
//				time_t t = time(0);
//				char tmp[64];
//				memset(tmp, 0, sizeof(tmp));
//				strftime(tmp, sizeof(tmp), "-%Y%m%d%H%M%S", localtime(&t));
//
//				file_name_front = file_name_front + tmp + file_type;  //complete filename
//
//			}
//			else
//			{
//				file_name_front = "default.avi";
//			}
//
//			delete[]filename;
//
//			char* buf_in_char = NULL;
//			buf_in_char = (body_data + loc + HEADER_END_LEN); //offset = loc + HEADER_END_LEN
//			is_first = false;
//			if (!(outfile.is_open()))
//			{
//				outfile.open(file_name_front, std::ios::binary | std::ios::app);
//			}
//
//			outfile.write(buf_in_char, body_datalen - (loc + HEADER_END_LEN)); //minus size of file description
//
//			len_of_file_to_recv += body_datalen;
//		}
//		else {
//			//parse file type
//			outfile.write(body_data, body_datalen);
//			len_of_file_to_recv += body_datalen;
//		}
//	}
//	if (outfile.is_open())
//	{
//		outfile.close();
//	}
//
//	// 返回HTTP头部
//	evhttp_add_header(req->output_headers, "Content-Type", "text/html; charset=UTF-8");
//	evhttp_add_header(req->output_headers, "Server", "my_http_zhangzhihuaHandler");
//	evhttp_add_header(req->output_headers, "Connection", "close");
//
//	evbuffer_add_printf(buf, "%s", "no msg, but everything is ok");
//	// send reply
//	evhttp_send_reply(req, HTTP_OK, "OK", buf);
//
//	// mem free
//	evhttp_clear_headers(&http_query);
//	evbuffer_free(buf);
//
//}