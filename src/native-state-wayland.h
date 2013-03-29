#ifndef GLMARK2_NATIVE_STATE_WAYLAND_H_
#define GLMARK2_NATIVE_STATE_WAYLAND_H_

#include "native-state.h"

class NativeStateWayland : public NativeState
{
public:
    NativeStateWayland();

    bool init_display();
    void* display();
    bool create_window(WindowProperties const& properties);
    void* window(WindowProperties& properties);
    void visible(bool v);
    bool should_quit();
    void flip();
};

#endif /* GLMARK2_NATIVE_STATE_WAYLAND_H_ */
