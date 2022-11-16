/*
 * @Author: SuBonan
 * @Date: 2022-11-16 20:22:46
 * @LastEditTime: 2022-11-16 20:30:56
 * @FilePath: \CS144-Computer-Networking\lab3\sponge\libsponge\byte_stream.cc
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) { 
    capa = capacity;
    readCnt = writeCnt = 0;
    inputEnded = false;
    q.clear();
}

size_t ByteStream::write(const string &data) {
    size_t lft = min(capa - q.size(), data.length());
    for (size_t i = 0;i < lft;i++)  q.push_back(data[i]);
    writeCnt += lft;
    return lft;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t l = min(len, q.size());
    return string(q.begin(), q.begin() + l);
}  

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t l = min(len, q.size());
    for (size_t i = 0;i < l;i++)    q.pop_front();
    readCnt += l;
    return;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    size_t l = min(len, q.size());
    string res = peek_output(l);
    pop_output(l);
    readCnt += l;
    return res;
}

void ByteStream::end_input() { inputEnded = true; }

bool ByteStream::input_ended() const { return inputEnded; }

size_t ByteStream::buffer_size() const { return q.size(); }

bool ByteStream::buffer_empty() const { return q.empty(); }

bool ByteStream::eof() const { return input_ended() && buffer_empty(); }

size_t ByteStream::bytes_written() const { return writeCnt; }

size_t ByteStream::bytes_read() const { return readCnt; }

size_t ByteStream::remaining_capacity() const { return capa - q.size(); }