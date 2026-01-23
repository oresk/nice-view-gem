#include <math.h>
#include <zephyr/kernel.h>
#include "wpm.h"
#include "../assets/custom_fonts.h"

LV_IMAGE_DECLARE(gauge);
LV_IMAGE_DECLARE(grid);

static void draw_gauge(lv_obj_t *canvas, lv_layer_t *layer, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &gauge;

    lv_area_t coords = {16, 44 + BUFFER_OFFSET_MIDDLE, 16 + gauge.header.w - 1, 44 + BUFFER_OFFSET_MIDDLE + gauge.header.h - 1};
    lv_draw_image(layer, &img_dsc, &coords);
}

static void draw_needle(lv_obj_t *canvas, lv_layer_t *layer, const struct status_state *state) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 1);

    int centerX = 33;
    int centerY = 67 + BUFFER_OFFSET_MIDDLE;
    int offset = 13;
    int value = state->wpm[9];

#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_WPM_FIXED_RANGE)
    float max = CONFIG_NICE_VIEW_GEM_WPM_FIXED_RANGE_MAX;
#else
    float max = 0;
    for (int i = 0; i < 10; i++) {
        if (state->wpm[i] > max) {
            max = state->wpm[i];
        }
    }
#endif
    if (max == 0)
        max = 100;
    if (value < 0)
        value = 0;
    if (value > max)
        value = max;

    float radius = 25.45585;
    float angleDeg = 225 + ((float)value / max) * 90;
    float angleRad = angleDeg * (3.14159 / 180.0f);

    int needleStartX = centerX + (int)(offset * cos(angleRad));
    int needleStartY = centerY + (int)(offset * sin(angleRad));
    int needleEndX = centerX + (int)(radius * cos(angleRad));
    int needleEndY = centerY + (int)(radius * sin(angleRad));

    line_dsc.p1.x = needleStartX;
    line_dsc.p1.y = needleStartY;
    line_dsc.p2.x = needleEndX;
    line_dsc.p2.y = needleEndY;
    lv_draw_line(layer, &line_dsc);
}

static void draw_grid(lv_obj_t *canvas, lv_layer_t *layer) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &grid;

    lv_area_t coords = {0, 65 + BUFFER_OFFSET_MIDDLE, grid.header.w - 1, 65 + BUFFER_OFFSET_MIDDLE + grid.header.h - 1};
    lv_draw_image(layer, &img_dsc, &coords);
}

static void draw_graph(lv_obj_t *canvas, lv_layer_t *layer, const struct status_state *state) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 2);

    int baselineY = 97 + BUFFER_OFFSET_MIDDLE;

#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_WPM_FIXED_RANGE)
    int max = CONFIG_NICE_VIEW_GEM_WPM_FIXED_RANGE_MAX;
    if (max == 0) {
        max = 100;
    }

    int value = 0;
    int prev_x = 0, prev_y = baselineY;
    for (int i = 0; i < 10; i++) {
        value = state->wpm[i];
        if (value > max) {
            value = max;
        }
        int cur_x = (int)(i * 7.4);
        int cur_y = baselineY - (value * 32 / max);
        if (i > 0) {
            line_dsc.p1.x = prev_x;
            line_dsc.p1.y = prev_y;
            line_dsc.p2.x = cur_x;
            line_dsc.p2.y = cur_y;
            lv_draw_line(layer, &line_dsc);
        }
        prev_x = cur_x;
        prev_y = cur_y;
    }
#else
    int max = 0;
    int min = 256;

    for (int i = 0; i < 10; i++) {
        if (state->wpm[i] > max) {
            max = state->wpm[i];
        }
        if (state->wpm[i] < min) {
            min = state->wpm[i];
        }
    }

    int range = max - min;
    if (range == 0) {
        range = 1;
    }

    int prev_x = 0, prev_y = baselineY;
    for (int i = 0; i < 10; i++) {
        int cur_x = (int)(i * 7.4);
        int cur_y = baselineY - (state->wpm[i] - min) * 32 / range;
        if (i > 0) {
            line_dsc.p1.x = prev_x;
            line_dsc.p1.y = prev_y;
            line_dsc.p2.x = cur_x;
            line_dsc.p2.y = cur_y;
            lv_draw_line(layer, &line_dsc);
        }
        prev_x = cur_x;
        prev_y = cur_y;
    }
#endif
}

static void draw_label(lv_obj_t *canvas, lv_layer_t *layer, const struct status_state *state) {
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    label_left_dsc.text = "WPM";
    lv_area_t coords1 = {0, 101 + BUFFER_OFFSET_MIDDLE, 24, 101 + BUFFER_OFFSET_MIDDLE + 15};
    lv_draw_label(layer, &label_left_dsc, &coords1);

    lv_draw_label_dsc_t label_dsc_wpm;
    init_label_dsc(&label_dsc_wpm, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char wpm_text[6] = {};

    snprintf(wpm_text, sizeof(wpm_text), "%d", state->wpm[9]);
    label_dsc_wpm.text = wpm_text;
    lv_area_t coords2 = {26, 101 + BUFFER_OFFSET_MIDDLE, 26 + 41, 101 + BUFFER_OFFSET_MIDDLE + 15};
    lv_draw_label(layer, &label_dsc_wpm, &coords2);
}

void draw_wpm_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    draw_gauge(canvas, &layer, state);
    draw_needle(canvas, &layer, state);
    draw_grid(canvas, &layer);
    draw_graph(canvas, &layer, state);
    draw_label(canvas, &layer, state);

    lv_canvas_finish_layer(canvas, &layer);
}
