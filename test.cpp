#include "uredis.h"

#include<iostream>
using std::cout;
using std::endl;

int main()
{
	URedis* p = new URedis();
	URedis& r = *p;
	if(!r.connect("127.0.0.1", 6379))
	{
		printf("connect error!\n");
		return 0;
	}
	r.set("name", "Mayuyu");
	std::string value;
	bool ok = false;
	ok = r.get("name", value);
	printf("Get the name %d is %s\n", (int)ok, value.c_str());
	ok = r.get("name1", value);
	printf("Get the name1 %d is %s\n", (int)ok, value.c_str());

	ok = r.rpush("t", "asdf");

	std::vector<std::string> values;
	ok = r.lrange("t", 0, -1, values);
	for(auto& it : values) { cout << it << endl;}

	delete p;

	return 0;
}

