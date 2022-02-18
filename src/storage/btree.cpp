#include "storage/btree.h"
#include <iostream>
#include <algorithm>

BTreeNode::BTreeNode(bool leaf) : is_leaf(leaf) {}

BTree::BTree(int t) : degree(t), min_keys(t - 1) {
  root = std::make_unique<BTreeNode>(true);
}

BTree::~BTree() = default;

void BTree::insert(const std::string& key, int value) {
  BTreeNode* r = root.get();

  if (r->keys.size() == 2 * degree - 1) {
    auto new_root = std::make_unique<BTreeNode>(false);
    new_root->children.push_back(std::move(root));
    root = std::move(new_root);
    split_child(root.get(), 0);
    insert_non_full(root.get(), key, value);
  }
  else {
    insert_non_full(root.get(), key, value);
  }
}

void BTree::split_child(BTreeNode* parent, int index) {
  BTreeNode* child = parent->children[index].get();
  auto new_node = std::make_unique<BTreeNode>(child->is_leaf);

  for (int i = 0; i < min_keys; ++i) {
    new_node->keys.push_back(child->keys[i + degree]);
    new_node->values.push_back(child->values[i + degree]);
  }

  if (!child->is_leaf) {
    for (int i = 0; i < degree; ++i) {
      new_node->children.push_back(std::move(child->children[i + degree]));
    }
  }

  child->keys.resize(min_keys);
  child->values.resize(min_keys);

  if (!child->is_leaf) {
    child->children.resize(degree);
  }

  parent->keys.insert(parent->keys.begin() + index, child->keys[min_keys]);
  parent->values.insert(parent->values.begin() + index, child->values[min_keys]);
  parent->children.insert(parent->children.begin() + index + 1, std::move(new_node));
}

void BTree::insert_non_full(BTreeNode* node, const std::string& key, int value) {
  int i = node->keys.size() - 1;

  if (node->is_leaf) {
    while (i >= 0 && key < node->keys[i]) {
      node->keys[i + 1] = node->keys[i];
      node->values[i + 1] = node->values[i];
      i--;
    }
    node->keys.insert(node->keys.begin() + i + 1, key);
    node->values.insert(node->values.begin() + i + 1, value);
  }
  else {
    while (i >= 0 && key < node->keys[i]) {
      i--;
    }
    i++;

    if (node->children[i]->keys.size() == 2 * degree - 1) {
      split_child(node, i);

      if (key > node->keys[i]) {
        i++;
      }
    }
    insert_non_full(node->children[i].get(), key, value);
  }
}

int BTree::search(const std::string& key) {
  BTreeNode* result = search_node(root.get(), key);

  if (result) {
    for (size_t i = 0; i < result->keys.size(); ++i) {
      if (result->keys[i] == key) {
        return result->values[i];
      }
    }
  }
  return -1;
}

BTreeNode* BTree::search_node(BTreeNode* node, const std::string& key) {
  if (!node) return nullptr;

  int i = 0;

  while (i < static_cast<int>(node->keys.size()) && key > node->keys[i]) {
    i++;
  }

  if (i < static_cast<int>(node->keys.size()) && key == node->keys[i]) {
    return node;
  }

  if (node->is_leaf) {
    return nullptr;
  }

  return search_node(node->children[i].get(), key);
}

bool BTree::remove(const std::string& key) {
  if (!root) return false;

  delete_key(root.get(), key);

  if (root->keys.empty() && !root->is_leaf) {
    root = std::move(root->children[0]);
  }

  return true;
}

void BTree::delete_key(BTreeNode* node, const std::string& key) {
  int index = 0;

  while (index < static_cast<int>(node->keys.size()) && key > node->keys[index]) {
    index++;
  }

  if (index < static_cast<int>(node->keys.size()) && key == node->keys[index]) {
    if (node->is_leaf) {
      node->keys.erase(node->keys.begin() + index);
      node->values.erase(node->values.begin() + index);
    }
    else {
      delete_internal_node(node, index);
    }
  }
  else {
    if (node->is_leaf) return;

    bool last_child = (index == static_cast<int>(node->keys.size()));

    if (node->children[index]->keys.size() < degree) {
      fill_child(node, index);
    }

    if (last_child && index > static_cast<int>(node->keys.size())) {
      delete_key(node->children[index - 1].get(), key);
    }
    else {
      delete_key(node->children[index].get(), key);
    }
  }
}

void BTree::delete_internal_node(BTreeNode* node, int index) {
  std::string key = node->keys[index];

  if (node->children[index]->keys.size() >= degree) {
    BTreeNode* pred = get_predecessor(node->children[index].get());
    node->keys[index] = pred->keys.back();
    node->values[index] = pred->values.back();
    delete_key(node->children[index].get(), pred->keys.back());
  }
  else if (node->children[index + 1]->keys.size() >= degree) {
    BTreeNode* succ = get_successor(node->children[index + 1].get());
    node->keys[index] = succ->keys[0];
    node->values[index] = succ->values[0];
    delete_key(node->children[index + 1].get(), succ->keys[0]);
  }
  else {
    merge_nodes(node, index);
    delete_key(node->children[index].get(), key);
  }
}

