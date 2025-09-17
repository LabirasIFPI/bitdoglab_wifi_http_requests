#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

// ========= Includes Essenciais =========
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "pico/cyw43_arch.h"

// Strutura para armazenar o estado de uma requisição HTTP
typedef struct HTTP_REQUEST_STATE_T {
    char *request;
    char *host;
    uint16_t port;
    struct tcp_pcb *pcb;
    ip_addr_t remote_addr;
} HTTP_REQUEST_STATE;

// ========= Funções Auxiliares =========
static err_t http_client_recv_cb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t http_client_connected_cb(void *arg, struct tcp_pcb *tpcb, err_t err);
static void http_client_err_cb(void *arg, err_t err);
static err_t http_client_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void http_client_close(HTTP_REQUEST_STATE *state);

// ========= Protótipos das Funções =========

// Inicia uma requisição HTTP GET.
err_t http_get_request(const char *host, const char *path, uint16_t port);

// Inicia uma requisição HTTP POST.
err_t http_post_request(const char *host, const char *path, uint16_t port, const char *data);

// Incia uma requisição HTTP PATCH.
err_t http_patch_request(const char *host, const char *path, uint16_t port, const char *data);

#endif // HTTP_CLIENT_H