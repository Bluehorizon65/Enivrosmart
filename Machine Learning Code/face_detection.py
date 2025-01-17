

# this code will use the cv2 haarcascades algorithm to detect faces and send the signal to the esp via local host


import cv2
import requests
import time

face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Define the ESP32 URLs
esp32_url_on = "http://192.168.137.38/face_detected"  # esp ip address
esp32_url_off = "http://192.168.137.38/no_face_detected"  
def send_request_to_esp32(url):
    try:
        response = requests.get(url)
        if response.status_code == 200:
            print(f"Request to {url} successful")
        else:
            print(f"Failed to send request to {url}, Status code:", response.status_code)
    except requests.exceptions.RequestException as e:
        print(f"Error: {e}")


cap = cv2.VideoCapture(0)
led_on = False
last_detection_time = None

delay = 10  # 10 seconds delay

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    if not ret:
        print("Failed to grab frame")
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    if len(faces) > 0:
        if not led_on:
            send_request_to_esp32(esp32_url_on)
            led_on = True
            last_detection_time = time.time()
        else:
            last_detection_time = time.time()
    else:
        if led_on and time.time() - last_detection_time >= delay:
            send_request_to_esp32(esp32_url_off)
            led_on = False

    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

    cv2.imshow('Face Detection', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
