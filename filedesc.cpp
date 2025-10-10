#include "filedesc.hpp"

#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

namespace filedesc
{

FileDesc::FileDesc(int id) : m_id(id)
{
    if (id == -1)
    {
        throw std::runtime_error("Cannot create FileDesc for -1");
    }
}

FileDesc::~FileDesc()
{
    if (m_id != -1)
    {
        close(m_id);
    }
}

std::vector<std::byte> FileDesc::read() const
{
    std::vector<std::byte> data;
    if (m_id != -1)
    {
        const size_t BUFFER_SIZE = 256;
        std::array<std::byte, BUFFER_SIZE> buffer = {};
        long int readBytes = 0;
        while ((readBytes = ::read(m_id, buffer.data(), BUFFER_SIZE)) > 0)
        {
            data.insert(data.end(), buffer.begin(), buffer.begin() + readBytes);
        }

        if (readBytes == -1)
        {
            throw std::runtime_error("Failed to read from file descriptor");
        }
    }
    return data;
}

bool FileDesc::write(const std::vector<std::byte>& data) const
{
    auto writtenBytes = ::write(m_id, data.data(), data.size());
    return (data.size() == static_cast<size_t>(writtenBytes));
}

// outside of class factory

FileDesc open(const std::string& file, int mode)
{
    // think about enum for mode
    const int id = ::open(file.c_str(), mode, 0644);
    if (id == -1)
    {
        throw std::runtime_error("Failed to open file");
    }
    return {id};
}
} // namespace filedesc
