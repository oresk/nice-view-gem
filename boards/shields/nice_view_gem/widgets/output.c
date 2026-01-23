#include <zephyr/kernel.h>
#include "output.h"
#include "../assets/custom_fonts.h"

LV_IMAGE_DECLARE(bt_no_signal);
LV_IMAGE_DECLARE(bt_unbonded);
LV_IMAGE_DECLARE(bt);
LV_IMAGE_DECLARE(usb);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void draw_usb_connected(lv_obj_t *canvas, lv_layer_t *layer) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &usb;

    lv_area_t coords = {45, 2, 45 + usb.header.w - 1, 2 + usb.header.h - 1};
    lv_draw_image(layer, &img_dsc, &coords);
}

static void draw_ble_unbonded(lv_obj_t *canvas, lv_layer_t *layer) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &bt_unbonded;

    lv_area_t coords = {44, 0, 44 + bt_unbonded.header.w - 1, bt_unbonded.header.h - 1};
    lv_draw_image(layer, &img_dsc, &coords);
}
#endif

static void draw_ble_disconnected(lv_obj_t *canvas, lv_layer_t *layer) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &bt_no_signal;

    lv_area_t coords = {49, 0, 49 + bt_no_signal.header.w - 1, bt_no_signal.header.h - 1};
    lv_draw_image(layer, &img_dsc, &coords);
}

static void draw_ble_connected(lv_obj_t *canvas, lv_layer_t *layer) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &bt;

    lv_area_t coords = {49, 0, 49 + bt.header.w - 1, bt.header.h - 1};
    lv_draw_image(layer, &img_dsc, &coords);
}

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    label_dsc.text = "SIG";
    lv_area_t label_coords = {0, 1, 24, 16};
    lv_draw_label(&layer, &label_dsc, &label_coords);

    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);
    lv_area_t rect_coords = {43, 0, 43 + 23, 14};
    lv_draw_rect(&layer, &rect_white_dsc, &rect_coords);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(canvas, &layer);
        break;

    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_bonded) {
            if (state->active_profile_connected) {
                draw_ble_connected(canvas, &layer);
            } else {
                draw_ble_disconnected(canvas, &layer);
            }
        } else {
            draw_ble_unbonded(canvas, &layer);
        }
        break;
    }
#else
    if (state->connected) {
        draw_ble_connected(canvas, &layer);
    } else {
        draw_ble_disconnected(canvas, &layer);
    }
#endif

    lv_canvas_finish_layer(canvas, &layer);
}
