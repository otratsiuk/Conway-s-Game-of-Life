#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <string>
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

void life_to_console(std::vector<std::vector<int>> curr_generation) {

  std::cout << "\n";
  for (int i = 0; i < curr_generation.size(); i++) {
    for (int j = 0; j < curr_generation[0].size(); j++) {
      std::cout << curr_generation[i][j] << " ";
    }
    std::cout << "\n";
  }
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

void draw_life(std::vector<std::vector<int>> curr_generation,
               sf::RenderWindow &window) {
  sf::RectangleShape alive_cell(sf::Vector2f(40.f, 40.f));
  alive_cell.setFillColor(sf::Color::Green);

  std::vector<std::vector<int>> prev_generation = curr_generation;

  do {
    sf::Event urgent_event;
    while (window.pollEvent(urgent_event)) {
      if (urgent_event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    draw_galaxy(window);

    life_to_console(curr_generation);
    for (int i = 0; i < curr_generation.size(); i++) {
      for (int j = 0; j < curr_generation[0].size(); j++) {

        if (curr_generation[i][j] == 1) {
          alive_cell.setPosition(i * 50 + 5, j * 50 + 5);
          window.draw(alive_cell);
        }
      }
    }

    window.display();
    usleep(500000);

    prev_generation = curr_generation;
    curr_generation = create_next_generation(curr_generation);
  } while (/*!(curr_generation == prev_generation) && */ window.isOpen());
}

void text_display(std::string message, float x, float y,
                  sf::RenderWindow &window) {
  sf::Text text;
  sf::Font font;
  font.loadFromFile("Ubuntu-Medium.ttf");
  text.setFont(font);
  text.setString(message);
  text.setCharacterSize(36);
  text.setFillColor(sf::Color::Red);
  text.setPosition(sf::Vector2f(x, y));

  window.clear();
  window.draw(text);
  window.display();
}

void choose_pattern(std::vector<std::vector<int>> &curr_generation) {
  int key;
  std::cout << "choose pattern: ";
  std::cin >> key;
  switch (key) {
    // Gilder
  case 1: {
    curr_generation[4][4] = 1;
    curr_generation[5][5] = 1;
    curr_generation[6][3] = 1;
    curr_generation[6][5] = 1;
    break;
  }
    //  Small exploder
  case 2: {
    curr_generation[3][4] = 1;
    curr_generation[4][3] = 1;
    curr_generation[4][4] = 1;
    curr_generation[4][5] = 1;
    curr_generation[5][3] = 1;
    curr_generation[5][5] = 1;
    curr_generation[6][4] = 1;
  }
  default:
    break;
  }
}

int main() {
  std::vector<std::vector<int>> curr_generation(10, std::vector<int>(10, 0));

  choose_pattern(curr_generation);

  sf::RenderWindow window(sf::VideoMode(500, 500), "Conway's Game of Life");

  draw_life(curr_generation, window);

  text_display("Game is over", 150, 200, window);

  sf::Event last_event;
  while (window.waitEvent(last_event)) {
    if (last_event.type == sf::Event::Closed)
      window.close();
  }

  return 0;
}
