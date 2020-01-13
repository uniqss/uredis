#include "uredis.h"
#include <vector>

#include "uv.h"

#include "uredis_helper.h"

#include<iostream>
using std::cout;
using std::endl;

class User
{
	public:
		User(int64_t u, int64_t mid)
			:
				uid(u),
				currmsgid(mid)
	{
	}
		int64_t uid;
		int64_t currmsgid;

		int64_t getset_lld;
		std::string getset_str;

		int64_t mgetset_lld1;
		int64_t mgetset_lld2;
		int64_t mgetset_lld3;

		std::string mgetset_str1;
		std::string mgetset_str2;
		std::string mgetset_str3;

		int64_t hgetset_lld;
		std::string hgetset_str;

		int64_t hmgetset_lld1;
		int64_t hmgetset_lld2;
		int64_t hmgetset_lld3;

		std::string hmgetset_str1;
		std::string hmgetset_str2;
		std::string hmgetset_str3;
};

enum EUserOperation
{
	EUO_setlld,
	EUO_getlld,
	EUO_setstr,
	EUO_getstr,
	EUO_msetlld,
	EUO_mgetlld,
	EUO_msetstr,
	EUO_mgetstr,

	EUO_hsetlld,
	EUO_hgetlld,
	EUO_hsetstr,
	EUO_hgetstr,
	EUO_hmsetlld,
	EUO_hmgetlld,
	EUO_hmsetstr,
	EUO_hmgetstr,

};
void test_cb(int64_t id, int64_t opid, redisReply* rpl, void* privdata)
{
	cout << __FUNCTION__ << " id:" << id << " opid:" << opid << endl;
	if (!rpl){ cout << __FUNCTION__ << "error occurred" << endl; return;}
	if (privdata == nullptr)
	{
		cout <<__FUNCTION__ << " privdata is null, ok" << endl;
		return;
	}

#define U (*(User*)(privdata))
#define POK do {\
if (ok != 1)\
cout << __FUNCTION__ << opid << " ok:" << ok << endl;\
} while(false);
#define PEQ(lhs, rhs) if(privdata) cout << __FUNCTION__ << lhs << " vs " << U.rhs << " ?equal:" << ((lhs == U.rhs) ? "true":"false") << endl;
#define PEQARR(v1, v2, v3, arr) if(privdata) cout << __FUNCTION__ << U.v1 << " vs " << arr[0] << "|" \
<< U.v2 << " vs " << arr[1] << "|" \
<< U.v3 << " vs " << arr[2] << " ?equal:" << ((U.v1 == arr[0] && U.v2 == arr[1] && U.v3 == arr[2]) ? "true":"false") << endl;

	bool ok = false;
	std::string s1, s2, s3;
	long long l1, l2, l3;
	if (opid == EUO_getlld) {
		ok = trim_get(rpl, l1);
		POK;
		PEQ(l1, getset_lld);
	}
	if (opid == EUO_getstr) {
		ok = trim_get(rpl, s1);
		POK;
		PEQ(s1, getset_str);
	}
	if (opid == EUO_mgetlld) {
		std::vector<long long> vl;
		ok = trim_mget(rpl, vl);
		POK;
		PEQARR(mgetset_lld1, mgetset_lld2, mgetset_lld3, vl);
	}
	if (opid == EUO_mgetstr) {
		std::vector<std::string> vs;
		ok = trim_mget(rpl, vs);
		POK;
		PEQARR(mgetset_str1, mgetset_str2, mgetset_str3, vs);
	}

	if (opid == EUO_hgetlld) {
		ok = trim_hget(rpl, l1);
		POK;
		PEQ(l1, hgetset_lld);
	}
	if (opid == EUO_hgetstr) {
		ok = trim_hget(rpl, s1);
		POK;
		PEQ(s1, hgetset_str);
	}
	if (opid == EUO_hmgetlld) {
		std::vector<long long> vl;
		ok = trim_hmget(rpl, vl);
		POK;
		PEQARR(hmgetset_lld1, hmgetset_lld2, hmgetset_lld3, vl);
	}
	if (opid == EUO_hmgetstr) {
		std::vector<std::string> vs;
		ok = trim_hmget(rpl, vs);
		POK;
		PEQARR(hmgetset_str1, hmgetset_str2, hmgetset_str3, vs);
	}

}


