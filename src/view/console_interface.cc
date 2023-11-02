#include "console_interface.h"

#include <algorithm>
#include <sstream>

ConsoleInterface::ConsoleInterface() {
  header_style_.SetUnderscore();
  std::cout << std::boolalpha;
}

auto ConsoleInterface::MainMenu() -> void {
  using std::cout;
  using std::endl;

  cout << "\t\t" << header_style_ << "TRANSACTIONS\n" << ClearStyle << endl;
  cout << "Chose chose type of storage:" << endl;
  cout << "1. Hast Table\n2. Self Balancing Binary Search Tree" << endl;
  cout << "q for exit" << endl;
  //  system("stty raw");
  while (true) {
    int in{0};
    in = getchar();
    if (in == '1') {
      storage = std::make_unique<s21::HashTable>();
      cout << "HashTable" << endl;
      break;
    } else if (in == '2') {
      storage = std::make_unique<s21::SelfBalancingBinarySearchTree>();
      cout << "Tree" << endl;
      break;
    } else if (in == 'q') {
      storage = nullptr;
      break;
    }
    std::cout << std::endl;
  }
  //  system("stty cooked");
  if (storage != nullptr) {
    CommandHandler();
  }
}

auto ConsoleInterface::CommandHandler() -> void {
  using std::cin;
  using std::cout;
  using std::endl;
  std::string command, str;
  while (true) try {
      cin >> command;
      std::transform(command.begin(), command.end(), command.begin(), tolower);
      if (command == "exit" or command == "q") break;
      getline(cin, str);
      auto args = SplitArgs(str);
      if (command == "set")
        Set(args);
      else if (command == "get")
        Get(args);
      else if (command == "exists")
        Exist(args);
      else if (command == "del")
        Del(args);
      else if (command == "update")
        Update(args);
      else if (command == "keys")
        Keys(args);
      else if (command == "rename")
        Rename(args);
      else if (command == "ttl")
        TTL(args);
      else if (command == "find")
        Find(args);
      else if (command == "showall")
        ShowAll(args);
      else if (command == "upload")
        Upload(args);
      else if (command == "export")
        Export(args);
      else
        std::cerr << "unknown command" << endl;
      command.clear();
      str.clear();

      //      for (const auto& i : args) cout << i << endl;
    } catch (std::exception& e) {
      std::cerr << "> " << e.what() << endl;
    }
}

auto ConsoleInterface::SplitArgs(const std::string& str)
    -> std::vector<std::string> {
  std::vector<std::string> args;
  std::stringstream stream(str);
  while (!stream.eof()) {
    std::string tmp;
    stream >> tmp;
    if (!tmp.empty()) args.push_back(tmp);
  }

  return args;
}

