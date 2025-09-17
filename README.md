# 📡 Cliente HTTP para Raspberry Pi Pico W

Um projeto de exemplo que demonstra como realizar requisições HTTP (`GET`, `POST`, `PATCH`) a partir de um **Raspberry Pi Pico W**, usando o SDK C e a pilha de rede LwIP. Ideal para projetos IoT que precisam se comunicar com APIs e servidores web.

-----

## 🌟 Recursos

  - 🌐 **Cliente HTTP Completo:** Implementação para os métodos `GET`, `POST` e `PATCH`.
  - 🧱 **Arquitetura Modular:** Código separado em um módulo de Wi-Fi (`wifi_manager`) e um de HTTP (`http_client`), facilitando a reutilização.
  - 🔒 **Gerenciamento de Credenciais:** Mantém as senhas de Wi-Fi em um arquivo separado (`wifi_credentials.h`) para maior segurança.
  - ⚙️ **Pronto para Compilar:** Inclui um `CMakeLists.txt` configurado com as bibliotecas corretas (`pico_cyw43_arch_lwip_threadsafe_background`).
  - 💬 **Feedback Detalhado:** Imprime cada etapa do processo no monitor serial, desde a conexão Wi-Fi até o recebimento da resposta do servidor.

## 🛠️ Pré-requisitos

### 🔩 Hardware

  - Raspberry Pi Pico W

### 💻 Software

  - [Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk) configurado;
  - [ARM GCC Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm);
  - CMake;
  - Uma API com endpoints configurados para o Pico W consumir;

## 🚀 Como Usar

### 1\. Clone o Repositório

```bash
git clone https://github.com/LabirasIFPI/bitdoglab_wifi_http_requests.git
cd bitdoglab_wifi_http_requests
```

### 2\. ✍️ Configure suas Credenciais

Edite o arquivo `wifi_credentials.h` com os dados da sua rede Wi-Fi.

```c
#ifndef WIFI_CREDENTIALS_H
#define WIFI_CREDENTIALS_H

#define WIFI_SSID "O_NOME_DA_SUA_REDE"
#define WIFI_PASSWORD "A_SENHA_DA_SUA_REDE"

#endif // WIFI_CREDENTIALS_H
```

> ⚠️ **AVISO DE SEGURANÇA** \>
> Para proteger suas senhas, em um projeto real, **SEMPRE** adicione o arquivo `wifi_credentials.h` ao seu `.gitignore`.

### 3\. 👨‍💻 Compile o Projeto

Crie um diretório de `build`, execute o `cmake` e o `make`.

```bash
mkdir build
cd build
cmake ..
make
```

### 4\. ✨ Flashe o Pico W

  - Pressione e segure o botão **`BOOTSEL`** no seu Pico W e conecte-o ao computador.
  - Ele aparecerá como um dispositivo de armazenamento USB.
  - Arraste o arquivo `.uf2` gerado na pasta `build` para dentro do Pico W.

A placa irá reiniciar. Abra um **monitor serial** para ver o log de conexão e as respostas das requisições HTTP\! 👀

-----

Este projeto serve como uma base robusta para qualquer aplicação IoT que precise interagir com a web. 🌐