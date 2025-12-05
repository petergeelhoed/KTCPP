
#include "filedesc.hpp"

#include <cstdio>
#include <fcntl.h>

#include <iostream>

int main()
{
    {
        auto fd = filedesc::FileDesc::open("myfile", O_WRONLY | O_CREAT);

        const std::vector<std::byte> byteVector = {std::byte{'h'},
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

        auto fd3 = std::move(fd);
        if (!fd3.write(byteVector))
        {
            perror("writefailure3");
            exit(-1);
        }

        // use moved from fd, it is illegal and flagged by clang-tidy
        auto fd4(std::move(fd));
        if (!fd4.write(byteVector))
        {
            // Will error out with Bad file descriptor
            // as it is set to -1  by the move constructor
            perror("This is expected: writefailure4");
            // No exit because this is expected
            // exit(-1);
        }
    }

    // this is impossible You need to use the open
    // FileDesc fd5(4);

    auto fd2 = filedesc::FileDesc::open("myfile", O_RDONLY);
    auto data = fd2.read();
    for (const auto& iter : data)
    {
        std::cout << static_cast<char>(iter);
    }
}
