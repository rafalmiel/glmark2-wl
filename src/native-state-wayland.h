/*
 * Copyright © 2013 Rafal Mielniczuk
 *
 * This file is part of the glmark2 OpenGL (ES) 2.0 benchmark.
 *
 * glmark2 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * glmark2 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * glmark2.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Rafal Mielniczuk <rafal.mielniczuk2@gmail.com>
 */

#ifndef GLMARK2_NATIVE_STATE_WAYLAND_H_
#define GLMARK2_NATIVE_STATE_WAYLAND_H_

#include <vector>
#include <csignal>
#include <EGL/egl.h>
#include <xkbcommon/xkbcommon.h>

#include "native-state.h"
#include <wayland-client.h>
#include <wayland-egl.h>


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
    static const struct wl_keyboard_listener keyboard_listener_;
    static const struct wl_seat_listener seat_listener_;
    static const struct wl_output_listener output_listener_;

    static void
    registry_handle_global(void *data, struct wl_registry *registry,
                           uint32_t id, const char *interface, uint32_t version);
    static void
    registry_handle_global_remove(void *data, struct wl_registry *registry,
                                  uint32_t name);

    static void
    output_handle_geometry(void *data, struct wl_output *wl_output,
             int32_t x, int32_t y, int32_t physical_width, int32_t physical_height,
             int32_t subpixel, const char *make, const char *model, int32_t transform);

    static void
    output_handle_mode(void *data, struct wl_output *wl_output, uint32_t flags,
             int32_t width, int32_t height, int32_t refresh);

    static void
    shell_surface_handle_ping(void *data, struct wl_shell_surface *shell_surface,
                              uint32_t serial);
    static void
    shell_surface_handle_configure(void *data, struct wl_shell_surface *shell_surface,
                                   uint32_t edges, int32_t width, int32_t height);
    static void
    shell_surface_handle_popup_done(void *data, struct wl_shell_surface *shell_surface);

    static void
    keyboard_handle_keymap(void *data, struct wl_keyboard *wl_keyboard,
                           uint32_t format, int32_t fd, uint32_t size);

    static void
    keyboard_handle_enter(void *data, struct wl_keyboard *wl_keyboard,
                          uint32_t serial, struct wl_surface *surface,
                          struct wl_array *keys);

    static void
    keyboard_handle_leave(void *data, struct wl_keyboard *wl_keyboard,
                          uint32_t serial, struct wl_surface *surface);

    static void
    keyboard_handle_key(void *data, struct wl_keyboard *wl_keyboard,
                        uint32_t serial, uint32_t time, uint32_t key,
                        uint32_t state);

    static void
    keyboard_handle_modifiers(void *data, struct wl_keyboard *wl_keyboard,
                              uint32_t serial, uint32_t mods_depressed,
                              uint32_t mods_latched, uint32_t mods_locked,
                              uint32_t group);

    static void
    seat_handle_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities);

    struct my_output {
        wl_output *output;
        int32_t width, height;
        int32_t refresh;
    };

    struct my_display {
        wl_display *display;
        wl_registry *registry;
        wl_compositor *compositor;
        wl_shell *shell;
        std::vector<struct my_output *> outputs;
        struct xkb_context *xkb_context;
    } *display_;

    struct my_window {
        WindowProperties properties;
        struct wl_surface *surface;
        struct wl_egl_window *native;
        struct wl_shell_surface *shell_surface;
    } *window_;

    struct my_input {
        wl_seat *seat;
        wl_keyboard *keyboard;

        struct {
            struct xkb_keymap *keymap;
            struct xkb_state *state;
            xkb_mod_mask_t control_mask;
            xkb_mod_mask_t alt_mask;
            xkb_mod_mask_t shift_mask;
        } xkb;
    } *input_;

    static volatile bool should_quit_;
};

#endif /* GLMARK2_NATIVE_STATE_WAYLAND_H_ */
