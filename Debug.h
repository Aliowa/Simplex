extern DallasTemperature sensors;
extern DeviceAddress inputSensor, outputSensor, outsideSensor;

void printAddress(DeviceAddress);

void debugSensors() {
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // method 1: by index
  if (!sensors.getAddress(inputSensor, 0))
    Serial.println("Unable to find address for Device 0");
  else if (!sensors.getAddress(outputSensor, 1))
    Serial.println("Unable to find address for Device 1");
  else if (!sensors.getAddress(outsideSensor, 2))
    Serial.println("Unable to find address for Device 2");
  else
    Serial.println("All sensors good");

  //Print addresses
  Serial.print("Device 0 Address: ");
  printAddress(inputSensor);
  Serial.println();
  Serial.print("Device 1 Address: ");
  printAddress(outputSensor);
  Serial.println();
  Serial.print("Device 2 Address: ");
  printAddress(outsideSensor);
  Serial.println();
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