auto ConsoleInterface::CheckSetUpd(const std::vector<std::string>& args)
    -> void {
  if (args.size() != 6 and args.size() != 8)
    throw std::invalid_argument("ERROR: too few or too much arguments");

  if (!std::all_of(args[3].begin(), args[3].end(), isdigit))
    throw std::invalid_argument(
        {"ERROR: unable to cast value \"" + args[3] + "\" to type int"});

  if (!std::all_of(args[5].begin(), args[5].end(), isdigit))
    throw std::invalid_argument(
        {"ERROR: unable to cast value \"" + args[5] + "\" to type int"});

  if (args.size() == 8) {
    if (args[6] != "ex" and args[6] != "EX")
      throw std::invalid_argument(
          {"ERROR: wrong arg \"" + args[6] + R"(" it can only be "EX/ex")"});
    if (!std::all_of(args[7].begin(), args[7].end(), isdigit))
      throw std::invalid_argument(
          {"ERROR: unable to cast value \"" + args[7] + "\" to type int"});
  }
}

auto ConsoleInterface::CheckFind(const std::vector<std::string>& args) -> void {
  if (args.size() > 5) throw std::invalid_argument("ERROR: too much arguments");

  if (!std::all_of(args[2].begin(), args[2].end(), isdigit))
    throw std::invalid_argument(
        {"ERROR: unable to cast value \"" + args[2] + "\" to type int"});

  if (!std::all_of(args[4].begin(), args[4].end(), isdigit))
    throw std::invalid_argument(
        {"ERROR: unable to cast value \"" + args[4] + "\" to type int"});

  if (args.size() == 8) {
    if (args[6] != "ex")
      throw std::invalid_argument(
          {"ERROR: wrong arg \"" + args[6] + R"(" it can only be "ex")"});
    if (!std::all_of(args[7].begin(), args[7].end(), isdigit))
      throw std::invalid_argument(
          {"ERROR: unable to cast value \"" + args[7] + "\" to type int"});
  }
}

auto ConsoleInterface::CleanSkippedArgs(std::vector<std::string>& args)
    -> void {
  if (args[2] == "-" or args[2].empty()) args[2] = "0";
  if (args[4] == "-" or args[4].empty()) args[4] = "0";
  if (args.size() == 8 and args[7] == "-") args[7] = "0";
  std::for_each(args.begin(), args.end(), [](std::string& s) {
    if (s == "-") s.clear();
  });
}

auto ConsoleInterface::Set(const std::vector<std::string>& args) -> void {
  CheckSetUpd(args);
  if (args.size() == 6)
    storage->Set(args[0], args[1], args[2], std::stoi(args[3]), args[4],
                 std::stoi(args[5]));
  else
    storage->Set(args[0], args[1], args[2], std::stoi(args[3]), args[4],
                 std::stoi(args[5]), std::stoi(args[7]));
  std::cout << "> " << green << "OK" << ClearStyle << std::endl;
}

auto ConsoleInterface::Get(const std::vector<std::string>& args) -> void {
  if (args.size() != 1)
    throw std::invalid_argument("ERROR: only 1 argument are accepted");
  auto peer = storage->Get(args[0]);
  if (peer)
    peer->print();
  else
    std::cout << "> " << red << "(null)" << std::endl << ClearStyle;
}

auto ConsoleInterface::Exist(const std::vector<std::string>& args) -> void {
  if (args.size() != 1)
    throw std::invalid_argument("ERROR: only 1 argument are accepted");
  if (storage->Exists(args[0]))
    std::cout << "> " << green << true << ClearStyle << std::endl;
  else
    std::cout << "> " << red << false << ClearStyle << std::endl;
}

auto ConsoleInterface::Del(const std::vector<std::string>& args) -> void {
  if (args.size() != 1)
    throw std::invalid_argument("ERROR: only 1 argument are accepted");
  if (storage->Del(args[0]))
    std::cout << "> " << green << true << ClearStyle << std::endl;
  else
    std::cout << "> " << red << false << ClearStyle << std::endl;
}

auto ConsoleInterface::Update(std::vector<std::string>& args) -> void {
  if (args.size() != 6)
    throw std::invalid_argument("ERROR: only 6 arguments are accepted");
  CheckSetUpd(args);
  CleanSkippedArgs(args);
  storage->Update(args[0], args[1], args[2], std::stoi(args[3]), args[4],
                  std::stoi(args[5]));
  std::cout << "> " << green << "OK" << ClearStyle << std::endl;
}

auto ConsoleInterface::Keys(const std::vector<std::string>& args) -> void {
  if (!args.empty()) throw std::invalid_argument("ERROR: too much arguments");
  unsigned count = 1;
  for (const auto& key : storage->Keys())
    std::cout << count++ << ") " << key << std::endl;
  std::cout << std::endl;
}

auto ConsoleInterface::Rename(const std::vector<std::string>& args) -> void {
  if (args.size() != 2)
    throw std::invalid_argument("ERROR: only 2 arguments are accepted");
  storage->Rename(args[0], args[1]);
  if (storage->Exists(args[1]))
    std::cout << "> " << green << "OK" << ClearStyle << std::endl;
  else
    std::cout << "> " << red << "(null)" << ClearStyle << std::endl;
}

auto ConsoleInterface::TTL(const std::vector<std::string>& args) -> void {
  if (args.size() != 1)
    throw std::invalid_argument("ERROR: only 1 argument are accepted");
  if (storage->Exists(args[0])) {
    int ttl = storage->TTL(args[0]);
    if (!ttl)
      std::cout << "> " << green << "inf" << ClearStyle << std::endl;
    else
      std::cout << "> " << red << ttl << ClearStyle << std::endl;
  } else
    std::cout << "> " << red << "(null)" << ClearStyle << std::endl;
}

auto ConsoleInterface::Find(std::vector<std::string>& args) -> void {
  if (args.size() < 5) args.resize(5, "");
  CleanSkippedArgs(args);
  CheckFind(args);
  if (args.size() > 6)
    throw std::invalid_argument("ERROR: 6 max arguments are accepted");
  auto keys = storage->Find(args[0], args[1], std::stoi(args[2]), args[3],
                            std::stoi(args[4]));
  unsigned count = 1;
  for (const auto& key : keys)
    std::cout << count++ << " ) " << key << std::endl;
}

auto ConsoleInterface::ShowAll(const std::vector<std::string>& args) -> void {
  if (!args.empty()) throw std::invalid_argument("ERROR: too much arguments");
  unsigned count = 1;
  std::cout << "> №  | Last Name | First Name | Year | City | Number of coins |"
            << std::endl;
  for (const auto& p : storage->ShowAll())
    std::cout << "> " << count++ << "\t\"" << p->last_name << "\"\t\""
              << p->first_name << "\"\t" << p->year_of_birth << "\t\""
              << p->city << "\"\t" << p->number_of_current_coins << std::endl;
}

auto ConsoleInterface::Upload(const std::vector<std::string>& args) -> void {
  if (args.size() != 1)
    throw std::invalid_argument("ERROR: only 1 argument are accepted");
  std::cout << "> " << storage->Upload(args[0]) << std::endl;
}

auto ConsoleInterface::Export(const std::vector<std::string>& args) -> void {
  if (args.size() != 1)
    throw std::invalid_argument("ERROR: only 1 argument are accepted");
  std::cout << "> " << storage->ExportData(args[0]);
  std::cout << std::endl;
}
