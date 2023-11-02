#include "self_balancing_binary_search_tree.h"
namespace s21 {

//  SelfBalancingBinarySearchTree
//  Iterators-------------------------------------------------

typename SelfBalancingBinarySearchTree::iterator
SelfBalancingBinarySearchTree::begin() {
  iterator it(*this);
  if (head_node_ != nullptr) {
    while (it._parent != nullptr) {
      it._current = it._parent;
    }
    while (it._current->p_left_ != nullptr) {
      it._current = it._current->p_left_;
    }
    it._parent = it._current->p_parent_;
  }
  return it;
}

typename SelfBalancingBinarySearchTree::iterator
SelfBalancingBinarySearchTree::end() {
  iterator it(*this);
  if (head_node_ != nullptr) {
    while (it._parent != nullptr) {
      it._current = it._parent;
    }
    while (it._current->p_right_ != nullptr) {
      it._current = it._current->p_right_;
    }
    it._parent = it._current;
    it._current = it._current->p_right_;
  }
  return it;
}

size_t SelfBalancingBinarySearchTree::MaxSize() {
  return std::numeric_limits<size_t>::max() /
         (sizeof(SelfBalancingBinarySearchTree) + sizeof(Node) +
          sizeof(size_t) * 2);
}

//  SelfBalancingBinarySearchTree
//  Modifiers--------------------------------------------

void SelfBalancingBinarySearchTree::clear() {
  if (head_node_ != nullptr) {
    ClearDeep(head_node_);
  }
}

void SelfBalancingBinarySearchTree::ClearDeep(Node*& node) {
  if (node->p_right_ != nullptr) {
    ClearDeep(node->p_right_);
  }
  if (node->p_left_ != nullptr) {
    ClearDeep(node->p_left_);
  }
  if (node) {
    delete node;
    node = nullptr;
    --size_;
  }
}

void SelfBalancingBinarySearchTree::EraseHead(Node* node) {
  if (node->p_right_ != nullptr) {
    node->p_right_->p_parent_ = nullptr;
    head_node_ = node->p_right_;
    Node* buf = node->p_right_;
    while (buf->p_left_ != nullptr) {
      buf = buf->p_left_;
    }
    if (node->p_left_ != nullptr) {
      node->p_left_->p_parent_ = buf;
      buf->p_left_ = node->p_left_;
    }
  } else {
    if (node->p_left_ != nullptr) {
      node->p_left_->p_parent_ = nullptr;
      head_node_ = node->p_left_;
    }
  }
  delete (node);
  --size_;
  if (!size_) head_node_ = nullptr;
  Balancing(head_node_);
}

void SelfBalancingBinarySearchTree::EraseLeftOrRightParents(Node* node) {
  if (node->p_parent_ != nullptr) {
    if (node == node->p_parent_->p_right_) {
      if (node->p_right_ != nullptr) {
        node->p_parent_->p_right_ = node->p_right_;
        FindLastNode(node);
      } else if (node->p_left_ != nullptr) {
        node->p_left_->p_parent_ = node->p_parent_;
        node->p_parent_->p_right_ = node->p_left_;
      } else {
        node->p_parent_->p_right_ = nullptr;
      }
    } else if (node == node->p_parent_->p_left_) {
      if (node->p_right_ != nullptr) {
        node->p_parent_->p_left_ = node->p_right_;
        FindLastNode(node);
      } else if (node->p_left_ != nullptr) {
        node->p_left_->p_parent_ = node->p_parent_;
        node->p_parent_->p_left_ = node->p_left_;
      } else {
        node->p_parent_->p_left_ = nullptr;
      }
    }
    delete (node);
    --size_;
    Balancing(head_node_);
  }
}

void SelfBalancingBinarySearchTree::FindLastNode(Node* node) {
  node->p_right_->p_parent_ = node->p_parent_;
  if (node->p_left_ != nullptr) {
    Node* buf_c = Cycle(node->p_right_, Left_Right::left);
    node->p_left_->p_parent_ = buf_c;
    buf_c->p_left_ = node->p_left_;
  }
}

typename SelfBalancingBinarySearchTree::Node*
SelfBalancingBinarySearchTree::Cycle(Node* node_cycle, Left_Right pos) {
  if (node_cycle != nullptr) {
    if (pos == Left_Right::right) {
      while (node_cycle->p_right_ != nullptr) {
        node_cycle = node_cycle->p_right_;
      }
    }
    if (pos == Left_Right::left) {
      while (node_cycle->p_left_ != nullptr) {
        node_cycle = node_cycle->p_left_;
      }
    }
  }
  return node_cycle;
}

typename SelfBalancingBinarySearchTree::SetIterator&
SelfBalancingBinarySearchTree::iterator::operator++() {
  if (_parent == nullptr && _current == nullptr) {
    throw std::invalid_argument("operator++ all is nullptr");
  }
  if (_parent != nullptr) {
    if (_current == nullptr) {
      _current = _parent;
      _parent = _current->p_parent_;
    } else if (_parent->p_left_ == _current && _current->p_right_ == nullptr) {
      _current = _parent;
      _parent = _parent->p_parent_;
    } else if (_current->p_right_ != nullptr) {
      _current = _current->p_right_;
      while (_current->p_left_ != nullptr) {
        _current = _current->p_left_;
      }
      _parent = _current->p_parent_;
    } else {
      while (_current->p_parent_ != nullptr &&
             _current != _current->p_parent_->p_left_) {
        _current = _current->p_parent_;
      }
      if (_current->p_parent_ == nullptr) {
        _current = nullptr;
        _parent = _parent->p_right_;
      } else {
        _current = _current->p_parent_;
        _parent = _current->p_parent_;
      }
    }
  } else {
    _parent = _current;
    _current = _current->p_right_;
    while (_current != nullptr && _current->p_left_ != nullptr) {
      _current = _current->p_left_;
      _parent = _current->p_parent_;
    }
  }
  return *this;
}

bool SelfBalancingBinarySearchTree::iterator::operator!=(
    const SetIterator& other) {
  return _current != other._current;
}

void SelfBalancingBinarySearchTree::iterator::ChangeIterPos(Node* other) {
  _current = other;
  _parent = other->p_parent_;
}

int SelfBalancingBinarySearchTree::MaxDepth(Node* node) {
  if (node == nullptr) {
    return (-1);
  } else {
    int leftDepth = MaxDepth(node->p_left_);
    int rightDepth = MaxDepth(node->p_right_);
    if (leftDepth > rightDepth)
      return (leftDepth + 1);
    else
      return (rightDepth + 1);
  }
}

void SelfBalancingBinarySearchTree::ChangeBalanceToCurrentNode(Node* curNode) {
  if (curNode != nullptr) {
    if (curNode->p_left_ == nullptr) {
      curNode->left_deph_ = 0;
    } else {
      curNode->left_deph_ = (MaxDepth(curNode->p_left_) * -1) - 1;
    }
    if (curNode->p_right_ == nullptr) {
      curNode->right_deph_ = 0;
    } else {
      curNode->right_deph_ = MaxDepth(curNode->p_right_) + 1;
    }
    curNode->balance_ = curNode->left_deph_ + curNode->right_deph_;
  }
}

void SelfBalancingBinarySearchTree::Balancing(Node* node) {
  while (node != nullptr) {
    if (node->p_left_ != nullptr) {
      node->left_deph_ =
          (node->p_left_->left_deph_ * -1) >= node->p_left_->right_deph_
              ? node->p_left_->left_deph_ - 1
              : (-1 * node->p_left_->right_deph_) - 1;
    } else {
      node->left_deph_ = 0;
    }
    if (node->p_right_ != nullptr) {
      node->right_deph_ =
          (node->p_right_->left_deph_ * -1) >= node->p_right_->right_deph_
              ? (node->p_right_->left_deph_ * -1) + 1
              : node->p_right_->right_deph_ + 1;
    } else {
      node->right_deph_ = 0;
    }
    node->balance_ = node->left_deph_ + node->right_deph_;
    if (node->balance_ == -2 && node->p_left_->balance_ <= 0) {
      RightTurn(node);
    } else if (node->balance_ == -2 && node->p_left_->balance_ > 0) {
      BigRightTurn(node);
    } else if (node->balance_ == 2 && node->p_right_->balance_ >= 0) {
      LeftTurn(node);
    } else if (node->balance_ == 2 && node->p_right_->balance_ < 0) {
      BigLeftTurn(node);
    } else {
      node = node->p_parent_;
    }
  }
}

void SelfBalancingBinarySearchTree::AddNode(Node* node, Peer a, iterator& it) {
  if (node->kV_ > a) {
    if (node->p_left_ == nullptr) {
      Node* bufL = new Node(a, node);
      node->p_left_ = bufL;
      Balancing(node->p_left_);
      it.ChangeIterPos(bufL);
      ++size_;
    } else {
      AddNode(node->p_left_, a, it);
    }
  } else if (node->kV_ < a) {
    if (node->p_right_ == nullptr) {
      Node* bufR = new Node(a, node);
      node->p_right_ = bufR;
      Balancing(node->p_right_);
      it.ChangeIterPos(bufR);
      ++size_;
    } else {
      AddNode(node->p_right_, a, it);
    }
  }
}

void SelfBalancingBinarySearchTree::LeftTurn(Node* node) {
  if (node == nullptr) {
    throw std::invalid_argument("LeftTurn, node = nullptr");
  }
  if (node->p_parent_ != nullptr) {
    if (node->p_parent_->p_right_ == node) {
      node->p_parent_->p_right_ = node->p_right_;
    } else {
      node->p_parent_->p_left_ = node->p_right_;
    }
    node->p_right_->p_parent_ = node->p_parent_;
  } else {
    head_node_ = node->p_right_;
    node->p_right_->p_parent_ = nullptr;
  }
  Node* tmp_right = node->p_right_;
  node->p_parent_ = node->p_right_;
  Node* tmp_right_left = node->p_right_->p_left_;
  node->p_right_->p_left_ = node;
  node->p_right_ = tmp_right_left;
  if (tmp_right_left != nullptr) {
    tmp_right_left->p_parent_ = node;
    tmp_right_left = nullptr;
  }
  ChangeBalanceToCurrentNode(node);
  ChangeBalanceToCurrentNode(tmp_right);
}

void SelfBalancingBinarySearchTree::RightTurn(Node* node) {
  if (node == nullptr) {
    throw std::invalid_argument("smal_right_turn, node = nullptr");
  }
  if (node->p_parent_ != nullptr) {
    if (node->p_parent_->p_left_ == node) {
      node->p_parent_->p_left_ = node->p_left_;
    } else {
      node->p_parent_->p_right_ = node->p_left_;
    }
    node->p_left_->p_parent_ = node->p_parent_;
  } else {
    head_node_ = node->p_left_;
    node->p_left_->p_parent_ = nullptr;
  }
  Node* tmp_left = node->p_left_;
  Node* tmp_left_right = node->p_left_->p_right_;
  node->p_parent_ = node->p_left_;
  node->p_left_->p_right_ = node;
  node->p_left_ = tmp_left_right;
  if (tmp_left_right != nullptr) {
    tmp_left_right->p_parent_ = node;
    tmp_left_right = nullptr;
  }
  ChangeBalanceToCurrentNode(node);
  ChangeBalanceToCurrentNode(tmp_left);
}

void SelfBalancingBinarySearchTree::BigRightTurn(Node* node) {
  LeftTurn(node->p_left_);
  RightTurn(node);
}

void SelfBalancingBinarySearchTree::BigLeftTurn(Node* node) {
  RightTurn(node->p_right_);
  LeftTurn(node);
}

typename SelfBalancingBinarySearchTree::Node*
SelfBalancingBinarySearchTree::FindNode(const std::string& key) {
  Node* buf = head_node_;
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
  return res ? buf : nullptr;
}

void SelfBalancingBinarySearchTree::Erase(Node* node) {
  if (node->p_parent_ == nullptr) {
    EraseHead(node);
  } else {
    EraseLeftOrRightParents(node);
  }
}

auto SelfBalancingBinarySearchTree::UpdateTimer() -> void {
  auto start = std::chrono::system_clock::now();
  auto iter = timer_.begin();
  while (iter != timer_.end()) {
    if (!(*iter).first ||
        ((*iter).second -=
         std::chrono::duration_cast<std::chrono::seconds>(start - old_time_)
             .count()) <= 0) {
      Erase((*iter).first);

      iter = timer_.erase(iter);
    } else {
      ++iter;
    }
  }
  old_time_ = start;
}

}  //  namespace s21
