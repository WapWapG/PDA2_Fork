#pragma once
#include <PDA2.h>

extern "C" {
#include <zbuffer.h>
}
#include <TGL/gl.h>

class TinyGLTestApp : public PDA2App {
public:
    TinyGLTestApp() { name = "TinyGL test"; }
    void onInit() override;
    void onOpen() override;
    void onTick(uint32_t delta_ms) override;
    void onClose() override;

private:
    ZBuffer* _zb = nullptr;
    float _angle = 0.0f;
};