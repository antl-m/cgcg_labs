#pragma once

#include <tuple>
#include <vector>

#include "Point.h"
#include "Classification.h"

inline bool is_point_left(
    Point chain_point_1,
    Point chain_point_2,
    Point point,
    bool is_upper
  )
{
  int val = ((chain_point_2.x - chain_point_1.x) * (point.y - chain_point_1.y) - (chain_point_2.y - chain_point_1.y) * (point.x - chain_point_1.x));
  return is_upper ? val > 0 : val < 0;
}

inline Classification define_point_type_left(
    Point q1_pred,
    Point q1,
    Point q1_suc,
    Point q2,
    bool is_upper
  )
{
  if (is_point_left(q2, q1, q1_pred, is_upper) && is_point_left(q2, q1, q1_suc, is_upper))
    return Classification::SUPPORTING;
  if (is_point_left(q2, q1, q1_pred, is_upper) && !is_point_left(q2, q1, q1_suc, is_upper))
    return Classification::CONCAVE;
  else
    return Classification::CONVEX;
}

inline Classification define_point_type_right(
    Point q2_pred,
    Point q2,
    Point q2_suc,
    Point q1,
    bool is_upper
  )
{
  if (!is_point_left(q1, q2, q2_pred, is_upper) && !is_point_left(q1, q2, q2_suc, is_upper))
    return Classification::SUPPORTING;
  if (is_point_left(q1, q2, q2_pred, is_upper) && !is_point_left(q1, q2, q2_suc, is_upper))
    return Classification::CONCAVE;
  else
    return Classification::CONVEX;
}

inline Point get_intersection_point(
    Point a,
    Point b,
    Point c,
    Point d
  )
{
  return Point(((a.x * b.y - a.y * b.x) * (c.x - d.x) - (a.x - b.x) * (c.x * d.y - c.y * d.x))
    / ((a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x)),
    ((a.x * b.y - a.y * b.x) * (c.y - d.y) - (a.y - b.y) * (c.x * d.y - c.y * d.x))
    / ((a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x)));
}


inline int concave_concave_case(
    Point q1,
    Point q1_successor,
    Point max_left,
    Point min_right,
    Point q2_predecessor,
    Point q2
  )
{
  int center_line_x = (max_left.x + min_right.x) / 2;
  if (get_intersection_point(q1, q1_successor, q2_predecessor, q2).x < center_line_x)
    return -1;
  else
  if (get_intersection_point(q1, q1_successor, q2_predecessor, q2).x > center_line_x)
    return 1;
  else
    return 0;
}

