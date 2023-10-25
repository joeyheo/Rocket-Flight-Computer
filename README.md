
![Nuri v14](https://github.com/joeyheo/Rocket-Flight-Computer-Software/assets/73047475/a9634ac0-c7a5-4cc3-b60b-ea12a889238b)


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

## Wiring: 

<img width="592" alt="Screenshot 2023-10-24 at 11 27 37 PM" src="https://github.com/joeyheo/Rocket-Flight-Computer-Software/assets/73047475/4606568a-ed60-4686-9915-87d26cd10677">


## PCB Download:

[Nuri_V1_2022-07-25.zip](https://github.com/joeyheo/Rocket-Flight-Computer-Software/files/13162097/Nuri_V1_2022-07-25.zip)


## Disclaimer

The software provided, along with the accompanying electronics, is shared for educational and informational purposes only. By using, applying, or implementing this software and associated electronics, you agree to the following terms:

1. **No Liability**: Joey Heo, Vega Space, Vega Propulsion Laboratory, and any associated entities shall not be held responsible or liable, under any circumstances, for any direct, indirect, special, incidental, or consequential damages, losses, or expenses arising out of or in connection with the use of this software and electronics. This includes, but is not limited to, any fault, error, omission, interruption, or delay with respect thereto.

2. **User's Responsibility**: The user assumes full responsibility for the application, installation, maintenance, and testing of the software and electronics. It is the user's responsibility to ensure safe usage and to understand the limitations and potential risks.

3. **Not for Critical Use**: This software and the associated electronics are not intended for critical applications where failures or discrepancies could result in harm to property, the environment, or human life.

4. **No Warranty**: The software and electronics are provided "as is" without warranty of any kind, either expressed or implied, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose.

By utilizing the provided software and electronics, you acknowledge that you have read, understood, and agreed to the terms set forth in this disclaimer. If you do not agree to these terms, you are advised not to use the software and electronics.

---
**Note:** This README provides an overview of the Nuri Telemetry System, its features, and components. Before deploying for critical applications, ensure thorough testing in a controlled environment.
