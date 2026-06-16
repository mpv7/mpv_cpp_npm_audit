#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <filesystem>
#include <fstream>

#include "audit/Utils.h"
#include "doctest.h"

namespace audit {
TEST_CASE("test trim") {
  CHECK(trim("  version is empty    ") == "version is empty");
  CHECK_FALSE(trim(" version is 12.2.1   ") == "version is 12.2.1   ");
}

TEST_CASE("test split") {
  std::vector<std::string> test = split("1,2,3", ',');
  CHECK(test[0] == "1");
  CHECK(test[2] == "3");
  CHECK_FALSE(test.size() > 3);
}

TEST_CASE("test replace") {
  CHECK(replace("hello world", "world", "C++") == "hello C++");
  CHECK(replace("a b a", "a", "x") == "x b x");
  CHECK(replace("abc", "d", "x") == "abc");
  CHECK_FALSE(replace("abc", "b", "x") == "abc");
}

TEST_CASE("test isValidSemanticVersion") {
  CHECK(isValidSemanticVersion("5"));
  CHECK(isValidSemanticVersion("0.0.1"));
  CHECK(isValidSemanticVersion("10.20.30"));
  CHECK_FALSE(isValidSemanticVersion("v1.2"));
  CHECK_FALSE(isValidSemanticVersion("1.2.3-alpha"));
  CHECK_FALSE(isValidSemanticVersion("01.2.3"));
}

TEST_CASE("test fileExists and isDirectory") {
  // Временный файл
  auto tempFile = std::filesystem::temp_directory_path() / "test_file.txt";
  std::ofstream ofs(tempFile);
  ofs << "test";
  ofs.close();

  CHECK(fileExists(tempFile.string()));
  CHECK_FALSE(isDirectory(tempFile.string()));

  // Временная папка
  auto tempDir = std::filesystem::temp_directory_path() / "test_dir";
  std::filesystem::create_directory(tempDir);

  CHECK(fileExists(tempDir.string()));
  CHECK(isDirectory(tempDir.string()));

  // Удаляем
  std::filesystem::remove(tempFile);
  std::filesystem::remove(tempDir);

  CHECK_FALSE(fileExists(tempFile.string()));
  CHECK_FALSE(fileExists(tempDir.string()));
}
}  // namespace audit