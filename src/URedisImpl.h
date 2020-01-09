#pragma once

#include<vector>

#include <hiredis.h>
#include <async.h>
#include <adapters/libuv.h>
#include "URedisMultiCfg.h"

class URedisImpl
{
	public:
		URedis(const int nConnMod, const std::vector<URedisMultiCfg>& rCfg);

		~URedis();

	private:
		std::vector<redisAsyncContext *> ctxs;
};

