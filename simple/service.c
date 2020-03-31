#include <uv.h>
#include <stdlib.h>
#include "service.h"
#include "utils.h"

void on_tick(uv_timer_t *handle)
{
    // slog("tick\n");
    context_t *ctx = (context_t *)handle->loop->data;
    slog("tick: %d\n", ctx->timer_config->count);
    ctx->timer_config->count++;
    uv_timer_start(handle, on_tick, ctx->timer_config->tick_interval, 0);
}

uv_loop_t *initialise()
{

    uv_loop_t *loop = malloc(sizeof(uv_loop_t));
    if (loop == NULL)
    {
        slog("error allocating loop memory\n");
        stop_server(loop);
    }
    uv_loop_init(loop);
    context_t *context = malloc(sizeof(context_t));
    if (context == NULL)
    {
        slog("error allocating context memory\n");
        stop_server(loop);
    }

    loop->data = (void *)context;
    return loop;
}

void initialise_timer(uv_loop_t *loop)
{
    context_t *ctx = (context_t *)loop->data;
    timer_config_t *tc = &ctx->timer_config;
    // tc = malloc(sizeof(timer_config_t));

    tc->count = 0;
    tc->tick_interval = TICK_INTERVAL;

    uv_timer_init(loop, &tc->timer_handle);
    uv_timer_start(&tc->timer_handle, on_tick, TICK_INTERVAL, 0);
}

uv_loop_t *start_server()
{
    uv_loop_t *loop = initialise();

    initialise_timer(loop);
    slog(
        "start: %d:%d\n",
        c->timer_config->tick_interval,
        c->timer_config->count);

    uv_loop_init(loop);
    // // t.data = (void *)context
    // uv_run(loop, UV_RUN_DEFAULT);
    return loop;
}

void stop_server(uv_loop_t *loop)
{
    // context_t *c = (context_t *)loop->data;
    // uv_timer_stop(c->timer_config->timer);
    // uv_loop_close(loop);

    // context_t *context = (context_t *)loop->data;

    // if (context != NULL)
    // {
    //     slog("stop_server: free context \n");
    //     free(context);
    // }

    if (loop != NULL)
    {
        slog("stop_server: free loop\n");
        free(loop);
    }

    // return 0;
}
