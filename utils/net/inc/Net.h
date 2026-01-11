#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace utils::net {
    class Net {
    public:
        Net(/* args */) = default;
        ~Net() = default;
    public:
    private:
        /* data */
        void* curl;
        nlohmann::json urlData_;
    };
    bool Init();
    void Close();
    bool Test();
}