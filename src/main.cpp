
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include <esp_now.h>
#include <WiFi.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

typedef struct {
  int x;
  int y;
} DataPacket;

DataPacket dataToSend;
esp_now_peer_info_t peerInfo;
uint8_t peerAddress[] = {0xEC, 0x62, 0x60, 0x57, 0x23, 0x2D};

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
  // Serial.print("Status Code: ");
  // Serial.println(status);
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

// Normalize the x-axis acceleration to a value between -1 and 1
float normalizeX(float acceleration, float ms2_limit) {
  float normalized = acceleration / ms2_limit;
  if (normalized > 1) normalized = 1;
  else if (normalized < -1) normalized = -1;
  return normalized;
}

// Normalize the y-axis acceleration to a value between 0 and 2
float normalizeY(float acceleration, float ms2_limit) {
  float normalized = acceleration / ms2_limit;
  if (normalized > 1) normalized = 1;
  else if (normalized < 0) normalized = 0;
  return normalized;
}

// Change x-axis values to servo motor angles (0-180)
int xToAngle(float normalizedX) {
  return static_cast<uint8_t>(90 + 90 * normalizedX);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Accelerometer Test"); 
  Serial.println("");

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
      Serial.println("ESP-NOW Initialization Failed on Transmitter!");
      return;
  }

  esp_now_register_send_cb(onDataSent);
  memcpy(peerInfo.peer_addr, peerAddress, sizeof(peerAddress));
  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer!");
  } else {
      Serial.println("Peer added successfully!");
  }

  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  // accel.setRange(ADXL345_RANGE_16_G);
  accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  // accel.setRange(ADXL345_RANGE_2_G);
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");
}

void loop(void) 
{
  //Get a new sensor event 
  sensors_event_t event; 
  accel.getEvent(&event);
  
  // Retrieve sensor details for max and min values
  sensor_t sensor;
  accel.getSensor(&sensor);

  float X_ms2_limit = 10.0;
  float Y_ms2_limit = 10.0;

  // Normalize x-axis for direction between -1 and 1
  float normalizedX = normalizeX(event.acceleration.x, X_ms2_limit);
  
  // Normalize y-axis for magnitude between 0 and 2
  float normalizedY = normalizeY(event.acceleration.y, Y_ms2_limit);

  // Print the normalized values
  Serial.print("Direction (X): "); Serial.print(normalizedX);
  Serial.print("  Magnitude (Y): "); Serial.println(normalizedY);

  // Convert the normalized x-axis value to a servo motor angle
  uint8_t steerAngle = xToAngle(normalizedX);
  Serial.print("Steer Angle: "); Serial.println(steerAngle);

  // Prepare the data packet to send to the receiver node. (x: steerAngle, y: gasPedal)
  dataToSend.x = steerAngle;
  dataToSend.y = static_cast<int>(normalizedY * 250);

  // Sending data to the receiver node.
  esp_now_send(peerAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  delay(15);

}