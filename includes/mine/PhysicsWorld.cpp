#include "PhysicsWorld.h"



PhysicsWorld::PhysicsWorld()
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0, -9.81, 0));
}

PhysicsWorld::~PhysicsWorld()
{
    delete dynamicsWorld;
    delete solver;
    delete broadphase;
    delete dispatcher;
    delete collisionConfiguration;
}

btRigidBody* PhysicsWorld::createSphereBody(
    Model& model,
    float mass,
    const btVector3& initPos,
    const btQuaternion& initRot
)
{
    btVector3 halfExtents = btVector3(model.scale.x / 2.f, model.scale.y / 2.f, model.scale.z / 2.f);

    btCollisionShape* shape = new btSphereShape(halfExtents.length());

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(initPos);
    transform.setRotation(initRot);

    btVector3 localInertia(0, 0, 0);
    if (mass != 0.f)
     shape->calculateLocalInertia(mass, localInertia);

   btDefaultMotionState* motionState = new btDefaultMotionState(transform);

   btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);

   btRigidBody* body = new btRigidBody(rbInfo);

   dynamicsWorld->addRigidBody(body);

   return body;
}