int main()
{
	URedisMultiCfg c1(0, 50, "127.0.0.1", 6379);
	URedisMultiCfg c2(50, 100, "127.0.0.1", 6379);

	std::vector<URedisMultiCfg> cfg;
	cfg.push_back(c1);
	cfg.push_back(c2);

	uv_loop_t* loop = uv_default_loop();
	URedis* p = new URedis(loop, EConnMod_100, EConnMod_1, cfg);

	URedis& r = *p;

	User u(123, 1001);

#define PR cout << __FUNCTION__ << __LINE__ << " result:" << result << endl;

	int result = 0;

	const char* key_ne = "key_not_exist";
	const char* field_ne = "field_not_exist";
	const char* field_ne1 = "field_not_exist1";
	const char* field_ne2 = "field_not_exist2";
	const char* field_ne3 = "field_not_exist3";

	// get set
	u.getset_lld = 666;
	u.getset_str = "qqqqqqqqqq777";
	const char* gsk = "getset_key";
	const char* gsks = "getset_key_str";
	result = r.cmd(u.uid, EUO_setlld, nullptr, nullptr, "SET %s %lld", gsk, u.getset_lld); PR;
	result = r.cmd(u.uid, EUO_getlld, test_cb, &u, "GET %s", gsk); PR;
	result = r.cmd(u.uid, EUO_setstr, nullptr, nullptr, "SET %s %s", gsks, u.getset_str.c_str()); PR;
	result = r.cmd(u.uid, EUO_getstr, test_cb, &u, "GET %s", gsks); PR;
	result = r.cmd(u.uid, EUO_getlld, test_cb, nullptr, "GET %s", key_ne); PR;
	
	// mget mset
	u.mgetset_lld1 = 888;
	u.mgetset_lld2 = 999;
	u.mgetset_lld3 = 12345;
	u.mgetset_str1 = "hello";
	u.mgetset_str2 = "world";
	u.mgetset_str3 = "hello world";
	const char *mgsk1 = "mgetset_key1", *mgsk2 = "mgetset_key2", *mgsk3 = "mgetset_key3";
	const char *mgsk1s = "mgetset_key1s", *mgsk2s = "mgetset_key2s", *mgsk3s = "mgetset_key3s";
	result = r.cmd(u.uid, EUO_msetlld, nullptr, nullptr, "MSET %s %lld %s %lld %s %lld", mgsk1, u.mgetset_lld1, mgsk2, u.mgetset_lld2, mgsk3, u.mgetset_lld3); PR;
	result = r.cmd(u.uid, EUO_mgetlld, test_cb, &u, "MGET %s %s %s", mgsk1, mgsk2, mgsk3);PR;
	result = r.cmd(u.uid, EUO_msetstr, nullptr, nullptr, "MSET %s %s %s %s %s %s", mgsk1s, u.mgetset_str1.c_str(), mgsk2s, u.mgetset_str2.c_str(), mgsk3s, u.mgetset_str3.c_str()); PR;
	result = r.cmd(u.uid, EUO_mgetstr, test_cb, &u, "MGET %s %s %s", mgsk1s, mgsk2s, mgsk3s);PR;
	result = r.cmd(u.uid, EUO_mgetstr, test_cb, nullptr, "MGET %s %s %s", field_ne1, field_ne2, field_ne3);PR;


	// hget hset
	u.hgetset_lld = 66666;
	u.hgetset_str = "77777asdf";
	const char* hgsk = "hgetset_key";
	const char* hgsf = "hgetset_field";
	const char* hgsks = "hgetset_key_str";
	const char* hgsfs = "hgetset_field_str";
	result = r.cmd(u.uid, EUO_hsetlld, nullptr, nullptr, "HSET %s %s %lld", hgsk, hgsf, u.hgetset_lld); PR;
	result = r.cmd(u.uid, EUO_hgetlld, test_cb, &u, "HGET %s, %s", hgsk, hgsf); PR;
	result = r.cmd(u.uid, EUO_hsetstr, nullptr, nullptr, "HSET %s %s %s", hgsks, hgsfs, u.hgetset_str.c_str()); PR;
	result = r.cmd(u.uid, EUO_hgetstr, test_cb, &u, "HGET %s %s", hgsks, hgsfs); PR;
	result = r.cmd(u.uid, EUO_hgetlld, test_cb, nullptr, "HGET %s %s", key_ne, field_ne); PR;
	
	// hmget hmset
	u.hmgetset_lld1 = 111888;
	u.hmgetset_lld2 = 222999;
	u.hmgetset_lld3 = 33312345;
	u.hmgetset_str1 = "hhhhello";
	u.hmgetset_str2 = "hhhworld";
	u.hmgetset_str3 = "hhhhello world";
	const char* hmgsk = "hmgetset_key";
	const char* hmgsks = "hmgetset_key_str";
	const char *hmgsf1 = "hmgetset_field1", *hmgsf2 = "hmgetset_field2", *hmgsf3 = "hmgetset_field3";
	const char *hmgsf1s = "hmgetset_field1s", *hmgsf2s = "hmgetset_field2s", *hmgsf3s = "hmgetset_field3s";
	result = r.cmd(u.uid, EUO_hmsetlld, nullptr, nullptr, "HMSET %s %s %lld %s %lld %s %lld", hmgsk, hmgsf1, u.hmgetset_lld1, hmgsf2, u.hmgetset_lld2, hmgsf3, u.hmgetset_lld3); PR;
	result = r.cmd(u.uid, EUO_hmgetlld, test_cb, &u, "HMGET %s %s %s %s", hmgsk, hmgsf1, hmgsf2, hmgsf3);PR;
	result = r.cmd(u.uid, EUO_hmsetstr, nullptr, nullptr, "HMSET %s %s %s %s %s %s %s", hmgsks, hmgsf1s, u.hmgetset_str1.c_str(), hmgsf2s, u.hmgetset_str2.c_str(), hmgsf3s, u.hmgetset_str3.c_str()); PR;
	result = r.cmd(u.uid, EUO_hmgetstr, test_cb, &u, "HMGET %s %s %s %s", hmgsks, hmgsf1s, hmgsf2s, hmgsf3s);PR;
	result = r.cmd(u.uid, EUO_hmgetstr, test_cb, nullptr, "HMGET %s %s %s %s", key_ne, field_ne1, field_ne2, field_ne3);PR;


	uv_run(loop, UV_RUN_DEFAULT);

	delete p;

	return 0;
}

