# _ * _ Projet, en cours de rédaction ! _ * _ 

# MQTT-Capt_Humidite_Plantes
Capteur d'humidité des sols de plante. Transfert des données à Domoticz, via MQTT

But : Remonter les information d'humidité de la terre d'une plante, et transfèrer l'information à Domoticz via MQTT.

Voici comment créer un capteur de tempèrature et d'himidité, à l'aide d'un module DHT11, support d'un ESP8266, et envoyant les informations au format JSON par protocole MQTT. L'intéret est de récupérer les informations de température et humidité dans un outils domotique. Dans l'exemple de la vidéo, nous nous appuieront sur Domoticz.

Partie logiciel necessaire : 
- Driver USB CH340G : https://wiki.wemos.cc/downloads 
- Logiciel Arduino IDE : https://www.arduino.cc/en/Main/Software 
- URL à ajouter pour le Bord manager : http://arduino.esp8266.com/stable/package_esp8266com_index.json 

Installer la prise en charge des cartes ESP8266

Bibliothéques :
- pubsubclient : https://github.com/knolleary/pubsubclient
- ArduinoJson : https://github.com/bblanchon/ArduinoJson 

Dans IDE : Faire Croquis / inclure une bibliothéque / ajouter la bibliothèque ZIP.

Adaptation pour reconnaissance dans Domoticz : Dans le fichier PubSubClient.h : La valeur du paramètre doit être augmentée à 512 octets. Cette définition se trouve à la ligne 26 du fichier, sinon cela ne fonctionne pas avec Domoticz


Vidéo explicative sur YouTube :
