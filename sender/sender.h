// Temperature sensor
#define DHTPIN 27
#define DHTTYPE DHT22

//Deepsleep
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  4        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR bool debug = true;
RTC_DATA_ATTR uint8_t receiverAddr[] = {0x84, 0x0D, 0x8E, 0xE6, 0x89, 0x09};
