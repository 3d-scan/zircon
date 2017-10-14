// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <zircon/compiler.h>
#include <zircon/types.h>

__BEGIN_CDECLS;

// flags for gpio_config()
typedef enum {
    GPIO_DIR_IN             = 0 << 0,
    GPIO_DIR_OUT            = 1 << 0,
    GPIO_DIR_MASK           = 1 << 0,

    GPIO_TRIGGER_EDGE       = 0 << 1,
    GPIO_TRIGGER_LEVEL      = 1 << 1,
    GPIO_TRIGGER_MASK       = 1 << 1,

    // for edge triggered
    GPIO_TRIGGER_RISING     = 1 << 2,
    GPIO_TRIGGER_FALLING    = 1 << 3,

    // for level triggered
    GPIO_TRIGGER_HIGH       = 1 << 2,
    GPIO_TRIGGER_LOW        = 1 << 3,
} gpio_config_flags_t;

// signals used for waiting for GPIO state change
#define GPIO_SIGNAL_LOW     ZX_USER_SIGNAL_0
#define GPIO_SIGNAL_HIGH    ZX_USER_SIGNAL_1
// signal reserved for exiting thread waiting on event handle
#define GPIO_SIGNAL_STOP    ZX_USER_SIGNAL_2

typedef struct {
    zx_status_t (*config)(void* ctx, unsigned pin, gpio_config_flags_t flags);
    zx_status_t (*read)(void* ctx, unsigned pin, unsigned* out_value);
    zx_status_t (*write)(void* ctx, unsigned pin, unsigned value);
    zx_status_t (*get_event_handle)(void* ctx, unsigned pin, zx_handle_t* out_handle);
} gpio_protocol_ops_t;

typedef struct {
    gpio_protocol_ops_t* ops;
    void* ctx;
} gpio_protocol_t;

// configures a GPIO
static inline zx_status_t gpio_config(gpio_protocol_t* gpio, unsigned pin,
                                      gpio_config_flags_t flags) {
    return gpio->ops->config(gpio->ctx, pin, flags);
}

// reads the current value of a GPIO (0 or 1)
static inline zx_status_t gpio_read(gpio_protocol_t* gpio, unsigned pin, unsigned* out_value) {
    return gpio->ops->read(gpio->ctx, pin, out_value);
}

// sets the current value of the GPIO (any non-zero value maps to 1)
static inline zx_status_t gpio_write(gpio_protocol_t* gpio, unsigned pin, unsigned value) {
    return gpio->ops->write(gpio->ctx, pin, value);
}

// returns a handle used for waiting on state changes
static inline zx_status_t gpio_get_event_handle(gpio_protocol_t* gpio, unsigned pin,
                                                zx_handle_t* out_handle) {
    return gpio->ops->get_event_handle(gpio->ctx, pin, out_handle);
}

__END_CDECLS;
