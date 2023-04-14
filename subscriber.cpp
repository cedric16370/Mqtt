#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <mqtt/async_client.h>

using namespace std;

typedef mqtt::async_client mqtt_client_t; // Nouveau type pour mqtt::async_client
typedef mqtt::connect_options mqtt_conn_opts_t; // Nouveau type pour mqtt::connect_options
typedef mqtt::const_message_ptr mqtt_msg_ptr_t; // Nouveau type pour mqtt::const_message_ptr

// Structure contenant les informations pour se connecter au broker MQTT
struct st_mqtt_conn_info {
    string server_address;
    string client_id;
    string topic;
} mqtt_conn_info = {
    "tcp://broker.emqx.io:1883",
    "Cedric.B",
    "/ynov/bordeaux/"
};

// Callback appelé lors de la réception d'un message sur le topic auquel on est abonné
void on_message_received(mqtt_msg_ptr_t msg) {
    cout << "Message reçu : " << msg->to_string() << endl;
}

int main(int argc, char* argv[]) {
    // Initialisation du client MQTT
    mqtt_client_t client(mqtt_conn_info.server_address, mqtt_conn_info.client_id);

    // Options de connexion au broker
    mqtt_conn_opts_t conn_opts;
    conn_opts.set_keep_alive_interval(20);
    conn_opts.set_clean_session(true);

    try {
        // Connexion au broker
        cout << "Connexion au broker MQTT..." << endl;
        mqtt_client_t::connect_options conn_options;
        client.connect(conn_options)->wait();
        cout << "Connecté au broker MQTT !" << endl;

        // Abonnement au topic spécifié dans mqtt_conn_info
        cout << "Abonnement au topic Ynov" << mqtt_conn_info.topic << "..." << endl;
        client.subscribe(mqtt_conn_info.topic, 0)->wait();
        cout << "Abonné au topic Ynov !" << endl;

        // Boucle infinie pour recevoir les messages
        while (true) {
            // Attente d'un message
            auto msg = client.consume_message();
            if (msg) {
                // Traitement du message reçu
                on_message_received(msg);
            }
        }
    }
    catch (const mqtt::exception& exc) {
        cerr << "Erreur : " << exc.what() << endl;
        return 1;
    }
    return 0;
}
