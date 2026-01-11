#include "../inc/File.h"
#include "utils/log/inc/Log.h"

namespace utils::file
{
    bool CreateDirectory(const std::string& filePath)
    {
        size_t pos = 0;
        while (true)
        {
            pos = filePath.find('/', pos + 1);
            std::string subPath = filePath.substr(0, pos);
            if (subPath.empty()) continue;
            SDL_PathInfo info;
            if (SDL_GetPathInfo(subPath.c_str(), &info)) {
                if (info.type != SDL_PATHTYPE_DIRECTORY) {
                    // 已存在但不是目录 → 删除并创建目录
                    log::Warn("路径存在但非目录，删除文件: {}", subPath);
                    if (!SDL_RemovePath(subPath.c_str())) {
                        log::Error("删除 {} 失败: {}", subPath, SDL_GetError());
                        return false;
                    }
                    if (!SDL_CreateDirectory(subPath.c_str())) {
                        log::Error("创建 {} 目录失败: {}", subPath, SDL_GetError());
                        return false;
                    }
                }
            } else {
                // 不存在 → 创建目录
                if (!SDL_CreateDirectory(subPath.c_str())) {
                    log::Error("创建 {} 目录失败: {}", subPath, SDL_GetError());
                    return false;
                }
            }
            if (pos == std::string::npos)break;
        }
        return true;
    }
    bool ReadFile(const std::string& filePath, std::string& outContent)
    {
        SDL_IOStream* ioStream = SDL_IOFromFile(filePath.c_str(), "r");
        if (!ioStream)
        {
            log::Error("无法打开文件：{}", filePath);
            return false;
        }
        Sint64 ioSize = SDL_GetIOSize(ioStream);
        if (ioSize <= 0)
        {
            log::Error("{} 文件为空: {}", filePath, SDL_GetError());
            SDL_CloseIO(ioStream);
            return false;
        }
        // 读取文件内容到字符串
        outContent.resize(ioSize);
        Sint64 readBytes = SDL_ReadIO(ioStream, outContent.data(), ioSize);
        SDL_CloseIO(ioStream);
        if (readBytes != ioSize) {
            log::Error("{} 读取文件失败: {}", filePath, SDL_GetError());
            outContent.clear();
            return false;
        }
        log::Debug("读取 {} 文件成功: {}", filePath, outContent.size());
        return true;
    }
}