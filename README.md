# Arduino Sense–Think–Act System 

This project demonstrates a modular Arduino Uno system using **Pin Change Interrupts (PCI)**, **Timer1 interrupts**, and **a polling mechanism** to implement a **Sense–Think–Act** pattern.

---

## Summary

This system includes:
- **3 Push Buttons:**
  - Button 1 (D8) and Button 2 (D9): Trigger interrupts using **Pin Change Interrupts (PCI)**.
  - Button 3 (D4): Polled manually in the `loop()` function.
- **Timer1 Interrupt**: Toggles an LED every 1 second.
- **Buzzer (D6)**: Sounds when any button is pressed.
- **Green LED (D5)**: Flashes on each timer interrupt.

Flags (`volatile bool`) are used to safely pass data from Interrupt Service Routines (ISRs) to the `loop()` function without doing logic inside the ISRs.

---

## Hardware Used

- Arduino Uno R3
- 3 Push Buttons
- 1 Green LED
- 1 Buzzer
- 3 × 10kΩ Pull-down Resistors
- 1 × 220Ω Resistor (for LED)
- Breadboard & Jumper Wires

---

## Circuit Overview

### Buttons
| Button      | Pin | Trigger Method | Resistor       |
|-------------|-----|----------------|----------------|
| button1     | D8  | PCI Interrupt  | 10kΩ pull-down |
| button2     | D9  | PCI Interrupt  | 10kΩ pull-down |
| button3     | D4  | Polled         | 10kΩ pull-down |

### Actuators
| Device | Pin | Description        |
|--------|-----|--------------------|
| LED    | D5  | Blinks every 1 sec |
| Buzzer | D6  | Beeps on button press |

---

## Code Behavior

- `ISR(PCINT0_vect)`: Detects rising edge on D8 and D9 and sets `button1Pressed` or `button2Pressed` flag.
- `ISR(TIMER1_COMPA_vect)`: Sets `timerFlag` every 1 second.
- `loop()`:
  - Polls D4.
  - If a button flag is set, the buzzer sounds.
  - If timerFlag is set, LED toggles.

---

## How to Run

1. **Wiring**: Follow the Tinkercad simulation -to run on on tnkercad simpy click on start simulation.
2. **Upload Code**:
    - Open the `.ino` file or paste the code into Arduino IDE.
    - Select the correct COM port and board (Arduino Uno).
    - Upload the sketch.
3. **Monitor Serial Output**:
    - Open Serial Monitor (9600 baud) to see logs for button presses and timer triggers.

---

