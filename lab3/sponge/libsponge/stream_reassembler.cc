/*
 * @Author: SuBonan
 * @Date: 2022-11-16 20:22:46
 * @LastEditTime: 2022-11-16 20:24:29
 * @FilePath: \CS144-Computer-Networking\lab3\sponge\libsponge\stream_reassembler.cc
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
    _eofIndex = capacity + 1;
    _storage.clear();
    startAt = 0;
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t index1 = index;
    string data1 = data;
    if (index < startAt){
        if (startAt - index >= data.length()) return;
        index1 = startAt;
        data1 = data.substr(startAt - index);
    }

    size_t l = min(_capacity - _output.buffer_size() - (index1 - startAt), data1.length());
    _storage.insert(Fragment(index1, data1.substr(0, l)));
    string putin = "";
    size_t lst = startAt;
    multiset<Fragment> :: iterator it = _storage.begin();
    for(;it != _storage.end();it++){
        size_t st = it -> index;
        string s = it -> data;
        if (st > lst) break;
        if (st + s.length() <= lst) continue;
        putin = putin + s.substr(lst - st);
        lst = lst + s.length() - (lst - st);
    }
    startAt = startAt + putin.length();
    while (!_storage.empty() && _storage.begin() -> index + _storage.begin() -> data.length() <= startAt) _storage.erase(_storage.begin());
    _output.write(putin);
    if (eof) _eofIndex = data.size() + index;
    if (startAt == _eofIndex) _output.end_input();
}

size_t StreamReassembler::unassembled_bytes() const { 
    size_t res = 0;
    size_t lst = startAt;
    for (multiset<Fragment> :: iterator it = _storage.begin();it != _storage.end();it++){
        size_t st = it -> index;
        string s = it -> data;
        if (st + s.length() <= lst) continue;
        if (lst < st) res = res + s.length();
        else res = res + s.length() - (lst - st);
        lst = st + s.length();
    }
    return res;
}

bool StreamReassembler::empty() const { return {}; }

