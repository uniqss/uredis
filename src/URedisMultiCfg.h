#pragma once

#include <string>

class URedisMultiCfg
{
	public:
		URedisMultiCfg() :
			start(0),
			stop(0),
			port(0)
		{
		}

		~URedisMultiCfg()
		{
		}

		void Reset()
		{
			start = 0;
			stop = 0;
			ip.clear();
			port = 0;
		}

	public:
		int start;
		int stop;
		std::string ip;
		int port;
};

