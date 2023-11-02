#ifndef A6_CONSOLE_INTERFACE_H
#define A6_CONSOLE_INTERFACE_H

#include <memory>

#include "../hashtable/hash_table.h"
#include "../other/key_value.h"
#include "../tree/self_balancing_binary_search_tree.h"
#include "console_style.h"

class ConsoleInterface {
 public:
  ConsoleInterface();
  ~ConsoleInterface() = default;

  auto Run() -> void { MainMenu(); }

 private:
  auto MainMenu() -> void;
  auto CommandHandler() -> void;
  static auto SplitArgs(const std::string &str) -> std::vector<std::string>;
  static auto CheckSetUpd(const std::vector<std::string> &args) -> void;
  static auto CheckFind(const std::vector<std::string> &args) -> void;
  static auto CleanSkippedArgs(std::vector<std::string> &args) -> void;

  auto Set(const std::vector<std::string> &args) -> void;
  auto Get(const std::vector<std::string> &args) -> void;
  auto Exist(const std::vector<std::string> &args) -> void;
  auto Del(const std::vector<std::string> &args) -> void;
  auto Update(std::vector<std::string> &args) -> void;
  auto Keys(const std::vector<std::string> &args) -> void;
  auto Rename(const std::vector<std::string> &args) -> void;
  auto TTL(const std::vector<std::string> &args) -> void;
  auto Find(std::vector<std::string> &args) -> void;
  auto ShowAll(const std::vector<std::string> &args) -> void;
  auto Upload(const std::vector<std::string> &args) -> void;
  auto Export(const std::vector<std::string> &args) -> void;

  ConsoleStyle header_style_{3};
  ConsoleStyle green{2};
  ConsoleStyle red{1};
  std::unique_ptr<KeyValue> storage = nullptr;
};

#endif  // A6_CONSOLE_INTERFACE_H
