#include "main.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <cstdlib>

int main() {
    sf::RenderWindow win(sf::VideoMode::getDesktopMode(), "Audio Visualizer", sf::Style::Default);

    struct AudioFile* a = getWav("../files/suck.wav");
    sf::Sound sound(a->buffer);
    sound.play();

    sf::RectangleShape bar;
    bar.setSize({50.f, 0.f});
    bar.setOrigin({25.f, 0.f});
    bar.setFillColor(sf::Color::Green);
    bar.setPosition({(float)win.getSize().x * 0.5f, (float)win.getSize().y});

    const i16* buff = a->buffer.getSamples();

    while (win.isOpen()) {
        // Event handling
        handleEvents(win);

        float t = sound.getPlayingOffset().asSeconds();
        int index = t * a->channelCount * a->Frequency;
        float height = -(float(abs(buff[index])) / float(INT16_MAX)) * 500.f;
        bar.setSize({50.f, height});

        // Render handling
        win.clear(sf::Color(230,215,255));

        win.draw(bar);
        
        win.display();
    }

    printf("Successfully parsed file\n");

    free(a);
    return 0;
}
