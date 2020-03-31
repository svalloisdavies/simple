#include <uv.h>

#ifndef SERVICE_H
#define SERVICE_H

#define TICK_INTERVAL 100

uv_loop_t *initialise();
uv_loop_t *start_server();
void stop_server(uv_loop_t *);

typedef struct timer_config_s
{
    uint64_t count;
    uint16_t tick_interval;
    uv_timer_t timer_handle;
} timer_config_t;

typedef struct context_s
{
    timer_config_t timer_config;
} context_t;

#endif // SERVICE_H
