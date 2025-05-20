#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Arduino.h>
#include <a515_magic_wand_inferencing.h>
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

// MPU6050 sensor
Adafruit_MPU6050 mpu;

// Sampling and capture variables
#define SAMPLE_RATE_MS 10  // 100Hz
#define CAPTURE_DURATION_MS 1000  // 1 second
#define FEATURE_SIZE EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE

// Capture state variables
bool capturing = false;
unsigned long last_sample_time = 0;
unsigned long capture_start_time = 0;
int sample_count = 0;

// Feature array to store accelerometer data
float features[FEATURE_SIZE];

// Button and LED pins
#define BUTTON_PIN D2    
#define RED_PIN D8        
#define GREEN_PIN D9      
#define BLUE_PIN D6      

bool button_was_pressed = false;

/**
 * @brief      Copy raw feature data in out_ptr
 */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void print_inference_result(ei_impulse_result_t result);
void run_inference(); 
void capture_accelerometer_data();

/**
 * Set LED color: pass true/false for R, G, B
 */
void setLEDColor(bool r, bool g, bool b) {
    digitalWrite(RED_PIN, r ? HIGH : LOW);
    digitalWrite(GREEN_PIN, g ? HIGH : LOW);
    digitalWrite(BLUE_PIN, b ? HIGH : LOW);
}

/**
 * Arduino setup function
 */
void setup() {
    Serial.begin(115200);

    // Initialize I2C + MPU6050
    Serial.println("Initializing MPU6050...");
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) delay(10);
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU6050 initialized successfully");
    Serial.println("Press button to start gesture capture");

    // Button + RGB LED setup
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    setLEDColor(false, false, false);  // LED off
}

/**
 * Capture accelerometer data for inference
 */
void capture_accelerometer_data() {
    if (millis() - last_sample_time >= SAMPLE_RATE_MS) {
        last_sample_time = millis();
        
        // Read accelerometer
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        if (sample_count < FEATURE_SIZE / 3) {
            int idx = sample_count * 3;
            features[idx] = a.acceleration.x;
            features[idx + 1] = a.acceleration.y;
            features[idx + 2] = a.acceleration.z;
            sample_count++;
        }

        if (millis() - capture_start_time >= CAPTURE_DURATION_MS) {
            capturing = false;
            Serial.println("Capture complete");
            run_inference();

            delay(800);
            setLEDColor(false, false, false); 
        }
    }
}

/**
 * Run inference on the captured data
 */
void run_inference() {
    if (sample_count * 3 < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        Serial.println("ERROR: Not enough data for inference");
        return;
    }

    ei_impulse_result_t result = { 0 };
    signal_t features_signal;
    features_signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
    features_signal.get_data = &raw_feature_get_data;

    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);
    if (res != EI_IMPULSE_OK) {
        Serial.print("ERR: Failed to run classifier (");
        Serial.print(res);
        Serial.println(")");
        return;
    }

    print_inference_result(result);
}

/**
 * Main loop - wait for button, then capture & infer
 */
void loop() {
    bool button_pressed = digitalRead(BUTTON_PIN) == LOW;

    if (button_pressed && !button_was_pressed && !capturing) {
        Serial.println("Button pressed! Starting gesture capture...");
        sample_count = 0;
        capturing = true;
        capture_start_time = millis();
        last_sample_time = millis();
        setLEDColor(true, true, true);  // White during capture
    }

    button_was_pressed = button_pressed;

    if (capturing) {
        capture_accelerometer_data();
    }
}

/**
 * Display prediction result and update LED color
 */
void print_inference_result(ei_impulse_result_t result) {
    float max_value = 0;
    int max_index = -1;

    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > max_value) {
            max_value = result.classification[i].value;
            max_index = i;
        }
    }

    if (max_index != -1) {
        const char* label = ei_classifier_inferencing_categories[max_index];
        Serial.print("Prediction: ");
        Serial.print(label);
        Serial.print(" (");
        Serial.print(max_value * 100);
        Serial.println("%)");

        // LED color mapping
        if (strcmp(label, "Z") == 0) {
            setLEDColor(true, false, false);   // Red
        } else if (strcmp(label, "V") == 0) {
            setLEDColor(false, true, false);   // Green
        } else if (strcmp(label, "O") == 0) {
            setLEDColor(false, false, true);   // Blue
        } else {
            setLEDColor(false, false, false);  // Unknown/off
        }
    }
}

