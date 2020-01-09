#pragma once

#include<vector>
#include "URedisMultiCfg.h"


class URedisImpl;
class URedis
{
	public:
		URedis(const int nConnMod, const std::vector<URedisMultiCfg>& rCfg);

		~URedis();
	protected:
		URedisImpl* pImpl;
};


