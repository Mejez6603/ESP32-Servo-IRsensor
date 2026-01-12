This is a great idea. A well-structured `README.md` not only helps you document your progress but also makes the project "portfolio-ready" if you want to show it to others.

---

# ESP32 4-Servo IR Control System

A versatile robotics foundation using an **ESP32** to control four 180-degree servos via **Infrared (IR) remote**. This project features custom toggle logic, a status LED, and staggered movement sequences for smooth operation.

## 🚀 Features

* **Multi-Servo Control:** Independent and grouped control of 4 servos (A, B, C, and D).
* **IR Remote Mapping:** Custom hex-code mapping for precise remote control.
* **Toggle Logic:** Pressing the same button twice returns the system to a default "neutral" (90°) position.
* **Staggered Sequence:** Button 4 triggers a specific timing sequence (CD moves before AB) to manage weight load and power draw.
* **Visual Feedback:** Onboard LED indicators for signal reception and action status.

## 🛠 Hardware Requirements

* **Microcontroller:** ESP32 (WROOM/DA Module)
* **Actuators:** 4x 180° Micro Servos (e.g., SG90 or MG90S)
* **Sensor:** IR Receiver (e.g., VS1838B)
* **Power:** 5V/6V External Power Supply (4x AA batteries recommended)
* **Indicator:** 1x Red LED + 220Ω Resistor

## 📟 Pinout Diagram

| Component | ESP32 GPIO |
| --- | --- |
| **IR Receiver (Data)** | 35 |
| **Red LED** | 32 |
| **Servo A** | 13 |
| **Servo B** | 12 |
| **Servo C** | 14 |
| **Servo D** | 27 |

> **Note:** Servos must share a **Common Ground** with the ESP32 but should be powered by an external source to avoid brown-outs.

## 🕹 Remote Control Map

| Button | Hex Code | Action | Toggle (2nd Press) |
| --- | --- | --- | --- |
| **1** | `0xE` | Move AC (Left Side) | Return to 90° |
| **2** | `0x6` | Move BD (Right Side) | Return to 90° |
| **3** | `0xF` | Move AC + BD (All) | Return to 90° |
| **4** | `0x12` | Special Sequence | **Staggered Reset** (CD then AB) |

## 💻 Installation & Setup

1. **Install Libraries:** Open the Arduino IDE Library Manager and install:
* `IRremote` (by Armin Joachimsmeyer)
* `ESP32Servo` (by Kevin Harrington)


2. **Configure Pins:** Ensure your hardware matches the GPIO pins defined in the `.ino` file.
3. **Upload:** Select your ESP32 board and the correct COM port, then hit upload.
4. **Monitor:** Open Serial Monitor at **115200 baud** to see real-time command feedback.

## ⚠️ Important: Weight Load & Power

When driving 4 servos under load, ensure your power supply can provide at least **2 Amps**. If the ESP32 restarts when a button is pressed, check your battery levels and ensure the ground wires are connected together.

---

## 🛠 Troubleshooting Guide

### 1. Servos are Jittering or Not Moving

* **Common Ground:** Ensure there is a jumper wire connecting the **GND** of your external battery pack to a **GND pin** on the ESP32. Without this, the signal has no reference point.
* **Power Supply:** A 9V "rectangular" battery is insufficient. Use 4x AA batteries (6V) or a dedicated 5V 2A power adapter.
* **Logic Level:** Most servos work with the ESP32's 3.3V signal, but some "high-torque" models require a 5V signal. If they don't respond, you may need a logic level shifter.

### 2. IR Remote Not Responding

* **Pin Conflict:** On the ESP32, GPIO 35 is "Input Only." This is fine for IR, but ensure you aren't trying to use it for an output task.
* **Sunlight/Interference:** Bright sunlight or fluorescent lights can interfere with IR signals. Test in a shaded area.
* **Protocol Mismatch:** If the Serial Monitor shows nothing, your remote might use a non-standard protocol (like Sony or JVC). Run the `SimpleReceiver` example from the `IRremote` library to confirm.

### 3. ESP32 Restarts (Brown-out)

* **Current Spikes:** Moving 4 servos at once creates a massive power draw. If the ESP32 red LED flashes and the Serial Monitor shows `Brownout detector was triggered`, your battery is too weak or your wires are too thin.
* **Capacitor Fix:** Placing a large electrolytic capacitor (e.g., 100uF - 1000uF) across the positive and negative rails of your breadboard can help "cushion" these power spikes.

### 4. Incorrect Movement Range

* **Calibration:** Some 180° servos have different pulse width requirements. If your servo only moves 90°, adjust the `attach` parameters in the code:
`servoA.attach(SERVO_A_PIN, 500, 2400);` (Min/Max pulse in microseconds).

---

### Project Status

* [x] IR Remote Mapping
* [x] Toggle Logic Implementation
* [x] Staggered Reset Sequence
* [ ] Physical Chassis Construction
* [ ] Battery Box Integration

---