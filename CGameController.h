#ifndef PLATFORMER_CGAMECONTROLLER_H
#define PLATFORMER_CGAMECONTROLLER_H

#include "CGameView.h"

class CGameController {
    CGameModel* m_GameModel;
    CGameView* m_GameView;
public:
    CGameController(CGameModel*, CGameView*);
    ~CGameController();

    void Run();
    void HandleEvent(const sf::Event&);
};


#endif //PLATFORMER_CGAMECONTROLLER_H
