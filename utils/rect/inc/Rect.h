#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

    typedef struct SDL_BPoint
    {
        bool x;
        bool y;
    }SDL_BPoint;

    inline bool SDL_RectContainsRectFloat(const SDL_FRect* A, const SDL_FRect* B)
    {
        return (B->x >= A->x)
            && (B->y >= A->y)
            && (B->x + B->w <= A->x + A->w)
            && (B->y + B->h <= A->y + A->h);
    }

#ifdef __cplusplus
}
#endif // __cplusplus

namespace utils
{
    enum class Alignment {
        NONE,               //不指定对齐方式，偏移量通常为{0，0}或手动设置
        TOP_LEFT,           //左上角
        TOP_CENTER,         //顶部中心
        TOP_RIGHT,          //右上角
        CENTER_LEFT,        //中心左側
        CENTER,             //正中心（几何中心）
        CENTER_RIGHT,       //中心右侧
        BOTTOM_LEFT,        //左下角
        BOTTOM_CENTER,       //底部中心
        BOTTOM_RIGHT,        //右下角
        ALIGNMENT_COUNT
    };

    typedef struct SDL_Position
    {
        float x;
        float y;
    }SDL_Position;

    typedef struct SDL_Size
    {
        float w;
        float h;
    }SDL_Size;
}

namespace utils::rect
{
    //SDL_Point加法操作
    inline SDL_FPoint operator+(const SDL_FPoint& a, const SDL_FPoint& b) {
        return { a.x + b.x, a.y + b.y };
    }

    inline SDL_FPoint operator+(const SDL_FPoint& a, const SDL_Point& b) {
        return { a.x + b.x, a.y + b.y };
    }

    inline SDL_FPoint operator+(const SDL_Point& a, const SDL_FPoint& b) {
        return { a.x + b.x, a.y + b.y };
    }

    inline SDL_FPoint operator+(float a, const SDL_FPoint& b) {
        return { a + b.x, a + b.y };
    }

    inline SDL_FPoint operator+(const SDL_FPoint& a, float b) {
        return { a.x + b, a.y + b };
    }

