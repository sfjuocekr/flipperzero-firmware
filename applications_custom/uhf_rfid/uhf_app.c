#include "uhf_app_i.h"

char* convertToHexString(const uint8_t* array, size_t length) {
    if(array == NULL || length == 0) {
        return NULL;
    }

    // Each byte takes 3 characters in the hex representation (2 characters + space), plus 1 for the null terminator
    size_t hexLength = (length * 3) + 1;

    char* hexArray = (char*)malloc(hexLength * sizeof(char));
    if(hexArray == NULL) {
        return NULL;
    }

    size_t index = 0;
    for(size_t i = 0; i < length; i++) {
        index += snprintf(&hexArray[index], hexLength - index, "%02x ", array[i]);
    }

    hexArray[hexLength - 1] = '\0';

    return hexArray;
}

bool uhf_custom_event_callback(void* ctx, uint32_t event) {
    furi_assert(ctx);
    UHFApp* uhf_app = ctx;
    return scene_manager_handle_custom_event(uhf_app->scene_manager, event);
}

bool uhf_back_event_callback(void* ctx) {
    furi_assert(ctx);
    UHFApp* uhf_app = ctx;
    return scene_manager_handle_back_event(uhf_app->scene_manager);
}

void uhf_tick_event_callback(void* ctx) {
    furi_assert(ctx);
    UHFApp* uhf_app = ctx;
    scene_manager_handle_tick_event(uhf_app->scene_manager);
}

UHFApp* uhf_alloc() {
    UHFApp* uhf_app = (UHFApp*)malloc(sizeof(UHFApp));
    uhf_app->worker = (UHFWorker*)uhf_worker_alloc();
    uhf_app->view_dispatcher = view_dispatcher_alloc();
    uhf_app->scene_manager = scene_manager_alloc(&uhf_scene_handlers, uhf_app);
    view_dispatcher_enable_queue(uhf_app->view_dispatcher);
    view_dispatcher_set_event_callback_context(uhf_app->view_dispatcher, uhf_app);
    view_dispatcher_set_custom_event_callback(uhf_app->view_dispatcher, uhf_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        uhf_app->view_dispatcher, uhf_back_event_callback);
    view_dispatcher_set_tick_event_callback(
        uhf_app->view_dispatcher, uhf_tick_event_callback, 100);

    // Open GUI record
    uhf_app->gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(
        uhf_app->view_dispatcher, uhf_app->gui, ViewDispatcherTypeFullscreen);

    // Open Notification record
    uhf_app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Submenu
    uhf_app->submenu = submenu_alloc();
    view_dispatcher_add_view(
        uhf_app->view_dispatcher, UHFViewMenu, submenu_get_view(uhf_app->submenu));

    // Popup
    uhf_app->popup = popup_alloc();
    view_dispatcher_add_view(
        uhf_app->view_dispatcher, UHFViewPopup, popup_get_view(uhf_app->popup));

    // Loading
    uhf_app->loading = loading_alloc();
    view_dispatcher_add_view(
        uhf_app->view_dispatcher, UHFViewLoading, loading_get_view(uhf_app->loading));

    // Text Input
    uhf_app->text_input = text_input_alloc();
    view_dispatcher_add_view(
        uhf_app->view_dispatcher, UHFViewTextInput, text_input_get_view(uhf_app->text_input));

    // Custom Widget
    uhf_app->widget = widget_alloc();
    view_dispatcher_add_view(
        uhf_app->view_dispatcher, UHFViewWidget, widget_get_view(uhf_app->widget));

    return uhf_app;
}

void uhf_free(UHFApp* uhf_app) {
    furi_assert(uhf_app);

    // Submenu
    view_dispatcher_remove_view(uhf_app->view_dispatcher, UHFViewMenu);
    submenu_free(uhf_app->submenu);

    // Popup
    view_dispatcher_remove_view(uhf_app->view_dispatcher, UHFViewPopup);
    popup_free(uhf_app->popup);

    // Loading
    view_dispatcher_remove_view(uhf_app->view_dispatcher, UHFViewLoading);
    loading_free(uhf_app->loading);

    // TextInput
    view_dispatcher_remove_view(uhf_app->view_dispatcher, UHFViewTextInput);
    text_input_free(uhf_app->text_input);

    // Custom Widget
    view_dispatcher_remove_view(uhf_app->view_dispatcher, UHFViewWidget);
    widget_free(uhf_app->widget);

    // Worker
    uhf_worker_stop(uhf_app->worker);
    uhf_worker_free(uhf_app->worker);

    // View Dispatcher
    view_dispatcher_free(uhf_app->view_dispatcher);

    // Scene Manager
    scene_manager_free(uhf_app->scene_manager);

    // GUI
    furi_record_close(RECORD_GUI);
    uhf_app->gui = NULL;

    // Notifications
    furi_record_close(RECORD_NOTIFICATION);
    uhf_app->notifications = NULL;

    free(uhf_app);
}

// void uhf_text_store_set(UHFApp* uhf_app, const char* text, ...) {
//     va_list args;
//     va_start(args, text);

//     vsnprintf(uhf_app->text_store, sizeof(uhf_app->text_store), text, args);

//     va_end(args);
// }

// void uhf_text_store_clear(UHFApp* uhf_app) {
//     memset(uhf_app->text_store, 0, sizeof(uhf_app->text_store));
// }
//  ==================
static const NotificationSequence uhf_sequence_blink_start_cyan = {
    &message_blink_start_10,
    &message_blink_set_color_cyan,
    &message_do_not_reset,
    NULL,
};

static const NotificationSequence uhf_sequence_blink_stop = {
    &message_blink_stop,
    NULL,
};

void uhf_blink_start(UHFApp* uhf_app) {
    notification_message(uhf_app->notifications, &uhf_sequence_blink_start_cyan);
}

void uhf_blink_stop(UHFApp* uhf_app) {
    notification_message(uhf_app->notifications, &uhf_sequence_blink_stop);
}

void uhf_show_loading_popup(void* ctx, bool show) {
    UHFApp* uhf_app = ctx;
    TaskHandle_t timer_task = xTaskGetHandle(configTIMER_SERVICE_TASK_NAME);

    if(show) {
        // Raise timer priority so that animations can play
        vTaskPrioritySet(timer_task, configMAX_PRIORITIES - 1);
        view_dispatcher_switch_to_view(uhf_app->view_dispatcher, UHFViewLoading);
    } else {
        // Restore default timer priority
        vTaskPrioritySet(timer_task, configTIMER_TASK_PRIORITY);
    }
}

int32_t uhf_app_main(void* ctx) {
    UNUSED(ctx);
    UHFApp* uhf_app = uhf_alloc();
    view_dispatcher_attach_to_gui(
        uhf_app->view_dispatcher, uhf_app->gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(uhf_app->scene_manager, UHFSceneStart);
    view_dispatcher_run(uhf_app->view_dispatcher);

    uhf_free(uhf_app);
    return 0;
}