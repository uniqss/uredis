#pragma once

#include <vector>

#include <hiredis.h>
#include <async.h>
#include <adapters/libuv.h>
#include "URedisMultiCfg.h"
#include "uredis_helper.h"

#include <iostream>

struct ConnCfg {
    redisAsyncContext* c = nullptr;
    std::string ip;
    int port = 0;
};

class PrivDataWrapper {
   public:
    PrivDataWrapper() : id(0), opid(0), privdata(nullptr), cbfn(nullptr) {}

    PrivDataWrapper(int64_t _id, int64_t _opid, void* _privdata, redisCbFn* fn)
        : id(_id), opid(_opid), privdata(_privdata), cbfn(fn) {}

    int64_t id;
    int64_t opid;
    void* privdata;
    redisCbFn* cbfn;
};

inline void redis_implcb(redisAsyncContext* c, void* r, void* privdata) {
    redisReply* reply = (redisReply*)r;
    if (reply == nullptr) {
        std::cout << __FUNCTION__ << "error occurred, reply == nullptr";
        return;
    }
    if (privdata == nullptr) {
        std::cout << __FUNCTION__ << "privdata is null" << std::endl;
        return;
    }
    PrivDataWrapper* p = (PrivDataWrapper*)privdata;
    if (p == nullptr) {
        std::cout << __FUNCTION__ << "PrivDataWrapper null";
        return;
    }
    if (p->cbfn != nullptr) {
        (p->cbfn)(p->id, p->opid, reply, p->privdata);
    }
    delete p;
}

inline void connectCallback(const redisAsyncContext* c, int status) {
    if (status != REDIS_OK) {
        std::cout << __FUNCTION__ << "Error: " << c->errstr << std::endl;
        return;
    }
    // std::cout << "Connected..." << std::endl;
}

inline void disconnectCallback(const redisAsyncContext* c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    // printf("Disconnected...\n");
}

class URedisImpl {
   public:
    URedisImpl(uv_loop_t* loop, EConnMod connMod, EConnMod dbMod,
               const std::vector<URedisMultiCfg>& rCfg) {
        if (rCfg.empty()) {
            throw new std::logic_error("rCfg must be not empty");
        }
        if (loop == nullptr) {
            throw new std::logic_error("loop is null");
        }
        this->loop = loop;
        cmod = connMod;
        dmod = dbMod;

        /*
            int mod = (int)connMod * (int)dbMod;
           if (mod > 1000)
           {
           throw new std::logic_error("mod too large. you should consider use multi processes.");
           }
           */

        cs.resize(connMod);
        int initsum = 0;
        for (const auto& it : rCfg) {
            if (it.port <= 0) {
                throw new std::logic_error(
                    (std::string("cfg error, port ") + std::to_string(it.port)));
            }
            for (int i = it.start; i < it.stop; ++i) {
                if (cs[i].port > 0) {
                    throw new std::logic_error("config multidefined.");
                }
                cs[i].ip = it.ip;
                cs[i].port = it.port;
                ++initsum;
            }
        }
        if (initsum < connMod) {
            throw new std::logic_error(
                "cfg not covered full mod range.check [start, stop) cfgs for detail.");
        }
    }

    ~URedisImpl() {}

    // privdata can be set null if not needed
    int cmd(int64_t id, int64_t opid, redisCbFn* cb, void* privdata, const char* format,
            va_list ap) {
        ConnCfg* cfg = nullptr;
        if (!_reconnIfNeeded(id, cfg)) return -1;

        PrivDataWrapper* p = nullptr;

        int status = 0;
        if (cb) {
            p = new PrivDataWrapper(id, opid, privdata, cb);
            status = redisvAsyncCommand(cfg->c, redis_implcb, p, format, ap);
        } else {
            status = redisvAsyncCommand(cfg->c, nullptr, nullptr, format, ap);
        }

        return status;
    }
    // privdata can be set null if not needed
    int cmd(int64_t id, int64_t opid, redisCbFn* cb, void* privdata, const char* cmd, int len) {
        ConnCfg* cfg = nullptr;
        if (!_reconnIfNeeded(id, cfg)) return -1;

        PrivDataWrapper* p = nullptr;

        int status = 0;
        if (cb) {
            p = new PrivDataWrapper(id, opid, privdata, cb);
            status = redisAsyncFormattedCommand(cfg->c, redis_implcb, p, cmd, len);
        } else {
            status = redisAsyncFormattedCommand(cfg->c, nullptr, nullptr, cmd, len);
        }

        return status;
    }

   private:
    bool _reconnIfNeeded(int64_t id, ConnCfg*& cfg) {
        // connection check.
        auto mod = id % cmod;
        cfg = &cs[mod];
        if (cfg->c == nullptr) {
            cfg->c = redisAsyncConnect(cfg->ip.c_str(), cfg->port);
            if (cfg->c->err) {
                /* Let *c leak for now... */
                printf("Error: %s\n", cfg->c->errstr);
                return false;
            }
            redisLibuvAttach(cfg->c, loop);
            redisAsyncSetConnectCallback(cfg->c, connectCallback);
            redisAsyncSetDisconnectCallback(cfg->c, disconnectCallback);
        }

        return true;
    }

   private:
    std::vector<ConnCfg> cs;
    uv_loop_t* loop;
    EConnMod cmod;
    EConnMod dmod;
};
