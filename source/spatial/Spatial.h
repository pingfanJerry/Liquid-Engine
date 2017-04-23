#include "../common/Entity.h"

namespace liquid { namespace spatial {
#ifndef _SPATIAL_H
#define _SPATIAL_H

#define SPATIAL_UNKNOWN 0x00000

class Spatial
{
public:
    Spatial(int32_t spatialType);
    ~Spatial();

    virtual void initialise() = 0;
    virtual void dispose() = 0;
    virtual void update() = 0;

    virtual void insertEntity(common::Entity* entityPtr) = 0;
    virtual void removeEntity(common::Entity* entityPtr) = 0;

    virtual std::vector<common::Entity*> query(std::array<float, 4> region) = 0;
    virtual std::vector<common::Entity*> query(std::array<float, 4> region, int32_t type) = 0;

    const int32_t getSpatialType() const;

protected:
    int32_t mSpatialType; ///< Is a stored type required?
};

#endif // _SPATIAL_H
}}
