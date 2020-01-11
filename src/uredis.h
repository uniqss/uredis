#pragma once

#include<vector>
#include "URedisMultiCfg.h"
#include "uv.h"
#include "hiredis.h"

enum EConnMod
{
	EConnMod_1 = 1,
	EConnMod_10 = 10,
	EConnMod_100 = 100,
	EConnMod_1000 = 1000
};

// userid, operationtypeid, redisReply, privdata      privdata's memory must be managed by user.
typedef void (redisCbFn)(int64_t, int64_t, redisReply*, void*);

class URedisImpl;
class URedis
{
	public:
		URedis(uv_loop_t* loop, EConnMod connMod, EConnMod dbMod, const std::vector<URedisMultiCfg>& rCfg);

		~URedis();
		

		// privdata can be set null if not needed
		int cmd(int64_t id, int64_t opid, redisCbFn* cb, void* privdata, const char* format, ...);
		// privdata can be set null if not needed
		int cmd(int64_t id, int64_t opid, redisCbFn* cb, void* privdata, const char* cmd, int len);

	protected:
		URedisImpl* pImpl;
};


