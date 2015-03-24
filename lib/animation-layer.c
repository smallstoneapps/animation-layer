#include <pebble.h>
#include "animation-layer.h"

typedef struct AnimationLayerData {
  uint32_t resource;
  GBitmapSequence* sequence;
  GBitmap* bitmap;
  AppTimer* timer;
} AnimationLayerData;

static void animation_layer_tick(void* data);
static AnimationLayerData* animation_layer_get_data(AnimationLayer* layer);
static void animation_layer_render(Layer* layer, GContext* ctx);

AnimationLayer* animation_layer_create(GRect frame) {
  AnimationLayer* layer = layer_create_with_data(frame, sizeof(AnimationLayerData));
  layer_set_update_proc(layer, animation_layer_render);
  AnimationLayerData* data = (AnimationLayerData*) layer_get_data(layer);
  data->sequence = NULL;
  data->bitmap = NULL;
  data->timer = NULL;
  return layer;
}

void animation_layer_destroy(AnimationLayer* layer) {
  AnimationLayerData* data = animation_layer_get_data(layer);
  if (data->bitmap != NULL) {
    gbitmap_destroy(data->bitmap);
  }
  if (data->sequence != NULL) {
    gbitmap_sequence_destroy(data->sequence);
  }
  layer_destroy(layer);
}

void animation_layer_set_resource(AnimationLayer* layer, uint32_t res_id) {
  AnimationLayerData* data = animation_layer_get_data(layer);
  data->resource = res_id;
  animation_layer_reset(layer);
}

Layer* animation_layer_get_layer(AnimationLayer* layer) {
  return (Layer*)layer;
}

void animation_layer_play(AnimationLayer* layer) {
  animation_layer_tick((void*)layer);
}

void animation_layer_pause(AnimationLayer* layer) {
  AnimationLayerData* data = animation_layer_get_data(layer);
  app_timer_cancel(data->timer);
}

void animation_layer_reset(AnimationLayer* layer) {
  AnimationLayerData* data = animation_layer_get_data(layer);
  if (data->sequence != NULL) {
    gbitmap_sequence_destroy(data->sequence);
  }
  if (data->bitmap != NULL) {
    gbitmap_destroy(data->bitmap);
  }
  data->sequence = gbitmap_sequence_create_with_resource(data->resource);
  data->bitmap = gbitmap_create_blank(gbitmap_sequence_get_bitmap_size(data->sequence), GBitmapFormat8Bit);
  layer_mark_dirty(animation_layer_get_layer(layer));
}

static AnimationLayerData* animation_layer_get_data(AnimationLayer* layer) {
  return (AnimationLayerData*) layer_get_data(layer);
}

static void animation_layer_tick(void* context) {
  AnimationLayer* layer = (AnimationLayer*)context;
  AnimationLayerData* data = animation_layer_get_data(layer);

  uint32_t delay;

  if (gbitmap_sequence_get_current_frame_idx(data->sequence) == gbitmap_sequence_get_total_num_frames(data->sequence)) {
    animation_layer_reset(layer);
  }
  gbitmap_sequence_update_bitmap_next_frame(data->sequence, data->bitmap, &delay);
  layer_mark_dirty(animation_layer_get_layer(layer));
  data->timer = app_timer_register(delay, animation_layer_tick, (void*)layer);
}

static void animation_layer_render(Layer* layer, GContext* ctx) {
  AnimationLayerData* data = animation_layer_get_data(layer);
  graphics_draw_bitmap_in_rect(ctx, data->bitmap, GRect(0, 0, 48, 48));
}