void BTree::fill_child(BTreeNode* node, int index) {
  if (index != 0 && node->children[index - 1]->keys.size() >= degree) {
    borrow_from_prev(node, index);
  }
  else if (index != static_cast<int>(node->keys.size()) &&
           node->children[index + 1]->keys.size() >= degree
  ) {
    borrow_from_next(node, index);
  }
  else {
    if (index != static_cast<int>(node->keys.size())) {
      merge_nodes(node, index);
    }
    else {
      merge_nodes(node, index - 1);
    }
  }
}

void BTree::borrow_from_prev(BTreeNode* node, int index) {
  BTreeNode* child = node->children[index].get();
  BTreeNode* sibling = node->children[index - 1].get();

  child->keys.insert(child->keys.begin(), node->keys[index - 1]);
  child->values.insert(child->values.begin(), node->values[index - 1]);

  if (!child->is_leaf) {
    child->children.insert(child->children.begin(),
                           std::move(sibling->children.back())
    );
    sibling->children.pop_back();
  }

  node->keys[index - 1] = sibling->keys.back();
  node->values[index - 1] = sibling->values.back();
  sibling->keys.pop_back();
  sibling->values.pop_back();
}

void BTree::borrow_from_next(BTreeNode* node, int index) {
  BTreeNode* child = node->children[index].get();
  BTreeNode* sibling = node->children[index + 1].get();

  child->keys.push_back(node->keys[index]);
  child->values.push_back(node->values[index]);

  if (!child->is_leaf) {
    child->children.push_back(std::move(sibling->children[0]));
    sibling->children.erase(sibling->children.begin());
  }

  node->keys[index] = sibling->keys[0];
  node->values[index] = sibling->values[0];
  sibling->keys.erase(sibling->keys.begin());
  sibling->values.erase(sibling->values.begin());
}

void BTree::merge_nodes(BTreeNode* parent, int index) {
  BTreeNode* child = parent->children[index].get();
  BTreeNode* sibling = parent->children[index + 1].get();

  child->keys.push_back(parent->keys[index]);
  child->values.push_back(parent->values[index]);

  for (size_t i = 0; i < sibling->keys.size(); ++i) {
    child->keys.push_back(sibling->keys[i]);
    child->values.push_back(sibling->values[i]);
  }

  if (!child->is_leaf) {
    for (size_t i = 0; i < sibling->children.size(); ++i) {
      child->children.push_back(std::move(sibling->children[i]));
    }
  }

  parent->keys.erase(parent->keys.begin() + index);
  parent->values.erase(parent->values.begin() + index);
  parent->children.erase(parent->children.begin() + index + 1);
}

BTreeNode* BTree::get_predecessor(BTreeNode* node) {
  while (!node->is_leaf) {
    node = node->children.back().get();
  }
  return node;
}

BTreeNode* BTree::get_successor(BTreeNode* node) {
  while (!node->is_leaf) {
    node = node->children[0].get();
  }
  return node;
}

std::vector<int> BTree::range_search(const std::string& start_key, const std::string& end_key) {
  std::vector<int> results;

  range_search_recursive(root.get(), start_key, end_key, results);
  return results;
}

void BTree::range_search_recursive(BTreeNode* node,
  const std::string& start_key,
  const std::string& end_key,
  std::vector<int>& results
) {
  if (!node) return;

  int i = 0;

  while (i < static_cast<int>(node->keys.size()) && node->keys[i] < start_key) {
    i++;
  }

  while (i < static_cast<int>(node->keys.size()) && node->keys[i] <= end_key) {
    if (!node->is_leaf) {
      range_search_recursive(node->children[i].get(), start_key, end_key, results);
    }
    results.push_back(node->values[i]);
    i++;
  }

  if (!node->is_leaf && i < static_cast<int>(node->children.size())) {
    range_search_recursive(node->children[i].get(), start_key, end_key, results);
  }
}

void BTree::print_tree() const {
  if (root) {
    print_node(root.get(), 0);
  }
}

void BTree::print_node(BTreeNode* node, int depth) const {
  std::string indent(depth * 2, ' ');

  std::cout << indent << "Node: ";

  for (size_t i = 0; i < node->keys.size(); ++i) {
    std::cout << node->keys[i] << "(" << node->values[i] << ") ";
  }
  std::cout << std::endl;

  if (!node->is_leaf) {
    for (const auto& child : node->children) {
      print_node(child.get(), depth + 1);
    }
  }
}