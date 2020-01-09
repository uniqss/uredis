#pragma once

#include <string>

class URedisMultiCfg
{
	public:
		URedisMultiCfg() :
			nStart(0),
			nStop(0),
			port(0)
		{
		}

		~URedisMultiCfg()
		{
		}

		void Reset()
		{
			nStart = 0;
			nStop = 0;
			strIp.clear();
			nPort = 0;
		}

	public:
		int nStart;
		int nStop;
		std::string strIp;
		int nPort;
};

