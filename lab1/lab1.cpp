#include <iostream>
#include <algorithm>
#include <vector>

struct Tree
{
  struct Point
  {
    int x{};
    int y{};
  };

  enum class Orientation
  {
    Vertical, Horizontal, Invalid
  };

  struct Node
  {
    Point       point;
    Orientation orientation = Orientation::Invalid;
    Node *      left        = nullptr;
    Node *      right       = nullptr;


    int getCoord() const
    {
      switch (orientation)
      {
        case Orientation::Vertical:   return point.x;
        case Orientation::Horizontal: return point.y;
        default: return 0;
      }
    }

    ~Node()
    {
      delete left;
      delete right;
    }
  };

  struct Region
  {
    int minX;
    int maxX;
    int minY;
    int maxY;

    bool contains(
        const Point point
      ) const
    {
      return point.x >= minX && point.x <= maxX &&
             point.y >= minY && point.y <= maxY;
    }

    std::pair<int, int> getBorders(
        const Orientation orientation
      ) const
    {
      switch (orientation)
      {
        case Orientation::Vertical:   return std::make_pair(minX, maxX);
        case Orientation::Horizontal: return std::make_pair(minY, maxY);
        default:                      return std::make_pair(0, 0);
      }
    }
  };

  Node * root = nullptr;

  Tree(
      std::vector<Point> points
    )
  {
    construct(Orientation::Vertical, root, points.begin(), points.end());
  }

  static Orientation getOppositeOrientation(
      Orientation orientation
    )
  {
    switch (orientation)
    {
      case Orientation::Vertical:   return Orientation::Horizontal;
      case Orientation::Horizontal: return Orientation::Vertical;
      default:                      return Orientation::Invalid;
    }
  }

  static void construct(
      Orientation                           orientation,
      Node * &                              node,
      typename std::vector<Point>::iterator begin,
      typename std::vector<Point>::iterator end
    )
  {
    const int pointsCount = end - begin;

    if (pointsCount < 1 || node != nullptr || orientation == Orientation::Invalid)
      return;

    node = new Node();
    node->orientation = orientation;

    if (pointsCount == 1)
    {
      node->point = *begin;
      return;
    }

    switch (orientation)
    {
      case Orientation::Vertical:
        std::sort(begin, end, [](const Point & lhs, const Point & rhs){ return lhs.x < rhs.x; });
        break;
      case Orientation::Horizontal:
        std::sort(begin, end, [](const Point & lhs, const Point & rhs){ return lhs.y < rhs.y; });
        break;
      default: return;
    }

    auto median = begin + pointsCount / 2;
    node->point = *median;

    const auto oppositeOrientation = getOppositeOrientation(orientation);
    construct(oppositeOrientation, node->left,  begin,      median);
    construct(oppositeOrientation, node->right, median + 1, end);
  }

  std::vector<Point> search(
      const Region & region
    )
  {
    std::vector<Point> result;
    search(root, region, result);
    return result;
  }

  void search(
      Node *               node,
      const Region &       region,
      std::vector<Point> & result
    )
  {
    if (node == nullptr)
      return;

    if (region.contains(node->point))
      result.push_back(node->point);

    const auto [left, right] = region.getBorders(node->orientation);
    const auto pointCoord    = node->getCoord();

    if (pointCoord >= left)
      search(node->left, region, result);
    if (pointCoord <= right)
      search(node->right, region, result);
  }

  ~Tree()
  {
    delete root;
  }
};

std::ostream & operator<<(
    std::ostream &                  out,
    const typename Tree::Point & point
  )
{
  out << "(" << point.x << ", " << point.y << ")";
  return out;
}

std::ostream & operator<<(
    std::ostream &                   out,
    const typename Tree::Region & region
  )
{
  out << "{ X in [" << region.minX << ", " << region.maxX << "], Y in [" << region.minY << ", " << region.maxY << "] }";
  return out;
}

template<typename T>
std::ostream & operator<<(
    std::ostream &         out,
    const std::vector<T> & vector
  )
{
  out << "[";
  bool first = true;
  for (const auto & elem : vector)
  {
    if (!first)
      out << ", ";
    first = false;
    out << elem;
  }
  out << "]";
  return out;
}

int main()
{
  std::vector<Tree::Point> input{
    { -1,  1 },
    {  1,  1 },
    {  3,  2 },
    {  1,  0 },
    {  2,  0 },
    {  0, -1 },
    { -1, -2 },
    {  2, -2 },
  };

  Tree::Region region{ -2, -1, -3, 2 };

  std::cout << "Input set of points:\n" << input << "\n\n";
  std::cout << "Input region:\n" << region << "\n\n";

  Tree tree{ input };

  std::cout << "Result of search:\n" << tree.search(region) << "\n\n";

  return 0;
}
