//
// Created by frede on 19/11/2024.
//

#include "Buffer.h"

Buffer::Buffer(int l, int c) : lines(l > 0 ? l : 1), columns(c > 0 ? c : 1), cursor_columns(0), cursor_lines(0) {
    buf = make_unique<unique_ptr<char[]>[]>(lines);
    for (int i = 0; i < lines; i++) {
        buf[i] = make_unique<char[]>(columns);
    }
    clear();
}

void Buffer::clear() {
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            buf[i][j] = ' ';
        }
    }
}

void Buffer::view() const
{
    for (int i = 0; i < lines; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            cout << buf[i][j];

        }
        cout << endl;
    }
}

void Buffer::moveCursor(int l, int c)
{
    if (l < 0 || l >= lines || c < 0 || c >= columns) {
        throw std::out_of_range("Cursor position out of bounds");
    }
    cursor_lines = l;
    cursor_columns = c;

}

Buffer& Buffer::operator<<(char c) {
    escrever(c);
    return *this;
}

Buffer& Buffer::operator<<(const std::string& s) {
    for (char c : s) {
        escrever(c);
    }
    return *this;
}

Buffer& Buffer::operator<<(int n) {
    *this << std::to_string(n);
    return *this;
}

// Alteração no método escrever, para resetar o cursor no caso de overflow
void Buffer::escrever(char c) {
    if (cursor_lines < lines && cursor_columns < columns) {
        buf[cursor_lines][cursor_columns] = c;
        cursor_columns++;
        if (cursor_columns >= columns) {
            cursor_columns = 0;
            cursor_lines++;
            if (cursor_lines >= lines) {
                cursor_lines = 0;
            }
        }
    }

    if (cursor_lines >= lines)
    {
        cursor_lines = 0;
    }
    if (cursor_columns >= columns)
    {
        cursor_columns = 0;
    }
}

void Buffer::render() const {
    for (int i = 0; i < lines; ++i) {
        for (int j = 0; j < columns; ++j) {
            std::cout << buf[i][j];
        }
        std::cout << '\n';
    }
}


void Buffer::saveBuffer(const std::string& bufferName) {

    auto newBuffer = std::make_unique<std::unique_ptr<char[]>[]>(lines);
    for (int i = 0; i < lines; ++i) {
        newBuffer[i] = std::make_unique<char[]>(columns);
        for (int j = 0; j < columns; ++j) {
            newBuffer[i][j] = buf[i][j];
        }
    }

    savedBuffers[bufferName] = std::move(newBuffer);
}

void Buffer::loadBuffer(const std::string& bufferName) {
    auto it = savedBuffers.find(bufferName);
    if (it != savedBuffers.end()) {

        const auto& savedBuffer = it->second;

        for (int i = 0; i < lines; ++i) {
            for (int j = 0; j < columns; ++j) {
                std::cout << savedBuffer[i][j];
            }
            std::cout << '\n';
        }
    } else {
        std::cout << "Buffer not found: " << bufferName << std::endl;
    }
}


void Buffer::listBuffer() const {
    std::cout << "Saved buffers:\n";
    for (const auto& pair : savedBuffers) {
        std::cout << pair.first << std::endl;
    }
}

void Buffer::deleteBuffer(const std::string& bufferName) {
    if (savedBuffers.contains(bufferName)) {
        savedBuffers.erase(bufferName);
        std::cout << "Buffer deleted: " << bufferName << std::endl;
    } else {
        std::cout << "Buffer not found: " << bufferName << std::endl;
    }
}