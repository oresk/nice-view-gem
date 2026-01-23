#include <zephyr/kernel.h>
#include "profile.h"

LV_IMAGE_DECLARE(profiles);

static void draw_inactive_profiles(lv_obj_t *canvas, lv_layer_t *layer, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &profiles;

    lv_area_t coords = {18, 129 + BUFFER_OFFSET_BOTTOM, 18 + profiles.header.w - 1, 129 + BUFFER_OFFSET_BOTTOM + profiles.header.h - 1};
    lv_draw_image(layer, &img_dsc, &coords);
}

static void draw_active_profile(lv_obj_t *canvas, lv_layer_t *layer, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    int offset = state->active_profile_index * 7;

    lv_area_t coords = {18 + offset, 129 + BUFFER_OFFSET_BOTTOM, 18 + offset + 2, 129 + BUFFER_OFFSET_BOTTOM + 2};
    lv_draw_rect(layer, &rect_white_dsc, &coords);
}

void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    draw_inactive_profiles(canvas, &layer, state);
    draw_active_profile(canvas, &layer, state);

    lv_canvas_finish_layer(canvas, &layer);
}
