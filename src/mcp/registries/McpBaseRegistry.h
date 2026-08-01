// BaseRegistry.h
// Contains a class that can be used to implement custom registries that are used to keep track of
// functions that can be identified by a string, such as tools, resources, prompts, MCP methods,
// etc.

#pragma once
#include <functional>
#include <map>
#include <ArduinoJson.h>

template<typename HandlerFn>
class McpBaseRegistry {
    public:
        static std::map<String, HandlerFn>& handlers() {
            static std::map<String, HandlerFn> instance;
            return instance;
        }

        static bool registerHandler(String method, const HandlerFn fn) {
            handlers()[method] = std::move(fn);
            return true;
        }
};