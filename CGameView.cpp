#include "CGameView.h"
#include <sstream>
#include <iostream>

CGameView::CGameView(CGameModel* Model) :
    m_GameModel(Model),
    Box(sf::Vector2f(250.0f, 250.0f)) {

    Box.setPosition(sf::Vector2f(250.0f, 250.0f));
    Box.setOutlineThickness(1.0f);
    Box.setOutlineColor(sf::Color::Red);
    Box.setFillColor(sf::Color::Transparent);

    Init();
}

CGameView::~CGameView() {
    m_GameModel = nullptr;
}

sf::RenderWindow& CGameView::GetWindow() {
    return m_Window;
}

bool CGameView::Init() {
    // todo window name
    m_Window.create(sf::VideoMode(m_GameModel->WINDOW_WIDTH, m_GameModel->WINDOW_HEIGHT), "Editor", sf::Style::Close);

    m_Text = sf::Text("Time: | FPS: ", CAssets::GetInstance().m_Font, 20);

    return true;
}

void CGameView::Render() {
    m_Window.clear();

    if (float(m_Window.getSize().x) != m_GameModel->WINDOW_WIDTH ||
        float(m_Window.getSize().y) != m_GameModel->WINDOW_HEIGHT) {
        const sf::VideoMode NewSize(uint(m_GameModel->WINDOW_WIDTH), uint(m_GameModel->WINDOW_HEIGHT));
        m_Window.create(NewSize, "Editor");
    }

    m_GameModel->Tick(m_DeltaTime);
    // todo change signature
    UpdateTime();
    drawFPS();


    if (bIsHandled) {
        float x = float(sf::Mouse::getPosition(GetWindow()).x);
        float y = float(sf::Mouse::getPosition(GetWindow()).y);

        Box.setPosition(x, y);
    }

    m_Window.draw(*this);

    m_Window.display();

    sf::sleep(sf::seconds(1.0/60));
}
void CGameView::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    /*// draw rectangles
    for (int i = 0; i < m_GameModel->Rects.size(); ++i) {
        sf::RectangleShape rect;
        sf::Vector2f size;
        size.x = m_GameModel->Rects[i].GetSize().X;
        size.y = m_GameModel->Rects[i].GetSize().Y;
        rect.setSize(size);

        float PosX = m_GameModel->Rects[i].TopLeft.X;
        float PosY = m_GameModel->Rects[i].TopLeft.Y;
        rect.setPosition(PosX, PosY);

        rect.setOutlineColor(sf::Color::Green);
        rect.setOutlineThickness(5.0);
        rect.setFillColor(sf::Color::Transparent);
        target.draw(rect, states);
    }

    // draw circles
    for (int i = 0; i < m_GameModel->Circles.size(); ++i) {
        sf::CircleShape circle;
        circle.setRadius(m_GameModel->Circles[i].GetRadius());

        float PosX = m_GameModel->Circles[i].GetCenter().X;
        float PosY = m_GameModel->Circles[i].GetCenter().Y;
        circle.setPosition(PosX, PosY);

        circle.setOutlineColor(sf::Color::Yellow);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(5.0);
        target.draw(circle, states);
    }
*/

    // draw QuadTree
    drawQuadTree(m_GameModel->Root, target, states);
}

void CGameView::drawQuadTree(const QuadTree::QuadTreeNode& Node, sf::RenderTarget& target, sf::RenderStates states) const {
    if (Node.IsLeaf()) {
        drawNode(Node, target, states);
    } else {
        for (int i = 0; i < Node.ChildrenNodes.size(); ++i) {
            drawQuadTree(Node.ChildrenNodes[i], target, states);
        }
    }
}

