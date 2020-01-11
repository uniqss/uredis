# uredis

- uniqs redis wrapper of hiredis.
- async only.
- with redis instance splitter and db spliter(most frequently used is different userid's cache saved in different redis instances or different dbs). currently db splitter not suported.
- if you want to use async zset functions for some rank usage, just have multi URedis instances, one for rank, and set the rank config to EConnMod_1, then the rank data will not be splitted to different instances or different dbs.

