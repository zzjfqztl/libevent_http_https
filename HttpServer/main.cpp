
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



/**
 * @brief This callback is responsible for creating a new SSL connection
  and wrapping it in an OpenSSL bufferevent.This is the way we implement 
  an https server instead of a plain old http server.
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

/**
 * @brief 设置服务器证书 和 服务器私钥 到OPENSSL ctx上下文句柄中
 * @param ctx  ssl句柄
 * @param certificate_chain 服务器证书 
 * @param private_key 服务器私钥
*/
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
	
	//启动https服务端
	https_server = evhttp_start(http_addr, https_port);
	//启动http服务端
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
	char DirectoryBuffer[1024] = { '\0' };
	int Length = GetModuleFileName(NULL, DirectoryBuffer, 1024);
	std::string strExePath = DirectoryBuffer;
	strExePath = strExePath.substr(0, strExePath.find_last_of("\\"));
	std::string certificate_chain = strExePath +"\\"+ "server-certificate-chain.pem";
	std::string private_key = strExePath + "\\" + "server-private-key.pem";
	/* 设置服务器证书 和 服务器私钥 到
	 OPENSSL ctx上下文句柄中 */
	server_setup_certs(ctx, certificate_chain.c_str(), private_key.c_str());

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
	evhttp_set_cb(https_server, "/download", FilesOperator::callback, NULL);
	evhttp_set_cb(http_server, "/login", LoginOperator::callback, NULL);
	evhttp_set_cb(http_server, "/download", FilesOperator::callback, NULL);

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

