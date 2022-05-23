#pragma once

#include <numeric>

template<typename NumT>
struct PointT
{
  NumT x;
  NumT y;
};

template<typename NumT>
bool operator<(
    PointT<NumT> lhs,
    PointT<NumT> rhs
  )
{
  return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

template<typename NumT>
bool operator==(
    PointT<NumT> lhs,
    PointT<NumT> rhs
  )
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

template<typename NumT>
bool operator!=(
    PointT<NumT> lhs,
    PointT<NumT> rhs
  )
{
  return !(lhs == rhs);
}

using Point = PointT<int>;

inline const Point NULL_POINT { INT_MAX, INT_MAX };
