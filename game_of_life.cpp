#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <vector>

int count_neighbours(int curr_i, int curr_j,
                     std::vector<std::vector<int>> &generation) {
  int sum = 0;

  for (int i = curr_i - 1; i < curr_i + 2; i++) {
    for (int j = curr_j - 1; j < curr_j + 2; j++) {

      int neighbour_i = i;
      int neighbour_j = j;

      if (i == 10) {
        neighbour_i = 0;
      }
      if (j == 10) {
        neighbour_j = 0;
      }
      if (i == -1) {
        neighbour_i = 9;
      }
      if (j == -1) {
        neighbour_j = 9;
      }

      sum += generation[neighbour_i][neighbour_j];
    }
  }
  sum -= generation[curr_i][curr_j];

  return sum;
}

bool is_alive(std::vector<std::vector<int>> &generation) {
  bool alive = true;
  int sum = 0;

  for (auto &rows : generation) {
    for (auto &cellar : rows) {
      sum += cellar;
    }
  }

  if (sum == 0) {
    alive = false;
  }

  return alive;
}

int count_alive_cells(std::vector<std::vector<int>> cells) {
  int alive_cells = 0;

  for (auto &rows : cells) {
    for (auto &cell : cells[0]) {
      if (cell == 1) {
        alive_cells++;
      }
    }
  }

  return alive_cells;
}

std::vector<std::vector<int>>
create_next_generation(std::vector<std::vector<int>> curr_generation) {
  std::vector<std::vector<int>> next_generation = curr_generation;

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      int neighbours = count_neighbours(i, j, curr_generation);

      if (curr_generation[i][j] == 0 && neighbours == 3) {
        next_generation[i][j] = 1;
      } else if (curr_generation[i][j] == 1 &&
                 (neighbours == 2 || neighbours == 3)) {
        next_generation[i][j] = 1;
      } else if (curr_generation[i][j] == 1 &&
                 (neighbours < 2 || neighbours > 3)) {
        next_generation[i][j] = 0;
      }
    }
  }

  return next_generation;
}

void draw_galaxy(sf::RenderWindow &window) {
  sf::VertexArray vertical_lines(sf::Lines, 18);
  sf::VertexArray horizontal_lines(sf::Lines, 18);

  for (int i = 0, pos = 50; i < 18; i++) {
    if (i % 2 == 0) {
      vertical_lines[i].position = sf::Vector2f(0, pos);
    } else {
      vertical_lines[i].position = sf::Vector2f(500, pos);
      pos += 50;
    }
    vertical_lines[i].color = sf::Color::Green;
  }

  for (int i = 0, pos = 50; i < 18; i++) {
    if (i % 2 == 0) {
      horizontal_lines[i].position = sf::Vector2f(pos, 0);
    } else {
      horizontal_lines[i].position = sf::Vector2f(pos, 500);
      pos += 50;
    }
    horizontal_lines[i].color = sf::Color::Green;
  }

  window.draw(vertical_lines);
  window.draw(horizontal_lines);
}

void draw_cells(std::vector<std::vector<int>> curr_generation,
                sf::RenderWindow &window) {

  sf::RectangleShape alive_cell(sf::Vector2f(50.f, 50.f));
  alive_cell.setFillColor(sf::Color::Green);

  std::vector<std::vector<int>> prev_generation = curr_generation;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    do {
      window.clear();
      draw_galaxy(window);

      std::cout << "\n";
      for (int i = 0; i < curr_generation.size(); i++) {
        for (int j = 0; j < curr_generation[0].size(); j++) {
          std::cout << curr_generation[i][j] << " ";

          if (curr_generation[i][j] == 1) {
            alive_cell.setPosition(i * 50, j * 50);
            window.draw(alive_cell);
          }
        }
        std::cout << "\n";
      }

      window.display();
      sleep(1);

      prev_generation = curr_generation;
      curr_generation = create_next_generation(curr_generation);
    } while (curr_generation != prev_generation);

    std::cout << "Game over "
              << "\n";
  }
}

int main() {
  std::vector<std::vector<int>> curr_generation(10, std::vector<int>(10, 0));
  std::vector<std::vector<int>> next_generation(10, std::vector<int>(10, 0));

  curr_generation[9][0] = 1;
  curr_generation[9][1] = 1;
  curr_generation[0][9] = 1;
  curr_generation[2][9] = 1;
  curr_generation[4][0] = 1;
  curr_generation[9][9] = 1;
  curr_generation[1][0] = 1;

  sf::RenderWindow window(sf::VideoMode(500, 500), "Game of life");

  draw_cells(curr_generation, window);

  return 0;
}