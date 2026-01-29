#include "main.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>

int main() {
    float framerate = 60.f;
    sf::RenderWindow win(sf::VideoMode::getDesktopMode(), "Audio Visualizer", sf::Style::Default);
    win.setFramerateLimit(framerate);

    printf("Gonna get my wav\n");
    struct AudioFile* a = getWav("../files/suck.wav");
    printf("Gonna build my input\n");
    a->buildInput();
    printf("Built input\n");
    sf::Sound sound(a->buffer);
    sound.play();

    float barWidth = 50.f;
    std::vector<sf::RectangleShape> bars(a->band_edges.size());
    for(int i = 0; i < bars.size(); i++) {
        auto& bar = bars[i];
        float midX = float(win.getSize().x) * 0.5f;
        float xPos = midX + (i - (bars.size() / 2)) * 50.f;

        bar.setSize({barWidth, 0.f});
        bar.setOrigin({25.f, 0.f});
        bar.setFillColor(sf::Color::Green);
        bar.setPosition({xPos, (float)win.getSize().y});
    }
    printf("Created bars\n");

    const i16* buff = a->buffer.getSamples();

    float timeOffset = 1.f / framerate;

    while (win.isOpen()) {
        // Event handling
        handleEvents(win);

        float t = sound.getPlayingOffset().asSeconds();
        int startIndex = t * a->Frequency;
        int endIndex = (t+timeOffset) * a->Frequency;

        std::vector<i16> frameData(
                a->fft_input.cbegin() + startIndex,
                a->fft_input.cbegin() + endIndex);
        printf("Got framedata\n");
        std::vector<float> barData = a->processFrame(frameData);
        printf("Got bar data\n");

        for (int i = 0; i < barData.size(); i++) {
            // Normalize dB values to 0-20 range for display
            float normalized = (barData[i] + 80.0f) / 4.0f;  // Assuming -80 to 0 dB range
            bars[i].setSize({barWidth, normalized * 70.f});
        }

        // Render handling
        win.clear(sf::Color(230,215,255));

        for (auto& bar : bars) { win.draw(bar); }
        
        win.display();
    }

    printf("Successfully parsed file\n");

    free(a);
    return 0;
}
