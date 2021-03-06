extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <LuaBridge/LuaBridge.h>
#include <string>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <array>
#include "../utilities/Vertex2.h"
#include "../ai/Agent.h"

namespace liquid { namespace common {
#ifndef _ENTITY_H
#define _ENTITY_H

/**
 * \class Entity
 *
 * \ingroup Common
 * \brief Structures the idea of something in the world, not necassarily visible
 *
 * \author Jamie Massey
 * \version 2.0
 * \date 03/04/2017
 *
 */

#define ENTITYTYPE_UNKNOWN 0x0000

class GameScene;
class Entity
{
public:
    /// State of the Entity as an enumerator
    enum eEntityState
    {
        ENTITYSTATE_ACTIVE = 0,
        ENTITYSTATE_SLEEP = 1,
        ENTITYSTATE_DEAD = 2,
    };

    enum ePrimitiveType
    {
        PRIMITIVETYPE_QUAD = 0,
        PRIMITIVETYPE_LINES = 1,
    };

public:
    /// Entity Constructor
    Entity();

    /// Entity Destructor
    ~Entity();

    /** \brief Initialised on running of this Entity
      *
      * Called when the Entity is officially created, this is usually
      * in the GameScene class. It is useful for initialising any useful
      * variable of this class, or any class that extends it, that are intented
      * to be made or used at creation of the Entity and not the allocation of it.
      */
    virtual void initialise();

    /** \brief Update step before actual Updating
      *
      * The pre-step to truly updating the Entity; this function is useful to
      * setup any pre-requisite variables to the updating of this Entity or
      * resetting any variables that were consumed in the previous game step.
      */
    virtual void updatePre();

    /** \brief Update every frame
      *
      * Intended to be called when the game updates, it is useful for moving
      * the Entity or computing something that is important to its running. Usually
      * called around every 1/60th of a second.
      *
      * If you need to get the delta time to compute something without relying on
      * the steps of frames look at the DeltaTime class in Utilities.
      */
    virtual void update();

    /** \brief Update step after actual Updating
      *
      * The Update step after actual updating. It can be used to reset any variables
      * that are useful in the next update step.
      */
    virtual void updatePost();

    /** \brief Sets the position of this Entity in 2D space
      * \param x X-Coordinate to be set
      * \param y Y-Coordinate to be set
      *
      * This function will set a new position for the Entity, additionally it will
      * also update the position of any children it has.
      */
    virtual void setPosition(float x, float y);

    /** \brief Adds to the position of this Entity in 2D space
      * \param x X-Coordinate to be added
      * \param y Y-Coordinate to be added
      *
      * This function will add to the current position of the Entity, additionally it will
      * also update the position of any children it has.
      */
    virtual void addPosition(float x, float y);

    /** \brief Sets the new size of the Entity
      * \param w New width of the Entity
      * \param h New height of the Entity
      */
    virtual void setSize(float w, float h);

    virtual void setTexCoords(float x, float y, float w, float h);

    /** \brief Computes if a point is inside of this Entity
      * \param x X-Coordinate to be tested
      * \param y Y-Coordinate to be tested
      * \return True if the point was inside of the Entity, False otherwise
      *
      * Tests for a point that could potentially be inside of the Entity, if it
      * is then the function will return true (simple point to AABB test).
      */
    virtual bool isPointInside(float x, float y) const;

    /// Sets m_State to eEntityState::ENTITYSTATE_SLEEP
    void sleep();

    /// Sets m_State to eEntityState::ENTITYSTATE_ACTIVE
    void wake();

    /// Sets m_State to eEntityState::ENTITYSTATE_DEAD and call m_FuncCallbackKilled
    void kill();

    /** \brief Sets and loads a Lua script, passing to relative LuaRef objects
      * \param Path to LuaScript with name as a std::string
      */
    virtual void setLuaScript(std::string luaScript);

    /** \brief Sets the Parent GameScene of this Entity
      * \param scene The parent scene to be set
      * \param remove Flag to denote if the Entity should be removed from the current scene
      *
      * Sets the Parent GameScene for this Entity, it can also remove this Entity
      * from its current Parent Scene if it has one by using the supplied remove
      * flag.
      */
    void setParentGameScene(GameScene* scene, bool remove = true);

    /** \brief Sets the Parent of this Entity
      * \param entity Entity to be set as this Entities Parent
      *
      * Sets the Parent Entity for this Entity to the one given, if the given Entity
      * is a nullptr then the function will instead remove this Entity as a child from
      * the current Parent Entity and set its Parent Entity to null, essentially resetting
      * itself.
      */
    void setParentEntity(Entity* entity);

    /// \brief Creates an AI Agent (ai::Agent) for this Entity
    void createAIAgent();

    /** \brief Sets m_Type to the given type
      * \param type The type represented as a 32-bit integers
      */
    void setEntityType(int32_t type);

    /** \brief Sets m_UniqueID to the given string ID
      * \param uid The unique identifier
      *
      * Sets the UniqueID of this Entity, when given the identifier will be tested
      * by the IDGenerator utility to make sure that it is not a duplicate, if it is
      * the generator will make sure it isn't by appending a number (this may be the
      * behaviour you want).
      */
    void setEntityUID(std::string uid);