    inline SDL_FPoint& operator+=(SDL_FPoint& a, const SDL_FPoint& b) {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    inline SDL_FPoint& operator+=(SDL_FPoint& a, float b) {
        a.x += b;
        a.y += b;
        return a;
    }

    inline SDL_FPoint operator-(const SDL_FPoint& a, const SDL_FPoint& b) {
        return { a.x - b.x, a.y - b.y };
    }

    inline SDL_FPoint operator-(const SDL_Point& a, const SDL_FPoint& b) {
        return { a.x - b.x, a.y - b.y };
    }

    inline SDL_FPoint operator-(const SDL_FPoint& a, const SDL_Point& b) {
        return { a.x - b.x, a.y - b.y };
    }

    //常量的两种减法的结果不一样，注意区分
    inline SDL_FPoint operator-(float a, const SDL_FPoint& b) {
        return { a - b.x, a - b.y };
    }
    //常量的两种减法的结果不一样，注意区分
    inline SDL_FPoint operator-(const SDL_FPoint& a, float b) {
        return { a.x - b, a.y - b };
    }

    inline SDL_FPoint& operator-=(SDL_FPoint& a, const SDL_FPoint& b) {
        a.x -= b.x;
        a.y -= b.y;
        return a;
    }

    inline SDL_FPoint& operator-=(SDL_FPoint& a, float b) {
        a.x -= b;
        a.y -= b;
        return a;
    }

    inline SDL_FPoint operator*(const SDL_FPoint& a, float s) {
        return { a.x * s, a.y * s };
    }

    inline SDL_FPoint operator*(float s, const SDL_FPoint& p) {
        return { p.x * s, p.y * s };
    }

    inline SDL_FPoint operator*(const SDL_FPoint& a, const SDL_FPoint& b) {
        return { a.x * b.x, a.y * b.y };
    }

    inline SDL_FPoint& operator*=(SDL_FPoint& a, float b) {
        a.x *= b;
        a.y *= b;
        return a;
    }
    //FPoint的除法
    inline SDL_FPoint operator/(const SDL_FPoint& a, float b) {
        return { a.x / b, a.y / b };
    }

    inline SDL_FPoint operator/(const SDL_FPoint& a, const SDL_FPoint& b) {
        return { a.x / b.x, a.y / b.y };
    }
    //点限制函数
    inline SDL_FPoint SDL_FPoint_clamp(const SDL_FPoint& a, float min, float max)
    {
        return { SDL_clamp(a.x,min,max),SDL_clamp(a.y,min,max) };
    }

    inline SDL_FPoint SDL_FPoint_clamp(const SDL_FPoint& a, const SDL_FPoint& min, const SDL_FPoint& max)
    {
        return { SDL_clamp(a.x,min.x,max.x),SDL_clamp(a.y,min.y,max.y) };
    }

    inline SDL_FPoint SDL_FPoint_abs(const SDL_FPoint& a)
    {
        return SDL_FPoint{ SDL_fabsf(a.x), SDL_fabsf(a.y) };;
    }

    inline float SDL_FPoint_distance(const SDL_FPoint& a, const SDL_FPoint& b)
    {
        const float dx = b.x - a.x;
        const float dy = b.y - a.y;
        return SDL_sqrt(dx * dx + dy * dy);
    }

    inline float SDL_FPoint_distance(const SDL_FRect& a, const SDL_FPoint& b)
    {
        const float dx = b.x - a.x;
        const float dy = b.y - a.y;
        return SDL_sqrt(dx * dx + dy * dy);
    }

    inline float SDL_FPoint_distance(const SDL_FPoint& a, const SDL_FRect& b)
    {
        const float dx = b.x - a.x;
        const float dy = b.y - a.y;
        return SDL_sqrt(dx * dx + dy * dy);
    }
    inline float SDL_FPoint_distance(const SDL_FRect& a, const SDL_FRect& b)
    {
        const float dx = b.x - a.x;
        const float dy = b.y - a.y;
        return SDL_sqrt(dx * dx + dy * dy);
    }

    inline SDL_FPoint SDL_FPoint_mix(const SDL_FPoint& a, const SDL_FPoint& b, float t)
    {
        return SDL_FPoint{ a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
    }


    //此函数只接受两个含有点的数组
    inline bool SDL_TwoSegmentInterset(const SDL_FPoint* a, const SDL_FPoint* b, SDL_FPoint* intersection)
    {
        if (!a || !b || !intersection)return false;
        float s1x = a[1].x - a[0].x;
        float s1y = a[1].y - a[0].y;
        float s2x = b[1].x - b[0].x;
        float s2y = b[1].y - b[0].y;

        float denom = -s2x * s1y + s1x * s2y;
        if (SDL_fabsf(denom) < SDL_FLT_EPSILON) return false; // 平行或共线
        float s = (-s1y * (a[0].x - b[0].x) + s1x * (a[0].y - b[0].y)) / denom;
        float t = (s2x * (a[0].y - b[0].y) - s2y * (a[0].x - b[0].x)) / denom;

        if (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f)
        {
            intersection->x = a[0].x + t * s1x;
            intersection->y = a[0].y + t * s1y;
            return true;
        }
        return true;
    }
    //SDL_Point

    inline SDL_Point operator+(const SDL_Point& a, const SDL_Point& b) {
        return { a.x + b.x, a.y + b.y };
    }

    inline SDL_Point operator+(int a, const SDL_Point& b) {
        return { a + b.x, a + b.y };
    }

    inline SDL_Point operator+(const SDL_Point& a, int b) {
        return { a.x + b, a.y + b };
    }

    inline SDL_Point& operator+=(SDL_Point& a, const SDL_Point& b) {
        a.x += b.x;
        a.y += b.y;
        return a;
    }

    inline SDL_Point& operator+=(SDL_Point& a, int b) {
        a.x += b;
        a.y += b;
        return a;
    }

    inline SDL_Point operator-(const SDL_Point& a, const SDL_Point& b) {
        return { a.x - b.x, a.y - b.y };
    }
    //常量的两种减法的结果不一样，注意区分
    inline SDL_Point operator-(int a, const SDL_Point& b) {
        return { a - b.x, a - b.y };
    }
    //常量的两种减法的结果不一样，注意区分
    inline SDL_Point operator-(const SDL_Point& a, int b) {
        return { a.x - b, a.y - b };
    }

    inline SDL_Point& operator-=(SDL_Point& a, const SDL_Point& b) {
        a.x -= b.x;
        a.y -= b.y;
        return a;
    }

    inline SDL_Point& operator-=(SDL_Point& a, int b) {
        a.x -= b;
        a.y -= b;
        return a;
    }

    inline SDL_Point operator*(const SDL_Point& a, int s) {
        return { a.x * s, a.y * s };
    }

    inline SDL_Point operator*(int s, const SDL_Point& p) {
        return { p.x * s, p.y * s };
    }

    inline SDL_Point operator*(const SDL_Point& a, const SDL_Point& b) {
        return { a.x * b.x, a.y * b.y };
    }

    inline SDL_Point& operator*(SDL_Point& a, int b) {
        a.x *= b;
        a.y *= b;
        return a;
    }

    //SDL_FRect
}


