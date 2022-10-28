#include <SPI.h>
#include "printf.h"
#include "RF24.h"

class RF24 {
  private:
    RF24* radio;
    byte* address;
    boolean sender = false;
    int CE_PIN;
    int CSN_PIN;
}
