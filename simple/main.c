#include "utils.h"

#include <stdlib.h>
#include <signal.h>
#include "service.h"

static uv_loop_t *loop;
// #include <pcap.h>
// #include <string.h>
// #include "packet.h"

// char *default_device = "enp0s25";
// int64_t counter = 0;

// // alloc_handle_t *pool;

// void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header);
// int get_device(char *devicename, char *errbuf);
// int get_pcap_handle(pcap_t **handle, char *device, char *errbuf);
// void got_packet(u_char *args, const struct pcap_pkthdr *header,
//                 const u_char *packet);

// typedef struct context_s
// {
//     int count;
//     int interval;
// } context_t;

// void got_packet(u_char *args, const struct pcap_pkthdr *header,
//                 const u_char *packet)
// {
//     printf("packet\n");
//     const struct sniff_ethernet *ethernet; /* The ethernet header [1] */
//     ethernet = (struct sniff_ethernet *)(packet);
//     printf("e: %d\n", ethernet->ether_type);

//     char src[18];
//     printf("src: %s\n", mac_toa_r(ethernet->ether_shost, src, sizeof(src)));
//     printf("dst: %s\n", mac_toa_r(ethernet->ether_dhost, src, sizeof(src)));
// }

// void on_timer(uv_timer_t *handle)
// {
//     context_t *context = (context_t *)handle->data;
//     printf("timer: %d\n", context->count);
//     context->count++;
//     uv_timer_start(handle, on_timer, context->interval, 0);
// }

// // int get_device(char *devicename, char *errbuf)
// // {
// //     pcap_if_t *device_ptr;
// //     int rv = pcap_findalldevs(&device_ptr, errbuf);
// //     if (rv < 0)
// //     {
// //         return rv;
// //     }
// //     while (device_ptr != NULL)
// //     {
// //         printf("%s:%s\n", devicename, device_ptr->name);
// //         if (strcmp(device_ptr->name, devicename) == 0)
// //         {
// //             printf("%s\n", device_ptr->name);
// //         }

// //         device_ptr = device_ptr->next;
// //     }

// //     int get_pcap_handle(pcap_t * *handle, char *device, char *errbuf)
// //     {
// //         int SNAPLEN = 0;
// //         int PROMISC = 1;
// //         int count = 10;
// //         *handle = pcap_open_live(
// //             device,
// //             SNAPLEN,
// //             PROMISC,
// //             count,
// //             errbuf);
// //         if (handle == NULL)
// //         {
// //             fprintf(stdout, "Couldn't open device %s: %s\n", device, errbuf);
// //             return (2);
// //         }
// //         return 0;
// //     }

//     int main()
//     {
//         char errbuf[256];
//         // get_device(default_device, errbuf);
//         context_t *ctx = malloc(sizeof(context_t));
//         ctx->interval = 100;
//         ctx->count = 0;

//         pcap_t *handle;
//         // handle = pcap_open_live(default_device, 0, 1, 1000, errbuf);
//         int rv = get_pcap_handle(&handle, default_device, errbuf);
//         if (rv != 0)
//         {
//             exit(rv);
//         }
//         printf("pcap handle %p '%s'\n", handle, errbuf);
//         if (handle == NULL)
//         {
//             printf("no pcap handle");
//             exit(1);
//         }
//         pcap_loop(handle, 10, got_packet, NULL);
//         pcap_close(handle);
//         exit(2);

//         // uv_idle_t idler;
//         // loop = uv_default_loop();

//         // context_t *ctx = malloc(sizeof(context_t));
//         // ctx->interval = 100;
//         // ctx->count = 0;
//         // uv_timer_t timer_req;
//         // timer_req.data = (void *)ctx;

//         // char *device;
//         // char error_buffer[PCAP_ERRBUF_SIZE];
//         // pcap_t *pcap_handle;

//         // const u_char *frame;
//         // struct pcap_pkthdr packet_header;
//         // int packet_count = 0;
//         // int timeout_limit = 10000;

//         // uv_timer_init(loop, &timer_req);
//         // uv_timer_start(&timer_req, on_timer, ctx->interval, 0);

