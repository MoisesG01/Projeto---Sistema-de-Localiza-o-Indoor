# Sistema de Posicionamento Interno com ESP32 e Python

Este projeto explora a criação de um sistema de posicionamento interno utilizando dispositivos ESP32 e Python. Utilizada a biblioteca `paho.mqtt.client` para comunicação MQTT e a biblioteca `turtle` para visualização gráfica.

## Introdução

Python é amplamente reconhecido por sua simplicidade, legibilidade e versatilidade. Possui uma vasta coleção de bibliotecas e frameworks que facilitam o desenvolvimento de uma ampla gama de aplicações. Neste projeto, utilizei duas bibliotecas principais:

- **Turtle**: Permite a criação de gráficos e animações de forma interativa.
- **paho.mqtt.client**: Utilizada para a comunicação entre dispositivos por meio do protocolo MQTT.

## Sumário

- [Descrição do Projeto](#descrição-do-projeto)
- [Funcionalidades](#funcionalidades)
- [Pré-requisitos](#pré-requisitos)
- [Instalação](#instalação)
  - [Arduino](#arduino)
  - [Python](#python)
- [Como Usar](#como-usar)
- [Configuração do MQTT](#configuração-do-mqtt)
- [Licença](#licença)

## Descrição do Projeto

Este projeto utiliza o ESP32 para realizar a varredura de dispositivos BLE (Bluetooth Low Energy), calcular a média do RSSI (Received Signal Strength Indicator) e estimar a distância desses dispositivos. As informações coletadas são publicadas em um tópico MQTT e são usadas por um script Python para determinar a posição de um beacon em uma sala.

A técnica de **trilateração** é aplicada para calcular a posição do beacon. Baseando-se nas distâncias estimadas de pelo menos três pontos de referência conhecidos, a posição do beacon é determinada por meio da interseção das esferas formadas ao redor desses pontos de referência.

![image](https://github.com/user-attachments/assets/72d64b20-e51a-4cdb-9c57-019e7d2282e3)


## Funcionalidades

- Escaneamento de dispositivos BLE e cálculo da média do RSSI
- Publicação de dados de distância em um broker MQTT
- Visualização da posição do beacon em uma interface gráfica com Python Turtle

## Pré-requisitos

- **Arduino IDE ou PlatformIO IDE para VSCode**
  - Bibliotecas:
    - WiFi
    - PubSubClient
    - BLEDevice
- **Broker MQTT** (por exemplo, test.mosquitto.org)
- **Python 3**
  - Bibliotecas Python:
    - Turtle
    - paho-mqtt

## Instalação

1. Clone este repositório:

    ```sh
    git clone https://github.com/MoisesG01/IndoorPositioningSystem.git
    ```

2. Abra o arquivo `IndoorPositioningSystem` na IDE.
3. Certifique-se de que você tem as bibliotecas necessárias instaladas:
   - WiFi
   - PubSubClient
   - BLEDevice
4. Adicione suas credenciais WiFi e MQTT no arquivo `Credentials.h`:

    ```cpp
    #ifndef CREDENTIALS_H_
    #define CREDENTIALS_H_
    const char* ssid = "Seu_SSID"; // Inserir o nome do WIFI
    const char* password = "Sua_Senha"; // Inserir a senha
    #endif
    ```

5. Faça o upload do código para o ESP32.

### Python

1. Instale as bibliotecas necessárias:

    ```sh
    pip install paho-mqtt
    ```

## Como Usar

1. Certifique-se de que o ESP32 está ligado e conectado à sua rede WiFi.
2. Execute o script Python para visualizar a posição do beacon:

    ```sh
    python positioning_system.py
    ```

## Configuração do MQTT

O código utiliza o broker público `test.mosquitto.org` e publica as informações no tópico `MOISES/ESP3`. Você pode modificar esses parâmetros conforme necessário.

```cpp
const char *mqtt_broker = "test.mosquitto.org"; 
const char *topic = "MOISES/ESP3";
const char *mqtt_username = ""; 
const char *mqtt_password = ""; 
const int mqtt_port = 1883; 
