#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

const auto FIELD_SIZE = 25;
const auto CELL_SIZE = 25;
const auto WINDOW_X = 825;
const auto WINDOW_Y = 625;
const auto SPARE_FIELD_WIDTH = 200;
const auto BUTTON_X = 150;
const auto BUTTON_Y = 50;
const auto START_BUTTON_X = 650;
const auto START_BUTTON_Y = 550;
const auto NEXT_BUTTON_X = 650;
const auto NEXT_BUTTON_Y = 25;

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

  for (int i = 0; i < curr_generation.size(); i++) {
    for (int j = 0; j < curr_generation[0].size(); j++) {

      if (curr_generation[i][j] == 1) {
        alive_cell.setPosition(j * CELL_SIZE + 5, i * CELL_SIZE + 5);
        window.draw(alive_cell);
      }
    }
  }
}

void draw_text(std::string message, float x, float y, int size,
               sf::RenderWindow &window) {
  sf::Text text;
  sf::Font font;
  font.loadFromFile("Ubuntu-Medium.ttf");
  text.setFont(font);
  text.setString(message);
  text.setCharacterSize(size);
  text.setFillColor(sf::Color::Red);
  text.setPosition(sf::Vector2f(x, y));

  window.draw(text);
}

void draw_button(float x, float y, std::string message,
                 sf::RenderWindow &window) {
  sf::RectangleShape button(sf::Vector2f(BUTTON_X, BUTTON_Y));
  button.setFillColor(sf::Color::Transparent);
  button.setOutlineColor(sf::Color::Green);
  button.setOutlineThickness(5);

  draw_text(message, x + 40, y + 15, 18, window);

  button.setPosition(sf::Vector2f(x, y));
  window.draw(button);
}

bool button_is_pressed(int x_button, int y_button, int x_pressed,
                       int y_pressed) {
  bool is_pressed = false;

  if (x_pressed > x_button && x_pressed < x_button + BUTTON_X &&
      y_pressed > y_button && y_pressed < y_button + BUTTON_X) {
    is_pressed = true;
  }

  return is_pressed;
};

struct Coordinates {
  int x;
  int y;
};

int mouse_pressed_coordinates(Coordinates &coordinates,
                              sf::RenderWindow &window) {
  coordinates.x = sf::Mouse::getPosition(window).x;
  coordinates.y = sf::Mouse::getPosition(window).y;
}

void get_pattern(std::vector<std::vector<int>> &curr_generation,
                 sf::RenderWindow &window) {

  Coordinates coordinates;

  draw_galaxy(window);
  draw_button(START_BUTTON_X, START_BUTTON_Y, "START", window);
  draw_button(NEXT_BUTTON_X, NEXT_BUTTON_Y, "CLEAR", window);
  window.display();

  sf::Event event;
  while (window.waitEvent(event) &&
         !button_is_pressed(START_BUTTON_X, START_BUTTON_Y, coordinates.x,
                            coordinates.y)) {
    switch (event.type) {
    case sf::Event::Closed: {
      window.close();
      break;
    }
    case sf::Event::MouseButtonPressed: {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        mouse_pressed_coordinates(coordinates, window);
        int cell_x = coordinates.x / CELL_SIZE;
        int cell_y = coordinates.y / CELL_SIZE;
        if (cell_x < FIELD_SIZE) {
          curr_generation[cell_y][cell_x] = 1;
          draw_life(curr_generation, window);
          window.display();
          life_to_console(curr_generation);
        }
      }
      break;
    }
    default:
      break;
    }
  }
}

void check_window(sf::RenderWindow &window) {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window.close();
  }
}

int main() {

  sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y),
                          "Conway's Game of Life");

  while (window.isOpen()) {
    window.clear();

    std::vector<std::vector<int>> curr_generation(
        FIELD_SIZE, std::vector<int>(FIELD_SIZE, 0));
    std::vector<std::vector<int>> prev_generation(
        FIELD_SIZE, std::vector<int>(FIELD_SIZE, 0));

    check_window(window);

    get_pattern(curr_generation, window);

    Coordinates coordinates;

    do {
      check_window(window);

      prev_generation = curr_generation;
      curr_generation = create_next_generation(curr_generation);

      life_to_console(curr_generation);

      window.clear();

      draw_galaxy(window);

      draw_button(START_BUTTON_X, START_BUTTON_Y, "START", window);
      draw_button(NEXT_BUTTON_X, NEXT_BUTTON_Y, "CLEAR", window);

      draw_life(curr_generation, window);

      window.display();

      coordinates.x = 0;
      coordinates.y = 0;

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        mouse_pressed_coordinates(coordinates, window);
      }

      usleep(200000);
    } while (!(curr_generation == prev_generation) && window.isOpen() &&
             !button_is_pressed(NEXT_BUTTON_X, NEXT_BUTTON_Y, coordinates.x,
                                coordinates.y));
    sleep(1);
    window.clear();
  }

  check_window(window);

  return 0;
}