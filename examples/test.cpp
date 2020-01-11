#include "uredis.h"
#include <vector>

#include "uv.h"

#include<iostream>
using std::cout;
using std::endl;

class User
{
	public:
		User(int64_t u, int64_t mid, int64_t sd)
			:
				uid(u),
				currmsgid(mid),
				somedata(sd)
	{
	}
		int64_t uid;
		int64_t currmsgid;
		int64_t somedata;
};

void test_cb(int64_t id, int64_t opid, redisReply* rpl, void* privdata)
{
	cout << __FUNCTION__ << " id:" << id << " opid:" << opid << endl;
	if (!rpl)
	{
		cout << __FUNCTION__ << "error occurred" << endl;
		return;
	}
	cout << "rpl->str:" << rpl->str << endl;

	User& u = *(User*)privdata;
	cout << "u:" << u.uid << "," << u.currmsgid << "," << u.somedata << endl;
}


int main()
{
	URedisMultiCfg c1(0, 50, "127.0.0.1", 6379);
	URedisMultiCfg c2(50, 100, "127.0.0.1", 6379);

	std::vector<URedisMultiCfg> cfg;
	cfg.push_back(c1);
	cfg.push_back(c2);

	uv_loop_t* loop = uv_default_loop();
	URedis* p = new URedis(loop, EConnMod_100, EConnMod_1, cfg);

	URedis& r = *p;

	User u(123, 1001, 666);

	std::string strUserCache = "usercache_" + std::to_string(u.uid);
	int result = r.cmd(u.uid, u.currmsgid, nullptr, nullptr, "SET %s %d", strUserCache.c_str(), u.somedata);

	cout << "result:" << result << endl;
	
	result = r.cmd(u.uid, u.currmsgid, test_cb, &u, "GET %s", strUserCache.c_str());

	uv_run(loop, UV_RUN_DEFAULT);

	delete p;

	return 0;
}

