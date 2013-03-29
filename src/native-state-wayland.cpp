#include "native-state-wayland.h"

NativeStateWayland::NativeStateWayland()
{
}

bool NativeStateWayland::init_display()
{
    return false;
}

void* NativeStateWayland::display()
{
    return 0;
}

bool NativeStateWayland::create_window(WindowProperties const&/* properties*/)
{
    return false;
}

void* NativeStateWayland::window(WindowProperties&/* properties*/)
{
    return 0;
}

void NativeStateWayland::visible(bool/* v*/)
{

}

bool NativeStateWayland::should_quit()
{
    return false;
}

void NativeStateWayland::flip()
{

}
