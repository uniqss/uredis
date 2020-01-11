#pragma once

#include <string>

class URedisMultiCfg
{
	public:
		// [start, stop)
		URedisMultiCfg() :
			start(0),
			stop(0),
			port(0)
		{
		}

		// [start, stop)
		URedisMultiCfg(int _start, int _stop, const std::string& _ip, int _port)
		{
			start = _start;
			stop = _stop;
			ip = _ip;
			port = _port;
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
		// [start, stop)
		int start;
		int stop;
		std::string ip;
		int port;
};

