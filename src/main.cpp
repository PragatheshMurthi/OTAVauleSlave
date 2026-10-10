#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

    void vmain_setup(void);
    void vmain_loop(void);

#ifdef __cplusplus
}
#endif

void setup() {
  // put your setup code here, to run once:
  vmain_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  vmain_loop();
}

