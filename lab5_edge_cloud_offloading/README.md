# Lab 5 - Edge Cloud Offloading
This repository contains all source code, models, training scripts, and documentation for the Edge-to-Cloud gesture recognition system using the ESP32 and Flask. Building on Lab 4, this project enhances system performance and user experience by offloading low-confidence predictions from the edge device to a cloud-based model deployed on Microsoft Azure. This edge-first, cloud-fallback allows for more accuracte gesture classification while balancing latency, reliability and resource constraints.

## Setup Instructions
1. Clone the repository
2. Set up `server` folder with python3.9 
   ```
    python3 -m venv venv
    source venv/bin/activate
    pip install -r app/requirements.txt
   ```
3. Run the flask server by running `python app.py`
4. Deploy on ESP32 by opening the `ESP32_to_cloud` `main.cpp` file in PlatformIO
5. Update the SSID, password and logURL and predictURL to match your local IP address.
6. Flash the code to your ESP32S3 board
7. Open the serial monitor to observe local inference and cloud fallback as you make the gestures

