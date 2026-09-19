#pragma once

#include <stdint.h>

#ifndef PDA2_SIM

namespace lgfx { inline namespace v1 {
    class LGFX_Device;   // inline — обязательно, иначе конфликт с реальным объявлением в LovyanGFX
    struct rgb565_t;     // forward-declare, полное определение — в LovyanGFX.hpp (только .cpp, куда он реально нужен)
} }

// Тип указателя на пиксель для writePixelsDMA() — платформенный.
// render.cpp вызывает writePixelsDMA() один раз, без #ifdef,
// приводя im.data() к этому типу; конкретный тип разный по платформам.
using pda2_pixel_ptr_t = lgfx::rgb565_t*;

#else

// Минимальная PC-совместимая замена LGFX_Device для raw().
// Раньше здесь был только pushImage() — высокоуровневый метод
// LovyanGFX, который сам конвертирует/свопает переданные пиксели.
// На нём был найден цветовой баг GLTestApp на физическом PDA2
// (фиолетовый уезжал в зелёный/циан, см. session doc, TGX-COLOR-1) —
// pushImage() убран. Вместо него — тот же низкоуровневый путь,
// что уже подтверждённо даёт верные цвета в LVGL flush_cb на обеих
// платформах: startWrite/setAddrWindow/writePixelsDMA/endWrite.
// Эмулирует контракт реального LGFX_Device под ту же сигнатуру вызова.
class PdaRawDisplay_Sim {
public:
    void startWrite();
    void setAddrWindow(int32_t x, int32_t y, int32_t w, int32_t h);
    void writePixelsDMA(const uint16_t* data, uint32_t len);
    void endWrite();

private:
    int32_t _win_x = 0, _win_y = 0, _win_w = 0, _win_h = 0;
};

using pda2_pixel_ptr_t = uint16_t*;

#endif

class Display_Class {
public:
    bool    begin(uint8_t brightness, uint8_t rotation);

    void    setBrightness(uint8_t v);
    uint8_t getBrightness();

    void    setRotation(uint8_t r);
    uint8_t getRotation();

    int     width();
    int     height();

    // ⚠️ Прямой доступ к display-объекту, в обход общей абстракции Display_Class.
    // Осознанное исключение из ПРАВИЛА 4 — только для GLTestApp (см. session doc).
    // Тип раздваивается по платформе: на ESP32 — реальный LGFX_Device,
    // на симуляторе — PdaRawDisplay_Sim с тем же набором методов
    // (startWrite/setAddrWindow/writePixelsDMA/endWrite). Вызывающий код
    // (render.cpp) берёт тип через auto&, поэтому ему всё равно, какая
    // из двух реализаций подставлена.
#ifndef PDA2_SIM
    lgfx::v1::LGFX_Device& raw();
#else
    PdaRawDisplay_Sim& raw();
#endif

private:
    uint8_t _brightness = 200;
    uint8_t _rotation   = 2;
};