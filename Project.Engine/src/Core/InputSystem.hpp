#pragma once

#include <EntityComponentSystem/ISystem.hpp>
#include <string>
#include <functional>
#include <map>
#include <vector>

namespace Common
{
    class IWindow;
}
namespace Core
{
    class InputSystem : public ECS::ISystem
    {
        public:
            InputSystem(Common::IWindow* win);
            void Subscribe(const std::string&  input, EntityID id, const std::function<void(EntityID id)>& fct);

            virtual void FirstRun() override;
            virtual void Run() override;
            virtual void LastRun() override;

        private:
            std::map<std::string, std::map<EntityID, std::function<void(EntityID id)>>> m_map;
            Common::IWindow* m_win;
    };
}
