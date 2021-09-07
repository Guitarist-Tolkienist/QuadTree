#include "CGameModel.h"
#include <fstream>
#include <iostream>
#include "random"



namespace QuadTree {
    using namespace Geometry_2D;
    bool QuadTreeNode::IsLeaf() const {
        return ChildrenNodes.size() == 0 && !IsDivided;
    }

    bool QuadTreeNode::Insert(QTree_ObjectData& Data) {
        std::cout << "| Insert |\n";
        std::cout << "Node depth is " << CurrentDepth << "\n";
        // CHECKS WHETHER THE OBJECT TO BE INSERTED IS WITHIN THE NODE BOUNDS
        if (!Collision::RectangleRectangle(NodeBounds, Data.ObjectBounds)) {
            std::cout << "this object is out of bounds\n";
            return false;
        };

        if (IsLeaf() && ObjectsData.size() + 1 > MAX_OBJECTS_PER_NODE) {
            Split();
        }

        if (IsLeaf()) { // THE NODE IS NOT DIVIDED
            std::cout << "THE NODE IS NOT DIVIDED\n";
            ObjectsData.push_back(&Data);
            std::cout << "Inserted into this root node\n";
        } else { // THE NODE IS DIVIDED
            std::cout << "THE NODE IS DIVIDED\n";
            for (int i = 0, size = ChildrenNodes.size(); i < size; ++i) {
                if (ChildrenNodes[i].Insert(Data)) {
                    std::cout << "Inserted into " << i + 1 << " node\n";
                    break;
                };
            }
        }

        return true;
    }

    void QuadTreeNode::Split() {
        std::cout << "| SPLIT |\n";

        // CANNOT SPLIT ANY FURTHER
        if(CurrentDepth + 1 >= MAX_TREE_DEPTH) return;

        SVector_2D min = NodeBounds.TopLeft;
        SVector_2D max = NodeBounds.BottomRight;
        SVector_2D center = min + ((max - min) * 0.5f);

        // CREATE BOUNDS (RECTS, AREAS) FOR CHILDREN NODES
        CRectangle DividedAreas[] = {
                CRectangle(
                        FromMinMax(
                                SVector_2D(min.X, min.Y),
                                SVector_2D(center.X, center.Y))),
                CRectangle(
                        FromMinMax(
                                SVector_2D(center.X, min.Y),
                                SVector_2D(max.X, center.Y))),
                CRectangle(
                        FromMinMax(
                                SVector_2D(center.X, center.Y),
                                SVector_2D(max.X, max.Y))),
                CRectangle(
                        FromMinMax(
                                SVector_2D(min.X, center.Y),
                                SVector_2D(center.X, max.Y))),
        };


        // CREATE CHILDREN NODES
        for (int i = 0; i < 4; ++i) {
            ChildrenNodes.push_back(QuadTreeNode(DividedAreas[i]));
            ChildrenNodes[i].CurrentDepth = CurrentDepth + 1;
        }

        for (int i = 0; i < ObjectsData.size(); ++i) {
            for (int j = 0; j < ChildrenNodes.size(); ++j) {
                std::cout << "#" << i + 1 << " put into " << j + 1 << " node\n";
                if (ChildrenNodes[j].Insert(*ObjectsData[i])) break;
            }
        }

        ObjectsData.clear();
        IsDivided = true;
        std::cout << "Splitting ended\n";
    }

    std::vector<QTree_ObjectData*> QuadTreeNode::Query(const Geometry_2D::CRectangle& area) const {
        std::vector<QTree_ObjectData *> result;
        if (!Collision::RectangleRectangle(area, NodeBounds)) return result;

        if (IsLeaf()) {
            for (int i = 0, size = ObjectsData.size(); i < size; ++i) {
                if (Collision::RectangleRectangle(ObjectsData[i]->ObjectBounds, area)) {
                    result.push_back(ObjectsData[i]);
                }
            }
        } else {
            for (int i = 0, size = ChildrenNodes.size(); i < size; ++i) {
                std::vector<QTree_ObjectData *> recurse = ChildrenNodes[i].Query(area);
                if (recurse.size() > 0) {
                    result.insert(result.end(), recurse.begin(), recurse.end());
                }
            }
        }
        return result;
    }

    QuadTreeNode::~QuadTreeNode() {

    }
}

using namespace QuadTree;
using namespace Geometry_2D;
using namespace Math;

