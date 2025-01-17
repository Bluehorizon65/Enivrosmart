


# this code is to get the gestures from the user using mediapipe and control the leds by sending each value 
# like 1 finger - will send 1 to esp for controlling a single led like that
import cv2
import mediapipe as mp
import requests

mp_draw = mp.solutions.drawing_utils
mp_hand = mp.solutions.hands

hands = mp_hand.Hands(min_detection_confidence=0.7, min_tracking_confidence=0.5)

base_url = "http://192.168.137.38/led_"  # Update esp ip
led_off_url = "http://192.168.137.38/led_off"  # Update eps ip 

video = cv2.VideoCapture(0)

tipIds = [4, 8, 12, 16, 20]

while True:
    ret, image = video.read()
    if not ret:
        print("Failed to grab frame")
        break

    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    image_rgb.flags.writeable = False

    results = hands.process(image_rgb)
    image_rgb.flags.writeable = True
    image = cv2.cvtColor(image_rgb, cv2.COLOR_RGB2BGR)

    lmList = []
    if results.multi_hand_landmarks:
        for hand_landmark in results.multi_hand_landmarks:
            for id, lm in enumerate(hand_landmark.landmark):
                h, w, c = image.shape
                cx, cy = int(lm.x * w), int(lm.y * h)
                lmList.append([id, cx, cy])
            mp_draw.draw_landmarks(image, hand_landmark, mp_hand.HAND_CONNECTIONS)

    fingers = []
    if len(lmList) != 0:
        if lmList[tipIds[0]][1] > lmList[tipIds[0] - 1][1]:
            fingers.append(1)
        else:
            fingers.append(0)

        for id in range(1, 5):
            if lmList[tipIds[id]][2] < lmList[tipIds[id] - 2][2]:
                fingers.append(1)
            else:
                fingers.append(0)

        total = fingers.count(1)

        for i in range(1, 6):
            if i <= total:
                requests.get(base_url + str(i))
            else:
                requests.get(led_off_url)

        cv2.rectangle(image, (20, 300), (270, 425), (0, 255, 0), cv2.FILLED)
        cv2.putText(image, str(total), (45, 375), cv2.FONT_HERSHEY_SIMPLEX,
                    2, (255, 0, 0), 5)
        cv2.putText(image, "LEDs", (100, 375), cv2.FONT_HERSHEY_SIMPLEX,
                    2, (255, 0, 0), 5)

    cv2.imshow("Frame", image)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

video.release()
cv2.destroyAllWindows()
