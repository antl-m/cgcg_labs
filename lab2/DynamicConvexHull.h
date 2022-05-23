#pragma once

#include "Point.h"
#include "ConvexHullTree.h"

#include <set>

struct DynamicConvexHull
{
  DynamicConvexHull()
  {
    upper_convex_hull.is_upper = true;
    lower_convex_hull.is_upper = false;
  }

  void insert(
      Point new_point
    )
  {
    upper_convex_hull.insert(new_point);
    lower_convex_hull.insert(new_point);
  }

  void erase(
      Point to_delete_point
    )
  {
    upper_convex_hull.erase(to_delete_point);
    lower_convex_hull.erase(to_delete_point);
  }

  std::vector<Point> get_hull()
  {
    std::vector<Point> result;

    if (upper_convex_hull.root != nullptr)
      result.insert(result.end(), upper_convex_hull.root->data.convex_hull.begin(), upper_convex_hull.root->data.convex_hull.end());

    if (lower_convex_hull.root != nullptr)
      result.insert(result.end(), lower_convex_hull.root->data.convex_hull.rbegin(), lower_convex_hull.root->data.convex_hull.rend());

    return result;
  }

  std::vector<Point> get_all_points()
  {
    std::vector<Point> result;

    get_all_impl(upper_convex_hull.root, result);

    return result;
  }

  void get_all_impl(
      Node * node,
      std::vector<Point> & result
    )
  {
    if (node == nullptr)
      return;

    if (node->left == nullptr && node->right == nullptr)
      return result.push_back(node->data.left_most_right_point);

    get_all_impl(node->left, result);
    get_all_impl(node->right, result);
  }

  ConvexHullTree upper_convex_hull;
  ConvexHullTree lower_convex_hull;
};