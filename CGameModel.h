#ifndef PLATFORMER_CGAMEMODEL_H
#define PLATFORMER_CGAMEMODEL_H

#include <chrono>
#include "MATH.h"
#include "QuadTree.h"
#include <array>
#include "CUpdateable.h"


namespace QuadTree {
    using namespace Geometry_2D;
    using Collision::FromMinMax;

    const int MAX_OBJECTS_PER_NODE = 5;
    const int MAX_TREE_DEPTH = 5;


    struct QTree_ObjectData {
        CRectangle ObjectBounds;
        CFigure* Object;
//        EOBJECT_TYPE Type;

        QTree_ObjectData(CFigure* obj, const CRectangle& bounds/*, EOBJECT_TYPE etype*/) :
                Object(obj),
                ObjectBounds(bounds)/*,
                Type(etype) */{}
    };

    class QuadTreeNode {
    public:
        CRectangle NodeBounds;
        std::vector<QTree_ObjectData*> ObjectsData;
        std::vector<QuadTreeNode> ChildrenNodes; // provided that is divided
        int CurrentDepth;

        bool IsDivided;

        QuadTreeNode(const CRectangle& Bounds) : NodeBounds(Bounds), CurrentDepth(0), IsDivided(false) {}
        ~QuadTreeNode();

        bool IsLeaf() const;

        bool Insert(QTree_ObjectData& Data);

        void Split();

        std::vector<QTree_ObjectData*>Query(const CRectangle& area) const;
    };
};


class CGameView;
class CGameController;

class CGameModel : public IUpdatable {
    Geometry_2D::EFIGURE_TYPE Obj_Type = Geometry_2D::EFIGURE_TYPE::ERECT;
public:
    const float WINDOW_WIDTH = 1200.0f;
    const float WINDOW_HEIGHT = 650.0f;

    QuadTree::QuadTreeNode Root;

    CGameModel();
    CGameModel(const float& W, const float& H, int RectsNum, int CirclesNum);
    ~CGameModel();

    virtual void Tick(float DeltaTime) override;

    void ChangeObjType();

    void SaveData();
    void LoadData();

    friend class CGameView;
    friend class CGameController;
};

#endif //PLATFORMER_CGAMEMODEL_H
