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

# Explications et Tuto vidéo
Dans la suite des créations d'objets connectés, nous allons voir dans cette vidéo, comment créer un détecteur de sécheresse pour plante. L’objet connecté fonctionne via MQTT, et transmet ses informations à Domoticz. Pour rappel, nous utilisons ici Domoticz, mais le code est fourni, et peut être facilement adapté pour d’autre logiciel de domotique.

Nous verrons également comment, via Domoticz, recevoir un SMS ou un Mail lorsque la plante aura besoin d’eau. Vous avez le choix dans le code, soit de recevoir le taux d’humidité de la plante (configuration par défaut), soit de recevoir le taux de sécheresse.

Les valeurs retournées à Domoticz n’ayant pas besoin d’être envoyées trop fréquentes, une fois par heure étant grandement suffisant, on va activer le mode « Deep-sleep » sur le WEMOS. Ce mode permet de mettre le WEMOS en veille prolongée pendant un temps défini dans le programme. Le but étant d’économiser de la batterie pendant cette phase de veille. 

En plus de la partie programme pour cette mise en veille, il faut pour cela relier broche « RST » à la broche « D0 ». L’inconvénient de cette liaison est qu’il ne sera plus possible de téléverser un programme dans le WEMOS sans retirer le shunt. Je vous explique dans la vidéo comment faire pour facilement remédier à cet inconvénient.

## Soft et bibliothèques à installer :
Driver USB CH340G : https://wiki.wemos.cc/downloads
Logiciel Arduino IDE : https://www.arduino.cc/en/Main/Software
URL à ajouter pour le Bord manager : http://arduino.esp8266.com/stable/package_esp8266com_index.json
Installer la prise en charge des cartes ESP8266

Bibliothéques :
 - pubsubclient : https://github.com/knolleary/pubsubclient
 - ArduinoJson : https://github.com/bblanchon/ArduinoJson
Dans IDE : Faire Croquis / inclure une bibliothéque / ajouter la bibliothèque ZIP.


Adaptation pour reconnaissance dans Domoticz :
Dans le fichier PubSubClient.h : La valeur du paramètre doit être augmentée à 512 octets. Cette définition se trouve à la ligne 26 du fichier, sinon cela ne fonctionne pas avec Domoticz

## Tuto vidéo
Vidéo explicative sur YouTube : https://youtu.be/_O1ubdDx38c
