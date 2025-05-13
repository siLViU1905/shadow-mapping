#ifndef __PHYSICSWORLD_H__
#define __PHYSICSWORLD_H__
#define GLM_ENABLE_EXPERIMENTAL
#include "../bullet/btBulletDynamicsCommon.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/gtx/quaternion.hpp"
#include "Model.h"

class PhysicsWorld
{
private:
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;

    static btVector3 glmToBullet(const glm::vec3& vec);

    static glm::vec3 bulletToGlm(const btVector3& vec);

    public:
    PhysicsWorld();

    btRigidBody* createBoxBody(
        Model& model,
        float mass,
        const btVector3& initPos = btVector3(0, 0, 0),
        const btQuaternion& initRot = btQuaternion(0,0,0,1)
    );

    btRigidBody* createSphereBody(
        Model& model,
        float mass,
        const btVector3& initPos = btVector3(0, 0, 0),
        const btQuaternion& initRot = btQuaternion(0,0,0,1)
    );
    
    void updateModelTransform(Model& model, const btRigidBody* body);

    void stepSimulation(float deltaTime);

    void addRigidBody(btRigidBody* body);

    void removeRigidBody(btRigidBody* body);

    btDiscreteDynamicsWorld* getDynamicsWorld();

    ~PhysicsWorld();
};

#endif // __PHYSICSWORLD_H__