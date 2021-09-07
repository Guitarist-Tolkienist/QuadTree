#include "CGameModel.h"
#include "CGameView.h"
#include "CGameController.h"
#include <iostream>

#include "GJK.h"

int main() {
    float LEVEL_WIDTH(0.0f);
    float LEVEL_HEIGHT(0.0f);
    int RectsQuantity(0);
    int CircleQuantity(0);

    std::cout << "Please enter a width of the scene:";
    std::cin >> LEVEL_WIDTH;

    std::cout << "Please enter a height of the scene:";
    std::cin >> LEVEL_HEIGHT;

    std::cout << "Please enter number of rectangles on the scene:";
    std::cin >> RectsQuantity;

    std::cout << "Please enter number of circles on the scene:";
    std::cin >> CircleQuantity;

    std::cout << "--------------\n";

    const float W = LEVEL_WIDTH;
    const float H = LEVEL_HEIGHT;

    CGameModel Model(W, H, RectsQuantity, CircleQuantity);
    CGameView View(&Model);
    CGameController Controller(&Model, &View);

    Controller.Run();
    return 0;
}