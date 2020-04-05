#pragma once


namespace ECS
{
    /**
     * @brief The base class for all components.
     * 
     */
    class IComponent
    {
        public:
            IComponent(){}
            virtual ~IComponent(){}
    };
}