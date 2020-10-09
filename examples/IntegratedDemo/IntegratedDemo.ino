#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <IRremote.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <U8g2lib.h>
#include <pcf8563.h>
#include "utilities.h"
#include "main.h"
#include <FS.h>
#include <SPIFFS.h>

U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);

IRrecv irrecv(IR_RECV);
PCF8563_Class rtc;
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp(&Wire1);
bool findAHT = false, findBMP = false, findRTC = false;
uint32_t timeStamp;

const char *ssid                = "YOUR_SSID";
const char *password            = "YOUR_PASS";
const char *ntpServer           = "pool.ntp.org";
const long  gmtOffset_sec       = 28800;
const int   daylightOffset_sec  = 0;

void scan();

void drawLogo()
{
    uint8_t mdy = 0;
    u8g2.setFlipMode(0);
    u8g2.setFontMode(1); // Transparent
    u8g2.setDrawColor(1);
    u8g2.setFontDirection(0);
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_inb24_mf);
        u8g2.drawStr(0, 30 - mdy, "LilyGo");
        u8g2.drawHLine(2, 35 - mdy, 47);
        u8g2.drawHLine(3, 36 - mdy, 47);
        u8g2.drawVLine(45, 32 - mdy, 12);
        u8g2.drawVLine(46, 33 - mdy, 12);
    } while ( u8g2.nextPage() );
    delay(5000);
}

void setup(void)
{
    Serial.begin(115200);

    /*Turn on power control*/
    pinMode(PWR_ON, OUTPUT);
    digitalWrite(PWR_ON, HIGH);

    /*Power on the display*/
    pinMode(OLED_PWR, OUTPUT);
    digitalWrite(OLED_PWR, HIGH);

    /*Set touch Pin as input*/
    pinMode(TOUCH_PIN, INPUT);

    /*Touch chip power control*/
    pinMode(TOUCH_PWR, PULLUP);
    digitalWrite(TOUCH_PWR, HIGH);

    int16_t ypos = 16;

    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.clearBuffer();

    Wire.begin(RTC_SDA, RTC_SCL);

    Wire1.begin(21, 22);

    drawLogo();

    scan();

    findRTC = rtc.begin();
    findAHT = aht.begin(&Wire1);
    findBMP = bmp.begin();

    u8g2_uint_t  pw = u8g2.getStrWidth("+");
    u8g2_uint_t  lw = u8g2.getStrWidth("-");

    u8g2_uint_t px = u8g2.getWidth() - pw - 2;
    u8g2_uint_t  lx = u8g2.getWidth() - lw - 5;

    ypos = 10;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_luRS08_tf);
    u8g2.setDrawColor(1);
    u8g2.setCursor(10, ypos);
    u8g2.print("PCF8563");
    u8g2.setCursor(findRTC ? px : lx, ypos);
    u8g2.print(findRTC ? "+" : "-");

    ypos += 12;
    u8g2.setCursor(10, ypos);
    u8g2.print("AHT20");
    u8g2.setCursor(findAHT ? px : lx, ypos);
    u8g2.print(findAHT ? "+" : "-");

    ypos += 12;
    u8g2.setCursor(10, ypos );
    u8g2.print("BMP280");
    u8g2.setCursor(findBMP ? px : lx, ypos);
    u8g2.print(findBMP ? "+" : "-");

    ypos += 12;
    bool setWifi = String(ssid) == "YOUR_SSID" && String(password) ==  "YOUR_PASS";
    u8g2.setCursor(10, ypos);
    u8g2.print("WiFi");
    u8g2.setCursor(setWifi ? lx : px, ypos);
    u8g2.print(  setWifi ? "-" : "+");

    u8g2.sendBuffer();

    if (findBMP) {
        /* Default settings from datasheet. */
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                        Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    }

    /******************************************************************
     *   WIFI TIME SYNC FUNCTIONS
     ******************************************************************/
    if (!setWifi && ESP_RST_DEEPSLEEP !=  esp_reset_reason())  {
        ypos += 12;
        Serial.printf("Connecting to %s \n", ssid);

        u8g2.setCursor(10, ypos);
        u8g2.print("Connecting to ");
        u8g2.println(ssid);
        u8g2.sendBuffer();

        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        //init and get the time
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Failed to obtain time, Restart in 3 seconds");
            delay(3000);
            esp_restart();
            while (1);
        }
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
        // Sync local time to external RTC
        rtc.syncToRtc();
    }

    irrecv.enableIRIn(); // Start the receiver

    if (ESP_RST_DEEPSLEEP !=  esp_reset_reason()) {
        delay(5000);
    }
}


extern uint8_t menuloop(void);
extern void gameloop(void);
void mainloop(void);
extern void game0loop(void);
extern void videoloop();

void loop(void)
{
    uint8_t val =  menuloop();
    Serial.printf("Select : %d\n", val);
    switch (val) {
    case 0:
        mainloop();
        break;
    case 1:
        gameloop();
        break;
    case 2:
        game0loop();
        break;
    case 3:
        irrecv.disableIRIn();
        videoloop();
        irrecv.enableIRIn();
        break;
    case 4:
        u8g2.sleepOn();
        esp_sleep_enable_ext1_wakeup(GPIO_SEL_15, ESP_EXT1_WAKEUP_ANY_HIGH);
        delay(500);
        esp_deep_sleep_start();
        break;
    default:
        break;
    }
}

