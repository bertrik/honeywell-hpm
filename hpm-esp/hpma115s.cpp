#include <stdbool.h>

#include "hpma115s.h"

#ifdef DEBUG
#include <stdio.h>
#define DBG printf
#else
#define DBG(x)
#endif

HPMA115S::HPMA115S(void)
{
    reset();
}

void
 HPMA115S::reset(void)
{
    DBG("reset\n");
    _state = HEAD_0;
}

bool HPMA115S::process(uint8_t c)
{
    DBG("state %d: ", _state);
    switch (_state) {
    case HEAD_0:
        _sum = c;
        if (c == 0x42) {
            DBG("HEAD_1");
            _state = HEAD_1;
        }
        break;
    case HEAD_1:
        _sum += c;
        if (c == 0x4D) {
            DBG("HEAD_2");
            _state = LEN_H;
        } else {
            reset();
            return process(c);
        }
        break;
    case LEN_H:
        _sum += c;
        _len = c;
        _state = LEN_L;
        DBG("len = %d", _len);
        break;
    case LEN_L:
        _sum += c;
        _len = (_len << 8) + c - 2;
        if ((_len < 0) || (_len > sizeof(_buf))) {
            reset();
            return process(c);
        } else {
            DBG("len = %d", _len);
            _idx = 0;
            _state = DATA;
        }
        break;
    case DATA:
        _sum += c;
        if (_idx < _len) {
            DBG("data[%ld] = 0x%02X", _idx, c);
            _buf[_idx++] = c;
            if (_idx == _len) {
                _state = CHECK_H;
            }
        } else {
            reset();
        }
        break;
    case CHECK_H:
        _chk = c;
        _state = CHECK_L;
        break;
    case CHECK_L:
        _chk = (_chk << 8) + c;
        DBG("sum = %d, chk = %d\n", _sum, _chk);
        if (_chk == _sum) {
            reset();
            return true;
        } else {
            reset();
            return process(c);
        }
        break;
    default:
        reset();
        return process(c);
    }
    DBG("\n");
    return false;
}

