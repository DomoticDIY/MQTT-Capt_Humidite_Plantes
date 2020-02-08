/*
 WEMOS MQTT - Testeur d'humidité du sol des plantes
 Création Dominique PAUL.
 Dépot Github : https://github.com/DomoticDIY/MQTT-Capt_Humidite_Plantes
 Chaine YouTube du Tuto Vidéo : https://www.youtube.com/c/DomoticDIY
 Bibliothéques nécessaires :
  - pubsubclient : https://github.com/knolleary/pubsubclient
  - ArduinoJson : https://github.com/bblanchon/ArduinoJson
 Télécharger les bibliothèques, puis dans IDE : Faire Croquis / inclure une bibliothéque / ajouter la bibliothèque ZIP.
 Dans le gestionnaire de bibliothéque, charger le module ESP8266Wifi.
 Installer le gestionnaire de carte "WEMOS" 
 Si besoin : URL à ajouter pour le Bord manager : http://arduino.esp8266.com/stable/package_esp8266com_index.json
 Adaptation pour reconnaissance dans Domoticz :
 Dans le fichier PubSubClient.h : La valeur du paramètre doit être augmentée à 512 octets. Cette définition se trouve à la ligne 26 du fichier.
 Sinon cela ne fonctionne pas avec Domoticz
 Pour prise en compte du matériel :
 Installer si besoin le Driver USB CH340G : https://wiki.wemos.cc/downloads
 dans Outils -> Type de carte : LOLIN (WEMOS) D1 mini pro
  Flash mode 'QIO' (régle générale, suivant votre ESP, si cela ne fonctionne pas, tester un autre mode.
  Flash size : 1M (no SPIFFS)
  Port : Le port COM de votre WEMOS vu par windows dans le gestionnaire de périphériques.
*/

// Inclure les librairies.
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


// Déclaration des constantes, données à adapter à votre réseau.
// ------------------------------------------------------------
const char* ssid = "_MON_SSID_";                // SSID du réseau Wifi
const char* password = "_MOT_DE_PASSE_WIFI_";   // Mot de passe du réseau Wifi.
const char* mqtt_server = "_IP_DU_BROKER_";     // Adresse IP ou DNS du Broker.
const int mqtt_port = 1883;                     // Port du Brocker MQTT
const char* mqtt_login = "_LOGIN_";             // Login de connexion à MQTT.
const char* mqtt_password = "_PASSWORD_";       // Mot de passe de connexion à MQTT.
// ------------------------------------------------------------
// Variables de configuration :
char* topicIn = "domoticz/out";                       // Nom du topic envoyé par Domoticz
char* topicOut = "domoticz/in";                       // Nom du topic écouté par Domoticz
#define tempsSleep 3600                               // Nbre de secondes d'endormissement (3600 = 1H00)
// ------------------------------------------------------------
// Variables et constantes utilisateur :
String nomModule = "Humidité plante";                 // Nom usuel de ce module. Sera visible uniquement dans les Log Domoticz.
int Humidite = 1;                                     // Si Humidite = 1, alors on cacul la Valeur Humidite, si = 0, alors on calcul le taux de secheresse.
int pinData = A0;                                     // Pin sur lequel est connecté la sonde.
int idxDevice = 29;                                   // Index du Device à actionner.
int valMin = 4;                                       // Valeur mini relevée avec programme test
int valMax = 1024;                                    // Valeur maxi relevée avec programme test
// ------------------------------------------------------------



WiFiClient espClient;
PubSubClient client(espClient);
int resultatCurrentData = 0;


// SETUP
// *****
void setup() {
  pinMode(pinData, INPUT);   
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

// BOUCLE DE TRAVAIL
// *****************
void loop() {
  if (!client.connected()) {
    reconnect();
  } else {
    // On interroge la sonde.
    SendData();
    // On met le système en sommeil pour un temps défini
    ESP.deepSleep(tempsSleep*1000000);
  }
}


// CONNEXION WIFI
// **************
void setup_wifi() {
  // Connexion au réseau Wifi
  delay(10);
  Serial.println();
  Serial.print("Connection au réseau : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    // Tant que l'on est pas connecté, on boucle.
    delay(500);
    Serial.print(".");
  }
  // Initialise la séquence Random
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connecté");
  Serial.print("Addresse IP : ");
  Serial.println(WiFi.localIP());
}

// CONNEXION MQTT
// **************
void reconnect() {
  // Boucle jusqu'à la connexion MQTT
  while (!client.connected()) {
    Serial.print("Tentative de connexion MQTT...");
    // Création d'un ID client aléatoire
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Tentative de connexion
    if (client.connect(clientId.c_str(), mqtt_login, mqtt_password)) {
      Serial.println("connecté");
      
      // Connexion effectuée, publication d'un message...
      String message = "Connexion MQTT de "+ nomModule + " réussi sous référence technique : " + clientId + ".";
      // String message = "Connexion MQTT de "+ nomModule + " réussi.";
      StaticJsonBuffer<256> jsonBuffer;
      // Parse l'objet root
      JsonObject &root = jsonBuffer.createObject();
      // On renseigne les variables.
      root["command"] = "addlogmessage";
      root["message"] = message;
      
      // On sérialise la variable JSON
      String messageOut;
      if (root.printTo(messageOut) == 0) {
        Serial.println("Erreur lors de la création du message de connexion pour Domoticz");
      } else  {
        // Convertion du message en Char pour envoi dans les Log Domoticz.
        char messageChar[messageOut.length()+1];
        messageOut.toCharArray(messageChar,messageOut.length()+1);
        client.publish(topicOut, messageChar);
      }
        
      // On souscrit (écoute)
      client.subscribe("#");
    } else {
      Serial.print("Erreur, rc=");
      Serial.print(client.state());
      Serial.println(" prochaine tentative dans 5s");
      // Pause de 5 secondes
      delay(5000);
    }
  }
}


void SendData () {
  int difference = valMax - valMin;
  // Selon la valeur demandée : Humidité ou Sécheresse.
  if (Humidite == 1) {
    /* Calcul du pourcentage d'humidité */
    resultatCurrentData = ((analogRead(pinData)-valMin)*100)/difference;
  } else {
    /* Calcul du pourcentage de sécheresse */
    resultatCurrentData = 100-(((analogRead(pinData)-valMin)*100)/difference);
  }
  
  Serial.print("Valeur calculée : ");
  Serial.println(resultatCurrentData);

  StaticJsonBuffer<256> jsonBuffer;
  // Parse l'objet root
  JsonObject &root = jsonBuffer.createObject();
  // On renseigne les variables.
  root["type"] = "command";
  root["param"] = "udevice";
  root["idx"] = idxDevice;
  root["nvalue"] = resultatCurrentData;
  root["svalue"] = "0";
      
  // On sérialise la variable JSON
  String messageOut;
  if (root.printTo(messageOut) == 0) {
    Serial.println("Erreur lors de la création du message de connexion pour Domoticz");
  } else  {
    // Convertion du message en Char pour envoi dans les Log Domoticz.
    char messageChar[messageOut.length()+1];
    messageOut.toCharArray(messageChar,messageOut.length()+1);
    client.publish(topicOut, messageChar);
    // Pause de 5 secondes
    delay(5000);
    Serial.println("Message envoyé à Domoticz");
  }
}