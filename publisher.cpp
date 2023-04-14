#include <iostream>
#include <cstring>
#include <cstdlib>
#include <mqtt/async_client.h>

using namespace std;

const string ADDRESS { "tcp://broker.emqx.io:1883" }; // adresse du broker
const string CLIENT_ID { "Cedric" }; // identifiant du client
const string TOPIC { "/ynov/bordeaux/" }; // topic sur lequel publier le message
const string PAYLOAD { "Cedric" }; // message à publier

const int QOS = 0; 

class callback : public virtual mqtt::callback // classe de callback pour traiter les messages reçus
{
public:
    void connection_lost(const string& cause) override { // appelée lorsque la connexion est perdue
        cout << "\nConnexion perdue" << endl;
        if (!cause.empty()) {
            cout << "\tCause : " << cause << endl;
        }
    }

    void delivery_complete(mqtt::delivery_token_ptr token) override { // appelée lorsque le message est envoyé avec succès
        cout << "\nMessage envoyé avec succès" << endl;
    }

    void message_arrived(const mqtt::const_message_ptr msg) override { // appelée lorsqu'un message est reçu
        cout << "Message reçu : "
            << msg->to_string() << endl; // affiche le message reçu
    }
};

int main(int argc, char* argv[]) {

    mqtt::async_client client(ADDRESS, CLIENT_ID); // création du client

    callback cb; // création de l'objet de callback
    client.set_callback(cb); // configuration du callback

    mqtt::connect_options connOpts; // création des options de connexion
    connOpts.set_clean_session(true); // définit une session propre

    cout << "Connexion au broker '" << ADDRESS << "'..." << flush;
    mqtt::token_ptr conntok = client.connect(connOpts); // connexion au broker
    conntok->wait(); // attente de la connexion
    cout << "Connecté !" << endl;

    mqtt::token_ptr subtok = client.subscribe(TOPIC, QOS); // abonnement au topic
    subtok->wait(); // attente de l'abonnement
    cout << "Abonnement au topic '" << TOPIC << "'..." << flush;

    mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, PAYLOAD); // création du message à publier
    pubmsg->set_qos(QOS); // définition de la qualité de service
    cout << "\nEnvoi du message '" << PAYLOAD << "' sur le topic '"
        << TOPIC << "'..." << flush;
    mqtt::token_ptr pubtok = client.publish(pubmsg); // publication du message
    pubtok->wait(); // attente de la publication

    client.disconnect(); // déconnexion du broker
    cout << "Déconnexion du broker." << endl;

    return 0;
}
