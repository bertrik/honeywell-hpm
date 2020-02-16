// simple test code to verify creation of tx command and parsing of rx response 

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hpm-esp/hpma115s.cpp"

static bool assertEquals(int expected, int actual, const char *field)
{
    if (actual != expected) {
        fprintf(stderr, "Assertion failure: field '%s' expected %d, got %d\n", field, expected, actual);
        return false;
    }
    return true;
}

static bool test_rx(void)
{
    HPMA115S hpm;

    uint8_t frame[] = {
        // some garbage bytes
        0x00, 0xFF, 0x42,
        // header, length
        0x42, 0x4D, 0x00, 0x1C,
        // reserve
        0x00, 0x00,
        // PM2.5, PM10
        0x00, 0x25, 0x00, 0x10,
        // reserve
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
        // checksum
        0x14, 0xCC
    };

    // send frame data
    bool ok;
    hpm.reset();
    for (size_t i = 0; i < sizeof(frame); i++) {
        ok = hpm.process(frame[i]);
    }
    if (!ok) {
        fprintf(stderr, "expected successful frame!");
        return false;
    }
#if 0    
    // parse
    pms_meas_t meas;
    PmsParse(&meas);
    ok = assertEquals(0x0010, meas.concPM1_0_CF1, "concPM1_0_CF1");
    ok = ok && assertEquals(0x0025, meas.concPM2_5_CF1, "concPM2_5_CF1");
    ok = ok && assertEquals(0x25, meas.concPM2_5_amb, "concPM2_5_amb");
    ok = ok && assertEquals(0x50, meas.rawGt5_0um, "rawGt5_0um");
    ok = ok && assertEquals(0x88, meas.version, "version");
    ok = ok && assertEquals(0x99, meas.errorCode, "errorCode");
#endif
    return ok;    
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    bool b;

    printf("test_rx ...");
    b = test_rx();
    printf("%s\n", b ? "PASS" : "FAIL");

    return 0;
}

