#include "http_client.h"

// Função para fechar a conexão e liberar recursos
static void http_client_close(HTTP_REQUEST_STATE *state) {
    if (state->pcb) {
        tcp_arg(state->pcb, NULL);
        tcp_poll(state->pcb, NULL, 0);
        tcp_sent(state->pcb, NULL);
        tcp_recv(state->pcb, NULL);
        tcp_err(state->pcb, NULL);
        tcp_close(state->pcb);
        state->pcb = NULL;
    }
    if (state->request) {
        free(state->request);
        state->request = NULL;
    }
    if (state->host) {
        free(state->host);
        state->host = NULL;
    }
    free(state);
}

// Callback: Dados recebidos do servidor
static err_t http_client_recv_cb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    HTTP_REQUEST_STATE *state = (HTTP_REQUEST_STATE *)arg;
    if (!p) {
        printf("Conexão fechada pelo servidor.\n");
        http_client_close(state);
        return ERR_OK;
    }

    printf("--- Resposta HTTP ---\n");
    for (struct pbuf *q = p; q != NULL; q = q->next) {
        printf("%.*s", q->len, (char *)q->payload);
    }
    printf("\n--- Fim da Resposta ---\n");

    pbuf_free(p);
    return ERR_OK;
}

// Callback: Conexão estabelecida
static err_t http_client_connected_cb(void *arg, struct tcp_pcb *tpcb, err_t err) {
    HTTP_REQUEST_STATE *state = (HTTP_REQUEST_STATE *)arg;
    if (err != ERR_OK) {
        printf("Falha ao conectar: %d\n", err);
        http_client_close(state);
        return err;
    }

    printf("Conectado a %s. Enviando requisição...\n", ipaddr_ntoa(&state->remote_addr));
    cyw43_arch_lwip_begin();
    err = tcp_write(tpcb, state->request, strlen(state->request), TCP_WRITE_FLAG_COPY);
    cyw43_arch_lwip_end();

    if (err != ERR_OK) {
        printf("Falha ao escrever dados: %d\n", err);
        http_client_close(state);
    }
    return err;
}

// Callback: Erro na conexão
static void http_client_err_cb(void *arg, err_t err) {
    HTTP_REQUEST_STATE *state = (HTTP_REQUEST_STATE *)arg;
    printf("Erro de TCP: %d\n", err);
    http_client_close(state);
}

// Callback: Dados enviados com sucesso
static err_t http_client_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    return ERR_OK;
}

// Callback: Resolução de DNS concluída
static void http_dns_found_cb(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    HTTP_REQUEST_STATE *state = (HTTP_REQUEST_STATE *)callback_arg;
    if (ipaddr) {
        state->remote_addr = *ipaddr;
        printf("DNS resolvido: %s -> %s\n", name, ipaddr_ntoa(ipaddr));

        cyw43_arch_lwip_begin();
        state->pcb = tcp_new();
        tcp_arg(state->pcb, state);
        tcp_recv(state->pcb, http_client_recv_cb);
        tcp_err(state->pcb, http_client_err_cb);
        tcp_sent(state->pcb, http_client_sent_cb);
        err_t err = tcp_connect(state->pcb, &state->remote_addr, state->port, http_client_connected_cb);
        cyw43_arch_lwip_end();

        if (err != ERR_OK) {
            printf("Falha em tcp_connect: %d\n", err);
            http_client_close(state);
        }
    } else {
        printf("Falha na requisição DNS\n");
        http_client_close(state);
    }
}

// Função auxiliar para iniciar uma requisição
static err_t start_http_request(const char *host, const char *path, uint16_t port, const char *method, const char* data) {
    HTTP_REQUEST_STATE *state = calloc(1, sizeof(HTTP_REQUEST_STATE));
    if (!state) return ERR_MEM;

    char *request_template;
    int request_len;
    if (data) {
        request_template = "%s %s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s";
        request_len = snprintf(NULL, 0, request_template, method, path, host, strlen(data), data);
    } else {
        request_template = "%s %s HTTP/1.1\r\nHost: %s\r\n\r\n";
        request_len = snprintf(NULL, 0, request_template, method, path, host);
    }

    state->request = malloc(request_len + 1);
    if (!state->request) {
        free(state);
        return ERR_MEM;
    }

    if(data) {
        sprintf(state->request, request_template, method, path, host, strlen(data), data);
    } else {
        sprintf(state->request, request_template, method, path, host);
    }

    state->host = strdup(host);
    state->port = port;

    cyw43_arch_lwip_begin();
    err_t err = dns_gethostbyname(host, &state->remote_addr, http_dns_found_cb, state);
    cyw43_arch_lwip_end();

    if (err == ERR_OK) { // IP já está no cache
        http_dns_found_cb(host, &state->remote_addr, state);
    } else if (err != ERR_INPROGRESS) {
        printf("Falha ao iniciar requisição DNS\n");
        http_client_close(state);
        return err;
    }

    return ERR_OK;
}

// Implementação das funções públicas
err_t http_get_request(const char *host, const char *path, uint16_t port) {
    return start_http_request(host, path, port, "GET", NULL);
}

err_t http_post_request(const char *host, const char *path, uint16_t port, const char *data) {
    return start_http_request(host, path, port, "POST", data);
}

err_t http_patch_request(const char *host, const char *path, uint16_t port, const char *data) {
    return start_http_request(host, path, port, "PATCH", data);
}