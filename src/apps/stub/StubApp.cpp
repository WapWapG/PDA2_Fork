// // ════════════════════════════════════════════════════════
// //  StubApp.cpp — см. StubApp.h.
// //  onOpen() строит screen лениво (только при первом вызове),
// //  onClose() не трогает screen (удаляется сам через auto_del /
// //  _screen_del_cb), onTick() ничего не создаёт и не удаляет —
// //  это принципиально для теста: StubApp не должен сам быть
// //  источником UAF.
// // ════════════════════════════════════════════════════════

// #include "StubApp.h"

// void StubApp::onInit() {
//     // без LVGL, состояния для инициализации нет
// }

// void StubApp::onOpen() {
//     if (!screen) {
//         screen = lv_obj_create(nullptr);

//         _label = lv_label_create(screen);
//         lv_label_set_text(_label, "Stub");
//         lv_obj_set_style_bg_opa(_label, LV_OPA_COVER, 0);
//         lv_obj_set_style_bg_color(_label, lv_color_black(), 0);
//         lv_obj_center(_label);
//     }
// }

// void StubApp::onClose() {
//     // намеренно пусто — screen не трогаем, удаляется сам
// }

// void StubApp::onTick(uint32_t delta_ms) {
//     (void)delta_ms;
//     // намеренно пусто — ничего не создаём и не удаляем
// }