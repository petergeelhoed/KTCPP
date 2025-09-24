#pragma once

#include <string>
#include <vector>

namespace filedesc
{
class FileDesc
{
  public:
    explicit FileDesc();
    ~FileDesc();

    FileDesc(const FileDesc& other) = delete;
    FileDesc& operator=(const FileDesc& rhs) = delete;

    [[nodiscard]] bool write(const std::vector<std::byte>& data) const;
    [[nodiscard]] std::vector<std::byte> read() const;
    [[nodiscard]] bool open(const std::string& file, int mode);

  private:
    int m_id = -1;
};
} // namespace filedesc
