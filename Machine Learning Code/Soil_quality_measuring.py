
# this code will take the soil data like the humidity, moisture ,and temperature to check if it is good for planting 
# this will be integrated with the esp for getting the soil moisture and also the temperature, humidity values
import requests
import time
import pandas as pd
import joblib as jb
def get_sensor_data():
    url = "http://192.168.137.187"  # Replace <ESP32_IP> with the actual IP address of your ESP32D
    try:
        model=jb.load('soil_prediction.joblib')
        response = requests.get(url)
        if response.status_code == 200:
            print("Sensor Data:")
            print(response.text)
            data = response.text.split('\n')
            
            temp = None
            humidity = None
            soil_moisture = None
            
            for line in data:
                if "Temperature" in line:
                    temp = line.split(":")[1].strip().split(' ')[0]
                elif "Humidity" in line:
                    humidity = line.split(":")[1].strip().split(' ')[0]
                elif "Soil Moisture" in line:
                    parts = line.split(":")
                    soil_moisture_part = parts[1].strip()
                    soil_moisture_value = soil_moisture_part.split(' ')[0]
                    soil_moisture = soil_moisture_value
            
            if temp is not None and humidity is not None and soil_moisture is not None:
                temp = float(temp)
                humidity = float(humidity)
                soil_moisture = int(soil_moisture)
                
                print(f"Parsed Data - Temperature: {temp} C, Humidity: {humidity} %, Soil Moisture: {soil_moisture}")
                data={'soil_moisture':[soil_moisture],'temperature':[temp],'humidity':[humidity]}
                df = pd.DataFrame(data)
                prediction = model.predict(df)
                
                if prediction== 1:
                    print("The conditions are good for planting.")
                else:
                    print("The conditions are not good for planting.")
            else:
                print("Failed to parse sensor data correctly.")
        else:
            print("Failed to get data from ESP32, Status code:", response.status_code)
    except requests.exceptions.RequestException as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    while True:
        get_sensor_data()
        time.sleep(5)  # Wait for 5 seconds before the next request
