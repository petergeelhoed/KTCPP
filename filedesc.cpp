#include "filedesc.hpp"

#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

namespace filedesc
{

FileDesc::FileDesc() {}

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
        std::byte buffer[BUFFER_SIZE];
        ssize_t readBytes;
        readBytes = ::read(m_id, buffer, BUFFER_SIZE);
        while (readBytes > 0)
        {
            data.insert(data.end(), buffer, buffer + readBytes);
            readBytes = ::read(m_id, buffer, BUFFER_SIZE);
        }
    }
    return data;
}

bool FileDesc::write(const std::vector<std::byte>& data) const
{
    if (m_id == -1)
    {
        return false;
    }
    auto writtenBytes = ::write(m_id, data.data(), data.size());
    return (data.size() == writtenBytes);
}

bool FileDesc::open(const std::string& file, int mode)
{
    // think about enum for mode
    m_id = ::open(file.c_str(), mode, 0644);
    return (m_id != -1);
}

} // namespace filedesc

int main()
{
    {
        auto fd = filedesc::FileDesc();
        if (!fd.open("myfile", O_WRONLY | O_CREAT))
        {
            perror("openfailure");
            exit(-1);
        }

        std::vector<std::byte> byteVector = {std::byte{'h'},
                                             std::byte{'e'},
                                             std::byte{'l'},
                                             std::byte{'l'},
                                             std::byte{'o'},
                                             std::byte{' '},
                                             std::byte{'w'},
                                             std::byte{'o'},
                                             std::byte{'r'},
                                             std::byte{'l'},
                                             std::byte{'d'},
                                             std::byte{'\n'}};

        if (!fd.write(byteVector))
        {
            perror("writefailure");
            exit(-1);
        }
    } // this closes the fd;
      // but if I don't I can still read from it

    auto fd2 = filedesc::FileDesc();
    if (!fd2.open("myfile", O_RDONLY))
    {
        perror("openfailure");
        exit(-1);
    }

    auto data = fd2.read();
    for (auto& it : data)
    {
        std::cout << static_cast<char>(it);
    }
}
