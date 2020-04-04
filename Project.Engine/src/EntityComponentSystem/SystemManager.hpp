#pragma once
#include <map>
#include <typeinfo>
#include <vector>
#include <list>
#include <iostream>
#include "ISystem.hpp"
#include "Event/IEventListener.hpp"

namespace ECS
{
    /**
     * @brief Manages the systems of the game.
     * 
     */
    class SystemManager : public Event::IEventListener
    {
        public:
            SystemManager();
            ~SystemManager();

            /**
             * @brief Checks if the the System C exists.
             * 
             * @tparam C : The type of system to check.
             * @return true if the system C exists.
             * @return false otherwise.
             */
            template<class C>
            bool HasSystem()
            {
                return HasSystem(&typeid(C));
            }

            /**
             * @brief Gets the system of type C.
             * 
             * @tparam C : The system type to retreive.
             * @return C* if the system exists, nullptr otherwise.
             */
            template<class C>
            C* GetSystem()
            {
                if(!HasSystem<C>())
                    return nullptr;
                else
                    return static_cast<C*>(m_systems[&typeid(C)]);
            }

            /**
             * @brief Adds a system of type C (initialized by args) to the manager.
             * If a system of type C already exists, does nothing.
             * 
             * @tparam C : The system type to be added.
             * @tparam args : The arguments passed to the constructor of C.
             */
            template<class C, typename ... args>
            void AddSystem(args&&... a)
            {
                if(!std::is_base_of<ISystem, C>())
                {
                    std::cerr << "Class " << typeid(C).name() << " is not derived of " << typeid(ISystem).name() << "\n";
                    return;
                }

                if(HasSystem<C>())
                {
                    std::cerr << "Only one instance of system " << typeid(C).name() << " is allowed.\n";
                    return;
                }

                m_systems[&typeid(C)] = new C(a...);
            }

            /**
             * @brief Adds a system dependency between C_Start and C_End
             * 
             * @tparam C_Start : The dependency parent.
             * @tparam C_End : The dependency child.
             */
            template<class C_Start, class C_End>
            void AddDependency()
            {
                if(!HasSystem<C_Start>())
                {
                    std::cerr << "A system of type " << typeid(C_Start).name() << " must be present.\n";
                    return;
                }

                if(!HasSystem<C_End>())
                {
                    std::cerr << "A system of type " << typeid(C_End).name() << " must be present.\n";
                    return;
                }

                ISystem* start = GetSystem<C_Start>();
                ISystem* end = GetSystem<C_End>();
                if(start == nullptr)
                {
                    std::cerr << "System of type " << typeid(C_Start).name() << " is null.\n";
                    return;
                }
                if(end == nullptr)
                {
                    std::cerr << "System of type " << typeid(C_End).name() << " is null.\n";
                    return;
                }

                Edge *edge = new Edge(start, end);
                m_edges.push_back(edge);
            }

            /**
             * @brief Runs a topological sorting algorithm (deep-first) 
             * to get the order of execution of the systems.
             * Must be run at least once.
             * 
             */
            void ProcessOrder();

            /**
             * @brief Gets the running order of execution of the systems.
             * 
             */
            std::list<ISystem*> RunningList() const { return m_running_list;}

            /**
             * @brief Executed at the first run, calls the FirstRun() method
             * of each system.
             * 
             */
            void FirstRun();

            /**
             * @brief Executes the Run() method of each system.
             * 
             */
            void Run();

            /**
             * @brief Executes the LastRun() method of each system.
             * 
             */

            void LastRun();

            /**
             * @brief Receives an event from the EventManager 
             * 
             * @param event : The event received.
             */
            virtual void ReceiveEvent(Event::IEvent* event) override;

        private:
            std::map<const std::type_info*, ISystem*> m_systems; // Hash map of systems
            std::list<ISystem*> m_running_list; // Running order of systems
            std::vector<Edge*> m_edges; // Edges (only used for the ProcessOrder, empty otherwise)

            /**
             * [Interal use only]
             * Gets the first unmarked ISystem.
             */
            ISystem* GetUnmarked();

            /**
             * [Internal use only]
             * Visits recursively the system given in parameter.
             */
            bool Visit(ISystem* system);

            /**
             * Returns true if the key type is present in the map of objects.
             */
            bool HasSystem(const std::type_info* type);
            
    };
}