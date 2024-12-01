# Adafruit 60 LED Light Strip Control via Website  

This project demonstrates how to control an **Adafruit 60 LED RGB light strip** using a website. The ESP32 microcontroller serves as both the **web server** and the LED controller.  

## Features  
1. **Rainbow Mode**: Automatically cycles through a spectrum of vibrant colors.  
2. **Single Color Mode**: Set the light strip to any custom color using a color picker.  
3. **User-Friendly Interface**: The website provides an intuitive way to interact with and control the LED strip.  

## Components  
- **Adafruit 60 LED RGB Light Strip** (WS2812-based)  
- **ESP32 Microcontroller**  

## Project Setup  

### 1. Circuit Diagram  
1. Connect the data pin of the LED strip to one of the ESP32 GPIO pins (code uses GPIO4).  
2. Ensure the power (to VIN on ESP32) and ground connections of the LED strip are properly connected.   

### 2. Software Requirements  
- Arduino IDE (or a similar development environment)  
- ESP32 board library installed  
- Required Arduino libraries:  
  - **Adafruit NeoPixel**: For controlling the LED strip  
  - **WiFi**: For handling ESP32 WiFi connections  
  - **ESPAsyncWebServer**: For hosting the website
  - **LittleFS**: For uploading the website's HTML

### 3. Deployment  
1. Clone this repository:  
   ```bash  
   git clone <repository_url>  
   ```  
2. Upload the Arduino code to the ESP32 via the Arduino IDE. Ensure you configure the WiFi credentials and LED GPIO pin in the code.
3. Upload data file via littleFS.
4. Access the ESP32's IP address in a web browser to control the LED strip.  

## Demonstration  
- **Rainbow Mode**: Select "Rainbow Mode" from the website to see the LEDs cycling through a continuous spectrum of colors.  
- **Single Color Mode**: Use the color picker to choose a specific color and watch the LEDs light up accordingly.  

## Improvements and Future Work  
- Adding more lighting effects (e.g., pulse, strobe, wave patterns).  
- Expanding the website functionality to include a timer or presets.  
- Hosting the website externally for remote control via the internet.  

## License  
This project is open-source under the [MIT License](LICENSE).  

## Contribution  
Contributions are welcome! Feel free to fork this repository, create a pull request, or submit issues to improve this project.  

## Acknowledgments  
- [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel)  
- Tutorials and resources from the open-source community  
