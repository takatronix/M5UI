#include <M5UI.h>

M5UIClass m5ui(&M5.Display);

void setup() {
  m5ui.begin();
  m5ui.Screen.printDeviceInfo();
  m5ui.Screen.println("Hello World");
}

void loop() {
  m5ui.update();
  //m5ui.update();
 // M5UI.update();
}
