# MT681
Library that simulates the behavior of an ISKRA MT681 smart meter.

It was designed be as portable as possible so it writes data with regard to endianness.

Example usage on an Arduino:
```c
#include <MT681.h>

MT681 sml;

void setup() {
    Serial.begin(9600);
}

void loop() {
    sml.injectActivePower(analogRead(A0));
  
    for(int i = 0; i < sml.getMessageLength(); i++) {
        Serial.write(MT681::message[i]);
    }
    Serial.flush();
    delay(1000);
}
```
