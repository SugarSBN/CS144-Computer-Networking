/*
 * @Author: SuBonan
 * @Date: 2022-11-16 20:22:46
 * @LastEditTime: 2022-11-17 21:52:21
 * @FilePath: \CS144-Computer-Networking\lab3\sponge\libsponge\tcp_sender.cc
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>
#include <iostream>
// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity) {cout << "============NEW TEST==================" << endl;}

uint64_t TCPSender::bytes_in_flight() const { 
    cout << "bytes in flight: " << _bytes_in_flight << endl; 
    return _bytes_in_flight; 
}

void TCPSender::fill_window() {
    cout << "to fill window!" << endl;
    cout << "window size: " << _window_size << endl;

    if (_send_base + _window_size <= _next_seqno) {
        cout << "window is full!" << endl;
        return;
    }

    Buffer payload = Buffer(_stream.read(_window_size + _send_base - _next_seqno));
    cout << "Buffer:" << payload.copy() << endl;
    TCPHeader header = TCPHeader();


    if (!_syn) {
        header.syn = true;
        _syn = true;
    }
    if (_stream.eof()) header.fin = true;

    header.seqno = wrap(_next_seqno, _isn);
    
    cout << "next seqno: " << _next_seqno << endl;
    cout << "stream written: " << _stream.bytes_written() << endl;

    TCPSegment tcp_segment;
    tcp_segment.header() = header;
    tcp_segment.payload() = payload;
    if (tcp_segment.length_in_sequence_space() == 0)    return;
    cout << "tcp segment length: " << tcp_segment.length_in_sequence_space() << endl;
    _next_seqno += tcp_segment.length_in_sequence_space();
    _bytes_in_flight += tcp_segment.length_in_sequence_space();
    _segments_out.push(tcp_segment);
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) { 
    cout << "ack received!" << endl;    
    uint64_t _rcv_base = unwrap(ackno, _isn, _send_base);
    cout << "rcv base: " << _rcv_base << endl;
    if (_rcv_base > _next_seqno)   return;
    _bytes_in_flight -= _rcv_base - _send_base;
    _window_size = window_size;
    _send_base = _rcv_base;
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) { 
    cout << "tick time: " << ms_since_last_tick << endl;
    if (ms_since_last_tick > _initial_retransmission_timeout) fill_window();
    DUMMY_CODE(ms_since_last_tick); 
}

unsigned int TCPSender::consecutive_retransmissions() const { return {}; }

void TCPSender::send_empty_segment() {}
