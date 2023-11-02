#include "hash_table.h"

#include <algorithm>

namespace s21 {

auto HashTable::Rehash() -> void {
  deleted_ = 0;
  size_ = 0;
  std::vector<Node *> newArr(capacity_);
  std::list<std::pair<Node *, time_t>> newList(std::move(timer_));
  std::swap(arr_, newArr);
  for (auto &iter : newList) {
    if (iter.first && iter.first->state) {
      Set(iter.first->peer, iter.second);
    }
  }
  for (size_t i = 0; i < newArr.size(); ++i) {
    if (newArr[i] && newArr[i]->state) Set(newArr[i]->peer);
  }
  ClearArr(newArr);
}

auto HashTable::Set(const Peer &peer, int time_of_life) -> void {
  UpdateTimer();
  if (size_ + 1 > static_cast<int>(kResizeCoef * capacity_)) {
    Resize();
  } else if (deleted_ > size_) {
    Rehash();
  }
  int h1 = Hash1(peer.key);
  int h2 = Hash2(peer.key);
  int i = 0;
  int first_deleted = -1;
  while (arr_[h1] && i < capacity_) {
    if (arr_[h1]->peer.key == peer.key && arr_[h1]->state) {
      return;
    }
    if (!arr_[h1]->state && first_deleted == -1) {
      first_deleted = h1;
    }
    h1 = (h1 + h2) % capacity_;
    ++i;
  }
  if (first_deleted == -1) {
    arr_[h1] = new Node(peer);
    if (time_of_life > 0) timer_.emplace_back(arr_[h1], time_of_life);
  } else {
    arr_[first_deleted]->peer = peer;
    arr_[first_deleted]->state = true;
    --deleted_;
    if (time_of_life > 0)
      timer_.emplace_back(arr_[first_deleted], time_of_life);
  }
  ++size_;
}

auto HashTable::Get(const std::string &key) -> Peer * {
  UpdateTimer();
  Node *node = FindNode(key);
  if (node && node->state) return &node->peer;
  return nullptr;
}

auto HashTable::FindNode(const std::string &key) -> Node * {
  int h1 = Hash1(key);
  int h2 = Hash2(key);
  int i = 0;
  while (arr_[h1] && i < capacity_) {
    if (arr_[h1]->peer.key == key && arr_[h1]->state) {
      return arr_[h1];
    }
    h1 = (h1 + h2) % capacity_;
    ++i;
  }
  return nullptr;
}

auto HashTable::Exists(const std::string &key) -> bool {
  UpdateTimer();
  return FindNode(key);
}

auto HashTable::Del(const std::string &key) -> bool {
  UpdateTimer();
  Node *node = FindNode(key);
  if (node && node->state) {
    node->state = false;
    ++deleted_;
    --size_;
    return true;
  }
  return false;
}

auto HashTable::Update(const std::string &key, const std::string &last_name,
                       const std::string &first_name, int year_of_birth,
                       const std::string &city, int number_of_current_coins)
    -> void {
  UpdateTimer();
  Node *node = FindNode(key);
  if (node && node->state) {
    if (!last_name.empty()) node->peer.last_name = last_name;
    if (!first_name.empty()) node->peer.first_name = first_name;
    if (year_of_birth) node->peer.year_of_birth = year_of_birth;
    if (!city.empty()) node->peer.city = city;
    if (number_of_current_coins)
      node->peer.number_of_current_coins = number_of_current_coins;
  }
}

auto HashTable::Keys() -> std::vector<std::string> {
  UpdateTimer();
  std::vector<std::string> result;
  result.reserve(size_);
  for (int i = 0; i < capacity_; ++i) {
    if (arr_[i] && arr_[i]->state) {
      result.push_back(arr_[i]->peer.key);
    }
  }
  return result;
}

auto HashTable::Rename(const std::string &key_old, const std::string &key_new)
    -> void {
  UpdateTimer();
  Node *node = FindNode(key_old);
  if (node) {
    node->state = false;
    node->peer.key = key_new;
    Set(node->peer);
  }
}

auto HashTable::TTL(const std::string &key) -> int {
  UpdateTimer();
  auto itr = std::find_if(timer_.begin(), timer_.end(),
                          [key](const std::pair<Node *, time_t> &a) {
                            return key == a.first->peer.key;
                          });
  if (itr != timer_.end()) return (int)itr->second;
  return 0;
}

auto HashTable::Find(const std::string &last_name,
                     const std::string &first_name, int year_of_birth,
                     const std::string &city, int number_of_current_coins)
    -> std::vector<std::string> {
  UpdateTimer();
  std::vector<std::string> result;
  for (int i = 0; i < capacity_; ++i) {
    if (arr_[i] && arr_[i]->state &&
        (last_name.empty() || arr_[i]->peer.last_name == last_name) &&
        (first_name.empty() || arr_[i]->peer.first_name == first_name) &&
        (!year_of_birth || arr_[i]->peer.year_of_birth == year_of_birth) &&
        (city.empty() || arr_[i]->peer.city == city) &&
        (number_of_current_coins == -1 ||
         arr_[i]->peer.number_of_current_coins == number_of_current_coins)) {
      result.push_back(arr_[i]->peer.key);
    }
  }
  return result;
}

auto HashTable::ShowAll() -> std::vector<Peer *> {
  UpdateTimer();
  std::vector<Peer *> result;
  result.reserve(size_);
  for (int i = 0; i < capacity_; ++i) {
    if (arr_[i] && arr_[i]->state) {
      result.push_back(&arr_[i]->peer);
    }
  }
  return result;
}

auto HashTable::Upload(const std::string &data_directory) -> int {
  UpdateTimer();
  std::ifstream file(data_directory);
  int lines = 0;
  if (file.is_open()) {
    char c;
    while ((c = file.get()) != EOF) {
      if (c == '\n') ++lines;
    }
    Resize(2 * (capacity_ + lines));
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

auto HashTable::ExportData(const std::string &data_directory) -> int {
  UpdateTimer();
  std::ofstream file(data_directory);
  if (file.is_open()) {
    for (int i = 0; i < capacity_; ++i) {
      if (arr_[i] && arr_[i]->state) {
        file << arr_[i]->peer.key << " ";
        file << arr_[i]->peer.last_name << " ";
        file << arr_[i]->peer.first_name << " ";
        file << arr_[i]->peer.year_of_birth << " ";
        file << arr_[i]->peer.city << " ";
        file << arr_[i]->peer.number_of_current_coins << "\n";
      }
    }
    file.close();
  }
  return size_;
}

auto HashTable::Horner(const std::string &key, int k) -> int {
  int result = 0;
  for (size_t i = 0; i < key.size(); ++i) {
    result = (k * result + key[i]) % capacity_;
  }
  result = (result * 2 + 1) % capacity_;
  return std::abs(result);
}

auto HashTable::Hash1(const std::string &key) -> int {
  return Horner(key, capacity_ - 1);
}

auto HashTable::Hash2(const std::string &key) -> int {
  return Horner(key, capacity_ + 1);
}

auto HashTable::ClearArr(const std::vector<Node *> &arr) -> void {
  for (size_t i = 0; i < arr.size(); ++i) {
    if (arr[i]) delete arr[i];
  }
}

auto HashTable::UpdateTimer() -> void {
  auto start = std::chrono::system_clock::now();
  auto iter = timer_.begin();
  while (iter != timer_.end()) {
    if (!(*iter).first ||
        ((*iter).second -=
         std::chrono::duration_cast<std::chrono::seconds>(start - old_time_)
             .count()) <= 0) {
      (*iter).first->state = false;
      iter = timer_.erase(iter);
    } else {
      ++iter;
    }
  }
  old_time_ = start;
}

auto HashTable::Resize() -> void { Resize(2 * capacity_); }

auto HashTable::Resize(int size) -> void {
  capacity_ = size;
  size_ = 0;
  deleted_ = 0;
  std::vector<Node *> newArr(capacity_);
  std::list<std::pair<Node *, time_t>> newList(std::move(timer_));
  std::swap(arr_, newArr);
  for (auto &iter : newList) {
    if (iter.first && iter.first->state) {
      Set(iter.first->peer, iter.second);
    }
  }
  for (size_t i = 0; i < newArr.size(); ++i) {
    if (newArr[i] && newArr[i]->state) Set(newArr[i]->peer);
  }
  ClearArr(newArr);
}
}  // namespace s21
