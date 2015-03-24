#include <pebble.h>

typedef Layer AnimationLayer;

AnimationLayer* animation_layer_create(GRect frame);
void animation_layer_destroy(AnimationLayer* layer);
void animation_layer_set_resource(AnimationLayer* layer, uint32_t res_id);
Layer* animation_layer_get_layer(AnimationLayer* layer);
void animation_layer_play(AnimationLayer* layer);
void animation_layer_pause(AnimationLayer* layer);
void animation_layer_reset(AnimationLayer* layer);

#define animation_layer_add_to_window(layer, window) layer_add_child(window_get_root_layer(window), animation_layer_get_layer(layer))
