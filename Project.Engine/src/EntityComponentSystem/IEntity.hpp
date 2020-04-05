#pragma once
#include "Types.hpp"

namespace ECS
{
    /**
     * @brief The base entity class.
     * 
     */
    class IEntity
    {
        public:
            /**
             * @brief Construct a new IEntity object.
             * 
             * @param id : The id generated from the EntityManager.
             */
            IEntity(EntityID id) : m_id(id){}
            virtual ~IEntity(){}
        private:
            /**
             * @brief The entity id generated from the EntityManager.
             * 
             */
            EntityID m_id;
    };
}