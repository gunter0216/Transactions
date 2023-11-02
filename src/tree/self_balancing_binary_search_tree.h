#ifndef A6_SELF_BALANCING_BINARY_SEARCH_TREE_H
#define A6_SELF_BALANCING_BINARY_SEARCH_TREE_H

#include <math.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>

#include "../other/key_value.h"

namespace s21 {
class SelfBalancingBinarySearchTree : public KeyValue {
 public:
  SelfBalancingBinarySearchTree() = default;
  ~SelfBalancingBinarySearchTree() override { clear(); }

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

 private:
  class Node {
   public:
    Peer kV_;
    Node *p_parent_;
    Node *p_right_;
    Node *p_left_;
    int balance_;
    int right_deph_;
    int left_deph_;

    explicit Node(const Peer &kV, Node *pParent = nullptr,
                  Node *pRight = nullptr, Node *pLeft = nullptr)
        : kV_(kV),
          p_parent_(pParent),
          p_right_(pRight),
          p_left_(pLeft),
          balance_(0),
          right_deph_(0),
          left_deph_(0) {}
  };

  class SetIterator {
   public:
    Node *_parent;
    Node *_current;

    SetIterator() : _parent(nullptr), _current(nullptr) {}

    SetIterator(const SetIterator &other)
        : _parent(other._parent), _current(other._current) {}

    explicit SetIterator(const SelfBalancingBinarySearchTree &map)
        : _parent(nullptr), _current(map.head_node_) {}

    SetIterator &operator++();
    SetIterator &operator--();
    Peer operator*();
    bool operator!=(const SetIterator &other);
    void ChangeIterPos(Node *other);
  };

  using iterator = SetIterator;

  iterator begin();
  iterator end();
  size_t MaxSize();
  void clear();
  void Erase(Node *node);

  auto UpdateTimer() -> void;

  enum class Left_Right { left, right };
  Node *FindNode(const std::string &key);
  void ClearDeep(Node *&node);
  void Balancing(Node *node);
  void ChangeBalanceToCurrentNode(Node *curNode);
  int MaxDepth(Node *node);
  void AddNode(Node *node, Peer a, iterator &it);
  void LeftTurn(Node *node);
  void RightTurn(Node *node);
  void BigLeftTurn(Node *node);
  void BigRightTurn(Node *node);
  void EraseHead(Node *node);
  void EraseLeftOrRightParents(Node *node);
  Node *Cycle(Node *node_cycle, Left_Right pos);
  void FindLastNode(Node *node);

  Node *head_node_{nullptr};
  size_t size_{0};

  std::list<std::pair<Node *, time_t>> timer_;
  std::chrono::system_clock::time_point old_time_;
};
}  //  namespace s21
#endif  // A6_SELF_BALANCING_BINARY_SEARCH_TREE_H
