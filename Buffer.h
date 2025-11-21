// File: Buffer.h

#ifndef BUFFER_H
#define BUFFER_H

#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
class Buffer {
private:
    int lines;
    int columns;
    int cursor_columns, cursor_lines;
    unique_ptr<unique_ptr<char[]>[]> buf;
    std::map<std::string, std::unique_ptr<std::unique_ptr<char[]>[]>> savedBuffers;

public:
    Buffer(int l, int c);
    ~Buffer() = default;
    void clear();
    void moveCursor(int l, int c);
    void escrever(char c);
    void render() const;
    Buffer& operator<<(char c);
    Buffer& operator<<(const std::string& s);
    Buffer& operator<<(int n);
    void view() const;
    void deleteBuffer(const string& string);
    void loadBuffer(const string& string);
    void listBuffer() const;
    void saveBuffer(const string& string);
};

#endif // BUFFER_H
