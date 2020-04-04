#pragma once
#include <EntityComponentSystem/IComponent.hpp>
#include <EntityComponentSystem/IEntity.hpp>
#include <EntityComponentSystem/ISystem.hpp>

namespace TestECS
{
	class MyComponent : public ECS::IComponent
	{
		public :
		MyComponent(int value) {}
	};

	class MySecondComponent : public ECS::IComponent
	{
		public :
	};

	class EntityOne : public ECS::IEntity
	{
		public:
			EntityOne(EntityID id) : IEntity(id){}
			EntityOne(EntityID id, const std::string& str) : IEntity(id), m_str(str)
			{
			}

			std::string GetStr() const
			{
				return m_str;
			}
		private:
			std::string m_str;
	};

	class EntityTwo : public ECS::IEntity
	{
		public:
			EntityTwo(EntityID id) : IEntity(id){}
			EntityTwo(EntityID id, int nb) : IEntity(id), m_nbr(nb)
			{

			}

			int GetNb() const
			{
				return m_nbr;
			}
		private:
			int m_nbr;
	};

	class SystemA : public ECS::ISystem
	{
		public:
			SystemA() : ISystem(){}
			~SystemA(){}

			virtual void FirstRun() override{ }
			virtual void Run() override{ }
			virtual void LastRun() override{ }
	};

	class SystemB : public ECS::ISystem
	{
		public:
			SystemB() : ISystem(){}
			~SystemB(){}

			virtual void FirstRun() override{ }
			virtual void Run() override{ }
			virtual void LastRun() override{ }
	};

	class SystemC : public ECS::ISystem
	{
		public:
			SystemC() : ISystem(){}
			~SystemC(){}

			virtual void FirstRun() override{ }
			virtual void Run() override{ }
			virtual void LastRun() override{ }
	};

	class SystemD : public ECS::ISystem
	{
		public:
			SystemD() : ISystem(){}
			~SystemD(){}

			virtual void FirstRun() override{ }
			virtual void Run() override{ }
			virtual void LastRun() override{ }
	};

	class SystemE : public ECS::ISystem
	{
		public:
			SystemE() : ISystem(){}
			~SystemE(){}

			virtual void FirstRun() override{ }
			virtual void Run() override{ }
			virtual void LastRun() override{ }
	};

	class SystemF : public ECS::ISystem
	{
		public:
			SystemF() : ISystem(){}
			~SystemF(){}

			virtual void FirstRun() override{ }
			virtual void Run() override{ }
			virtual void LastRun() override{ }
	};

	class SystemG : public ECS::ISystem
	{
		public:
			SystemG() : ISystem(){}
			~SystemG(){}

			virtual void FirstRun() override{ }
			virtual void Run() override{ }
			virtual void LastRun() override{ }
	};

	class SystemH : public ECS::ISystem
	{
		public:
			SystemH() : ISystem(){}
			~SystemH(){}

			virtual void FirstRun() override{ }
			virtual void Run() override{ }
			virtual void LastRun() override{ }
	};

	class SystemI : public ECS::ISystem
	{
		public:
			SystemI() : ISystem(){}
			~SystemI(){}

			virtual void FirstRun() override{ }
			virtual void Run() override{ }
			virtual void LastRun() override{ }
	};
}
