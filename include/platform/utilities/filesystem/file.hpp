// ----------------------------------------------------------------------------
// Copyright (C) 2019 Korobov Vladislav
// ----------------------------------------------------------------------------
#pragma once

// std
#include <utility>
// boost
#include <boost/filesystem.hpp>

namespace platform {
namespace filesystem {

template<class FILE_STREAM>
class File {
 public:
    File() = delete;
    File(const File& other) = delete;
    File(File&& other)
      : m_filePath{other.m_filePath}
      , m_fileStream{other.m_fileStream} {
    }
    explicit File(const boost::filesystem::path& file_path)
      : m_filePath{file_path} {
    }
    explicit File(boost::filesystem::path&& file_path)
      : m_filePath{std::move(file_path)} {
    }

    File<FILE_STREAM>& operator=(const File<FILE_STREAM>& rhs) = delete;
    File<FILE_STREAM>& operator=(File<FILE_STREAM>&& rhs) {
      if (this == &rhs)
        return *this;
      m_filePath = std::move(rhs.m_filePath);
      m_fileStream = std::move(rhs.m_fileStream);
    }

    const boost::filesystem::path& filePath() const noexcept {
      return m_filePath;
    }

 private:
    boost::filesystem::path m_filePath;
                FILE_STREAM m_fileStream;
};  // struct File

}  // namespace filesystem
}  // namespace platform
