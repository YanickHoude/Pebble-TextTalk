#include <pebble.h>
#define KEY_BUTTON_SELECT 0
int btstatus = 0;
int counter = 0;

static Window *app_main_window;
static TextLayer *bt_output_layer;
static TextLayer *start_output_layer;
static TextLayer *result_output_layer;

static void send(int key, int value) {
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    
    dict_write_int(iter, key, &value, sizeof(int), true);
    
    app_message_outbox_send();
}



static void outbox_sent_handler(DictionaryIterator *iter, void *context) {
    text_layer_set_text(start_output_layer, "Conversation Started");
}

/*static void outbox_failed_handler(DictionaryIterator *iter, AppMessageResult reason, void *context) {
 text_layer_set_text(start_output_layer, "Send failed!");
 APP_LOG(APP_LOG_LEVEL_ERROR, "Fail reason: %d", (int)reason);
 }*/

static void bt_handler(bool connected) {
    if (connected) {
        text_layer_set_text(bt_output_layer, "Ready For Conversation");
        btstatus++;
    }
    else {
        text_layer_set_text(bt_output_layer, "Not Connected");
        btstatus--;
    }
}
static void main_window_load(Window *window){
    Layer *window_layer = window_get_root_layer(window);
    GRect window_bounds = layer_get_bounds(window_layer);
    
    bt_output_layer = text_layer_create(GRect(0,0, window_bounds.size.w, window_bounds.size.h));
    text_layer_set_text_alignment(bt_output_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(bt_output_layer));
    
    start_output_layer = text_layer_create(GRect(0, 120, window_bounds.size.w, window_bounds.size.h));
    text_layer_set_text_alignment(start_output_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(start_output_layer));
    
    result_output_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
    text_layer_set_text_alignment(result_output_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(result_output_layer));
    
    
    bt_handler(bluetooth_connection_service_peek());
}

static void received_handler(DictionaryIterator *iter, void *context) {
    Tuple *result_tuple = dict_find(iter, 1234);
    char* final = result_tuple->value->cstring;
    text_layer_set_text(result_output_layer, final);
}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    if(btstatus == 1){
        if(counter == 0){
            counter++;
            text_layer_set_text(start_output_layer, "Conversation Started");
            send(KEY_BUTTON_SELECT, 0);
            
        }
        else {
            counter--;
            text_layer_set_text(start_output_layer, "Conversation Ended");
            
        }
    }else text_layer_set_text(start_output_layer, "Enable Bluetooth");
}

static void click_config_provider(void *context){
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}



static void main_window_unload(Window *window){
    text_layer_destroy(bt_output_layer);
    text_layer_destroy(result_output_layer);
    text_layer_destroy(start_output_layer);
}

static void init() {
    app_main_window = window_create();
    
    window_set_window_handlers(app_main_window, (WindowHandlers){
        .load = main_window_load,
        .unload = main_window_unload
    });
    window_set_click_config_provider(app_main_window, click_config_provider);
    window_stack_push(app_main_window, true);
    
    // Open AppMessage
    app_message_register_outbox_sent(outbox_sent_handler);
    app_message_register_inbox_received(received_handler);
    
    //app_message_register_outbox_failed(outbox_failed_handler);
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    
    //app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());*/
    
    bluetooth_connection_service_subscribe(bt_handler);
}

static void deinit(){
    window_destroy(app_main_window);
}

int main(void){
    init();
    app_event_loop();
    deinit();
    
}
   