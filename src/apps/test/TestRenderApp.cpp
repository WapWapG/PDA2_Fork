// #include "TestRenderApp.h"
// #include <math.h>

// #ifdef PDA2_TEST_PARTIAL_RENDER
// extern uint32_t pda2_test_get_flush_count();
// #endif

// // ── onInit ────────────────────────────────────────────────
// void TestRenderApp::onInit() {
//     // LVGL не трогаем здесь — по паттерну ClockApp,
//     // построение экрана — в onOpen()
// }

// // ── onOpen — строим UI один раз, дальше просто сброс состояния ──────────
// void TestRenderApp::onOpen() {
//     if (!screen) {
//         screen = lv_obj_create(NULL);
//         lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), 0);
//         lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
//         lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

//         // Квадрат — двигается по кругу
//         square = lv_obj_create(screen);
//         lv_obj_set_size(square, SQUARE_SIZE, SQUARE_SIZE);
//         lv_obj_set_style_bg_color(square, lv_color_hex(0xFF3333), 0);
//         lv_obj_set_style_bg_opa(square, LV_OPA_COVER, 0);
//         lv_obj_set_style_border_width(square, 0, 0);
//         lv_obj_set_pos(square, CX + RADIUS - SQUARE_SIZE / 2, CY - SQUARE_SIZE / 2);

//         // FPS-лейбл — bg_opa=COVER обязателен (правило 9)
//         fpsLabel = lv_label_create(screen);
//         lv_obj_set_style_bg_opa(fpsLabel, LV_OPA_COVER, 0);
//         lv_obj_set_style_bg_color(fpsLabel, lv_color_hex(0x000000), 0);
//         lv_obj_set_style_text_color(fpsLabel, lv_color_hex(0xFFFFFF), 0);
//         lv_label_set_text(fpsLabel, "FPS: 000");
//         lv_obj_align(fpsLabel, LV_ALIGN_TOP_LEFT, 5, 5);

//         // Заголовок — статичный, COVER не обязателен, но не мешает
//         lv_obj_t* title = lv_label_create(screen);
//         lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
//         lv_label_set_text(title, "PARTIAL TEST");
//         lv_obj_align(title, LV_ALIGN_TOP_RIGHT, -5, 5);

//         // Мигающая строка внизу
//         blinkLabel = lv_label_create(screen);
//         lv_obj_set_style_bg_opa(blinkLabel, LV_OPA_COVER, 0);
//         lv_obj_set_style_bg_color(blinkLabel, lv_color_hex(0x000000), 0);
//         lv_obj_set_style_text_color(blinkLabel, lv_color_hex(0x00FF00), 0);
//         lv_label_set_text(blinkLabel, "BLINK: ON");
//         lv_obj_align(blinkLabel, LV_ALIGN_BOTTOM_MID, 0, -5);
//     }

//     // ── Сброс состояния (выполняется при каждом открытии) ──────────────
//     angle       = 0.0f;
//     frameCount  = 0;
//     fpsAcc_ms   = 0;
//     blinkAcc_ms = 0;
//     blinkState  = false;
// }

// // ── onClose ───────────────────────────────────────────────
// void TestRenderApp::onClose() {}

// // ── onTick ────────────────────────────────────────────────
// void TestRenderApp::onTick(uint32_t delta_ms) {
//     // 1. Движение квадрата — без изменений
//     angle += ANGLE_STEP_PER_SEC * (delta_ms / 1000.0f);
//     if (angle > 2 * (float)M_PI) angle -= 2 * (float)M_PI;

//     int x = CX + (int)(RADIUS * cosf(angle)) - SQUARE_SIZE / 2;
//     int y = CY + (int)(RADIUS * sinf(angle)) - SQUARE_SIZE / 2;
//     lv_obj_set_pos(square, x, y);

//     // 2. FPS раз в секунду — считаем реальные flush, не onTick-вызовы
//     fpsAcc_ms += delta_ms;
//     if (fpsAcc_ms >= 1000) {
// #ifdef PDA2_TEST_PARTIAL_RENDER
//         uint32_t flushes = pda2_test_get_flush_count();
//         uint32_t fps = flushes * 1000 / fpsAcc_ms;
//         static uint32_t lastFlushes = 0;
//         fps = (flushes - lastFlushes) * 1000 / fpsAcc_ms;
//         lastFlushes = flushes;
// #else
//         uint32_t fps = 0; // счётчик flush недоступен вне теста — не считаем
// #endif
//         char buf[16];
//         snprintf(buf, sizeof(buf), "FPS: %03lu", (unsigned long)fps);
//         lv_label_set_text(fpsLabel, buf);
//         fpsAcc_ms = 0;
//     }

//     // 3. Мигание — без изменений
//     blinkAcc_ms += delta_ms;
//     if (blinkAcc_ms >= 1000) {
//         blinkState = !blinkState;
//         lv_label_set_text(blinkLabel, blinkState ? "BLINK: ON" : "BLINK: OFF");
//         lv_obj_set_style_text_color(blinkLabel,
//             blinkState ? lv_color_hex(0x00FF00) : lv_color_hex(0x333333), 0);
//         blinkAcc_ms = 0;
//     }
// }