//         // uv_run(loop, UV_RUN_DEFAULT);

//         // uv_loop_close(uv_default_loop());
//         return 0;
//     }

//     // #include <stdio.h>
//     // #include <stdlib.h>
//     // #include <uv.h>
//     // #include <string.h>

//     // uv_loop_t *loop;
//     // struct sockaddr_in addr;

//     // void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
//     // {
//     // }

//     // void on_write(uv_write_t *req, int status)
//     // {
//     // }

//     // static void on_alloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
//     // {
//     //     buf->base = malloc(suggested_size);
//     //     buf->len = suggested_size;
//     // }

//     // void on_connect(uv_connect_t *connect_req, int status)
//     // {
//     //     // fprintf(stderr, "on_connect %s\n", uv_err_name(status));
//     //     // uv_write_t *write_req;
//     //     // uv_buf_t buf;
//     //     // if (status < 0)
//     //     // {
//     //     //     abort();
//     //     // }
//     //     // write_req = malloc(sizeof(uv_write_t));

//     //     // buf.base = "GET / HTTP/1.0\r\n"
//     //     //            "Host: nyan.cat\r\n"
//     //     //            "\r\n";

//     //     // buf.len = strlen(buf.base);
//     //     // uv_write(write_req, connect_req->handle, &buf, 1, on_write);
//     //     // uv_read_start(connect_req->handle, on_alloc, on_read);
//     //     // free(connect_req);
//     // }

//     // void on_resolved(uv_getaddrinfo_t *resolver, int status, struct addrinfo *ai)
//     // {
//     //     char addr[17] = {'\0'};
//     //     uv_ip4_name((const struct sockaddr_in *)ai->ai_addr, addr, sizeof(addr));

//     //     fprintf(stderr, "on_resolved: %s %d\n", addr, resolver->addrinfo->ai_protocol);
//     //     if (status < 0)
//     //     {
//     //         /* handle error */
//     //         // abort();
//     //         fprintf(stderr, "getaddrinfo callback error %s\n", uv_err_name(status));
//     //         return;
//     //     }

//     //     uv_connect_t *connect_req = (uv_connect_t *)malloc(sizeof(uv_connect_t));
//     //     uv_tcp_t *tcp_handle = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
//     //     uv_tcp_init(loop, tcp_handle);
//     //     uv_tcp_connect(connect_req,
//     //                    tcp_handle,
//     //                    (const struct sockaddr *)ai->ai_addr,
//     //                    on_connect);
//     //     free(connect_req);
//     //     uv_freeaddrinfo(ai);
//     // }

//     // void on_timer(uv_timer_t *t)
//     // {
//     //     printf("timer\n");
//     // }

//     // int main(int argc, char *argv[])
//     // {
//     //     fprintf(stderr, "main\n");

//     //     loop = uv_default_loop();

//     //     uv_loop_init(loop);
//     //     uv_timer_t t;
//     //     uv_timer_init(loop, &t);
//     //     uv_timer_start(&t, on_timer, 100, 0);

//     //     uv_getaddrinfo_t resolver;

//     //     struct addrinfo hints;
//     //     hints.ai_family = PF_INET;
//     //     hints.ai_socktype = SOCK_STREAM;
//     //     hints.ai_protocol = IPPROTO_TCP;
//     //     hints.ai_flags = 0;

//     //     int r = uv_getaddrinfo(
//     //         loop,
//     //         &resolver,
//     //         on_resolved,
//     //         "www.google.com",
//     //         "80",
//     //         &hints);

//     //     if (r)
//     //     {
//     //         fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(r));
//     //         return 1;
//     //     }

//     //     uv_run(loop, UV_RUN_DEFAULT);

//     //     uv_loop_close(loop);
//     //     // free(loop);
//     //     return 0;
//     // }

void start()
{
    loop = start_server();
}

void stop()
{
    // printf("stop\n");
    stop_server(loop);
}

void setup()
{
    signal(SIGINT, stop);
}

int main()
{
    start();
    stop();
}