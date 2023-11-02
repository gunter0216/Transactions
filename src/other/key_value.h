#ifndef A6_KEY_VALUE_H
#define A6_KEY_VALUE_H

#include <iostream>
#include <string>
#include <vector>

struct Peer {
  std::string key{};
  std::string last_name{};
  std::string first_name{};
  int year_of_birth{0};
  std::string city{};
  int number_of_current_coins{0};

  auto print() -> void {
    std::cout << key << "\t" << last_name << "\t" << first_name << "\t"
              << year_of_birth << "\t" << city << "\t"
              << number_of_current_coins << std::endl;
  }

  auto operator==(const Peer &peer) const -> bool { return key == peer.key; }

  auto operator<(const Peer &peer) const -> bool { return key < peer.key; }

  auto operator>(const Peer &peer) const -> bool { return key > peer.key; }
};

class KeyValue {
 public:
  virtual ~KeyValue() = default;

  /// @brief Команда используется для установки ключа и его значения.
  /// @param key
  /// @param last_name
  /// @param first_name
  /// @param year_of_birth
  /// @param city
  /// @param number_of_current_coins
  /// @param time_of_life
  auto Set(const std::string &key, const std::string &last_name,
           const std::string &first_name, int year_of_birth,
           const std::string &city, int coins, int ttl = 0) -> void {
    Set({key, last_name, first_name, year_of_birth, city, coins}, ttl);
  }

  virtual auto Set(const Peer &peer, int time_of_life = 0) -> void = 0;

  /// @brief Команда используется для получения значения, связанного с ключом.
  /// @param key
  /// @return Если такой записи нет, то будет возвращён (null):
  virtual auto Get(const std::string &key) -> Peer * = 0;

  /// @brief Эта команда проверяет, существует ли запись с данным ключом.
  /// @param key
  /// @return Возвращает true если объект существует или false если нет:
  virtual auto Exists(const std::string &key) -> bool = 0;

  /// @brief Команда удаляет ключ и соответствующее значение.
  /// @param key
  /// @return Возвращает true, если запись успешно удалена, в противном случае -
  /// false
  virtual auto Del(const std::string &key) -> bool = 0;

  /// @brief Команда обновляет значение по соответствующему ключу, если такой
  /// ключ существует
  /// @param key
  /// @param last_name
  /// @param first_name
  /// @param year_of_birth
  /// @param city
  /// @param number_of_current_coins
  virtual auto Update(const std::string &key, const std::string &last_name,
                      const std::string &first_name, int year_of_birth,
                      const std::string &city, int number_of_current_coins)
      -> void = 0;

  /// @brief Возвращает все ключи, которые есть в хранилище:
  /// @return
  virtual auto Keys() -> std::vector<std::string> = 0;

  /// @brief Команда используется для переименования ключей
  /// @param key_old
  /// @param key_new
  /// @return
  virtual auto Rename(const std::string &key_old, const std::string &key_new)
      -> void = 0;

  /// @brief Когда ключ установлен с истечением срока действия, эту команду
  /// можно использовать для просмотра оставшегося времени.
  /// @param key
  /// @return Если записи с заданным ключом не существует, то возвращается 0
  virtual auto TTL(const std::string &key) -> int = 0;

  /// @brief Эта команда используется для восстановления ключа (или ключей) по
  /// заданному значению.
  /// @param last_name
  /// @param first_name
  /// @param year_of_birth
  /// @param city
  /// @param number_of_current_coins
  /// @return
  virtual auto Find(const std::string &last_name, const std::string &first_name,
                    int year_of_birth, const std::string &city,
                    int number_of_current_coins)
      -> std::vector<std::string> = 0;

  /// @brief Команда для получения всех записей, которые содержатся в key-value
  /// хранилище на текущий момент
  /// @return
  virtual auto ShowAll() -> std::vector<Peer *> = 0;

  /// @brief Данная команда используется для загрузки данных из файла.
  /// @param data_directory
  /// @return Выводится число загруженных строк из файла.
  virtual auto Upload(const std::string &data_directory) -> int = 0;

  /// @brief Данная команда используется для выгрузки данных, которые находятся
  /// в текущий момент в key-value хранилище в файл.
  /// @param data_directory
  /// @return Число выгруженных строк из файла.
  virtual auto ExportData(const std::string &data_directory) -> int = 0;
};

#endif  // A6_KEY_VALUE_H
