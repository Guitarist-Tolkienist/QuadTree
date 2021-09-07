#ifndef PLATFORMER_CGAMEVIEW_H
#define PLATFORMER_CGAMEVIEW_H

#include "SFML/Graphics.hpp"
#include "CGameModel.h"
#include "CAssets.h"

class CGameView : public sf::Drawable, public sf::Transformable {
    CGameModel* m_GameModel;

    sf::RenderWindow m_Window;
    sf::Text m_Text;

    sf::Clock clock1;

protected:
    float m_FPS;
    float m_DeltaTime = 0.0f;
    int m_Frames1 = 0;
    int m_Frames2 = 0;

    float sTime1 = 0.0f;
    float sTime2 = 0.0f;
public:
    // GameObjects
    bool bIsHandled = false;
    sf::RectangleShape Box;

    CGameView(CGameModel* Model);
    ~CGameView();

    sf::RenderWindow& GetWindow();
    bool Init();
    void Render();

private:
    void drawQuadTreeLeaf(const QuadTree::QuadTreeNode& Node, sf::RenderTarget &target, sf::RenderStates states) const;

    void drawNode(const QuadTree::QuadTreeNode& Node, sf::RenderTarget &target, sf::RenderStates states) const;
    void drawQuadTree(const QuadTree::QuadTreeNode &Node, sf::RenderTarget &target, sf::RenderStates states) const;

    // DRAW RECTANGLE BOUNDS OF A NODE
    void drawNodeBounds(const QuadTree::QuadTreeNode& Node, sf::RenderTarget &target, sf::RenderStates states) const;


    void UpdateTime();
    void drawFPS();

public:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


void drawRect(const sf::Vector2f& Pos, const sf::Vector2f& size, float OutLineThickness, const sf::Color& OutLineColor, sf::RenderTarget& target, sf::RenderStates states);
void drawCircle(const sf::Vector2f& Pos, float Radius, float OutLineThickness, const sf::Color& OutLineColor, sf::RenderTarget& target, sf::RenderStates states);

#endif //PLATFORMER_CGAMEVIEW_H
