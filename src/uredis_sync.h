#pragma once

#include <string>
#include <vector>
#include <hiredis/hiredis.h>

class URedis
{
	public:
		// section. ctor dtors
		URedis();
		~URedis();


		// section. connect reconnect
		bool connect(const std::string& host, int port);


		// section. Key
		bool get(const std::string& key, std::string& value);
		void set(const std::string& key, const std::string& value);


		// section. String


		// section. Hash


		// section. List
		bool lrange(const std::string& key, int start, int stop, std::vector<std::string>& values);
		long long rpush(const std::string& key, const std::string& value);


		// section. Set


		// section. SortedSet


		// section. others


	private:
		redisContext* ctx;
		redisReply* reply;
};

