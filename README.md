
# ESP8266 Web Server with OTA Firmware Update

This project is a simple web server designed for the ESP8266 microcontroller. It provides a user-friendly web interface for controlling and configuring the device, along with over-the-air (OTA) firmware update capabilities, making it easy to update your device's firmware remotely.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Copying WiFi Credentials](#copying-wifi-credentials)
  - [Initial USB Upload](#initial-usb-upload)
- [Usage](#usage)
  - [Configuring the Web Server](#configuring-the-web-server)
  - [Accessing the Web Interface](#accessing-the-web-interface)
  - [OTA Firmware Updates](#ota-firmware-updates)
  - [Controlling GPIO](#controlling-gpio)
- [Contributing](#contributing)
- [License](#license)

## Features

- Simple web server for ESP8266 microcontroller.
- User-friendly web interface for controlling and configuring the device.
- Over-the-air (OTA) firmware update capabilities.
- Easily customizable for your specific application.

## Getting Started

### Prerequisites

Before getting started, ensure you have the following:

- An ESP8266 microcontroller (e.g., NodeMCU, Wemos D1 Mini).
- Visual Studio Code (VSCode) with PlatformIO installed.
- A stable Wi-Fi connection for your ESP8266 to connect to.

### Installation

1. Clone or download this repository to your local machine.

```bash
git clone https://github.com/your-username/esp8266-webserver-ota.git
```

2. Open the project folder in Visual Studio Code (VSCode).

3. In VSCode, install the PlatformIO extension if you haven't already.

### Setting WiFi Credentials

To set up your Wi-Fi credentials, follow these steps:

1. Make a copy of the file `secrets.h.example` and rename it to `secrets.h`.

2. Update the `secrets.h` file with your Wi-Fi credentials. This file should contain your Wi-Fi SSID and password.

### Initial USB Upload

For the initial sketch upload to the ESP8266, use a USB connection. Follow these steps:

1. Connect your ESP8266 to your computer via USB.

2. Build and upload the code to the ESP8266 by clicking the PlatformIO toolbar's "Upload" button.

3. Connect the serial monitor and note the IP address the device prints after rebooting

4. Customize the project settings in the `platformio.ini` file, specifically the IP address of the device

5. Build and upload the code to your ESP8266 by clicking the PlatformIO toolbar's "Upload" button.

### OTA Firmware Updates

To perform over-the-air (OTA) firmware updates, follow these steps:

1. Build the updated firmware using the PlatformIO "Build" command.

2. Upload the new firmware to the ESP8266 by clicking the PlatformIO toolbar's "Upload" button.

3. The new firmware will be automatically detected and applied over-the-air the next time the device connects to the Wi-Fi network.

## Usage

### Accessing the Web Interface

Once the code is uploaded to your ESP8266, the device will create a local web server. You can access the web interface by opening a web browser and entering the ESP8266's IP address in the address bar.

```http
http://esp8266-ip-address/
```

Here, you can interact with your ESP8266 through the web interface.

### Controlling GPIO

You can control a GPIO pin by making HTTP requests to the following endpoints:

- `/RELAY/ON` - Turns the GPIO pin low.
- `/RELAY/OFF` - Turns the GPIO pin high.

You can set the specific GPIO pin in the source code by editing
```
#define RELAY 0
``` 


## Contributing

If you'd like to contribute to this project, please feel free to submit issues, pull requests, or feature suggestions.

## License

This project is licensed under the [MIT License](LICENSE), which means you are free to use, modify, and distribute the code for your own purposes.
