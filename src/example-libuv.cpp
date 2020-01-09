#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <hiredis.h>
#include <async.h>
#include <adapters/libuv.h>
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

auto getMS()
{
	auto time_now = chrono::system_clock::now();
	auto duration_in_ms = chrono::duration_cast<chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}

void connectCallback(const redisAsyncContext *c, int status) {
	if (status != REDIS_OK)
	{
		cout << __FUNCTION__ << getMS() << "Error: " << c->errstr << endl;
		return;
	}
	cout << "Connected..." << endl;
}

void disconnectCallback(const redisAsyncContext *c, int status) {
	if (status != REDIS_OK)
	{
		printf("Error: %s\n", c->errstr);
		return;
	}
	printf("Disconnected...\n");
}

void getCallback(redisAsyncContext *c, void *r, void *privdata) {
	redisReply *reply = (redisReply *)r;
	if (reply == nullptr)
	{
		cout << __FUNCTION__ << getMS() << "error occurred, reply == nullptr";
	}

	static int i = 0;
	if (i++ % 100 == 0)
		cout << __FUNCTION__ << getMS() << "should be:" << (char*)privdata << " str:" << reply->str << endl;

	//redisAsyncDisconnect(c);
}

int main (int argc, char **argv) {
	if (argc < 2)
	{
		cout << "should have at least 2 args" << endl;
		return -1;
	}

	const char* pszTestValue = nullptr;
	pszTestValue = argv[1];

	signal(SIGPIPE, SIG_IGN);
	uv_loop_t* loop = uv_default_loop();

	redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
	if (c->err) {
		/* Let *c leak for now... */
		printf("Error: %s\n", c->errstr);
		return 1;
	}

	redisLibuvAttach(c,loop);
	redisAsyncSetConnectCallback(c,connectCallback);
	redisAsyncSetDisconnectCallback(c,disconnectCallback);
	cout << __FUNCTION__ << getMS() << "pre SET key" << endl;
	redisAsyncCommand(c, NULL, NULL, "SET key %s", pszTestValue);
	cout << __FUNCTION__ << getMS() << "post SET key" << endl;

	cout << __FUNCTION__ << getMS() << "pre GET key " << endl;
	for(int i = 0;i < 1000;++i)
	{
		redisAsyncCommand(c, getCallback, (char*) pszTestValue, "GET key");
	}
	cout << __FUNCTION__ << getMS() << "post GET key " << endl;
	uv_run(loop, UV_RUN_DEFAULT);

	redisAsyncDisconnect(c);
	return 0;
}

