/*
 * @Author: SuBonan
 * @Date: 2022-09-09 10:10:24
 * @LastEditTime: 2022-09-12 11:08:47
 * @FilePath: \lab1\sponge\libsponge\stream_reassembler.cc
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include "stream_reassembler.hh"
#include <iostream>
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {
    startAt = 0;
    st = 0;
    storage = vector<char>();
    assembled = vector<bool>();
    for (size_t i = 0;i < capacity;i++) storage.push_back('\0');
    for (size_t i = 0;i < capacity;i++) assembled.push_back(false);
    _eofIndex = _capacity + 1;
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    string data1 = data;
    size_t index1 = index;
    if (index < startAt) {
        if (startAt - index >= data.length()) return;
        index1 = startAt;
        data1 = data.substr(startAt - index);
    } 

    size_t lft = _capacity - _output.buffer_size();
    while (storage.size() - st < lft) {
        storage.push_back('\0');
        assembled.push_back(false);
    }

    size_t l = min(data1.length(), lft - (index1 - startAt));

    for (size_t i = 0;i < l;i++)    {
        storage[i + index1 - startAt + st] = data1[i];
        assembled[i + index1 - startAt + st] = true;
    }
    size_t maxpre = st;
    for (;maxpre < assembled.size() && assembled[maxpre] == true;maxpre++);
    string putin = "";
    for (size_t i = st;i < maxpre;i++)   putin = putin + storage[i];
    startAt = startAt + maxpre - st;
    st = maxpre;
    _output.write(putin);   
    
    if (eof) _eofIndex = data.size() + index;
    if (startAt == _eofIndex) _output.end_input();
}

size_t StreamReassembler::unassembled_bytes() const { return {}; }

bool StreamReassembler::empty() const { return {}; }
