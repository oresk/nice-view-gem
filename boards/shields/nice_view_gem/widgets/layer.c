#include <zephyr/kernel.h>
#include "layer.h"
#include "../assets/custom_fonts.h"

void draw_layer_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_CENTER);

    char text[10] = {};

    if (state->layer_label == NULL) {
        sprintf(text, "Layer %i", state->layer_index);
    } else {
        strncpy(text, state->layer_label, 9);
        to_uppercase(text);
    }

    label_dsc.text = text;
    lv_area_t coords = {0, 146 + BUFFER_OFFSET_BOTTOM, 67, 146 + BUFFER_OFFSET_BOTTOM + 15};
    lv_draw_label(&layer, &label_dsc, &coords);

    lv_canvas_finish_layer(canvas, &layer);
}
