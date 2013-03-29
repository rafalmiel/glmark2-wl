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

#include "native-state-wayland.h"

#include <cstring>
#include <cstdio>

const struct wl_registry_listener NativeStateWayland::registry_listener_ = {
    NativeStateWayland::registry_handle_global,
    NativeStateWayland::registry_handle_global_remove
};

const struct wl_shell_surface_listener NativeStateWayland::shell_surface_listener_ = {
    NativeStateWayland::handle_ping,
    NativeStateWayland::handle_configure,
    NativeStateWayland::handle_popup_done
};

NativeStateWayland::NativeStateWayland() : display_(0), window_(0)
{
}

NativeStateWayland::~NativeStateWayland()
{
    if (display_->compositor)
        wl_compositor_destroy(display_->compositor);

    wl_registry_destroy(display_->registry);
    wl_display_flush(display_->display);
    wl_display_disconnect(display_->display);

    delete display_;
    display_ = 0;
}

void
NativeStateWayland::registry_handle_global(void *data, struct wl_registry *registry,
               uint32_t id, const char *interface, uint32_t version)
{
    (void) version;
    struct my_display *display = static_cast<struct my_display *>(data);
    if (strcmp(interface, "wl_compositor") == 0) {
        display->compositor =
                static_cast<struct wl_compositor *>(
                    wl_registry_bind(registry,
                                     id, &wl_compositor_interface, 1));
    } else if (strcmp(interface, "wl_shell") == 0) {
        display->shell =
                static_cast<struct wl_shell *>(
                    wl_registry_bind(registry,
                                     id, &wl_shell_interface, 1));
    }
}

void
NativeStateWayland::registry_handle_global_remove(void *data, struct wl_registry *registry,
                  uint32_t name)
{
    (void) data;
    (void) registry;
    (void) name;
}

void
NativeStateWayland::handle_ping(void *data, struct wl_shell_surface *shell_surface,
                            uint32_t serial)
{
    (void) data;
    wl_shell_surface_pong(shell_surface, serial);
}

void
NativeStateWayland::handle_configure(void *data, struct wl_shell_surface *shell_surface,
         uint32_t edges, int32_t width, int32_t height)
{
    (void) data;
    (void) shell_surface;
    (void) edges;
    (void) width;
    (void) height;
}

void
NativeStateWayland::handle_popup_done(void *data, struct wl_shell_surface *shell_surface)
{
    (void) data;
    (void) shell_surface;
}

bool
NativeStateWayland::init_display()
{
    display_ = new struct my_display;

    if (!display_) {
        return false;
    }

    display_->display = wl_display_connect(NULL);

    if (!display_->display) {
        return false;
    }

    display_->registry = wl_display_get_registry(display_->display);

    wl_registry_add_listener(display_->registry, &registry_listener_, display_);

    wl_display_roundtrip(display_->display);

    return true;
}

void*
NativeStateWayland::display()
{
    return static_cast<void *>(display_->display);
}

bool
NativeStateWayland::create_window(WindowProperties const& properties)
{
    window_ = new struct my_window;
    window_->properties = properties;
    window_->surface = wl_compositor_create_surface(display_->compositor);
    window_->native = wl_egl_window_create(window_->surface, properties.width, properties.height);
    window_->shell_surface = wl_shell_get_shell_surface(display_->shell, window_->surface);

    if (window_->shell_surface)
            wl_shell_surface_add_listener(window_->shell_surface,
                              &shell_surface_listener_, window_);

    wl_shell_surface_set_title(window_->shell_surface, "glmark2");

    wl_shell_surface_set_toplevel(window_->shell_surface);

    return true;
}

void*
NativeStateWayland::window(WindowProperties &properties)
{
    if (window_) {
        properties = window_->properties;
        return window_->native;
    }

    return 0;
}

void
NativeStateWayland::visible(bool/* v*/)
{
}

bool
NativeStateWayland::should_quit()
{
    return false;
}

void
NativeStateWayland::flip()
{
    wl_display_dispatch(display_->display);
}
