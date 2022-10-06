/*
 * @Author: SuBonan
 * @Date: 2022-09-13 14:09:36
 * @LastEditTime: 2022-10-06 18:43:45
 * @FilePath: \sponge\libsponge\wrapping_integers.cc
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include "wrapping_integers.hh"
#include <iostream>
#include <cmath>
// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    n = n % (1ll << 32);
    uint64_t rua = (n + isn.raw_value()) % (1ll << 32);
    return WrappingInt32(rua);
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t n64 = n.raw_value();
    uint64_t isn64 = isn.raw_value();
    uint64_t dis = (n64 - isn64 + (1ll << 32)) % (1ll << 32);
    
    
    //uint64_t mindis1 = dis + (1ll << 32) * w;
    //uint64_t mindis2 = dis + (1ll << 32) * w + 1;

    uint64_t ans = 0;
    if (checkpoint <= dis) ans = dis;
    else {
        uint64_t w = (checkpoint - dis) / (1ll << 32);
        uint64_t mindis1 = checkpoint - dis - w * (1ll << 32);
        uint64_t mindis2 = dis + (w + 1) * (1ll << 32) - checkpoint;
        ans = mindis1 < mindis2 ? dis + w * (1ll << 32) : dis + (w + 1) * (1ll << 32);
    }
    
    return ans;
}