void CGameView::drawNode(const QuadTree::QuadTreeNode& Node, sf::RenderTarget &target, sf::RenderStates states) const{
    drawNodeBounds(Node, target, states);

    for (int i = 0; i < Node.ObjectsData.size(); ++i) {
        Geometry_2D::CFigure* FigurePtr = static_cast<Geometry_2D::CFigure *>(Node.ObjectsData[i]->Object);;
        switch (FigurePtr->Type) {
            case Geometry_2D::EFIGURE_TYPE::ERECT: {
                Geometry_2D::CRectangle* RectPtr = static_cast<Geometry_2D::CRectangle *>(Node.ObjectsData[i]->Object);

                sf::Vector2f Size;
                Geometry_2D::SVector_2D RectSize = RectPtr->GetSize();
                Size.x = RectSize.X;
                Size.y = RectSize.Y;

                sf::Vector2f Pos;
                Pos.x = RectPtr->TopLeft.X;
                Pos.y = RectPtr->TopLeft.Y;
                drawRect(Pos, Size, 2.5, sf::Color::Blue,target, states);
            } break;
            case Geometry_2D::EFIGURE_TYPE::ECIRCLE: {
                Geometry_2D::CCircle* CirclePtr = static_cast<Geometry_2D::CCircle *>(Node.ObjectsData[i]->Object);

                sf::Vector2f Pos;
                Geometry_2D::SVector_2D Center = CirclePtr->GetCenter();
                Pos.x = Center.X;
                Pos.y = Center.Y;
                drawCircle(Pos, CirclePtr->GetRadius(), 2.5, sf::Color::Yellow, target, states);
            } break;
            default: break;
        }
    }
}

// Draws a rectangle representing QuadTree bounds
void CGameView::drawNodeBounds(const QuadTree::QuadTreeNode& Node, sf::RenderTarget &target, sf::RenderStates states) const {
    int bound_offset = 1;

    sf::Vector2f size;
    size.x = Node.NodeBounds.GetSize().X - 2 * bound_offset;
    size.y = Node.NodeBounds.GetSize().Y - 2 * bound_offset;

    sf::Vector2f Pos;
    Pos.x = Node.NodeBounds.TopLeft.X + bound_offset;
    Pos.y = Node.NodeBounds.TopLeft.Y + bound_offset;

    sf::Color DepthColor;
    switch (Node.CurrentDepth) {
        case 1:
            DepthColor = sf::Color::Yellow;
            break;
        case 2:
            DepthColor = sf::Color::Blue;
            break;
        case 3:
            DepthColor = sf::Color::Magenta;
            break;
        case 4:
            DepthColor = sf::Color::Green;
            break;
        case 5:
            DepthColor = sf::Color::Red;
            break;
        default:
            DepthColor = sf::Color::White;
            break;
    }
    drawRect(Pos, size, bound_offset, DepthColor, target, states);
}

void drawRect(const sf::Vector2f& Pos, const sf::Vector2f& size, float OutLineThickness, const sf::Color& OutLineColor, sf::RenderTarget& target, sf::RenderStates states) {
    sf::RectangleShape rect;
    rect.setSize(size);
    rect.setPosition(Pos);

    rect.setOutlineThickness(float(OutLineThickness));
    rect.setOutlineColor(OutLineColor);
    rect.setFillColor(sf::Color::Transparent);

    target.draw(rect, states);
}

void drawCircle(const sf::Vector2f& Pos, float Radius, float OutLineThickness, const sf::Color& OutLineColor, sf::RenderTarget& target, sf::RenderStates states) {
    sf::CircleShape circle;

    circle.setRadius(Radius);
    circle.setPosition(Pos);

    circle.setOutlineColor(OutLineColor);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(OutLineThickness);
    target.draw(circle, states);
}


void CGameView::drawFPS() {
    std::ostringstream buff;
    buff << m_FPS;
    std::string s3(buff.str());

    m_Text = sf::Text(std::string("FPS: " + s3) , CAssets::GetInstance().m_Font, 20);

    m_Window.draw(m_Text);
}

void CGameView::UpdateTime() {
    m_Frames2++;

    m_DeltaTime = clock1.getElapsedTime().asSeconds() - sTime2;
    sTime2 = clock1.getElapsedTime().asSeconds();

    if (sTime2 - sTime1 > 1) {
        m_FPS = float(m_Frames2 - m_Frames1);

        m_Frames1 = m_Frames2;
        sTime1 = sTime2;
    }

}