    /** \brief Adds a new Vertex pointer for rendering
      * \param vertex Vertex2 to add
      */
    void addVertex2(utilities::Vertex2* vertex);

    void removeVertex2(uint32_t index);

    /** \brief Gets the type of the Entity as 32-bit integer
      * \return Type of the Entity as a 32-bit integer, default: ENTITYTYPE_UNKNOWN/0x0000
      */
    int32_t getEntityType() const;

    /** \brief Gets the state of the Entity as an enumerator
      * \return State of the Entity: active, sleep or dead
      */
    eEntityState getEntityState() const;

    /** \brief Gets the X-Coordinate of the Entity in 2D space
      * \return X-Coordinate of the Entity, default: 0.0f
      */
    const float getPositionX() const;

    /** \brief Gets the Y-Coordinate of the Entity in 2D space
      * \return Y-Coordinate of the Entity, default: 0.0f
      */
    const float getPositionY() const;

    const float getOriginX() const;
    const float getOriginY() const;

    /** \brief Gets the unique string identifier of this Entity
      * \return Unique ID of the Entity, empty string if nothing
      */
    std::string getEntityUID() const;

    /** \brief Gets the children of this Entity
      * \return Children of the Entity as a std::vector
      */
    std::vector<Entity*> getChildren() const;

    /** \brief Gets the Parent of this Entity
      * \return Parent of this Entity, nullptr if there isn't one
      */
    Entity* getParentEntity() const;

    /** \brief Gets the Parent GameScene of this Entity
      * \return Parent GameScene, nullptr if there isn't one
      */
    GameScene* getParentGameScene() const;

    /** \brief Gets the ai::Agent linked with this Entity
      * \return The ai::Agent, nullptr if none created
      */
    ai::Agent* getAIAgent() const;

    /** \brief Gets the stored Vertex2* objects
      * \return Array of the Vertex objects
      */
    virtual std::vector<utilities::Vertex2*> getVertices();

    /// \return Number of Vertices in this Entity
    const uint32_t getVerticesCount() const;

    void clearVertices();

protected:
    /** \brief Adds a child to this Entity
      * \param child The child Entity to add
      *
      * Adds a child to this Entity, this can only be done indirectly by the
      * setParentEntity function.
      *
      * Note: If you want to remove any children or subsequent children of that
      * Entity you must search the mChildren vector and go from bottom up pruning
      * the tree as you intend, using setParentEntity(nullptr).
      */
    void addChild(Entity* child);

    /** \brief Removes a child from this Entity
      * \param child The child Entity to remove
      *
      * Adds a child to this Entity, this can only be done indirectly by the
      * setParentEntity function
      *
      * Note: If you want to remove any children or subsequent children of that
      * Entity you must search the mChildren vector and go from bottom up pruning
      * the tree as you intend, using setParentEntity(nullptr).
      */
    void removeChild(Entity* child);

public:
    std::string mTextureName;
    // TODO: HIDE THIS
    int32_t mAtlasID;
    int32_t mShaderID;
    int32_t mBlendMode;
    int32_t mPrimitiveType;

public:
    std::function<void(Entity*)>      mFuncCallbackUpdate;      ///< Function callback for when Entity is updated
    std::function<void(float, float)> mFuncCallbackSetPosition; ///< Function callback for when position is set
    std::function<void(float, float)> mFuncCallbackAddPosition; ///< Function callback for when position is added
    std::function<void()>             mFuncCallbackKilled;      ///< Function callback for when this Entity is destroyed
    
protected:
    std::string       mLuaScript;
    luabridge::LuaRef mLuaFuncCreate;
    luabridge::LuaRef mLuaFuncUpdate;
    luabridge::LuaRef mLuaFuncKill;

protected:
    int32_t              mType;            ///< Numerical representation of the entity type
    eEntityState         mState;           ///< The current state of the entity: active, sleep or dead
    float                mPositionX;       ///< X-Coordinate of the Entity in 2D space
    float                mPositionY;       ///< Y-Coordinate of the Entity in 2D space
    float                mOriginX;         ///< Origin on X-Axis relative to position (0-1)
    float                mOriginY;         ///< Origin on Y-Axis relative to position (0-1)
    float                mWidth;           ///< Width of the Entity in 2D space
    float                mHeight;          ///< Height of the Entity in 2D space
    std::string          mUniqueID;        ///< Unique identifier of the Entity
    std::vector<Entity*> mChildren;        ///< Collection of Entity ptrs that represent the children
    std::list<int32_t>   mFrameEvents;     ///< Collection of integers that denotes what has happened
    Entity*              mParentEntity;    ///< Pointer to the parent entity of this entity
    GameScene*           mParentGameScene; ///< Pointer to the parent scene of this entity
    ai::Agent*           mAIAgent;         ///< AI Agent that is linked with this Entity
    
protected:
    std::vector<utilities::Vertex2*> mVertices;
    uint32_t                         mVerticesCount;
};

#endif // _ENTITY_H
}}
