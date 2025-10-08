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

FileDesc::~FileDesc() { close_fd(); }

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
        int file_id = dup(rhs.m_id);
        if (file_id == -1)
        {
            throw std::runtime_error("dup failed in copy assignment");
        }

        // only close and assign if dup succeeded
        close_fd();
        m_id = file_id;
    }
    return *this;
}

FileDesc::FileDesc(FileDesc&& other) noexcept : m_id(other.m_id)
{
    // make rhs invalid
    // no need to close as that will be done in the destructor of the new
    // object
    other.m_id = -1;
}

FileDesc& FileDesc::operator=(FileDesc&& rhs) noexcept
{
    if (this != &rhs)
    {
        close_fd();

        m_id = rhs.m_id;
        // make rhs invalid
        // no need to close as that will be done in the destriuctor of the new
        // object
        rhs.m_id = -1;
    }
    return *this;
}

void FileDesc::close_fd() noexcept
{
    if (m_id != -1)
    {
        if (close(m_id) != 0)
        {
            perror("Error closing file");
        }
        m_id = -1;
    }
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
    if (m_id == -1)
    {
        errno = EINVAL;
        return false;
    }

    auto iter = data.begin();
    auto end = data.end();

    while (iter != end)
    {
        ssize_t written = ::write(m_id, &(*iter), std::distance(iter, end));
        if (written == -1)
        {
            return false;
        }
        std::advance(iter, written);
    }

    return true;
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
