#ifndef GLMARK2_NATIVE_STATE_WAYLAND_H_
#define GLMARK2_NATIVE_STATE_WAYLAND_H_

#include <stdint.h>

#include "native-state.h"
#include <wayland-client.h>
#include <wayland-egl.h>

#include <EGL/egl.h>

class NativeStateWayland : public NativeState
{
public:
    NativeStateWayland();
    ~NativeStateWayland();

    bool init_display();
    void* display();
    bool create_window(WindowProperties const& properties);
    void* window(WindowProperties& properties);
    void visible(bool v);
    bool should_quit();
    void flip();

private:
    static const struct wl_registry_listener registry_listener_;
    static const struct wl_shell_surface_listener shell_surface_listener_;

    static void
    registry_handle_global(void *data, struct wl_registry *registry,
                   uint32_t id, const char *interface, uint32_t version);
    static void
    registry_handle_global_remove(void *data, struct wl_registry *registry,
                      uint32_t name);

    static void
    handle_ping(void *data, struct wl_shell_surface *shell_surface,
                                uint32_t serial);
    static void
    handle_configure(void *data, struct wl_shell_surface *shell_surface,
             uint32_t edges, int32_t width, int32_t height);
    static void
    handle_popup_done(void *data, struct wl_shell_surface *shell_surface);

    struct my_display {
        wl_display *display;
        wl_registry *registry;
        wl_compositor *compositor;
        wl_shell *shell;
    } *display_;

    struct my_window {
        WindowProperties properties;
        struct wl_surface *surface;
        struct wl_egl_window *native;
        struct wl_shell_surface *shell_surface;
    } *window_;
};

#endif /* GLMARK2_NATIVE_STATE_WAYLAND_H_ */
