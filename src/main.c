#include <pebble.h>

static Window *s_main_window;

static TextLayer *s_output_layer;
   
static void bt_handler(bool connected) {
  if (connected) {
    text_layer_set_text(s_output_layer, "Ready For Conversation");
  }
  else {
    text_layer_set_text(s_output_layer, "Not Connected");
  }
}

static void main_window_load(Window *window){
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  s_output_layer = text_layer_create(GRect(0,0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
  
  #ifdef PBL_SDK_2
  bt_handler(bluetooth_connection_service_peek());
  #elif PBL_SDK_3
  bt_handler(connection_service_peek_pebble_app_connection());
  #endif
}

static void main_window_unload(Window *window){
  text_layer_destroy(s_output_layer);
}

static void init() {
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load, 
    .unload = main_window_unload                        
  });
  window_stack_push(s_main_window, true);

#ifdef PBL_SDK_2
  bluetooth_connection_service_subscribe(bt_handler);
#elif PBL_SDK_3
connection_service_subscribe((ConnectionHandlers) {
  .pebble_app_connection_handler = bt_handler
});
#endif
  
 /*app_message_register_inbox_received(inbox);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    window_stack_push(s_main_window, true);*/
}

static void deinit(){
  window_destroy(s_main_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}


/*void process_tuple(Tuple *t){
  int key = t->key;
  int value = t->value->int32;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Got key %d, value %d", key, value);
  switch(key){
    case 0:
      APP_LOG(APP_LOG_LEVEL_INFO, "Got 'hello' message!");
      break;
    case 1:;
      DictionaryIterator *iter;
      app_message_outbox_begin(&iter);
 
      if (iter == NULL) {
        APP_LOG(APP_LOG_LEVEL_ERROR, "Iter is null! Returning");
        return;
      }
 
      dict_write_uint8(iter, 1, rand() % 3);
      dict_write_end(iter);
 
      app_message_outbox_send();
      break;
  }
}*/
 

/*void inbox(DictionaryIterator *iter, void *context){
  Tuple *t = dict_read_first(iter);
  if(t){
    process_tuple(t);
  }
  while(t != NULL){
    t = dict_read_next(iter);
    if(t){
      process_tuple(t);
    }
  }
}*/