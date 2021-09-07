#include "CGameController.h"
#include <iostream>

CGameController::CGameController(CGameModel* GameModel, CGameView* GameView):
    m_GameView(GameView),
    m_GameModel(GameModel)
    {
    }

CGameController::~CGameController() {
    m_GameView = nullptr;

    m_GameModel = nullptr;
}

void CGameController::Run() {
    sf::Event Event;

    while (m_GameView->GetWindow().isOpen()) {
        while (m_GameView->GetWindow().pollEvent(Event)) {
            HandleEvent(Event);
        }

        sf::Thread RenderingThread(&CGameView::Render, m_GameView);
        RenderingThread.launch();
    }
}

void CGameController::HandleEvent(const sf::Event& Event) {

    switch (Event.type) {
        case sf::Event::Closed:
            m_GameView->GetWindow().close();
            break;
        case sf::Event::KeyPressed:
            switch (Event.key.code) {
                case sf::Keyboard::Escape:
                    m_GameView->GetWindow().close();
                    break;
                case sf::Keyboard::Space:

                    break;

                case sf::Keyboard::S:
                    m_GameModel->SaveData();
                    break;
                case sf::Keyboard::L:
                    m_GameModel->LoadData();
                    break;
                default:
                    break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (Event.key.code) {
                case sf::Keyboard::Tab:
                    m_GameModel->ChangeObjType();
                    break;
                default: break;
            }
            break;
        case sf::Event::MouseButtonPressed: {
            int PosX = sf::Mouse::getPosition(m_GameView->GetWindow()).x;
            int PosY = sf::Mouse::getPosition(m_GameView->GetWindow()).y;

            QuadTree::QTree_ObjectData* QTree_Data;

            std::cout << m_GameModel->Root.ObjectsData.size() << " - size\n";

            if (m_GameModel->Obj_Type == Geometry_2D::EFIGURE_TYPE::ERECT) {
                // rect
                float size = 5.0f;
                Geometry_2D::SVector_2D TopLeft(PosX, PosY);
                Geometry_2D::SVector_2D BottomRight(TopLeft.X + size, TopLeft.Y + size);
                Geometry_2D::CRectangle* Rect = new Geometry_2D::CRectangle(TopLeft, BottomRight);
                Geometry_2D::CFigure* ptr = Rect;
                QTree_Data = new QuadTree::QTree_ObjectData(ptr, *Rect/*, QuadTree::EOBJECT_TYPE::ERECT*/);
            } else if (m_GameModel->Obj_Type == Geometry_2D::EFIGURE_TYPE::ECIRCLE){
                // circle
                float Radius = 5.0f;
                Geometry_2D::CCircle* Circle = new Geometry_2D::CCircle(Radius, Geometry_2D::SVector_2D(PosX, PosY));
                Geometry_2D::SVector_2D BottomRight = {Circle->GetCenter().X+Circle->GetRadius(), Circle->GetCenter().Y+Circle->GetRadius()};
                Geometry_2D::CRectangle bounds(Circle->GetCenter(), BottomRight);
                QTree_Data = new QuadTree::QTree_ObjectData(Circle, bounds/*, QuadTree::EOBJECT_TYPE::ECIRCLE*/);
            }

            if (QTree_Data) {
                m_GameModel->Root.Insert(*QTree_Data);
            }
        } break;
        case sf::Event::MouseButtonReleased: {
            if (m_GameView->bIsHandled) {
//                m_GameView->bIsHandled = false;
            }
        } break;
        default:
            break;
    }
}
