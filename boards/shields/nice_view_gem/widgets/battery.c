#include <zephyr/kernel.h>
#include "battery.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bolt);

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_BATTERY_BARS)
    // === BAR MODE ===
    const int bar_w = 26;
    const int bar_h = 12;
    const int bar_y = 20;
    const int left_x = 0;
    const int right_x = 68 - bar_w;

    lv_draw_rect_dsc_t rect_dsc;

    // Draw left (central) battery bar outline
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = LVGL_FOREGROUND;
    canvas_draw_rect(canvas, left_x, bar_y, bar_w, bar_h, &rect_dsc);
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = LVGL_BACKGROUND;
    canvas_draw_rect(canvas, left_x + 1, bar_y + 1, bar_w - 2, bar_h - 2, &rect_dsc);

    // Left fill: dark anchored left, drains toward left (empty from right)
    int left_pct = state->battery > 99 ? 99 : state->battery;
    int fill_w = (left_pct * (bar_w - 2)) / 99;
    if (fill_w > 0) {
        lv_draw_rect_dsc_init(&rect_dsc);
        rect_dsc.bg_color = LVGL_FOREGROUND;
        canvas_draw_rect(canvas, left_x + 1, bar_y + 1, fill_w, bar_h - 2, &rect_dsc);
    }

    // Draw right (peripheral) battery bar outline
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = LVGL_FOREGROUND;
    canvas_draw_rect(canvas, right_x, bar_y, bar_w, bar_h, &rect_dsc);
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = LVGL_BACKGROUND;
    canvas_draw_rect(canvas, right_x + 1, bar_y + 1, bar_w - 2, bar_h - 2, &rect_dsc);

    // Right fill: dark anchored right, drains toward right (empty from left)
    if (state->peripheral_connected) {
        int right_pct = state->peripheral_battery > 99 ? 99 : state->peripheral_battery;
        int pfill_w = (right_pct * (bar_w - 2)) / 99;
        if (pfill_w > 0) {
            lv_draw_rect_dsc_init(&rect_dsc);
            rect_dsc.bg_color = LVGL_FOREGROUND;
            int pfill_x = right_x + 1 + (bar_w - 2 - pfill_w);
            canvas_draw_rect(canvas, pfill_x, bar_y + 1, pfill_w, bar_h - 2, &rect_dsc);
        }
    }

    // Lightning icon between bars (only when charging)
    if (state->charging) {
        lv_draw_image_dsc_t img_dsc;
        lv_draw_image_dsc_init(&img_dsc);
        canvas_draw_img(canvas, 30, bar_y + 1, &bolt, &img_dsc);
    }

#else
    // === TEXT MODE ===
    const int text_y = 22;

    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char text[10] = {};
    int left_pct = state->battery > 99 ? 99 : state->battery;
    sprintf(text, "%i%%", left_pct);
    canvas_draw_text(canvas, 0, text_y, 28, &label_dsc, text);

    if (state->peripheral_connected) {
        int right_pct = state->peripheral_battery > 99 ? 99 : state->peripheral_battery;
        sprintf(text, "%i%%", right_pct);
        canvas_draw_text(canvas, 40, text_y, 28, &label_dsc, text);
    } else {
        canvas_draw_text(canvas, 40, text_y, 28, &label_dsc, "--");
    }

    // Charging bolt next to left percentage
    if (state->charging) {
        lv_draw_image_dsc_t img_dsc;
        lv_draw_image_dsc_init(&img_dsc);
        canvas_draw_img(canvas, 30, text_y, &bolt, &img_dsc);
    }

    // Divider line between percentages
    lv_draw_rect_dsc_t line_dsc;
    lv_draw_rect_dsc_init(&line_dsc);
    line_dsc.bg_color = LVGL_FOREGROUND;
    canvas_draw_rect(canvas, 33, text_y - 2, 1, 12, &line_dsc);
#endif

#else
    // Peripheral: simple battery display
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    canvas_draw_text(canvas, 0, 19, 25, &label_left_dsc, "BAT");

    lv_draw_label_dsc_t label_right_dsc;
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char text[10] = {};
    int pct = state->battery > 99 ? 99 : state->battery;
    sprintf(text, "%i%%", pct);
    canvas_draw_text(canvas, 26, 19, 42, &label_right_dsc, text);

    if (state->charging) {
        lv_draw_image_dsc_t img_dsc;
        lv_draw_image_dsc_init(&img_dsc);
        canvas_draw_img(canvas, 62, 21, &bolt, &img_dsc);
    }
#endif
}