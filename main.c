#include <stdio.h>
#include "pico/stdlib.h"
#include "http_client.h"
#include "wifi_manager.h"

int main(){
	// Inicializações do seu sistema
    stdio_init_all();
    sleep_ms(2000); // Pausa para abrir o monitor serial

    printf("========== INICIANDO SISTEMA ==========\n");
    printf("Pico Wi-Fi HTTP Client - Teste de Conectividade\n");
    printf("========================================\n\n");

    // 1. Inicializa o hardware do Wi-Fi
    printf("[1/4] Inicializando hardware Wi-Fi...\n");
    if (!wifi_arch_init()) {
        printf("ERRO: Falha ao inicializar hardware Wi-Fi!\n");
        return -1;
    }

	// 2. Tenta conectar à rede
    printf("[2/4] Conectando ao Wi-Fi...\n");
    if (!wifi_connect()) {
        printf("ERRO: Falha ao conectar ao Wi-Fi!\n");
        printf("Verifique as credenciais em wifi_credentials.h\n");
        return -1;
    }

    // 3. Verifica status da conexão
    printf("[3/4] Verificando status da conexão...\n");
    wifi_print_status();
    printf("\n");

    // 4. Executa exemplos de requisições HTTP
    printf("[4/4] Executando exemplos HTTP...\n");
    if (wifi_is_connected()) {
        printf("Conexão Wi-Fi ativa - Iniciando exemplos HTTP\n\n");
        
        // Exemplo 1: GET Request
        printf("=== EXEMPLO 1: GET Request ===\n");
        printf("Fazendo GET para http://minha-api/get...\n");
        if (http_get_request("http://minha-api", "/get", 80) == ERR_OK) {
            printf("GET Request enviado com sucesso!\n");
        } else {
            printf("Erro ao enviar GET Request\n");
        }
        
        sleep_ms(3000); // Aguarda resposta
        
        // Exemplo 2: POST Request
        printf("\n=== EXEMPLO 2: POST Request ===\n");
        const char *post_data = "{\"nome\": \"Pico\", \"sensor\": \"temperatura\", \"valor\": 25.5}";
        printf("Fazendo POST para http://minha-api/post com dados: %s\n", post_data);
        if (http_post_request("http://minha-api", "/post", 80, post_data) == ERR_OK) {
            printf("POST Request enviado com sucesso!\n");
        } else {
            printf("Erro ao enviar POST Request\n");
        }
        
        sleep_ms(3000); // Aguarda resposta
        
        // Exemplo 3: PATCH Request
        printf("\n=== EXEMPLO 3: PATCH Request ===\n");
        const char *patch_data = "{\"status\": \"ativo\", \"ultima_atualizacao\": \"2024-01-01T12:00:00Z\"}";
        printf("Fazendo PATCH para http://minha-api/patch com dados: %s\n", patch_data);
        if (http_patch_request("http://minha-api", "/patch", 80, patch_data) == ERR_OK) {
            printf("PATCH Request enviado com sucesso!\n");
        } else {
            printf("Erro ao enviar PATCH Request\n");
        }
        
    } else {
        printf("ERRO: Wi-Fi desconectado - Não é possível executar exemplos HTTP\n");
        return -1;
    }

    // Loop principal simples
    while (true) {
        // Processa eventos de rede
        cyw43_arch_poll();
        sleep_ms(100);
    }
}