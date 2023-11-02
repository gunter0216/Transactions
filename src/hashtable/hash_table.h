#ifndef A6_HASH_TABLE_H
#define A6_HASH_TABLE_H

#include <chrono>
#include <fstream>
#include <iostream>
#include <list>

#include "../other/key_value.h"

namespace s21 {
class HashTable : public KeyValue {
 public:
  HashTable() : arr_(kStartSize) {}
  ~HashTable() override { ClearArr(arr_); }

  /// @brief Команда используется для установки ключа и его значения.
  /// @param key
  /// @param last_name
  /// @param first_name
  /// @param year_of_birth
  /// @param city
  /// @param number_of_current_coins
  /// @param time_of_life
  auto Set(const Peer &peer, int time_of_life = 0) -> void override;

  /// @brief Команда используется для получения значения, связанного с ключом.
  /// @param key
  /// @return Если такой записи нет, то будет возвращён (null):
  auto Get(const std::string &key) -> Peer * override;

  /// @brief Эта команда проверяет, существует ли запись с данным ключом.
  /// @param key
  /// @return Возвращает true если объект существует или false если нет:
  auto Exists(const std::string &key) -> bool override;

  /// @brief Команда удаляет ключ и соответствующее значение.
  /// @param key
  /// @return Возвращает true, если запись успешно удалена, в противном случае -
  /// false
  auto Del(const std::string &key) -> bool override;

  /// @brief Команда обновляет значение по соответствующему ключу, если такой
  /// ключ существует
  /// @param key
  /// @param last_name
  /// @param first_name
  /// @param year_of_birth
  /// @param city
  /// @param number_of_current_coins
  auto Update(const std::string &key, const std::string &last_name = "",
              const std::string &first_name = "", int year_of_birth = 0,
              const std::string &city = "", int number_of_current_coins = 0)
      -> void override;

  /// @brief Возвращает все ключи, которые есть в хранилище:
  /// @return
  auto Keys() -> std::vector<std::string> override;

  /// @brief Команда используется для переименования ключей
  /// @param key_old
  /// @param key_new
  /// @return
  auto Rename(const std::string &key_old, const std::string &key_new)
      -> void override;

  /// @brief Когда ключ установлен с истечением срока действия, эту команду
  /// можно использовать для просмотра оставшегося времени.
  /// @param key
  /// @return Если записи с заданным ключом не существует, то возвращается 0
  auto TTL(const std::string &key) -> int override;

  /// @brief Эта команда используется для восстановления ключа (или ключей) по
  /// заданному значению.
  /// @param last_name
  /// @param first_name
  /// @param year_of_birth
  /// @param city
  /// @param number_of_current_coins
  /// @return
  auto Find(const std::string &last_name = "",
            const std::string &first_name = "", int year_of_birth = 0,
            const std::string &city = "", int number_of_current_coins = -1)
      -> std::vector<std::string> override;

  /// @brief Команда для получения всех записей, которые содержатся в key-value
  /// хранилище на текущий момент
  /// @return
  auto ShowAll() -> std::vector<Peer *> override;

  /// @brief Данная команда используется для загрузки данных из файла.
  /// @param data_directory
  /// @return Выводится число загруженных строк из файла.
  auto Upload(const std::string &data_directory) -> int override;

  /// @brief Данная команда используется для выгрузки данных, которые находятся
  /// в текущий момент в key-value хранилище в файл.
  /// @param data_directory
  /// @return Число выгруженных строк из файла.
  auto ExportData(const std::string &data_directory) -> int override;

 public:
  const int kStartSize = 8;
  const double kResizeCoef = 0.5;

  struct Node {
    Peer peer;
    bool state;

    explicit Node(const Peer &peer) : peer(peer), state(true) {}
  };

  auto Horner(const std::string &key, int k) -> int;
  auto Hash1(const std::string &key) -> int;
  auto Hash2(const std::string &key) -> int;
  auto Resize() -> void;
  auto Resize(int size) -> void;
  auto Rehash() -> void;
  auto ClearArr(const std::vector<Node *> &arr) -> void;
  auto FindNode(const std::string &key) -> Node *;
  auto UpdateTimer() -> void;

  std::vector<Node *> arr_;
  std::list<std::pair<Node *, time_t>> timer_;
  std::chrono::system_clock::time_point old_time_;
  int size_ = 0;
  int deleted_ = 0;
  int capacity_ = kStartSize;
};

}  // namespace s21

#endif  // A6_HASH_TABLE_H
