// // TestRenderApp.h
// #pragma once
// #include <PDA2.h>

// class TestRenderApp : public PDA2App {
// public:
//     TestRenderApp() { name = "TestRender"; }

//     void onInit()  override;
//     void onOpen()  override;
//     void onClose() override;
//     void onTick(uint32_t delta_ms) override;

// private:
//     // screen — унаследовано из PDA2App, свой не объявляем (см. ClockApp)
//     lv_obj_t* square    = nullptr;
//     lv_obj_t* fpsLabel  = nullptr;
//     lv_obj_t* blinkLabel = nullptr;

//     float    angle = 0.0f;

//     uint32_t frameCount   = 0;
//     uint32_t fpsAcc_ms    = 0;
//     uint32_t blinkAcc_ms  = 0;
//     bool     blinkState   = false;

//     static constexpr int   CX = 160;
//     static constexpr int   CY = 240;
//     static constexpr int   RADIUS = 80;
//     static constexpr int   SQUARE_SIZE = 30;
//     static constexpr float ANGLE_STEP_PER_SEC = 3.0f;
// };