#include "filedesc.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <array>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace filedesc
{
FileDesc::FileDesc(int file_id) : m_id(file_id)
{
    if (file_id == -1)
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

FileDesc::FileDesc(const FileDesc& other) : m_id(dup(other.m_id))
{
    if (m_id == -1)
    {
        throw std::runtime_error("dup failed in copy constructor");
    }
}

FileDesc& FileDesc::operator=(const FileDesc& rhs)
{
    if (this != &rhs)
    {
        if (m_id != -1)
        {
            close(m_id);
        }

        m_id = dup(rhs.m_id);
        if (m_id == -1)
        {
            throw std::runtime_error("dup failed in copy assignment");
        }
    }
    return *this;
}

FileDesc::FileDesc(FileDesc&& other) noexcept : m_id(other.m_id)
{
    // make rhs invalid
    other.m_id = -1;
}

FileDesc& FileDesc::operator=(FileDesc&& rhs) noexcept
{
    if (this != &rhs)
    {
        if (m_id != -1)
        {
            close(m_id);
        }

        m_id = rhs.m_id;
        // make rhs invalid
        rhs.m_id = -1;
    }
    return *this;
}

std::vector<std::byte> FileDesc::read() const
{
    std::vector<std::byte> data;
    if (m_id != -1)
    {
        constexpr size_t BUFFER_SIZE = 256;
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
    if (m_id != -1)
    {
        auto writtenBytes = ::write(m_id, data.data(), data.size());
        return (data.size() == static_cast<size_t>(writtenBytes));
    }
    return -1;
}

FileDesc FileDesc::open(const std::string& file, int mode)
{
    // think about enum for mode
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    const int file_id = ::open(file.c_str(), mode, 0644);
    if (file_id == -1)
    {
        throw std::runtime_error("Failed to open file");
    }
    return FileDesc(file_id);
}
} // namespace filedesc
