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
    EState _state;
    int _sum;
    uint8_t _buf[32];
    size_t _idx;
    int _len;
    int _chk;

  public:
     HPMA115S(void);
    void reset(void);
    bool process(uint8_t c);

};

