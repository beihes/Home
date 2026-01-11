#include "../inc/TextRenderer.h"
#include "../inc/Camera.h"
#include "resource/inc/ResourceManager.h"
#include "utils/log/inc/Log.h"
#include <stdexcept>

using namespace utils;

namespace engine::render {

    TextRenderer::TextRenderer(SDL_Renderer* renderer, resource::ResourceManager* resourceManager)
        : renderer_(renderer),
        resourceManager_(resourceManager)
    {
        if (!this->renderer_ || !this->resourceManager_) {
            throw std::runtime_error("TextRenderer 需要一个有效的 SDL_Renderer 和 ResourceManager。");
        }
        // 初始化 SDL_ttf
        if (!TTF_WasInit() && TTF_Init() == false) {
            throw std::runtime_error("初始化 SDL_ttf 失败: " + std::string(SDL_GetError()));
        }

        this->textEngine_ = TTF_CreateRendererTextEngine(this->renderer_);
        if (!this->textEngine_) {
            log::Error("[{}]创建 TTF_TextEngine 失败: {}",this->GetClassName(), SDL_GetError());
            throw std::runtime_error("创建 TTF_TextEngine 失败。");
        }
        log::Trace("[{}]TextRenderer 初始化成功.",this->GetClassName());
    }

    TextRenderer::~TextRenderer() {
        if (this->textEngine_) {
            close();
        }
    }

    void TextRenderer::close()
    {
        if (this->textEngine_) {
            TTF_DestroyRendererTextEngine(this->textEngine_);
            this->textEngine_ = nullptr;
            log::Trace("[{}]TTF_TextEngine 销毁。",this->GetClassName());
        }
        TTF_Quit();     // 一定要确保在ResourceManager销毁之后调用
    }

    void TextRenderer::DrawUIText(const std::string& text, const std::string& fontId, int fontSize, const SDL_FPoint& position, const SDL_FColor& color)
    {
        /* 构造函数已经保证了必要指针不会为空，这里不需要再检查 */
        TTF_Font* font = this->resourceManager_->GetFont(fontId, fontSize);
        if (!font) {
            log::Warn("[{}]DrawUIText 获取字体失败: {} 大小 {}",this->GetClassName(), fontId, fontSize);
            return;
        }

        // 创建临时 TTF_Text 对象   (目前效率不高，未来可以考虑使用缓存优化)
        TTF_Text* tempTextObject = TTF_CreateText(this->textEngine_, font, text.data(), 0);
        if (!tempTextObject) {
            log::Error("[{}]DrawUIText 创建临时 TTF_Text 失败: {}", this->GetClassName(),SDL_GetError());
            return;
        }

        // 先渲染一次黑色文字模拟阴影
        TTF_SetTextColorFloat(tempTextObject, 0.0f, 0.0f, 0.0f, 1.0f);
        if (!TTF_DrawRendererText(tempTextObject, position.x + 2, position.y + 2)) {
            log::Error("[{}]DrawUIText 绘制临时 TTF_Text 失败: {}",this->GetClassName(), SDL_GetError());
        }

        // 然后正常绘制
        TTF_SetTextColorFloat(tempTextObject, color.r, color.g, color.b, color.a);
        if (!TTF_DrawRendererText(tempTextObject, position.x, position.y)) {
            log::Error("[{}]DrawUIText 绘制临时 TTF_Text 失败: {}",this->GetClassName(), SDL_GetError());
        }
        // 销毁临时 TTF_Text 对象
        TTF_DestroyText(tempTextObject);
    }

    void TextRenderer::DrawText(const Camera& camera, const std::string& text, const std::string& fontId, int fontSize, const SDL_FPoint& position, const SDL_FColor& color)
    {
        // 应用相机变换
        SDL_FPoint screenPos = camera.WorldToScreen(position);

        // 用新坐标调用drawUIText即可
        this->DrawUIText(text, fontId, fontSize, screenPos, color);
    }

    SDL_FPoint TextRenderer::GetTextSize(const std::string& text, const std::string& fontId, int fontSize) {
        /* 构造函数已经保证了必要指针不会为空，这里不需要再检查 */
        TTF_Font* font = this->resourceManager_->GetFont(fontId, fontSize);
        if (!font) {
            log::Warn("[{}]getTextSize 获取字体失败: {} 大小 {}", this->GetClassName(),fontId, fontSize);
            return { 0 };
        }

        // 创建临时 TTF_Text 对象
        TTF_Text* tempTextObject = TTF_CreateText(this->textEngine_, font, text.data(), 0);
        if (!tempTextObject) {
            log::Error("[{}]getTextSize 创建临时 TTF_Text 失败: {}",this->GetClassName(), SDL_GetError());
            return { 0 };
        }

        int width, height;
        TTF_GetTextSize(tempTextObject, &width, &height);

        // 销毁临时 TTF_Text 对象
        TTF_DestroyText(tempTextObject);

        return SDL_FPoint{ static_cast<float>(width), static_cast<float>(height) };
    }

} // namespace engine::render 