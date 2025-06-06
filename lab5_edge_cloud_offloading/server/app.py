print("Starting import...")
from flask import Flask, request, jsonify
print("Flask imported.")
from tensorflow.keras.models import load_model
print("TensorFlow/Keras imported.")
import numpy as np
print("NumPy imported.")


print("App is loading...")

app = Flask(__name__)

print("Before loading model...")
model = load_model("wand_model.h5", compile=False)
print("Model loaded.")  # This won't show if loading fails silently

# Load model once at startup
gesture_labels = ["V", "O", "Z"]  # Update if needed

@app.route("/log", methods=["POST"])
def log_prediction():
    data = request.get_json()
    print("📝 Logged prediction:", data)
    return jsonify({"status": "logged"})
    
@app.route("/", methods=["GET"])
def home():
    return "Wand Gesture API is running!"

@app.route("/predict", methods=["POST"])
def predict():
    try:
        data = request.get_json().get("data")
        if not data:
            raise ValueError("Missing 'data' field")

        input_array = np.array(data).reshape(1, -1)  # Reshape for model input
        prediction = model.predict(input_array)

        top_index = int(np.argmax(prediction))
        label = gesture_labels[top_index]
        confidence = float(prediction[0][top_index]) * 100

        return jsonify({
            "gesture": label,
            "confidence": confidence
        })

    except Exception as e:
        return jsonify({"error": str(e)}), 400

if __name__ == "__main__":
    print("Starting Flask app...")
    app.run(host="0.0.0.0", port=8000, debug=True)