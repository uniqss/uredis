#pragma once

#include<vector>

#include <hiredis.h>
#include <async.h>
#include <adapters/libuv.h>
#include "URedisMultiCfg.h"
#include "uredis.h"


class ConnCfg
{
	public:
		ConnCfg() :
			c(nullptr),
			port(0)
	{
	}

		redisAsyncContext* c;
		std::string ip;
		int port;
};

class PrivDataWrapper
{
	public:
		PrivDataWrapper()
			:
				id(0),
				opid(0),
				privdata(nullptr),
				cbfn(nullptr)
	{
	}

		PrivDataWrapper(int64_t _id, int64_t _opid, void* _privdata, redisCbFn* fn)
			:
				id(_id),
				opid(_opid),
				privdata(_privdata),
				cbfn(fn)
	{
	}

		int64_t id;
		int64_t opid;
		void* privdata;
		redisCbFn* cbfn;
};


class URedisImpl
{
	public:
		URedisImpl(uv_loop_t* loop, EConnMod connMod, EConnMod dbMod, const std::vector<URedisMultiCfg>& rCfg);

		~URedisImpl();


		// privdata can be set null if not needed
		int cmd(int64_t id, int64_t opid, redisCbFn* cb, void* privdata, const char* format, va_list ap);
		// privdata can be set null if not needed
		int cmd(int64_t id, int64_t opid, redisCbFn* cb, void* privdata, const char* cmd, int len);

	private:
		bool _reconnIfNeeded(int64_t id, ConnCfg*& cfg);

	private:
		std::vector<ConnCfg> cs;
		uv_loop_t* loop;
		EConnMod cmod;
		EConnMod dmod;
};

