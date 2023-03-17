/*
 * Copyright 2022-2023 Wiimag Inc. All rights reserved.
 * License: https://equals-forty-two.com/LICENSE
 * 
 * Application service management
 */

#pragma once

#include <framework/string.h>
#include <framework/common.h>

#include <foundation/hash.h>

typedef void (*service_initialize_handler_t)(void);
typedef void (*service_shutdown_handler_t)(void);

typedef function<void()> service_invoke_handler_t;

#define SERVICE_PRIORITY_CRITICAL   (-100)
#define SERVICE_PRIORITY_SYSTEM      (-20)
#define SERVICE_PRIORITY_REALTIME    (-10)
#define SERVICE_PRIORITY_BASE          (0)
#define SERVICE_PRIORITY_HIGH         (10)
#define SERVICE_PRIORITY_MODULE       (20)
#define SERVICE_PRIORITY_LOW          (30)
#define SERVICE_PRIORITY_TESTS       (100)
#define SERVICE_PRIORITY_UI_HEADLESS (190)
#define SERVICE_PRIORITY_UI          (200)

FOUNDATION_FORCEINLINE const char* service_name_to_lower_static(const char* name, size_t len)
{
    static thread_local char ts_name_buffer[16];
    string_t lname = string_to_lower_ascii(STRING_BUFFER(ts_name_buffer), name, len);
    return lname.str;
}

/*! @def DEFINE_SERVICE
 * 
 * Register a service to be initialized and shutdown at the appropriate time.
 *
 * @param NAME          Name of the service.
 * @param initialize_fn Function to call to initialize the service.
 * @param shutdown_fn   Function to call to shutdown the service.
 * @param ...           Optional priority of the service.
 */
#define DEFINE_SERVICE(NAME, initialize_fn, shutdown_fn, ...)   \
    const Service __##NAME##_service(#NAME, HASH_##NAME, [](){  \
        memory_context_push(HASH_##NAME);                       \
        initialize_fn();                                        \
        memory_context_pop();                                   \
    }, shutdown_fn, __VA_ARGS__)

/*! @def DEFINE_MODULE
 * 
 * Register a module to be initialized and shutdown at the appropriate time.
 *
 * @param NAME          Name of the module.
 * @param CONTENT       Body of the module (it can include anything a struct can contain).
 *                      The body must contain minimally the following functions:
 *                          - void initialize()
 *                          - void shutdown()
 * @param ...           Optional priority of the module.
 */
#define DEFINE_MODULE(NAME, CONTENT, ...)                               \
    const hash_t HASH_##NAME = string_hash(service_name_to_lower_static(#NAME, sizeof(#NAME)-1), sizeof(#NAME)-1);  \
    struct NAME                                                         \
        CONTENT                                                         \
        *_module = nullptr;                                             \
    const Service __##NAME##_service(#NAME, HASH_##NAME, []() {         \
        static_hash_store(service_name_to_lower_static(#NAME, sizeof(#NAME)-1), sizeof(#NAME)-1, HASH_##NAME);         \
        memory_context_push(HASH_##NAME);                               \
        _module = MEM_NEW(HASH_##NAME, NAME);                           \
        _module->initialize();                                          \
        memory_context_pop();                                           \
    }, []() {                                                           \
        _module->shutdown();                                            \
        MEM_DELETE(_module);                                            \
        _module = nullptr;                                              \
    }, __VA_ARGS__)

 /*! Service object to be instantiated in the global scope to manage 
  *  the service initialization and shutdown sequence. 
  */
class Service
{
public:
    FOUNDATION_NOINLINE Service(const char* FOUNDATION_RESTRICT name, hash_t service_hash,
        service_initialize_handler_t initialize_handler,
        service_shutdown_handler_t shutdown_handler,
        int priority);

    FOUNDATION_NOINLINE Service(const char* FOUNDATION_RESTRICT name, hash_t service_hash,
        service_initialize_handler_t initialize_handler,
        service_shutdown_handler_t shutdown_handler)
        : Service(name, service_hash, initialize_handler, shutdown_handler, SERVICE_PRIORITY_LOW)
    {
    }
};

/*! Initialize the service system and all statically registered services. */
void service_initialize();

/*! Shutdown the service system and all other registered services. */
void service_shutdown();

/*! Register a service handler that can be invoked for all services later on.
 * 
 * @param service_key Hash key of the registered service.
 * @param handler_key Hash key for the handler.
 * @param handler Handler function to invoke. 
 */
void service_register_handler(hash_t service_key, hash_t handler_key, const service_invoke_handler_t& handler);

/*! Register a service to act to menu events.
 * 
 * @param service_key Hash key of the registered service.
 * @param menu_handler Handler function to invoke.
 */
void service_register_menu(hash_t service_key, const service_invoke_handler_t& menu_handler);

/*! Register a service to act to menu status events.
 *
 * @param service_key Hash key of the registered service.
 * @param menu_status_handler Handler function to invoke.
 */
void service_register_menu_status(hash_t service_key, const service_invoke_handler_t& menu_status_handler);

/*! Register a service to act to tab events.
 *
 * @param service_key Hash key of the registered service.
 * @param tabs_handler Handler function to invoke.
 */
void service_register_tabs(hash_t service_key, const service_invoke_handler_t& tabs_handler);

/*! Register a service to render new windows.
 *
 * @param service_key Hash key of the registered service.
 * @param window_handler Handler function to invoke.
 */
void service_register_window(hash_t service_key, const service_invoke_handler_t& window_handler);

/*! Register a service to update the application.
 *
 * @param service_key Hash key of the registered service.
 * @param update_handler Handler function to invoke.
 */
void service_register_update(hash_t service_key, const service_invoke_handler_t& update_handler);

/*! Loop through all services to invoke the registered handler.
 *
 * @param handler_key Hash key of the service handler to invoke.
 */
void service_foreach(hash_t handler_key);

/*! Loop through all services to invoke the registered menu handler. */
void service_foreach_menu();

/*! Loop through all services to invoke the registered menu status handler. */
void service_foreach_menu_status();

/*! Loop through all services to invoke the registered tabs handler. */
void service_foreach_tabs();

/*! Loop through all services to invoke the registered window handler. */
void service_foreach_window();

/*! Loop through all services to invoke the registered update handler. */
void service_update();
