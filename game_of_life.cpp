#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

const auto FIELD_SIZE = 25;
const auto CELL_SIZE = 25;
const auto WINDOW_X = 725;
const auto WINDOW_Y = 625;
const auto SPARE_FIELD_WIDTH = 100;

int count_neighbours(int curr_i, int curr_j,
                     std::vector<std::vector<int>> &generation) {
  int sum = 0;

  for (int i = curr_i - 1; i < curr_i + 2; i++) {
    for (int j = curr_j - 1; j < curr_j + 2; j++) {

      int neighbour_i = i;
      int neighbour_j = j;

      if (i == FIELD_SIZE) {
        neighbour_i = 0;
      }
      if (j == FIELD_SIZE) {
        neighbour_j = 0;
      }
      if (i == -1) {
        neighbour_i = FIELD_SIZE - 1;
      }
      if (j == -1) {
        neighbour_j = FIELD_SIZE - 1;
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

  for (int i = 0; i < FIELD_SIZE; i++) {
    for (int j = 0; j < FIELD_SIZE; j++) {
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
  for (int i = 0; i < FIELD_SIZE; i++) {
    for (int j = 0; j < FIELD_SIZE; j++) {
      std::cout << curr_generation[i][j] << " ";
    }
    std::cout << "\n";
  }
}

void draw_galaxy(sf::RenderWindow &window) {
  sf::VertexArray vertical_lines(sf::Lines, 50);
  sf::VertexArray horizontal_lines(sf::Lines, 48);

  for (int i = 0, pos = CELL_SIZE; i < 48; i++) {
    if (i % 2 == 0) {
      horizontal_lines[i].position = sf::Vector2f(0, pos);
    } else {
      horizontal_lines[i].position =
          sf::Vector2f(WINDOW_X - SPARE_FIELD_WIDTH, pos);
      pos += CELL_SIZE;
    }
    horizontal_lines[i].color = sf::Color::Green;
  }

  for (int i = 0, pos = CELL_SIZE; i < 50; i++) {
    if (i % 2 == 0) {
      vertical_lines[i].position = sf::Vector2f(pos, 0);
    } else {
      vertical_lines[i].position = sf::Vector2f(pos, WINDOW_Y);
      pos += CELL_SIZE;
    }
    vertical_lines[i].color = sf::Color::Green;
  }

  window.draw(vertical_lines);
  window.draw(horizontal_lines);
}

void draw_life(std::vector<std::vector<int>> curr_generation,
               sf::RenderWindow &window) {
  sf::RectangleShape alive_cell(
      sf::Vector2f(CELL_SIZE - 10.f, CELL_SIZE - 10.f));
  alive_cell.setFillColor(sf::Color::Green);

  sf::Event urgent_event;
  while (window.pollEvent(urgent_event)) {
    if (urgent_event.type == sf::Event::Closed)
      window.close();
  }

  window.clear();
  draw_galaxy(window);

  for (int i = 0; i < curr_generation.size(); i++) {
    for (int j = 0; j < curr_generation[0].size(); j++) {

      if (curr_generation[i][j] == 1) {
        alive_cell.setPosition(j * CELL_SIZE + 5, i * CELL_SIZE + 5);
        window.draw(alive_cell);
      }
    }
  }

  window.display();
  usleep(200000);
}

void text_display(std::string message, float x, float y, int size,
                  sf::RenderWindow &window) {
  sf::Text text;
  sf::Font font;
  font.loadFromFile("Ubuntu-Medium.ttf");
  text.setFont(font);
  text.setString(message);
  text.setCharacterSize(size);
  text.setFillColor(sf::Color::Red);
  text.setPosition(sf::Vector2f(x, y));

  // window.clear();
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
    curr_generation[6][4] = 1;
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
  // Exploder
  case 3: {
    curr_generation[2][2] = 1;
    curr_generation[2][4] = 1;
    curr_generation[2][6] = 1;
    curr_generation[3][2] = 1;
    curr_generation[3][6] = 1;
    curr_generation[4][2] = 1;
    curr_generation[4][6] = 1;
    curr_generation[5][2] = 1;
    curr_generation[5][6] = 1;
    curr_generation[6][2] = 1;
    curr_generation[6][4] = 1;
    curr_generation[6][6] = 1;
  }
  default:
    break;
  }
}

void get_pattern(std::vector<std::vector<int>> &curr_generation,
                 sf::RenderWindow window) {}

int main() {
  std::vector<std::vector<int>> curr_generation(
      FIELD_SIZE, std::vector<int>(FIELD_SIZE, 0));
  std::vector<std::vector<int>> prev_generation(
      FIELD_SIZE, std::vector<int>(FIELD_SIZE, 0));

  choose_pattern(curr_generation);

  sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y),
                          "Conway's Game of Life");

  int generation_counter = 0;

  do {
    prev_generation = curr_generation;
    curr_generation = create_next_generation(curr_generation);

    life_to_console(curr_generation);
    draw_life(curr_generation, window);

    generation_counter++;

  } while (!(curr_generation == prev_generation) && window.isOpen());

  window.clear();
  text_display("Game is over", 250, 250, 36, window);

  sf::Event last_event;
  while (window.waitEvent(last_event)) {
    if (last_event.type == sf::Event::Closed)
      window.close();
  }

  return 0;
}