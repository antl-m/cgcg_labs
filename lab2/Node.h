#pragma once

#include "NodeData.h"

struct Node
{
  Node(
      NodeData _data
    ) :
      data{_data}
  {}

  NodeData data;
  Node * parent = nullptr;
  Node * left   = nullptr;
  Node * right  = nullptr;
};

bool operator<(
    const Node & lhs,
    const Node & rhs
  )
{
  return lhs.data < rhs.data;
}