inline std::tuple<std::vector<Point>, std::vector<Point>, std::vector<Point>, std::vector<Point>, int> merge_chains(
    const std::vector<Point> chain_1,
    const std::vector<Point> chain_2,
    const bool is_upper
  )
{
  if (chain_2.size() == 1)
  {
    if (chain_1.size() == 1)
      return { chain_1, {}, {}, chain_2, 0 };

    if (chain_1.size() == 2)
    {
      if (is_point_left(chain_1[0], chain_1[1], chain_2[0], is_upper))
        return { { chain_1.begin(), chain_1.begin() + 1 }, { chain_1.begin() + 1, chain_1.end() }, {}, chain_2, 0 };
      else
        return { chain_1, {}, {}, chain_2, 1 };
    }
  }

  if (chain_1.size() == 1)
  {
    if (is_point_left(chain_2[0], chain_2[1], chain_1[0], is_upper))
      return { chain_1, {}, { chain_2.begin(), chain_2.begin() + 1 }, { chain_2.begin() + 1, chain_2.end() }, 0 };
    else
      return { chain_1, {}, {}, chain_2, 0 };
  }

  int index_1 = (static_cast<int>(chain_1.size()) - 1) / 2;
  int index_2 = (static_cast<int>(chain_2.size()) - 1) / 2;

  int temp_min_1 = 0;
  int temp_max_1 = static_cast<int>(chain_1.size()) - 1;

  int temp_min_2 = 0;
  int temp_max_2 = static_cast<int>(chain_2.size()) - 1;

  bool extreme_left_1 = false;
  bool extreme_right_1 = false;

  bool extreme_left_2 = false;
  bool extreme_right_2 = false;

  while (true)
  {
    extreme_left_1 = false;
    extreme_right_1 = false;

    extreme_left_2 = false;
    extreme_right_2 = false;

    if (index_1 == temp_min_1)
      extreme_left_1 = true;
    if (index_1 == temp_max_1)
      extreme_right_1 = true;

    if (index_2 == temp_min_2)
      extreme_left_2 = true;
    if (index_2 == temp_max_2)
      extreme_right_2 = true;

    Classification type_1 = Classification::ERROR;
    Classification type_2 = Classification::ERROR;

    if (extreme_left_1 && extreme_right_1)
      type_1 = Classification::SUPPORTING;
    else
    if (extreme_left_1)
      type_1 = define_point_type_left(Point(chain_1[index_1].x, chain_1[index_1].y - 1), chain_1[index_1], chain_1[index_1 + 1], chain_2[index_2], is_upper);
    else
    if (extreme_right_1)
      type_1 = define_point_type_left(chain_1[index_1 - 1], chain_1[index_1], Point(chain_1[index_1].x, chain_1[index_1].y - 1), chain_2[index_2], is_upper);
    else
      type_1 = define_point_type_left(chain_1[index_1 - 1], chain_1[index_1], chain_1[index_1 + 1], chain_2[index_2], is_upper);

    if (extreme_left_2 && extreme_right_2)
      type_2 = Classification::SUPPORTING;
    else
    if (extreme_left_2)
      type_2 = define_point_type_right(Point(chain_2[index_2].x, chain_2[index_2].y - 1), chain_2[index_2], chain_2[index_2 + 1], chain_1[index_1], is_upper);
    else
    if (extreme_right_2)
      type_2 = define_point_type_right(chain_2[index_2 - 1], chain_2[index_2], Point(chain_2[index_2].x, chain_2[index_2].y - 1), chain_1[index_1], is_upper);
    else
      type_2 = define_point_type_right(chain_2[index_2 - 1], chain_2[index_2], chain_2[index_2 + 1], chain_1[index_1], is_upper);

    if (type_1 == Classification::CONCAVE && type_2 == Classification::CONCAVE)
    {
      int check_result = concave_concave_case(chain_1[index_1], chain_1[index_1 + 1], chain_1[temp_max_1],
                                              chain_2[temp_min_2], chain_2[index_2 - 1], chain_2[index_2]);
      if (check_result == -1)
      {
        temp_min_1 = index_1;
        if (temp_max_1 - index_1 != 1)
          index_1 = (index_1 + temp_max_1) / 2;
        else
          index_1 = temp_max_1;
      }
      else
      if (check_result == 1)
      {
        temp_max_2 = index_2;
        index_2 = (temp_min_2 + index_2) / 2;
      }
      else
      {
        temp_min_1 = index_1;
        if (temp_max_1 - index_1 != 1)
          index_1 = (index_1 + temp_max_1) / 2;
        else
          index_1 = temp_max_1;
        temp_max_2 = index_2;
        index_2 = (temp_min_2 + index_2) / 2;
      }
    }
    else
    if (type_1 == Classification::CONCAVE && type_2 == Classification::SUPPORTING)
    {
      temp_min_1 = index_1;
      if (temp_max_1 - index_1 != 1)
        index_1 = (index_1 + temp_max_1) / 2;
      else
        index_1 = temp_max_1;

      temp_min_2 = index_2;
    }
    else
    if (type_1 == Classification::CONCAVE && type_2 == Classification::CONVEX)
    {
      temp_min_2 = index_2;
      if (temp_max_2 - index_2 != 1)
        index_2 = (index_2 + temp_max_2) / 2;
      else
        index_2 = temp_max_2;
    }
    else
    if (type_1 == Classification::SUPPORTING && type_2 == Classification::CONCAVE)
    {
      temp_max_1 = index_1;

      temp_max_2 = index_2;
      index_2 = (temp_min_2 + index_2) / 2;
    }
    else
    if (type_1 == Classification::SUPPORTING && type_2 == Classification::SUPPORTING)
    {
      break;
    }
    else
    if (type_1 == Classification::SUPPORTING && type_2 == Classification::CONVEX)
    {
      temp_max_1 = index_1;

      temp_min_2 = index_2;
      if (temp_max_2 - index_2 != 1)
        index_2 = (index_2 + temp_max_2) / 2;
      else
        index_2 = temp_max_2;
    }
    else
    if (type_1 == Classification::CONVEX && type_2 == Classification::CONCAVE)
    {
      temp_max_1 = index_1;
      index_1 = (temp_min_1 + index_1) / 2;
    }
    else
    if (type_1 == Classification::CONVEX && type_2 == Classification::SUPPORTING)
    {
      temp_max_1 = index_1;
      index_1 = (temp_min_1 + index_1) / 2;

      temp_min_2 = index_2;
    }
    else
    if (type_1 == Classification::CONVEX && type_2 == Classification::CONVEX)
    {
      temp_max_1 = index_1;
      index_1 = (temp_min_1 + index_1) / 2;

      temp_min_2 = index_2;
      if (temp_max_2 - index_2 != 1)
        index_2 = (index_2 + temp_max_2) / 2;
      else
        index_2 = temp_max_2;
    }
  }

  return {
      { chain_1.begin(), chain_1.begin() + index_1 + 1 },
      { chain_1.begin() + index_1 + 1, chain_1.end() },
      { chain_2.begin(), chain_2.begin() + index_2 },
      { chain_2.begin() + index_2, chain_2.end() },
      index_1
    };
}