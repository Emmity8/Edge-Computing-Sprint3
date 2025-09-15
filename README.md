# Challenge "Passa a Bola" - Sprint 3: Arquitetura IoT

## 👥 Integrantes
* Guilherme Moura Gama - RM: 562162
* Guilherme Ruiz Costa - RM: 563236
* João Batista Lima Neto - RM: 563426
* Júlio César Augusto Vieira - RM: 563366

## Descrição do Projeto
O "Passa a Bola" é uma plataforma de engajamento para fãs de futebol que transforma dados de jogo em tempo real em conteúdo interativo. Utilizando uma arquitetura baseada em Internet das Coisas (IoT), o sistema coleta informações diretamente do campo através de sensores e as disponibiliza em um hub central para torcedores, gerando posts, notificações e análises de performance de forma automatizada.

## ☁️ Arquitetura Proposta
A arquitetura escolhida é baseada na plataforma open-source **FIWARE**, que é modular, escalável e projetada para o gerenciamento de dados de contexto em tempo real.

![Arquitetura FIWARE](https://i.imgur.com/3CIbVFW.png)

**Fluxo de Dados:**
1.  **Camada IoT:** Dispositivos (sensores) coletam dados brutos.
2.  **Comunicação (MQTT):** Os dispositivos publicam os dados em um `MQTT Broker`.
3.  **Back-end (FIWARE):** Um `IoT Agent` para MQTT traduz os dados para o `Orion Context Broker`, que mantém o estado atual do jogo.
4.  **Camada de Aplicação:** Outras aplicações se inscrevem no Orion para receber notificações e acionar lógicas de negócio (criar posts, enviar alertas, etc.).

## Recursos Necessários
* **Docker e Docker Compose:** Para orquestrar e executar os contêineres da plataforma FIWARE.
* **Compilador C (GCC):** Para compilar o código do dispositivo simulado.
* **Biblioteca Paho MQTT C:** A biblioteca cliente para comunicação MQTT em C.
* **Postman ou cURL:** Para interagir com a API do FIWARE (Orion Context Broker).

## ⚙️ Instruções de Uso e Simulação

1.  **Clonar o Repositório:**
    ```bash
    git clone [URL-DO-SEU-REPOSITORIO]
    cd [NOME-DO-REPOSITORIO]
    ```

2.  **Instalar a Biblioteca Paho MQTT (Exemplo para Debian/Ubuntu):**
    ```bash
    sudo apt-get update
    sudo apt-get install build-essential git
    sudo apt-get install libssl-dev
    git clone [https://github.com/eclipse/paho.mqtt.c.git](https://github.com/eclipse/paho.mqtt.c.git)
    cd paho.mqtt.c
    make
    sudo make install
    sudo ldconfig
    cd ..
    ```

3.  **Iniciar a Plataforma FIWARE:**
    ```bash
    docker-compose up -d
    ```

4.  **Provisionar o IoT Agent:**
    Execute um script para configurar o serviço que representará nossos sensores.
    ```bash
    ./provision_agent.sh
    ```

5.  **Compilar e Executar o Simulador do Dispositivo IoT (Sensor de Gol):**
    Navegue até a pasta do código-fonte e compile o programa em C.
    ```bash
    cd codigos-fonte/
    gcc simulate_goal_sensor.c -o simulate_sensor -lpaho-mqtt3c
    ```
    Execute o programa compilado.
    ```bash
    ./simulate_sensor
    ```
    *Esta ação simula o sensor da trave confirmando um gol via MQTT.*

6.  **Verificar o Contexto no Orion Broker:**
    Use o Postman ou cURL para consultar a entidade e verificar se o status do gol foi atualizado.
    ```bash
    curl -X GET 'http://localhost:1026/v2/entities/urn:ngsi-ld:GoalSensor:001' -H 'fiware-service: passaabola' -H 'fiware-servicepath: /'
    ```
    *A resposta deve mostrar o atributo "status" como "confirmed".*
