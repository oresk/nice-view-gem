#include <zephyr/kernel.h>
#include "battery.h"
#include "../assets/custom_fonts.h"

LV_IMAGE_DECLARE(bolt);

static void draw_level(lv_obj_t *canvas, lv_layer_t *layer, const struct status_state *state) {
    lv_draw_label_dsc_t label_right_dsc;
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char text[10] = {};

    sprintf(text, "%i%%", state->battery);
    label_right_dsc.text = text;
    lv_area_t coords = {26, 19, 26 + 41, 19 + 15};
    lv_draw_label(layer, &label_right_dsc, &coords);
}

static void draw_charging_level(lv_obj_t *canvas, lv_layer_t *layer, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &bolt;

    lv_draw_label_dsc_t label_right_dsc;
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char text[10] = {};

    sprintf(text, "%i%%", state->battery);
    label_right_dsc.text = text;
    lv_area_t label_coords = {26, 19, 26 + 34, 19 + 15};
    lv_draw_label(layer, &label_right_dsc, &label_coords);

    lv_area_t img_coords = {62, 21, 62 + bolt.header.w - 1, 21 + bolt.header.h - 1};
    lv_draw_image(layer, &img_dsc, &img_coords);
}

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    label_left_dsc.text = "BAT";
    lv_area_t coords = {0, 19, 24, 19 + 15};
    lv_draw_label(&layer, &label_left_dsc, &coords);

    if (state->charging) {
        draw_charging_level(canvas, &layer, state);
    } else {
        draw_level(canvas, &layer, state);
    }

    lv_canvas_finish_layer(canvas, &layer);
}
