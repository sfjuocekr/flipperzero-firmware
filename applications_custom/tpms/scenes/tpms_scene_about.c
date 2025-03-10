#include "../tpms_app_i.h"
#include "../helpers/tpms_types.h"

void tpms_scene_about_widget_callback(GuiButtonType result, InputType type, void* context) {
    TPMSApp* app = context;
    if(type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, result);
    }
}

void tpms_scene_about_on_enter(void* context) {
    TPMSApp* app = context;

    FuriString* temp_str;
    temp_str = furi_string_alloc();
    furi_string_printf(temp_str, "\e#%s\n", "Information");

    furi_string_cat_printf(temp_str, "Version: %s\n", TPMS_VERSION_APP);
    furi_string_cat_printf(temp_str, "Developed by: %s\n", TPMS_DEVELOPED);
    furi_string_cat_printf(temp_str, "Github: %s\n\n", TPMS_GITHUB);

    furi_string_cat_printf(temp_str, "\e#%s\n", "Description");
    furi_string_cat_printf(
        temp_str, "Reading messages from\nTPMS sensors that work\nwith SubGhz sensors\n\n");

    furi_string_cat_printf(temp_str, "Supported protocols:\n");
    size_t i = 0;
    const char* protocol_name =
        subghz_environment_get_protocol_name_registry(app->txrx->environment, i++);
    do {
        furi_string_cat_printf(temp_str, "%s\n", protocol_name);
        protocol_name = subghz_environment_get_protocol_name_registry(app->txrx->environment, i++);
    } while(protocol_name != NULL);

    widget_add_text_box_element(
        app->widget, 0, 2, 128, 14, AlignCenter, AlignBottom, "TPMS reader\n", false);
    widget_add_text_scroll_element(app->widget, 0, 16, 128, 50, furi_string_get_cstr(temp_str));
    furi_string_free(temp_str);

    view_dispatcher_switch_to_view(app->view_dispatcher, TPMSViewWidget);
}

bool tpms_scene_about_on_event(void* context, SceneManagerEvent event) {
    TPMSApp* app = context;
    bool consumed = false;
    UNUSED(app);
    UNUSED(event);

    return consumed;
}

void tpms_scene_about_on_exit(void* context) {
    TPMSApp* app = context;

    // Clear views
    widget_reset(app->widget);
}
