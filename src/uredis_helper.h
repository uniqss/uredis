#pragma once

#include <hiredis.h>
#include <vector>
#include <string>
#include <cstddef>
#include <stdlib.h>

bool trim_get(redisReply* r, std::string& val)
{
	if (r && r->type == REDIS_REPLY_STRING)
	{
		val = r->str;
		return true;
	}
	return false;
}
bool trim_get(redisReply* r, long long& val)
{
	if (r && r->type == REDIS_REPLY_STRING)
	{
		val = atoll(r->str);
		return true;
	}
	if (r && r->type == REDIS_REPLY_INTEGER)
	{
		val = r->integer;
		return true;
	}
	return false;
}

bool trim_mget(redisReply* r, std::vector<std::string>& vals)
{
	if (r && r->type == REDIS_REPLY_ARRAY)
	{
		vals.resize(r->elements);
		for(std::size_t i = 0;i < r->elements;++i)
		{
			vals[i] = r->element[i]->str;
		}
		return true;
	}
	return false;
}
bool trim_mget(redisReply* r, std::vector<long long>& vals)
{
	if (r && r->type == REDIS_REPLY_ARRAY)
	{
		vals.resize(r->elements);
		for(std::size_t i = 0;i < r->elements;++i)
		{
			vals[i] = atoll(r->element[i]->str);
		}
		return true;
	}
	return false;
}

bool trim_hget(redisReply* r, std::string& val)
{
	if (r && r->type == REDIS_REPLY_STRING)
	{
		val = r->str;
		return true;
	}
	return false;
}
bool trim_hget(redisReply* r, long long& val)
{
	if (r && r->type == REDIS_REPLY_STRING)
	{
		val = atoll(r->str);
		return true;
	}
	if (r && r->type == REDIS_REPLY_INTEGER)
	{
		val = r->integer;
		return true;
	}
	return false;
}

bool trim_hmget(redisReply* r, std::vector<std::string>& vals)
{
	if (r && r->type == REDIS_REPLY_ARRAY)
	{
		vals.resize(r->elements);
		for(std::size_t i = 0;i < r->elements;++i)
		{
			vals[i] = r->element[i]->str;
		}
		return true;
	}
	return false;
}
bool trim_hmget(redisReply* r, std::vector<long long>& vals)
{
	if (r && r->type == REDIS_REPLY_ARRAY)
	{
		vals.resize(r->elements);
		for(std::size_t i = 0;i < r->elements;++i)
		{
			vals[i] = atoll(r->element[i]->str);
		}
		return true;
	}
	return false;
}
bool trim_zrange(redisReply* r, std::vector<std::string>& vals)
{
	if (r && r->type == REDIS_REPLY_ARRAY)
	{
		vals.resize(r->elements);
		for(std::size_t i = 0;i < r->elements;++i)
		{
			vals[i] = r->element[i]->str;
		}
		return true;
	}
	return false;
}

bool trim_zrangescore(redisReply* r, std::vector<std::pair<std::string, long long> >& vals)
{
	if (r && r->type == REDIS_REPLY_ARRAY)
	{
		vals.resize(r->elements / 2);
		for(std::size_t i = 0;i < r->elements / 2;++i)
		{
			//vals[i] = std::make_pair(r->element[i / 2]->str, r->element[i / 2 + 1]->integer);
			vals[i] = std::make_pair(r->element[i / 2]->str, atoll(r->element[i / 2 + 1]->str));
		}
		return true;
	}
	return false;
}

