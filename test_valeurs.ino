void setup() {
	Serial.begin(115200);
	pinMode(A0, INPUT); 				// capteur en lecture
}


void loop() {
	Serial.println(analogRead(A0)); 	//affichage humidité
	delay(5000); 						// pause de 5 secondes
}