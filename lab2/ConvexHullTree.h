#pragma once

#include "Point.h"
#include "Node.h"
#include "Utils.h"

struct ConvexHullTree
{
  void insert(
      Point key
    )
  {
    Node * node = new Node{ NodeData{key} };
    node->parent = nullptr;
    node->data.left_most_right = node;
    node->left = nullptr;
    node->right = nullptr;

    Node * x = root;

    if (x == nullptr)
    {
      root = node;
      return;
    }

    auto [left_neighbour, right_neighbour] = down(x, key, nullptr, nullptr);

    Node * new_node_parent = new Node{ NodeData{NULL_POINT} };
    node->parent = new_node_parent;

    if (left_neighbour == nullptr)
    {
      new_node_parent->left = node;
      new_node_parent->right = right_neighbour;

      new_node_parent->parent = right_neighbour->parent;

      if (right_neighbour->parent == nullptr)
      {
        root = new_node_parent;
        new_node_parent->parent = nullptr;
      }
      else
      {
        right_neighbour->parent->left = new_node_parent;
      }

      right_neighbour->parent = new_node_parent;
    }
    else
    if (right_neighbour == nullptr)
    {
      new_node_parent->right = node;
      new_node_parent->left = left_neighbour;

      new_node_parent->parent = left_neighbour->parent;

      if (left_neighbour->parent == nullptr)
      {
        root = new_node_parent;
        new_node_parent->parent = nullptr;
      }
      else
      {
        left_neighbour->parent->right = new_node_parent;
      }

      left_neighbour->parent = new_node_parent;
    }
    else
    if (find_brother(left_neighbour).first == right_neighbour)
    {
      new_node_parent->left = left_neighbour;
      new_node_parent->right = node;

      new_node_parent->parent = left_neighbour->parent;
      left_neighbour->parent->left = new_node_parent;

      left_neighbour->parent = new_node_parent;
    }
    else
    {
      new_node_parent->left = node;
      new_node_parent->right = right_neighbour;

      new_node_parent->parent = right_neighbour->parent;

      if (node_side(right_neighbour) == -1)
        right_neighbour->parent->left = new_node_parent;
      else
        right_neighbour->parent->right = new_node_parent;

      right_neighbour->parent = new_node_parent;
    }

    up(node);
  }

  int node_side(
      Node * node
    )
  {
    if (node->parent->left == node)
      return -1;
    else
    if (node->parent->right == node)
      return 1;
    else
      return 0;
  }

  std::pair<Node *, Node *> down(
      Node * current_node,
      Point point,
      Node * left_neighbour,
      Node * right_neighbour
    )
  {
    if (current_node->left == nullptr)
    {
      if (point.x <= current_node->data.left_most_right_point.x)
        right_neighbour = current_node;
      else
        left_neighbour = current_node;

      return { left_neighbour, right_neighbour };
    }

    std::vector<Point> left_queue{ current_node->data.convex_hull.begin(), current_node->data.convex_hull.begin() + current_node->data.separating_index + 1 };
    std::vector<Point> right_queue{ current_node->data.convex_hull.begin() + current_node->data.separating_index + 1, current_node->data.convex_hull.end() };

    Node * left_son  = current_node->left;
    Node * right_son = current_node->right;

    if (left_son->left != nullptr)
    {
      left_son->data.convex_hull.clear();
      left_son->data.convex_hull.insert(left_son->data.convex_hull.end(), left_queue.begin(), left_queue.end());
      left_son->data.convex_hull.insert(left_son->data.convex_hull.end(), left_son->data.points_array.begin(), left_son->data.points_array.end());
    }

    if (right_son->left != nullptr)
    {
      right_son->data.convex_hull.clear();
      right_son->data.convex_hull.insert(right_son->data.convex_hull.end(), right_son->data.points_array.begin(), right_son->data.points_array.end());
      right_son->data.convex_hull.insert(right_son->data.convex_hull.end(), right_queue.begin(), right_queue.end());
    }

    if (point.x <= current_node->data.left_most_right_point.x)
    {
      right_neighbour = current_node;
      current_node = current_node->left;
    }
    else
    {
      left_neighbour = current_node->data.left_most_right;
      current_node = current_node->right;
    }

    return down(current_node, point, left_neighbour, right_neighbour);
  }

  void up(
      Node * current_node
    )
  {
    if (current_node == root)
    {
      current_node->data.points_array = current_node->data.convex_hull;
      return;
    }

    auto [current_brother, side] = find_brother(current_node);

    std::vector<Point> q_1, q_2, q_3, q_4;
    int j = 0;

    if (side == -1)
      std::tie(q_1, q_2, q_3, q_4, j) = merge_chains(current_brother->data.convex_hull, current_node->data.convex_hull, is_upper);
    else
    if (side == 1)
      std::tie(q_1, q_2, q_3, q_4, j) = merge_chains(current_node->data.convex_hull, current_brother->data.convex_hull, is_upper);

    current_node->parent->left->data.points_array = q_2;
    current_node->parent->right->data.points_array = q_3;

    current_node->parent->data.convex_hull.clear();
    current_node->parent->data.convex_hull.insert(current_node->parent->data.convex_hull.end(), q_1.begin(), q_1.end());
    current_node->parent->data.convex_hull.insert(current_node->parent->data.convex_hull.end(), q_4.begin(), q_4.end());
    current_node->parent->data.separating_index = j;

    current_node->parent->data.left_most_right = find_left_most_right(current_node->parent);
    current_node->parent->data.left_most_right_point = current_node->parent->data.left_most_right->data.left_most_right_point;

    up(current_node->parent);
  }

  std::pair<Node *, int> find_brother(
      Node * node
    )
  {
    if (node->parent->left == node)
      return { node->parent->right, 1 };
    else
    if (node->parent->right == node)
      return { node->parent->left, -1 };
    return { nullptr, 0 };
  }

  Node * find_left_most_right(
      Node * node
    )
  {
    if (node->left != nullptr)
      node = node->left;

    while (node->right != nullptr)
      node = node->right;

    return node->data.left_most_right;
  }

  void erase(
      Point data
    )
  {
    Node * node = new Node{ NodeData{data} };
    node->parent = nullptr;
    node->data.left_most_right = node;
    node->left = nullptr;
    node->right = nullptr;

    auto [_, to_delete_node] = down(root, data, nullptr, nullptr);

    if (to_delete_node == nullptr || to_delete_node->data.left_most_right_point.x != data.x || to_delete_node->data.left_most_right_point.y != data.y)
      return;
    else
    if (to_delete_node == root)
      root = nullptr;
    else
    if (to_delete_node->parent->parent == nullptr)
    {
      auto [brother, _] = find_brother(to_delete_node);

      brother->data.points_array = brother->data.convex_hull;

      root = brother;
      brother->parent = nullptr;
    }
    else
    {
      Node * node_parent = to_delete_node->parent;
      auto [brother, _] = find_brother(to_delete_node);

      int side = node_side(node_parent);

      if (side == -1)
        node_parent->parent->left = brother;
      else
      if (side == 1)
        node_parent->parent->right = brother;

      brother->parent = node_parent->parent;
      up(brother);
    }
  }

  Node * root = nullptr;
  bool is_upper = true;
};
