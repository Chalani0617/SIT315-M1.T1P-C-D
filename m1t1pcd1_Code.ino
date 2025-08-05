//Flags
volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
volatile bool timerFlag = false;

//Pins
const int button1Pin = 8;    // PCI
const int button2Pin = 9;    // PCI
const int button3Pin = 4;    // Polled
const int ledPin     = 5;
const int buzzerPin  = 6;

void setup() {
  Serial.begin(9600);

  //Input Pins
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);

  //Actuators
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  //Enable Pin Change Interrupts (PCI for D8 and D9)
  PCICR |= (1 << PCIE0); // Enable PCI for PCINT[7:0] (D8–D13)
  PCMSK0 |= (1 << PCINT0); // Enable D8 (PCINT0)
  PCMSK0 |= (1 << PCINT1); // Enable D9 (PCINT1)

  //Setup Timer1 for 1Hz (1-second) blink
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624;                      // 16MHz / (1024*1Hz) - 1
  TCCR1B |= (1 << WGM12);             // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
  TIMSK1 |= (1 << OCIE1A);            // Enable Timer1 interrupt
  sei();
}

void loop() {
  // PCI Button Handling
  if (button1Pressed) {
    Serial.println("Button 1 Pressed (D8 - PCI)");
    buzz();
    button1Pressed = false;
  }

  if (button2Pressed) {
    Serial.println("Button 2 Pressed (D9 - PCI)");
    buzz();
    button2Pressed = false;
  }

  // Polling Button 3
  if (digitalRead(button3Pin) == HIGH) {
    delay(50); // debounce
    if (digitalRead(button3Pin) == HIGH) {
      Serial.println("Button 3 Pressed (D4 - Polled)");
      buzz();
      delay(200);
    }
  }

  // Timer Flag LED toggle
  if (timerFlag) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    Serial.println("Timer Interrupt Triggered");
    timerFlag = false;
  }
}

//PCI ISR
ISR(PCINT0_vect) {
  static bool lastStateD8 = LOW;
  static bool lastStateD9 = LOW;

  bool currentD8 = digitalRead(button1Pin);
  bool currentD9 = digitalRead(button2Pin);

  // Detect rising edge
  if (currentD8 && !lastStateD8) button1Pressed = true;
  if (currentD9 && !lastStateD9) button2Pressed = true;

  lastStateD8 = currentD8;
  lastStateD9 = currentD9;
}

//Timer ISR
ISR(TIMER1_COMPA_vect) {
  timerFlag = true;
}

//Buzzer
void buzz() {
  digitalWrite(buzzerPin, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
}
