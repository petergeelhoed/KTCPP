#pragma once

#include <string>
#include <vector>

namespace filedesc
{
class FileDesc
{
  public:
    ~FileDesc();

    FileDesc(const FileDesc& other) = delete;
    FileDesc& operator=(const FileDesc& rhs) = delete;

    [[nodiscard]] bool write(const std::vector<std::byte>& data) const;
    [[nodiscard]] std::vector<std::byte> read() const;

    [[nodiscard]] static FileDesc open(const std::string& file, int mode);

  private:
    explicit FileDesc(int file_id);
    int m_id = -1;
};
} // namespace filedesc
