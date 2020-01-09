#include "uredis.h"

// section. ctor dtors
URedis::URedis() : 
	ctx(nullptr),
	reply(nullptr)
{
}
URedis::~URedis()
{
	if (ctx != nullptr) {redisFree(ctx); ctx = nullptr;}
	reply = nullptr;
}


// section. connect reconnect
bool URedis::connect(const std::string& host, int port)
{
	ctx = redisConnect(host.c_str(), port);
	if(ctx != nullptr && ctx->err)
	{
		printf("connect error: %s\n", ctx->errstr);
		return 0;
	}
	return 1;
}


// section. Key
bool URedis::get(const std::string& key, std::string& value)
{
	reply = (redisReply*)redisCommand(ctx, "GET %s", key.c_str());
	if (reply->str)
	{
		value = reply->str;
		freeReplyObject(reply);
		return true;
	}
	else
	{
		freeReplyObject(reply);
		return false;
	}
}
void URedis::set(const std::string& key, const std::string& value)
{
	redisCommand(ctx, "SET %s %s", key.c_str(), value.c_str());
}


// section. String


// section. Hash


// section. List
bool URedis::lrange(const std::string& key, int start, int stop, std::vector<std::string>& values)
{
	reply = (redisReply*)redisCommand(ctx,"LRANGE %s %d %d", key.c_str(), start, stop);
	if (reply->type == REDIS_REPLY_ARRAY)
	{
		for (auto j = 0; j < reply->elements; j++)
		{
			values.push_back(reply->element[j]->str);
		}
		freeReplyObject(reply);
		return true;
	}
	else
	{
		freeReplyObject(reply);
		return false;
	}
}

long long URedis::rpush(const std::string& key, const std::string& value)
{
	reply = (redisReply*)redisCommand(ctx,"RPUSH %s %s", key.c_str(), value.c_str());

	if (reply && reply->type == REDIS_REPLY_INTEGER)
		return reply->integer;

	return (long long)-1;
}


// section. Set


// section. SortedSet


// section. others



