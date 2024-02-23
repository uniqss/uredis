#include "uredis.h"
#include "URedisImpl.h"

URedis::URedis(uv_loop_t* loop, EConnMod connMod, EConnMod dbMod,
               const std::vector<URedisMultiCfg>& rCfg) {
    pImpl = new URedisImpl(loop, connMod, dbMod, rCfg);
}

URedis::~URedis() {
    if (pImpl != nullptr) {
        delete pImpl;
        pImpl = nullptr;
    }
}


// privdata can be set null if not needed
int URedis::cmd(int64_t id, int64_t opid, redisCbFn* cb, void* privdata, const char* format, ...) {
    int status = 0;
    va_list ap;
    va_start(ap, format);
    status = pImpl->cmd(id, opid, cb, privdata, format, ap);
    va_end(ap);

    return status;
}

// privdata can be set null if not needed
int URedis::cmd(int64_t id, int64_t opid, redisCbFn* cb, void* privdata, const char* cmd, int len) {
    return pImpl->cmd(id, opid, cb, privdata, cmd, len);
}
