/*
 * @Author: SuBonan
 * @Date: 2022-11-16 20:22:46
 * @LastEditTime: 2022-11-16 20:24:54
 * @FilePath: \CS144-Computer-Networking\lab3\sponge\libsponge\tcp_receiver.cc
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include "tcp_receiver.hh"
#include <iostream>
// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void print_header(const TCPHeader &header){
    cout << "===================================================" << endl;
    cout << "ACK: " << header.ack << endl;
    cout << "seqno: " << header.seqno << endl;
    cout << "SYN: " << header.syn << endl;
    cout << "FIN: " << header.fin << endl;
    cout << "===================================================" << endl;
}

void TCPReceiver::segment_received(const TCPSegment &seg) {
    TCPHeader header = seg.header();
    // print_header(header);

    if (!syn && header.syn) {
        syn = true;
        isn = header.seqno;
    }
    if (!syn)   return;
    // cout << "payload: " << seg.payload().copy() << endl;
    uint64_t absolute_seqno = unwrap(header.seqno + header.syn, isn, stream_out().bytes_written());
    // cout << "absolute seqno: " << absolute_seqno << endl;
    _reassembler.push_substring(seg.payload().copy(), absolute_seqno - 1, header.fin);
    if (header.fin) fin = true;
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if (!syn)   return {};
    // cout << "rua: " << stream_out().buffer_empty() << " " << fin << " " << stream_out().bytes_written() << " " << isn << endl;
    if (_reassembler.unassembled_bytes() == 0 && fin) return wrap(stream_out().bytes_written() + 2, isn);
    return wrap(stream_out().bytes_written() + 1, isn);
}

size_t TCPReceiver::window_size() const { return stream_out().remaining_capacity(); }
