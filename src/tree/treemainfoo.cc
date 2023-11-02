#include <algorithm>

#include "self_balancing_binary_search_tree.h"

namespace s21 {

auto SelfBalancingBinarySearchTree::Set(const Peer &peer, int time_of_life)
    -> void {
  UpdateTimer();
  if (size_ == MaxSize()) throw std::out_of_range("ERROR: Tree if full");
  iterator it;
  if (head_node_ == nullptr) {
    head_node_ = new Node(peer);
    ++size_;
    if (time_of_life > 0)
      timer_.push_back(std::pair<Node *, time_t>{head_node_, time_of_life});
  } else {
    if (!FindNode(peer.key)) {
      AddNode(head_node_, peer, it);
      if (time_of_life > 0)
        timer_.push_back(std::pair<Node *, time_t>{it._current, time_of_life});
    }
  }
}

auto SelfBalancingBinarySearchTree::Get(const std::string &key) -> Peer * {
  UpdateTimer();
  Node *buf = head_node_;
  bool res = false;
  while (buf != nullptr) {
    if (buf->kV_.key == key) {
      res = true;
      break;
    } else if (key > buf->kV_.key) {
      buf = buf->p_right_;
    } else {
      buf = buf->p_left_;
    }
  }
  return res ? &buf->kV_ : nullptr;
}

auto SelfBalancingBinarySearchTree::Exists(const std::string &key) -> bool {
  UpdateTimer();
  return Get(key) != nullptr;
}

auto SelfBalancingBinarySearchTree::Del(const std::string &key) -> bool {
  UpdateTimer();
  Node *pos = FindNode(key);
  if (pos) {
    Erase(pos);
    return true;
  }
  return false;
}

auto SelfBalancingBinarySearchTree::Update(
    const std::string &key, const std::string &last_name,
    const std::string &first_name, int year_of_birth, const std::string &city,
    int number_of_current_coins) -> void {
  UpdateTimer();
  Node *node = FindNode(key);
  if (node) {
    if (!last_name.empty()) node->kV_.last_name = last_name;
    if (!first_name.empty()) node->kV_.first_name = first_name;
    if (year_of_birth) node->kV_.year_of_birth = year_of_birth;
    if (!city.empty()) node->kV_.city = city;
    //    if (number_of_current_coins)
    node->kV_.number_of_current_coins = number_of_current_coins;
  }
}

auto SelfBalancingBinarySearchTree::Keys() -> std::vector<std::string> {
  UpdateTimer();
  std::vector<std::string> result;
  result.reserve(size_);
  iterator iter = begin();
  iterator iend = end();
  while (iter != iend) {
    result.push_back(iter._current->kV_.key);
    ++iter;
  }
  return result;
}

auto SelfBalancingBinarySearchTree::Rename(const std::string &key_old,
                                           const std::string &key_new) -> void {
  UpdateTimer();
  Node *node = FindNode(key_old);
  if (node) {
    node->kV_.key = key_new;
  }
}

auto SelfBalancingBinarySearchTree::TTL(const std::string &key) -> int {
  UpdateTimer();
  auto itr = std::find_if(timer_.begin(), timer_.end(),
                          [key](const std::pair<Node *, time_t> &a) {
                            return key == a.first->kV_.key;
                          });
  if (itr != timer_.end()) return (int)itr->second;
  return 0;
}

auto SelfBalancingBinarySearchTree::Find(const std::string &last_name,
                                         const std::string &first_name,
                                         int year_of_birth,
                                         const std::string &city,
                                         int number_of_current_coins)
    -> std::vector<std::string> {
  UpdateTimer();
  std::vector<std::string> result;
  iterator iter = begin();
  iterator iend = end();
  while (iter != iend) {
    if ((last_name.empty() || iter._current->kV_.last_name == last_name) &&
        (first_name.empty() || iter._current->kV_.first_name == first_name) &&
        (!year_of_birth || iter._current->kV_.year_of_birth == year_of_birth) &&
        (city.empty() || iter._current->kV_.city == city) &&
        (number_of_current_coins == -1 ||
         iter._current->kV_.number_of_current_coins ==
             number_of_current_coins)) {
      result.push_back(iter._current->kV_.key);
    }
    ++iter;
  }
  return result;
}

auto SelfBalancingBinarySearchTree::ShowAll() -> std::vector<Peer *> {
  UpdateTimer();
  std::vector<Peer *> result;
  result.reserve(size_);
  iterator iter = begin();
  iterator iend = end();
  while (iter != iend) {
    result.push_back(&iter._current->kV_);
    ++iter;
  }
  return result;
}

auto SelfBalancingBinarySearchTree::Upload(const std::string &data_directory)
    -> int {
  UpdateTimer();
  std::ifstream file(data_directory);
  int lines = 0;
  if (file.is_open()) {
    char c;
    while ((c = file.get()) != EOF) {
      if (c == '\n') ++lines;
    }
    file.clear();
    file.seekg(0, std::ios_base::beg);
    for (int i = 0; i < lines; ++i) {
      Peer peer;
      file >> peer.key;
      file >> peer.last_name;
      file >> peer.first_name;
      file >> peer.year_of_birth;
      file >> peer.city;
      file >> peer.number_of_current_coins;
      Set(peer);
    }
    file.close();
  }
  return lines;
}

auto SelfBalancingBinarySearchTree::ExportData(
    const std::string &data_directory) -> int {
  UpdateTimer();
  std::ofstream file(data_directory);
  iterator iter = begin();
  iterator iend = end();
  if (file.is_open()) {
    while (iter != iend) {
      file << iter._current->kV_.key << " ";
      file << iter._current->kV_.last_name << " ";
      file << iter._current->kV_.first_name << " ";
      file << iter._current->kV_.year_of_birth << " ";
      file << iter._current->kV_.city << " ";
      file << iter._current->kV_.number_of_current_coins << "\n";
      ++iter;
    }
  }
  return size_;
}

}  // namespace s21
