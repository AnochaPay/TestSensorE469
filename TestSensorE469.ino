#pragma region ===================================== Version =====================================
//V2022.01
//Create project 
#pragma endregion

#include <Wire.h>

#pragma region ===================================== Input =====================================
/// @brief Define pin of Interrupt 1
#define pinInt1 2
/// @brief Define pin of Interrupt 2
#define pinInt2 3
/// @brief Define pin of Select i2c
#define pinSelect 4

/// @brief For read input serial monitor event
String CMD_HELP = "help";
/// @brief For read input serial monitor event
String inputString;
/// @brief Flag read serial event complete
bool stringComplete;

/// @brief Flag of interrupt sensor 1
bool flagSensorInt1;
/// @brief Flag of interrupt sensor 2
bool flagSensorInt2;
#pragma endregion

#pragma region ===================================== Cal =====================================
/// @brief Event serial input interrup
void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}
/// @brief Check end cmd serial to set flag stringComplete on
void CheckInputSerial()
{
  if (stringComplete)
  {
    inputString.toLowerCase();
    inputString.replace(" ", "");
    inputString.replace("\n", "");
  }
}
/// @brief Clear string inputString to string.Emply
void ClearInputString()
{
  inputString = "";
}
/// @brief Check cmd serial to select function process
void SelectCmd()
{
  if (!stringComplete)
  {
    return;
  }
  stringComplete = false;
  SendReturnCmd();

//  if (inputString == LED_OFF + LED_ALL)
//  {
//    SetOffLed(ledAll);
//  }
//  else if (inputString.substring(0, 9).equalsIgnoreCase(TEST_FAIL))
//  {
//    String StrHeader = SeparateStringByCharacter(inputString, ',', 1);
//  }
//  else
//  {
//    SendErrorCmd();
//  }
  ClearInputString();
}
/// @brief Function split string
/// @param data Input string
/// @param separator Char split
/// @param index Select index string separated
/// @return Return string follow index
String SeparateStringByCharacter(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
/// @brief Set pin select SDA i2c chanel 1
void SetPinSelectI2cChanel1(){
  digitalWrite(pinSelect, HIGH);
}
/// @brief Set pin select SDA i2c chanel 2
void SetPinSelectI2cChanel2(){
  digitalWrite(pinSelect, LOW);
}
/// @brief Event interrupt sensor 1 on to set flagSensorInt1 true
void InterRuptSensor1()          
{                   
   flagSensorInt1 = true;
}
/// @brief Event interrupt sensor 2 on to set flagSensorInt2 true
void InterRuptSensor2()          
{                   
   flagSensorInt2 = true;
}
#pragma endregion

#pragma region ===================================== Display =====================================
/// @brief Send Message Error to serial monitor
void SendErrorCmd()
{
  Serial.println(F("Cmd Error!!"));
}
/// @brief Send Message StartUp to serial monitor
void SendWelcom()
{
  Serial.println(F("//================ Welcom to Test Sensor E469 ===============//"));
}
/// @brief Send message cmd return back to serial monitor
void SendReturnCmd()
{
  Serial.println(inputString);
}
/// @brief Show all cmd in project
void SendHelpCmd(){
  Serial.println(F("****************************************************************"));
  Serial.println(F("* 1. setoff,[led]                                              *"));
  Serial.println(F("*  Cmd set led off                                             *"));
  Serial.println(F("* Parameter:                                                   *"));
  Serial.println(F("*  led - is select led [red, green, yellow, all]               *"));
  Serial.println(F("* 2. seton,[led]                                               *"));
  Serial.println(F("*  Cmd set led on                                              *"));
  Serial.println(F("* Parameter:                                                   *"));
  Serial.println(F("*  led - is select led [red, green, yellow, all]               *"));
  Serial.println(F("* 3. get                                                       *"));
  Serial.println(F("*  Cmd get status switch testPin and power                     *"));
  Serial.println(F("* 4. testpass                                                  *"));
  Serial.println(F("*  Cmd for sim test mode to test pass                          *"));
  Serial.println(F("* 5. testfail,[step]                                           *"));
  Serial.println(F("*  Cmd for sim test mode to test fail                          *"));
  Serial.println(F("* Parameter:                                                   *"));
  Serial.println(F("*  step - is select step fail [1yellow, 1red, 2yellow, 2red    *"));
  Serial.println(F("*         2green, 3red, 3yellow, 3green, 4red, 4yellow, 4green,*"));
  Serial.println(F("*         5red, 5yellow, 5green]                               *"));
  Serial.println(F("* 6. help                                                      *"));
  Serial.println(F("*  Show all cmd                                                *"));
  Serial.println(F("****************************************************************"));
}
#pragma endregion

#pragma region ===================================== Main =====================================
/// @brief Void Setup of arduino
void setup()
{
  pinMode(pinInt1, INPUT_PULLUP);
  pinMode(pinInt2, INPUT_PULLUP);
  pinMode(pinSelect, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pinInt1),InterRuptSensor1,FALLING); 
  attachInterrupt(digitalPinToInterrupt(pinInt2),InterRuptSensor2,FALLING); 
  Wire.begin();
  Serial.begin(9600);
  SendWelcom();
}
/// @brief Void loop of arduino
void loop()
{
  CheckInputSerial();
  SelectCmd();
  delay(100);
}
#pragma endregion
