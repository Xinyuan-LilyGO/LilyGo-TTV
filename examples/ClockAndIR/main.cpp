

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <IRremote.h>
#include <U8g2lib.h>
#include "pcf8563.h"
#include "utilities.h"

IRrecv irrecv(IR_RECV);
PCF8563_Class rtc;

U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);


const char *ssid                = "YOUR_SSID";
const char *password            = "YOUR_PASS";

const char *ntpServer           = "pool.ntp.org";
const long  gmtOffset_sec       = 28800;
const int   daylightOffset_sec  = 0;

decode_results results;


uint32_t mm = 0, timeout = 0;
bool ir_recv = false;

#define SUN 0
#define SUN_CLOUD  1
#define CLOUD 2
#define RAIN 3
#define THUNDER 4

void drawWeatherSymbol(u8g2_uint_t x, u8g2_uint_t y, uint8_t symbol)
{
    // fonts used:
    // u8g2_font_open_iconic_embedded_6x_t
    // u8g2_font_open_iconic_weather_6x_t
    // encoding values, see: https://github.com/olikraus/u8g2/wiki/fntgrpiconic

    switch (symbol) {
    case SUN:
        u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
        u8g2.drawGlyph(x, y, 69);
        break;
    case SUN_CLOUD:
        u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
        u8g2.drawGlyph(x, y, 65);
        break;
    case CLOUD:
        u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
        u8g2.drawGlyph(x, y, 64);
        break;
    case RAIN:
        u8g2.setFont(u8g2_font_open_iconic_weather_6x_t);
        u8g2.drawGlyph(x, y, 67);
        break;
    case THUNDER:
        u8g2.setFont(u8g2_font_open_iconic_embedded_6x_t);
        u8g2.drawGlyph(x, y, 67);
        break;
    }
}

void drawWeather(uint8_t symbol, int degree)
{
    drawWeatherSymbol(0, 55, symbol);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.setCursor(48 + 3, 49);
    u8g2.print(degree);
    u8g2.print("°C");        // requires enableUTF8Print()
}

void draw(const char *s, uint8_t symbol, int degree)
{
    u8g2.clearBuffer();                     // clear the internal memory
    drawWeather(symbol, degree);            // draw the icon and degree only once
    u8g2.sendBuffer();
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

    uint8_t ypos = 16;
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_luRS08_tf);
    u8g2.setCursor(10, ypos);
    u8g2.setDrawColor(1);

    Wire.begin(RTC_SDA, RTC_SCL);
    Wire.beginTransmission(0x51);
    if (0 == Wire.endTransmission()) {
        u8g2.println("RTC check pass");
        Serial.println("RTC PASS");
    } else {
        u8g2.println("RTC check failed");
        Serial.println("RTC FAILED");
    }
    u8g2.sendBuffer();

    rtc.begin();

    /******************************************************************
     *   WIFI TIME SYNC FUNCTIONS
     ******************************************************************/
    if (String(ssid) == "YOUR_SSID" && String(password) ==  "YOUR_PASS") {
        Serial.println("WiFi authentication is not set, skip time synchronization");

        ypos += 16;
        u8g2.setCursor(10, ypos);
        u8g2.println("WiFi auth is not set,");
        ypos += 16;
        u8g2.setCursor(10, ypos);
        u8g2.println("skip time sync");
        u8g2.sendBuffer();
        delay(5000);
        return;
    }

    ypos += 16;
    u8g2.setCursor(10, ypos);

    u8g2.print("Connecting to ");
    u8g2.println(ssid);
    Serial.printf("Connecting to %s \n", ssid);
    u8g2.sendBuffer();


    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    ypos += 16;
    u8g2.setCursor(10, ypos);
    u8g2.print("\"");
    u8g2.print(ssid);
    u8g2.print("\"");
    u8g2.println(" Connected");
    Serial.println(" CONNECTED");
    u8g2.sendBuffer();

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

    Serial.println("Enabling IRin");
    irrecv.enableIRIn(); // Start the receiver
    delay(5000);
}

void clock_run(void)
{
    RTC_Date t = rtc.getDateTime();

    u8g2.setFont(u8g2_font_logisoso38_tr);

    u8g2.firstPage();
    do {
        u8g2.setCursor(3, 50);
        u8g2.printf("%02d", t.hour);
        if (t.second % 2) {
            u8g2.setDrawColor(0);
            u8g2.print(" ");
            u8g2.setDrawColor(1);
            u8g2.setCursor(u8g2.tx - 10, 50);
        } else {
            u8g2.setDrawColor(1);
            u8g2.print(":");
        }
        u8g2.printf("%02d", t.minute);
    } while ( u8g2.nextPage() );
}



void loop(void)
{
    if (millis() - mm > 1000 && !ir_recv) {
        clock_run();
        mm += millis();
    }

    if (millis() - timeout > 10000 && ir_recv) {
        ir_recv = false;
        timeout += 10000;
    }

    if (irrecv.decode(&results)) {
        results.value &= 0xFFFFFF;
        Serial.println(results.value, HEX);
        if (results.value == 0xFFA857) {
            ir_recv = true;
            draw("What a beautiful day!", SUN, 27);
        } else if (results.value == 0xFFE01F) {
            ir_recv = true;
            draw("The sun's come out!", SUN_CLOUD, 19);
        } else if (results.value == 0xFF906F) {
            u8g2.sleepOn();
            //Set wake on touch
            esp_sleep_enable_ext1_wakeup(GPIO_SEL_15, ESP_EXT1_WAKEUP_ANY_HIGH);
            delay(500);
            esp_deep_sleep_start();
        }
        irrecv.resume(); // Receive the next value
    }
}

