#include <reg51.h>

sbit rs = P3^7;
sbit rw = P3^6;
sbit en = P3^5;

char count = 0;
char input[13]; // 12 chars + null terminator
char ch;

char valid1[] = "000217755403";
char valid2[] = "000252816903";

bit present1 = 0;
bit present2 = 0;

void delay(unsigned int);
void cmdwrt(unsigned char);
void datawrt(unsigned char);
char rxdata(void);
bit compare(char*, char*);

void main(void) {
    unsigned char cmd[] = {0x38, 0x01, 0x06, 0x0c, 0x80};
    unsigned char msg[] = "Scan your Card";
    char found;
    unsigned char i;

    TMOD = 0x20; // Timer1, Mode 2
    SCON = 0x50; // 8-bit UART
    TH1  = 0xFD; // 9600 baud
    TR1  = 1;

    for (i = 0; i < 5; i++) {
        cmdwrt(cmd[i]);
        delay(1);
    }

    for (i = 0; msg[i] != '\0'; i++) {
        datawrt(msg[i]);
        delay(1);
    }

    while (1) {
        count = 0;
        found = 0;

        cmdwrt(0xC0); // move cursor to second line

        while (count < 12) {
            input[count] = rxdata();
            count++;
        }
        input[12] = '\0'; // null-terminate

        cmdwrt(0x01); // clear LCD
        delay(2);

        if (compare(input, valid1)) {
            if (!present1) {
                present1 = 1;
                found = 1;
                cmdwrt(0x80);
                for (i = 0; i < 16; i++) datawrt("Welcome User 1 "[i]);
            } else {
                found = 1;
                cmdwrt(0x80);
                for (i = 0; i < 16; i++) datawrt("Already Present"[i]);
            }
        }

        if (compare(input, valid2)) {
            if (!present2) {
                present2 = 1;
                found = 1;
                cmdwrt(0x80);
                for (i = 0; i < 16; i++) datawrt("Welcome User 2 "[i]);
            } else {
                found = 1;
                cmdwrt(0x80);
                for (i = 0; i < 16; i++) datawrt("Already Present"[i]);
            }
        }

        if (!found) {
            cmdwrt(0x80);
            for (i = 0; i < 14; i++) datawrt("Access Denied"[i]);
        }

        delay(200);
        cmdwrt(0x01); // clear LCD

        for (i = 0; msg[i] != '\0'; i++) {
            cmdwrt(0x80 + i);
            datawrt(msg[i]);
            delay(1);
        }
    }
}

void cmdwrt(unsigned char x) {
    P1 = x;
    rs = 0;
    rw = 0;
    en = 1;
    delay(1);
    en = 0;
}

void datawrt(unsigned char y) {
    P1 = y;
    rs = 1;
    rw = 0;
    en = 1;
    delay(1);
    en = 0;
}

char rxdata(void) {
    while (RI == 0);
    RI = 0;
    ch = SBUF;
    return ch;
}

void delay(unsigned int z) {
    unsigned int p, q;
    for (p = 0; p < z; p++) {
        for (q = 0; q < 1375; q++);
    }
}

bit compare(char* a, char* b) {
    unsigned char i;
    for (i = 0; i < 12; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}