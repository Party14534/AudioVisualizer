#include "main.h"

int main() {
    sf::RenderWindow win(sf::VideoMode::getDesktopMode(), "Audio Visualizer", sf::Style::Default);

    struct AudioFile* a = getWav("../files/suck.wav");

    /*
    while (win.isOpen()) {
        // Event handling
        handleEvents(win);

        // Render handling
        handleRendering(win);
    }*/

    printf("Successfully parsed file\n");

    free(a);
    return 0;
}
