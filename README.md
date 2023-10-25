
![Nuri v15](https://github.com/joeyheo/Rocket-Flight-Computer-Software/assets/73047475/9ba5bd42-088b-41bf-b206-558890208a37)

# Nuri Telemetry System (Transmitter & Receiver)

Developed and maintained by Vega Space & Vega Propulsion Laboratory (Joey Heo), this telemetry system consists of both transmitter and receiver software. It provides comprehensive telemetry data for rocketry applications, enabling real-time monitoring and post-flight analysis.

## Features:
1. Uses nRF24L01+ wireless modules for communication.
2. Telemetry data includes accelerometer, gyroscope, altitude, humidity, temperature, pressure, and deployment status.
3. Real-time pitch and roll angle calculation based on accelerometer data.
4. Basic complementary filter applied to roll and pitch values for smoothing.
5. Modular structure for easy debugging and modifications.

## Components:

### Libraries and Dependencies:
- **SPI Communication**: Essential for interfacing with SPI devices, particularly the nRF24L01+.
- **nRF24L01+ Radio Module**: Facilitates wireless communication between transmitter and receiver.

### Data Structures:
- `package` struct: A data structure that aggregates telemetry data for easy transmission and processing.

### Transmitter:
- Collects data from various sensors.
- Sends telemetry data wirelessly using nRF24L01+.
- Sets a specific radio address for transmission.
  
### Receiver:
- Initialized with a corresponding radio address for reception.
- Continually checks for incoming data from the nRF24L01+ module.
- Calculates roll and pitch angles based on received accelerometer data.
- Sends the telemetry data via serial communication for visualization, monitoring, or further processing.

## Usage:

1. **Hardware Setup**: Connect the nRF24L01+ module and sensors to the respective Arduino boards.
2. **Software Setup**: Load the transmitter code onto the board connected with the sensors and the receiver code onto the board connected with the nRF24L01+ receiver module.
3. **Data Visualization**: Connect the receiver Arduino board to a computer and open a serial monitor. Set the baud rate to 9600 to view the transmitted telemetry data.

## Future Enhancements:
1. Implement error handling for lost packets or communication failures.
2. Integration with more advanced data visualization tools.
3. Extend telemetry data with additional sensors as required.

---

**Note:** This README provides an overview of the Nuri Telemetry System, its features, and components. Before deploying for critical applications, ensure thorough testing in a controlled environment.
