/*
  based on  hackffm/ESP32_BadApple
  github:https://github.com/hackffm/ESP32_BadApple
*/

#include <U8g2lib.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Wire.h>
#include "heatshrink_decoder.h"
#include "utilities.h"

#define RLEBUFSIZE          4096
#define READBUFSIZE         2048

U8G2_SSD1309_128X64_NONAME2_F_4W_SW_SPI u8g2(U8G2_R0,
        /* clock=*/ OLED_SCLK,
        /* data=*/  OLED_MOSI,
        /* cs=*/    OLED_CS,
        /* dc=*/    OLED_DC,
        /* reset=*/ OLED_RST);


static heatshrink_decoder hsd;
// global storage for putPixels
int16_t curr_x = 0;
int16_t curr_y = 0;

// global storage for decodeRLE
int32_t runlength = -1;
int32_t c_to_dup = -1;

uint32_t lastRefresh = 0;

void putPixels(uint8_t c, int32_t len)
{
    uint8_t b = 0;
    while (len--) {
        b = 128;
        for (int i = 0; i < 8; i++) {
            if (c & b) {
                u8g2.setDrawColor(0);
            } else {
                u8g2.setDrawColor(1);
            }
            b >>= 1;
            u8g2.drawPixel(curr_x, curr_y);
            curr_x++;
            if (curr_x >= 128) {
                curr_x = 0;
                curr_y++;
                if (curr_y >= 64) {
                    curr_y = 0;
                    u8g2.sendBuffer();
                    // 30 fps target rate
                    if (digitalRead(0)) while ((millis() - lastRefresh) < 33) ;
                    lastRefresh = millis();
                }
            }
        }
    }
}

void decodeRLE(uint8_t c)
{
    if (c_to_dup == -1) {
        if ((c == 0x55) || (c == 0xaa)) {
            c_to_dup = c;
        } else {
            putPixels(c, 1);
        }
    } else {
        if (runlength == -1) {
            if (c == 0) {
                putPixels(c_to_dup & 0xff, 1);
                c_to_dup = -1;
            } else if ((c & 0x80) == 0) {
                if (c_to_dup == 0x55) {
                    putPixels(0, c);
                } else {
                    putPixels(255, c);
                }
                c_to_dup = -1;
            } else {
                runlength = c & 0x7f;
            }
        } else {
            runlength = runlength | (c << 7);
            if (c_to_dup == 0x55) {
                putPixels(0, runlength);
            } else {
                putPixels(255, runlength);
            }
            c_to_dup = -1;
            runlength = -1;
        }
    }
}

void readFile(fs::FS &fs, const char *path)
{
    static uint8_t rle_buf[RLEBUFSIZE];
    size_t rle_bufhead = 0;
    size_t rle_size = 0;

    size_t filelen = 0;
    size_t filesize;
    static uint8_t compbuf[READBUFSIZE];

    Serial.printf("Reading file: %s\n", path);
    File file = fs.open(path);
    if (!file || file.isDirectory()) {
        Serial.println("Failed to open file for reading");
        return;
    }
    filelen = file.size();
    filesize = filelen;
    Serial.printf("File size: %d\n", filelen);

    u8g2.clearBuffer();
    u8g2.sendBuffer();
    curr_x = 0;
    curr_y = 0;
    runlength = -1;
    c_to_dup = -1;
    lastRefresh = millis();

    // init decoder
    heatshrink_decoder_reset(&hsd);
    size_t   count  = 0;
    uint32_t sunk   = 0;
    size_t toRead;
    size_t toSink = 0;
    uint32_t sinkHead = 0;

    // Go through file...
    while (filelen) {
        if (toSink == 0) {
            toRead = filelen;
            if (toRead > READBUFSIZE) toRead = READBUFSIZE;
            file.read(compbuf, toRead);
            filelen -= toRead;
            toSink = toRead;
            sinkHead = 0;
        }

        // uncompress buffer
        heatshrink_decoder_sink(&hsd, &compbuf[sinkHead], toSink, &count);
        toSink -= count;
        sinkHead = count;
        sunk += count;
        if (sunk == filesize) {
            heatshrink_decoder_finish(&hsd);
        }

        HSD_poll_res pres;
        do {
            rle_size = 0;
            pres = heatshrink_decoder_poll(&hsd, rle_buf, RLEBUFSIZE, &rle_size);
            if (pres < 0) {
                Serial.print("POLL ERR! ");
                Serial.println(pres);
                return;
            }

            rle_bufhead = 0;
            while (rle_size) {
                rle_size--;
                if (rle_bufhead >= RLEBUFSIZE) {
                    Serial.println("RLE_SIZE ERR!");
                    return;
                }
                decodeRLE(rle_buf[rle_bufhead++]);
            }
        } while (pres == HSDR_POLL_MORE);
    }
    file.close();
    Serial.println("Done.");
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

    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_luRS08_tf);
    u8g2.setCursor(10, 16);
    u8g2.setDrawColor(1);

    if (!SPIFFS.begin()) {
        u8g2.println("SPIFFS mount failed");
        Serial.println("SPIFFS mount failed");
        u8g2.sendBuffer();
        while (1);
    }
}

void loop(void)
{
    readFile(SPIFFS, "/video.hs");
}



