const std::string FILE_TO_READ_BIN = "file.bin";

CGameModel::CGameModel() : Root(Geometry_2D::CRectangle(Geometry_2D::SVector_2D(0.0f, 0.0f),
                                                Geometry_2D::SVector_2D(WINDOW_WIDTH, WINDOW_HEIGHT))) {}
CGameModel::CGameModel(const float& W,
                       const float& H,
                       int RectsNum,
                       int CirclesNum) : WINDOW_WIDTH(W),
                                         WINDOW_HEIGHT(H),
                                         Root(Geometry_2D::CRectangle(Geometry_2D::SVector_2D(0.0f, 0.0f),
                                                                             Geometry_2D::SVector_2D(W, H))) {
    srand(time(0));

    // instantiate rectangles
    for (int i = 0; i < RectsNum; ++i) {
        float PosX = float(0.0f + rand() % int(WINDOW_WIDTH));
        float PosY = float(0.0f + rand() % int(WINDOW_HEIGHT));
        int size = 5;

        SVector_2D TopLeft(PosX, PosY);
        SVector_2D BottomRight(TopLeft.X + size, TopLeft.Y + size);

        CRectangle* Rect = new CRectangle(TopLeft, BottomRight);
        CFigure* ptr = Rect;
        QTree_ObjectData* QTree_Data = new QTree_ObjectData(ptr, *Rect/*, QuadTree::EOBJECT_TYPE::ERECT*/);

//        std::cout << "rect" << i+1 << ": size - " << Rect.GetSize().X << ", " << Rect.GetSize().Y;
//        std::cout << " | pos - " << Rect.TopLeft.X << ", " << Rect.TopLeft.Y << "\n";

        Root.Insert(*QTree_Data);
    }

    // instantiate circles
    for (int i = 0; i < CirclesNum; ++i) {
        float PosX = float(0.0f + rand() % int(WINDOW_WIDTH));
        float PosY = float(0.0f + rand() % int(WINDOW_HEIGHT));
        float Radius = 5.0f;

        CCircle* Circle = new CCircle(Radius, Geometry_2D::SVector_2D(PosX, PosY));
        SVector_2D BottomRight = {Circle->GetCenter().X+Circle->GetRadius(), Circle->GetCenter().Y+Circle->GetRadius()};
        CRectangle bounds(Circle->GetCenter(), BottomRight);

        CFigure* ptr = Circle;
        QTree_ObjectData* QTree_Data = new QTree_ObjectData(ptr, bounds/*, QuadTree::EOBJECT_TYPE::ECIRCLE*/);

//        std::cout << "circle" << i+1 << ": radius - " << Circle->GetRadius();
//        std::cout << " | pos - " << Circle->GetCenter().X << ", " << Circle->GetCenter().Y << "\n";

        Root.Insert(*QTree_Data);
    }
}

CGameModel::~CGameModel() {

}

void CGameModel::Tick(float DeltaTime) {

}

void CGameModel::SaveData() {
    std::fstream File;
    File.open(FILE_TO_READ_BIN, std::ios::binary | std::ios::out);
    if(!File) {
        std::cerr<<"File error " << FILE_TO_READ_BIN<<">\n";
        exit(1);
    }

    File.write( (char*)&WINDOW_WIDTH, sizeof(WINDOW_WIDTH));
    File.write( (char*)&WINDOW_HEIGHT, sizeof(WINDOW_HEIGHT));

    File.close();
    std::cout << "<<saved>>\n";
}

void CGameModel::LoadData() {
    std::fstream File;
    File.open(FILE_TO_READ_BIN, std::ios::binary | std::ios::in);
    if(!File) {
        std::cerr<<"File error " << FILE_TO_READ_BIN<<">\n";
        exit(1);
    }

    File.read( (char*)&WINDOW_WIDTH, sizeof(WINDOW_WIDTH));
    File.read( (char*)&WINDOW_HEIGHT, sizeof(WINDOW_HEIGHT));

    File.close();
    std::cout << "<<loaded>>\n";
}

void CGameModel::ChangeObjType() {
    if (Obj_Type == EFIGURE_TYPE::ERECT) {
        Obj_Type = EFIGURE_TYPE::ECIRCLE;
    } else if (Obj_Type == EFIGURE_TYPE::ECIRCLE) {
        Obj_Type = EFIGURE_TYPE::ERECT;
    }
}

