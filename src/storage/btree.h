#pragma once

#include <string>
#include <vector>
#include <memory>

struct BTreeNode {
  std::vector<std::string> keys;
  std::vector<int> values;
  std::vector<std::unique_ptr<BTreeNode>> children;
  bool is_leaf;

  BTreeNode(bool leaf = true);
};

class BTree {
private:
  std::unique_ptr<BTreeNode> root;
  int degree;
  int min_keys;

  void split_child(BTreeNode* parent, int index);
  void insert_non_full(BTreeNode* node, const std::string& key, int value);
  BTreeNode* search_node(BTreeNode* node, const std::string& key);
  void delete_key(BTreeNode* node, const std::string& key);
  void merge_nodes(BTreeNode* parent, int index);
  void borrow_from_prev(BTreeNode* parent, int index);
  void borrow_from_next(BTreeNode* parent, int index);

public:
  explicit BTree(int t = 3);
  ~BTree();

  void insert(const std::string& key, int value);
  bool remove(const std::string& key);
  int search(const std::string& key);
  std::vector<int> range_search(const std::string& start_key, const std::string& end_key);

  void print_tree() const;
  bool is_empty() const { return !root; }

private:
  void range_search_recursive(BTreeNode* node,
                              const std::string& start_key,
                              const std::string& end_key,
                              std::vector<int>& results
  );
  void print_node(BTreeNode* node, int depth) const;
  void delete_internal_node(BTreeNode* node, int index);
  void fill_child(BTreeNode* node, int index);
  BTreeNode* get_predecessor(BTreeNode* node);
  BTreeNode* get_successor(BTreeNode* node);
};