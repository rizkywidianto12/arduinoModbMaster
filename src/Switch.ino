const byte ButtonPin = 2;  // Wire button between this pin and Ground
unsigned ButtonPressCount = 0;


void setup()
{
  pinMode(ButtonPin, INPUT_PULLUP);
}


void loop()
{
  static unsigned long buttonPressTime = 0;


  if (!digitalRead(ButtonPin) && (millis() - buttonPressTime > 10))
  {
    buttonPressTime = millis();  // Debounce timer start
    ButtonPressCount++;
  }
  
  if (ButtonPressCount == 3)
  {
    // Something
  }
}
