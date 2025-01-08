void setup() {
  // Stelle die serielle Kommunikation her
  Serial.begin(9600);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  // Stelle sicher, dass alles ausgeschaltet und Ventile geschlossen sind
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
}

// Ermittelt den Stromwert über den Sensor
float readCurrent() {
  unsigned int x = 0;
  float AcsValue = 0.0, Samples = 0.0, AvgAcs = 0.0, AcsValueF = 0.0;

  for (x = 0; x < 150; x++) {   // 150 Proben entnehmen
    AcsValue = analogRead(A0);  // Stromsensorwerte lesen
    Samples += AcsValue;        // Proben zusammenzählen
    delay(3);                   // ADC vor der nächsten Probe 3ms setzen lassen
  }
  AvgAcs = Samples / 150.0;  // Durchschnitt der Proben berechnen

  // Konvertiere den Spannungswert und berechne den Strom
  AcsValueF = (2.515 - (AvgAcs * (5.0 / 1024.0))) / 0.100;

  Serial.print(AcsValueF);  // Den gemessenen Strom im seriellen Monitor ausgeben
  Serial.println("A");
  delay(50);

  return AcsValueF;
}

// Bewegt den Motor, bis die Endstellung erreicht wird
void moveToEnd(bool forward) {
  if (forward) {
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
  } else {
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
  }

  while (true) {
    float current = readCurrent();
    if (abs(current) <= 0.05) {  // Schwellenwert für die Endstellung
      Serial.println("ende");
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      break;
    }
    delay(50);
  }
}

void startStirMotor() {
  // Rührmotor einschalten
  digitalWrite(8, LOW);
}

void stopStirMotor() {
  // Rührmotor ausschalten
  digitalWrite(8, HIGH);
}

void startHydraulicPump() {
  // Hydraulikpumpe einschalten
  digitalWrite(9, LOW);
}

void stopHydraulicPump() {
  // Hydraulikpumpe ausschalten
  digitalWrite(9, HIGH);
}

// Ventil vollständig öffnen
void openValve() {
  moveToEnd(true);
}

// Ventil vollständig schließen
void closeValve() {
  moveToEnd(false);
}

// Prüft, ob die Starttaste gedrückt ist
bool isStartButtonPressed() {
  return digitalRead(7) == LOW;
}

// Prüft, ob Endtaster 1 gedrückt ist
bool isEndButton1Pressed() {
  return digitalRead(5) == LOW;
}

// Prüft, ob Endtaster 2 gedrückt ist
bool isEndButton2Pressed() {
  return digitalRead(6) == LOW;
}

// Variablen für den Programmablauf
int step = 0;
unsigned long step2StartTime = 0;

void loop() {
  closeValve();
  stopHydraulicPump();
  while (!isEndButton1Pressed())
    ;
  stopStirMotor();
  while (!(isStartButtonPressed() && isEndButton1Pressed()))
    ;
  openValve();
  startHydraulicPump();
  while (!(isEndButton2Pressed() || i++ > 1000))
    delay(1);
  ;
  stopHydraulicPump();
  startStirMotor();
}
