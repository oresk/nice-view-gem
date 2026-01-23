#include <zephyr/kernel.h>
#include "util.h"
#include <ctype.h>

void to_uppercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]) {
    static lv_color_t cbuf_tmp[BUFFER_SIZE * BUFFER_SIZE];
    memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_image_dsc_t img;
    img.header.cf = LV_COLOR_FORMAT_NATIVE;
    img.header.magic = LV_IMAGE_HEADER_MAGIC;
    img.header.w = BUFFER_SIZE;
    img.header.h = BUFFER_SIZE;
    img.header.stride = BUFFER_SIZE * sizeof(lv_color_t);
    img.data_size = BUFFER_SIZE * BUFFER_SIZE * sizeof(lv_color_t);
    img.data = (const uint8_t *)cbuf_tmp;

    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.rotation = 900;  /* 90 degrees in 0.1 degree units */
    img_dsc.scale_x = LV_SCALE_NONE;
    img_dsc.scale_y = LV_SCALE_NONE;
    img_dsc.pivot.x = BUFFER_SIZE / 2;
    img_dsc.pivot.y = BUFFER_SIZE / 2;
    img_dsc.src = &img;

    lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);

    lv_area_t coords = {0, 0, BUFFER_SIZE - 1, BUFFER_SIZE - 1};
    lv_draw_image(&layer, &img_dsc, &coords);

    lv_canvas_finish_layer(canvas, &layer);
}

void fill_background(lv_obj_t *canvas) {
    lv_draw_rect_dsc_t rect_black_dsc;
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t coords = {0, 0, BUFFER_SIZE - 1, BUFFER_SIZE - 1};
    lv_draw_rect(&layer, &rect_black_dsc, &coords);
    lv_canvas_finish_layer(canvas, &layer);
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font,
                    lv_text_align_t align) {
    lv_draw_label_dsc_init(label_dsc);
    label_dsc->color = color;
    label_dsc->font = font;
    label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
    lv_draw_rect_dsc_init(rect_dsc);
    rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width) {
    lv_draw_line_dsc_init(line_dsc);
    line_dsc->color = color;
    line_dsc->width = width;
}
