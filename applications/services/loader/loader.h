#pragma once
#include <furi.h>
#include <applications.h>
#include "loader_mainmenu.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RECORD_LOADER "loader"
#define LOADER_APPLICATIONS_NAME "Applications"

typedef struct Loader Loader;

typedef enum {
    LoaderStatusOk,
    LoaderStatusErrorAppStarted,
    LoaderStatusErrorUnknownApp,
    LoaderStatusErrorInternal,
    LoaderStatusErrorApiMismatch,
    LoaderStatusErrorApiMismatchExit,
} LoaderStatus;

typedef enum {
    LoaderEventTypeApplicationStarted,
    LoaderEventTypeApplicationStopped
} LoaderEventType;

typedef struct {
    LoaderEventType type;
} LoaderEvent;

/**
 * @brief Start application
 * @param[in] instance loader instance
 * @param[in] name application name
 * @param[in] args application arguments
 * @param[out] error_message detailed error message, can be NULL
 * @return LoaderStatus 
 */
LoaderStatus
    loader_start(Loader* instance, const char* name, const char* args, FuriString* error_message);

/**
 * @brief Start application with GUI error message
 * @param[in] instance loader instance
 * @param[in] name application name
 * @param[in] args application arguments
 * @return LoaderStatus 
 */
LoaderStatus loader_start_with_gui_error(Loader* loader, const char* name, const char* args);

/** 
 * @brief Lock application start
 * @param[in] instance loader instance
 * @return true on success
 */
bool loader_lock(Loader* instance);

/**
 * @brief Unlock application start
 * @param[in] instance loader instance
 */
void loader_unlock(Loader* instance);

/**
 * @brief Check if loader is locked
 * @param[in] instance loader instance
 * @return true if locked
 */
bool loader_is_locked(Loader* instance);

/**
 * @brief Show loader menu
 * @param[in] instance loader instance
 */
void loader_show_menu(Loader* instance);

/**
 * @brief Show loader gamesmenu
 * @param[in] instance loader instance
 */
void loader_show_gamesmenu(Loader* instance);

/**
 * @brief Get loader pubsub
 * @param[in] instance loader instance
 * @return FuriPubSub* 
 */
FuriPubSub* loader_get_pubsub(Loader* instance);

MainMenuList_t* loader_get_mainmenu_apps(Loader* loader);

GamesMenuList_t* loader_get_gamesmenu_apps(Loader* loader);

#ifdef __cplusplus
}
#endif