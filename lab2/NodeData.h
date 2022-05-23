#pragma once

#include <vector>
#include "Point.h"

struct Node;

struct NodeData
{
  NodeData(
      Point key
    ) :
      left_most_right_point{key}
  {
    if (key != NULL_POINT)
      convex_hull.push_back(key);
  }

  Point              left_most_right_point;
  Node *             left_most_right = nullptr;
  std::vector<Point> points_array;
  int                separating_index = 0;
  std::vector<Point> convex_hull;
  std::vector<Point> graph_hull;
};

bool operator<(
    const NodeData & lhs,
    const NodeData & rhs
  )
{
  return lhs.left_most_right_point < rhs.left_most_right_point;
}
