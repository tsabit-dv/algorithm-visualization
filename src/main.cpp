#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;
const int BAR_WIDTH = 5;
const int NUM_BARS = WIDTH / BAR_WIDTH;

// Fungsi untuk menggambar array ke layar
void drawBars(sf::RenderWindow& window, const std::vector<int>& arr, int highlight1 = -1, int highlight2 = -1) {
    window.clear();
    for (size_t i = 0; i < arr.size(); ++i) {
        sf::RectangleShape bar(sf::Vector2f(static_cast<float>(BAR_WIDTH - 1), static_cast<float>(arr[i])));
        bar.setPosition(sf::Vector2f(static_cast<float>(i * BAR_WIDTH), static_cast<float>(HEIGHT - arr[i])));
        
        // Warnai batang yang sedang dibandingkan
        if (i == highlight1 || i == highlight2) {
            bar.setFillColor(sf::Color::Red);
        } else {
            bar.setFillColor(sf::Color::White);
        }

        window.draw(bar);
    }
    window.display();
}

// Algoritma Bubble Sort dengan animasi dan suara
void bubbleSort(std::vector<int>& arr, sf::RenderWindow& window, sf::Sound& beep) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        for (size_t j = 0; j < arr.size() - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                beep.play(); // Mainkan suara saat swap
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Delay agar suara terdengar
            }

            drawBars(window, arr, j, j + 1);
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Sorting Visualization", sf::Style::Close);
    window.setFramerateLimit(60);

    std::vector<int> bars(NUM_BARS);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Inisialisasi array dengan bilangan acak
    for (int i = 0; i < NUM_BARS; ++i) {
        bars[i] = std::rand() % HEIGHT;
    }

    // Inisialisasi suara
    sf::SoundBuffer buffer;
    sf::Sound beep;
    
    if (!buffer.loadFromFile("src/beep.wav")) {
        std::cerr << "Error loading sound file!" << std::endl;
        return -1;
    }
    beep.setBuffer(buffer);

    bool sorted = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Jalankan sorting hanya sekali
        if (!sorted) {
            bubbleSort(bars, window, beep);
            sorted = true;
        }

        drawBars(window, bars);
    }

    return 0;
}
