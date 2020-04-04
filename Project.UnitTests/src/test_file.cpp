#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "EntityComponentSystem/ComponentManager.hpp"
#include "EntityComponentSystem/EntityManager.hpp"
#include "EntityComponentSystem/SystemManager.hpp"
#include "Event/EventManager.hpp"

#include "Classes.hpp"

using namespace TestECS;

BOOST_AUTO_TEST_CASE( test_EntityManager )
{
    BOOST_CHECK( 1 == 1 );

    ECS::EntityManager entityManager;
	EntityID my_entity = entityManager.CreateEntity<EntityOne>("coucou");
	EntityID my_second_entity = entityManager.CreateEntity<EntityTwo>(3);

    BOOST_CHECK(entityManager.HasEntity<EntityOne>(my_entity));
    BOOST_CHECK(entityManager.HasEntity<EntityTwo>(my_second_entity));

    auto ent = entityManager.GetEntity<EntityOne>(my_entity);
	BOOST_CHECK(ent != nullptr);
	BOOST_CHECK(!ent->GetStr().compare("coucou"));

	auto ent2 = entityManager.GetEntity<EntityTwo>(my_second_entity);
	BOOST_CHECK(ent2 != nullptr);

	auto ent3 = entityManager.GetEntity<EntityTwo>(my_entity);
	BOOST_CHECK(ent3 == nullptr);

    entityManager.RemoveEntity(my_entity);
	BOOST_CHECK(!entityManager.HasEntity<EntityOne>(my_entity));
	BOOST_CHECK(entityManager.HasEntity<EntityTwo>(my_second_entity));

	entityManager.RemoveEntity(my_second_entity);
	BOOST_CHECK(!entityManager.HasEntity<EntityOne>(my_entity));
	BOOST_CHECK(!entityManager.HasEntity<EntityTwo>(my_second_entity));
}

BOOST_AUTO_TEST_CASE( test_ComponentManager )
{
    ECS::EntityManager entityManager;
	EntityID my_entity = entityManager.CreateEntity<EntityOne>("coucou");
	EntityID my_second_entity = entityManager.CreateEntity<EntityTwo>(3);

    ECS::ComponentManager cmpManager;
	cmpManager.AddComponent<MyComponent>(my_entity, 10);
	cmpManager.AddComponent<MySecondComponent>(my_entity);

	Event::EventManager::AddListener(&entityManager);
	Event::EventManager::AddListener(&cmpManager);

	auto cmp = cmpManager.GetComponent<MyComponent>(my_entity);
	BOOST_CHECK(cmp != nullptr);

	auto cmp2 = cmpManager.GetComponent<MySecondComponent>(my_entity);
	BOOST_CHECK(cmp != nullptr);

	BOOST_CHECK(cmpManager.HasComponent<MyComponent>(my_entity));
	BOOST_CHECK(cmpManager.HasComponent<MySecondComponent>(my_entity));

	cmpManager.DestroyComponent<MySecondComponent>(my_entity);
	BOOST_CHECK(!cmpManager.HasComponent<MySecondComponent>(my_entity));
	BOOST_CHECK(cmpManager.HasComponent<MyComponent>(my_entity));

	cmpManager.DestroyComponent<MyComponent>(my_entity);
	BOOST_CHECK(!cmpManager.HasComponent<MySecondComponent>(my_entity));
	BOOST_CHECK(!cmpManager.HasComponent<MyComponent>(my_entity));
}


BOOST_AUTO_TEST_CASE(test_SystemManager_order)
{
    ECS::SystemManager systManager;

    systManager.AddSystem<SystemA>();
	systManager.AddSystem<SystemB>();
	systManager.AddSystem<SystemC>();
	systManager.AddSystem<SystemD>();
	systManager.AddSystem<SystemE>();
	systManager.AddSystem<SystemF>();
	systManager.AddSystem<SystemG>();
	systManager.AddSystem<SystemH>();
	systManager.AddSystem<SystemI>();

	systManager.AddDependency<SystemI, SystemB>();
	systManager.AddDependency<SystemI, SystemC>();
	systManager.AddDependency<SystemB, SystemF>();
	systManager.AddDependency<SystemC, SystemE>();
	systManager.AddDependency<SystemD, SystemG>();
	systManager.AddDependency<SystemE, SystemF>();
	systManager.AddDependency<SystemE, SystemG>();
	systManager.AddDependency<SystemH, SystemF>();
	systManager.AddDependency<SystemF, SystemA>();
	systManager.AddDependency<SystemG, SystemA>();

	systManager.ProcessOrder();

	std::list<ECS::ISystem*> syst = systManager.RunningList();

    BOOST_CHECK(syst.size() == 9);

    //TODO check order
}