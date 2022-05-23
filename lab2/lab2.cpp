#include <SFML/Graphics.hpp>

#include "DynamicConvexHull.h"

int main()
{
  DynamicConvexHull dynamicConvexHull;

  sf::RenderWindow window(sf::VideoMode(1600, 900), "lab2");

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::MouseButtonPressed)
      {
        Point p(event.mouseButton.x, event.mouseButton.y);

        bool was_erased = false;

        for (auto existed_p : dynamicConvexHull.get_all_points())
        {
          if (p.x >= existed_p.x - 5 && p.x <= existed_p.x + 5 &&
              p.y >= existed_p.y - 5 && p.y <= existed_p.y + 5)
          {
            dynamicConvexHull.erase(existed_p);
            was_erased = true;
            break;
          }
        }

        if (!was_erased)
          dynamicConvexHull.insert(p);
      }
    }

    window.clear();

    std::vector<sf::CircleShape> shapes;

    for (auto point : dynamicConvexHull.get_all_points())
    {
      auto & shape = shapes.emplace_back(5.f);
      shape.setFillColor(sf::Color::White);
      shape.setPosition(sf::Vector2f{ point.x - 5.f, point.y - 5.f });
      window.draw(shape);
    }

    std::vector<sf::Vertex> hull;
    for (auto point : dynamicConvexHull.get_hull())
      hull.emplace_back(sf::Vector2f{ static_cast<float>(point.x), static_cast<float>(point.y) });

    std::vector<sf::Vertex> to_draw;
    for (int i = 0; i < hull.size(); ++i)
    {
      to_draw.push_back(hull[i]);

      if (i != 0 && i != hull.size() - 1)
        to_draw.push_back(hull[i]);
    }

    window.draw(to_draw.data(), to_draw.size(), sf::Lines);

    window.display();
  }

  return 0;
}