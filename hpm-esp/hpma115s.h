#include <stdbool.h>
#include <stdint.h>

// parsing state
typedef enum {
    HEAD_0 = 0,
    HEAD_1,
    LEN_H,
    LEN_L,
    DATA,
    CHECK_H,
    CHECK_L
} EState;

class HPMA115S {

private:
    EState      _state;
    uint8_t     _sum;
    uint8_t     _buf[32];
    uint16_t    _idx;
    uint16_t    _len;
    uint16_t    _chk;

public:
    HPMA115S(void);
    void reset(void);
    bool process(uint8_t c);

};


