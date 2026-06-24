#include <zephyr/kernel.h>
#include "battery.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bolt);

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);

    lv_draw_label_dsc_t label_right_dsc;
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char text[10] = {};

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    // Central: show local battery + peripheral battery if connected
    sprintf(text, "%i%%", state->battery);
    canvas_draw_text(canvas, 26, 16, 42, &label_right_dsc, text);

    if (state->charging) {
        lv_draw_image_dsc_t img_dsc;
        lv_draw_image_dsc_init(&img_dsc);
        canvas_draw_img(canvas, 62, 18, &bolt, &img_dsc);
    }

    if (state->peripheral_connected) {
        canvas_draw_text(canvas, 0, 30, 25, &label_left_dsc, "PRL");
        sprintf(text, "%i%%", state->peripheral_battery);
        canvas_draw_text(canvas, 26, 30, 42, &label_right_dsc, text);
    } else {
        canvas_draw_text(canvas, 0, 16, 25, &label_left_dsc, "BAT");
    }
#else
    // Peripheral: simple battery display
    canvas_draw_text(canvas, 0, 19, 25, &label_left_dsc, "BAT");
    sprintf(text, "%i%%", state->battery);
    canvas_draw_text(canvas, 26, 19, 42, &label_right_dsc, text);
    if (state->charging) {
        lv_draw_image_dsc_t img_dsc;
        lv_draw_image_dsc_init(&img_dsc);
        canvas_draw_img(canvas, 62, 21, &bolt, &img_dsc);
    }
#endif
}