#pragma once
#include "Types.hpp"

namespace ECS
{
    /**
     * @brief The base system class.
     * 
     */
    class ISystem
    {
        public:
            /**
             * @brief Construct a new ISystem object, tagged by SYSTEM_MASK_UNMARKED (for the SystemManager)
             * 
             */
            ISystem() : mask(SYSTEM_MASK_UNMARKED){}

            /**
             * @brief Get the mask of the system.
             * 
             */
            SystemMask GetMask() const {return mask;}

            /**
             * @brief Set the mask.
             * 
             */
            void SetMask(SystemMask value) {mask = value;}

            /**
             * @brief Runs all the systems once at start.
             * 
             */
            virtual void FirstRun() = 0;

            /**
             * @brief Runs all the systems. This is the main 'update' function
             * which might be called in the main program loop.
             */
            virtual void Run() = 0;

            /**
             * @brief Executes one last execution of all the systems.
             * 
             */
            virtual void LastRun() = 0;

        private:
            SystemMask mask;
    };

    /**
     * @brief The edge class, used by the SystemManager to order system execution.
     * 
     */
    class Edge final
    {
        public:
            Edge(ISystem* start, ISystem* end) : m_start(start), m_end(end){}
            ISystem* GetStart() { return m_start; }
            ISystem* GetEnd() { return m_end; }
        private:
            ISystem* m_start;
            ISystem* m_end;

    };

}