bool touched()
{
    return digitalRead(TOUCH_PIN) == HIGH;
}

uint8_t u8x8_GetMenuEvent(u8x8_t *u8x8)
{
    uint8_t result_msg = 0;   /* invalid message, no event */
    decode_results results;
    static uint32_t prev_value = 0;

    int  ret = irrecv.decode(&results);
    if (!ret) {
        return result_msg;
    }
    // Serial.println(results.value, HEX);
    if (results.value != 0xFFFFFFFF) {
        prev_value = results.value;
    }
    switch (prev_value) {
    case 0xFFA25D:  //CH-
        result_msg = U8X8_MSG_GPIO_MENU_HOME;
        break;
    case 0xFF629D:  //CH
        break;
    case 0xFFE21D:  //CH+
        break;
    case 0xFF6897:  //0
        break;
    case 0xFF9867:  //100+
        break;
    case 0xFFB04F:  //200+
        break;
    case 0xFF30CF:  //1
        break;
    case 0xFF18E7:  //2
        result_msg = U8X8_MSG_GPIO_MENU_UP;
        break;
    case 0xFF7A85://3
        break;
    case 0xFF10EF://4
        result_msg = U8X8_MSG_GPIO_MENU_PREV;
        break;
    case 0xFF38C7://5
        break;
    case 0xFF5AA5://6
        result_msg = U8X8_MSG_GPIO_MENU_NEXT;
        break;
    case 0xFF42BD://7
        break;
    case 0xFF4AB5://8
        result_msg = U8X8_MSG_GPIO_MENU_DOWN;
        break;
    case 0xFF52AD://9
        break;
    case 0xFFA857:  //+
        break;
    case 0xFFE01F:  // -
        break;
    case 0xFF22DD://NEXT
        result_msg = U8X8_MSG_GPIO_MENU_PREV;
        break;
    case 0xFFC23D://PLAY
        result_msg = U8X8_MSG_GPIO_MENU_SELECT;
        break;
    case 0xFF02FD://PREV
        result_msg = U8X8_MSG_GPIO_MENU_NEXT;
        break;
    case 0xFF906F:  //EQ
        break;
    default:
        break;
    }
    irrecv.resume(); // Receive the next value
    return result_msg;
}


void mainloop()
{
    for (;;) {
        uint8_t val = u8g2.getMenuEvent();
        if (val == U8X8_MSG_GPIO_MENU_HOME) {
            return;
        }
        if (millis() >  timeStamp) {
            u8g2.setFont(u8g2_font_6x10_tr);
            u8g2.firstPage();
            do {
                u8g2.drawRFrame(2, 2, 60, 30, 3);
                u8g2.drawRFrame(64, 2, 60, 30, 3);

                if (findAHT) {
                    sensors_event_t humidity, temp;
                    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

                    u8g2.setCursor(6, 15);
                    u8g2.print(temp.temperature);

                    u8g2.print(" °C");
                    u8g2.setCursor(6, 28);
                    u8g2.print(temp.relative_humidity);
                    u8g2.print(" %rH");

                    Serial.print("Temperature: ");
                    Serial.print(temp.temperature);
                    Serial.println(" degrees C");
                    Serial.print("Humidity: ");
                    Serial.print(humidity.relative_humidity);
                    Serial.println(" %rH");
                }

                if (findBMP) {
                    float tempBMP =  bmp.readTemperature();
                    float tempPRESS = bmp.readPressure() / 1000.0F;
                    float tempALTITUDE = bmp.readAltitude(1013.25);

                    u8g2.setCursor(66, 15);
                    u8g2.print(tempBMP);
                    u8g2.print(" °C");

                    u8g2.setCursor(66, 28);
                    u8g2.print(tempPRESS);
                    u8g2.print("kPa");

                    Serial.print(F("Temperature = "));
                    Serial.print(tempBMP);
                    Serial.println(" *C");
                    Serial.print(F("Pressure = "));
                    Serial.print(tempPRESS);
                    Serial.println(" Pa");
                    Serial.print(F("Approx altitude = "));
                    Serial.print(tempALTITUDE);
                    Serial.println(" m");
                }

                RTC_Date t = rtc.getDateTime();
                u8g2.setFont(u8g2_font_8x13B_tf);
                u8g2.setCursor(42, 50);
                u8g2.printf("%02d", t.hour);
                if (t.second % 2) {
                    u8g2.setDrawColor(0);
                    u8g2.print(" ");
                    u8g2.setDrawColor(1);
                    u8g2.setCursor(u8g2.tx, 50);
                } else {
                    u8g2.setDrawColor(1);
                    u8g2.print(":");
                }
                u8g2.printf("%02d", t.minute);
            } while ( u8g2.nextPage() );
            timeStamp = millis() + 1000;
        }
    }
}

void scan()
{
    byte error, address;
    int nDevices;
    Serial.println("Scanning...");
    nDevices = 0;
    for (address = 1; address < 127; address++) {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire1.beginTransmission(address);
        error = Wire1.endTransmission();
        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println(" !");
            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknow error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
    // delay(5000); // wait 5 seconds for next scan
}
