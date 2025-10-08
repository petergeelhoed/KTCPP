#pragma once

#include <string>
#include <vector>

namespace filedesc
{
/**
  This class manages and takes owneship of the filedescriptor
  It can only be constructed by the open function
  */
class FileDesc final
{
  public:
    ~FileDesc();
    FileDesc(const FileDesc& other);
    FileDesc& operator=(const FileDesc& rhs);
    FileDesc(FileDesc&& other) noexcept;
    FileDesc& operator=(FileDesc&& rhs) noexcept;

    [[nodiscard]] bool write(const std::vector<std::byte>& data) const;
    [[nodiscard]] std::vector<std::byte> read() const;

    // this is the only way to construct a FileDesc
    [[nodiscard]] static FileDesc open(const std::string& file, int mode);
    void close_fd() noexcept;

  private:
    explicit FileDesc(int file_id);
    int m_id = -1;
};
} // namespace filedesc
