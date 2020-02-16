#include <stdbool.h>

#include "hpma115s.h"

HPMA115S::HPMA115S(void)
{
    reset();
}

void HPMA115S::reset(void)
{
    _state = HEAD_0;
}

bool HPMA115S::process(uint8_t c)
{
    switch (_state) {
    case HEAD_0:
        _sum = c;
        if (c == 0x42) {
            _state = HEAD_1;
        }
        break;
    case HEAD_1:
        _sum += c;
        if (c == 0x4D) {
            _state = LEN_H;
        } else {
            _state = HEAD_0;
            return process(c);
        }
        break;
    case LEN_H:
        _sum += c;
        _len = c;
        _state = LEN_L;
        break;
    case LEN_L:
        _sum += c;
        _len = (_len << 8) + c;
        if (_len <= sizeof(_buf)) {
            _idx = 0;
            _state = DATA;
        } else {
            reset();
            return process(c);
        }
        break;
    case DATA:
        _sum += c;
        if (_idx < _len) {
            _buf[_idx++] = c;
        } else {
            _state = CHECK_H;
        }
        break;
    case CHECK_H:
        _chk = c;     
        break;
    case CHECK_L:
        _chk = (_chk << 8) + c;
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
    return false;
}


