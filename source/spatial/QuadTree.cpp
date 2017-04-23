#include "QuadTree.h"

namespace liquid {
namespace spatial {

    QuadTree::QuadTree(int32_t capacity, std::array<float, 2> centre, std::array<float, 2> size) :
        Spatial(0x00001)
    {
        mRootNode = new QuadNode(capacity, true);
        mRootNode->setCentre(centre);
        mRootNode->setSize(size);
        mRootNode->setParentQuadTree(this);
    }

    QuadTree::~QuadTree()
    {}

    void QuadTree::initialise()
    {}
    void QuadTree::dispose()
    {}
    void QuadTree::update()
    {}

    void QuadTree::insertEntity(common::Entity* entityPtr)
    {
        if (mRootNode->insertEntity(entityPtr))
            mNumEntities++;
    }

    void QuadTree::removeEntity(common::Entity* entityPtr)
    {
        if (mRootNode->removeEntity(entityPtr))
            mNumEntities--;
    }

    std::vector<common::Entity*> QuadTree::query(std::array<float, 4> region)
    {
        return depthSearch(mRootNode, region);
    }

    std::vector<common::Entity*> QuadTree::query(std::array<float, 4> region, int32_t type)
    {
        return depthSearch(mRootNode, region);;
    }

    std::vector<common::Entity*> QuadTree::depthSearch(QuadNode* node, std::array<float, 4> region)
    {
        std::vector<common::Entity*> entities;
        std::array<float, 2> centre = node->getCentre();
        std::array<float, 2> size = node->getSize();

        if (centre[0] - (size[0] / 2.0f) >= region[0] && centre[0] + (size[0] / 2.0f) <= region[0] + region[2] &&
            centre[1] - (size[1] / 2.0f) >= region[1] && centre[1] + (size[1] / 2.0f) <= region[1] + region[3])
        {
            std::vector<common::Entity*> nodeEntities = node->getEntities();
            entities.insert(entities.begin(), nodeEntities.begin(), nodeEntities.end());
        }

        std::array<QuadNode*, 4> childNodes = node->getChildNodes();
        for (uint32_t i = 0; i < 4; i++)
        {
            if (childNodes[i] != nullptr)
            {
                std::vector<common::Entity*> srch = depthSearch(node->getChildNodes()[i], region);
                entities.insert(entities.end(), srch.begin(), srch.end());
            }
        }

        return entities;
    }

}}
