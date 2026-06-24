#include <zephyr/kernel.h>
#include "output.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void draw_usb_connected(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    canvas_draw_img(canvas, 28, 2, &usb, &img_dsc);
}

static void draw_ble_unbonded(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    canvas_draw_img(canvas, 27, 0, &bt_unbonded, &img_dsc);
}
#endif

static void draw_ble_disconnected(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    canvas_draw_img(canvas, 32, 0, &bt_no_signal, &img_dsc);
}

static void draw_ble_connected(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    canvas_draw_img(canvas, 32, 2, &bt, &img_dsc);
}

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
    // Draw outlined header bar (black border, white fill)
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = LVGL_FOREGROUND;
    canvas_draw_rect(canvas, 0, 0, 68, 15, &rect_dsc);
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = LVGL_BACKGROUND;
    canvas_draw_rect(canvas, 1, 1, 66, 13, &rect_dsc);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(canvas);
        break;

    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_bonded) {
            if (state->active_profile_connected) {
                draw_ble_connected(canvas);
            } else {
                draw_ble_disconnected(canvas);
            }
        } else {
            draw_ble_unbonded(canvas);
        }
        break;
    }
#else
    if (state->connected) {
        draw_ble_connected(canvas);
    } else {
        draw_ble_disconnected(canvas);
    }
#endif
}