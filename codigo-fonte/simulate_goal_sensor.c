#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

// Endereço do Broker MQTT (executando no Docker)
#define ADDRESS     "tcp://localhost:1883"
// ID do Cliente que estamos simulando
#define CLIENTID    "GoalSensorSimulator_C"
// Tópico para o qual o IoT Agent está inscrito
#define TOPIC       "/sensor/goal"
// Mensagem que o IoT Agent espera para a entidade
// Formato: <nome_do_atributo>|<valor>
#define PAYLOAD     "s|confirmed"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc; // Código de retorno

    // Cria o cliente MQTT
    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
         printf("Falha ao criar o cliente, código de retorno %d\n", rc);
         exit(EXIT_FAILURE);
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Conecta ao broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Falha ao conectar, código de retorno %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Conectado ao broker MQTT em %s\n", ADDRESS);

    // Configura a mensagem
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = (int)strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    // Publica a mensagem no tópico
    if ((rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token)) != MQTTCLIENT_SUCCESS) {
         printf("Falha ao publicar a mensagem, código de retorno %d\n", rc);
         exit(EXIT_FAILURE);
    }

    printf("Mensagem '%s' publicada no tópico '%s'\n", PAYLOAD, TOPIC);
    
    // Espera a publicação ser completada
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Entrega da mensagem confirmada (token: %d)\n", token);

    // Desconecta do broker e destrói o cliente
    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS) {
        printf("Falha ao desconectar, código de retorno %d\n", rc);
    }
    MQTTClient_destroy(&client);
    
    return rc;
}