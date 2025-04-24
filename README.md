![](media/MotionDriveRC_logo.png)

# MotionDriveRC-Glove

**MotionDriveRC** is a wearable gesture-based control system for an RC car. The glove uses an ADXL345 accelerometer and ESP32 microcontroller to capture hand motions and translate them into real-time steering and throttle commands via ESP-NOW wireless communication.

---

## Features

- Real-time gesture recognition using ADXL345 (pitch, roll, yaw)
- Low-latency ESP-NOW communication (under 15 ms)
- Wireless range up to 80 meters outdoors
- Modular system architecture
- Ergonomic, wearable glove design
- Normalized gesture mapping for consistent control

---

## Hardware Components

### Glove Module

- ESP32 microcontroller
- ADXL345 accelerometer (I²C)
- 3.7V 950mAh Li-Po battery
- 2A DC-DC booster to 5V
- Glove base for component mounting

### Optional Car Module (Paired)

- ESP32 microcontroller
- TB6612FNG or L298N motor driver
- 2 × 6V 1360 RPM DC motors
- SG90 servo motor
- 2 × 3.7V Li-ion batteries (7.4V total)
- Custom 3D-printed chassis and steering

---

## Sensor Processing and Data Mapping

The glove captures hand tilt using the ADXL345 sensor:

- X-axis (roll) is mapped to steering angle between 35° and 145°, with 90° as neutral.
- Y-axis (pitch) is mapped to PWM throttle control between 0 and 150.
- Data is normalized for consistency and sent wirelessly as two integer values representing steering and throttle power.

---

## Communication Protocol: ESP-NOW

- Peer-to-peer communication (no router required)
- MAC addresses are hardcoded for secure pairing
- Data transmission frequency is around 50–100 Hz
- Communication latency is under 15 ms
- Wireless range reaches up to 80 meters outdoors

---

## Getting Started

### Prerequisites

- Arduino IDE or PlatformIO
- ESP32 board
- Required libraries:
  - Wire
  - Adafruit_ADXL345
  - esp_now
  - WiFi

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/emiran0/MotionDriveRC-Glove.git
   ```
2. Open `src/main.cpp` in your IDE.

3. Set the MAC address of the RC car's ESP32 in the glove firmware.

4. Flash the firmware to your ESP32 board.

5. Power the glove module using a Li-Po battery and booster.

6. Mount components securely on the glove.

---

## Usage

1. Power on both glove and car modules.
2. The glove starts reading and transmitting tilt data wirelessly.
3. Move your hand:

- Tilt forward/backward to control throttle
- Tilt left/right to steer

4. Keep within wireless range for best performance.

---

## Demo

![MotionDriveRC Demo](media/demo_visualization.gif)

---

## Performance

| Feature               | Value                   |
| --------------------- | ----------------------- |
| Steering accuracy     | ±10°                    |
| Throttle accuracy     | ±5%                     |
| Communication latency | < 15 ms                 |
| Wireless range        | ~70–80 meters (outdoor) |
| Battery life (glove)  | ~2 hours                |

---

## Contributing

Contributions are welcome!  
Feel free to open issues or submit pull requests to improve functionality or add new features.

---

## License

This project is licensed under the [MIT License](LICENSE).

---

## Related Projects

- [MotionDriveRC-Car](https://github.com/emiran0/MotionDriveRC-Car)
