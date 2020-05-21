#include "SystemManager.hpp"
#include "ISystem.hpp"
#include <Logging/Logger.hpp>

namespace ECS
{
    SystemManager::SystemManager()
    {

    }

    SystemManager::~SystemManager()
    {
        for(auto i : m_systems)
        {
            delete i.second;
        }
        m_running_list.clear();
        m_systems.clear();
    }

    ISystem* SystemManager::GetUnmarked()
    {
        for(auto i : m_systems)
        {
            if(i.second->GetMask() == SYSTEM_MASK_UNMARKED)
                return i.second;
        }
        return nullptr;
    }

    bool SystemManager::Visit(ISystem* system)
    {
        if(system == nullptr)
        {
            LOG_ERROR("The system is null\n");
            return false;
        }
        if(system->GetMask() == SYSTEM_MASK_PERM)
            return true;
        if(system->GetMask() == SYSTEM_MASK_TEMP)
        {
            LOG_ERROR("The systems are not in a directed acyclic graph !\n")
            return false;
        }

        system->SetMask(SYSTEM_MASK_TEMP);

        for(auto i : m_edges)
        {
            if(i->GetStart() == system)
            {
                bool val = Visit(i->GetEnd());
                if(!val)
                {
                    LOG_ERROR("Errors occurred while processing the graph.\n")
                    return false;
                }
            }
        }

        system->SetMask(SYSTEM_MASK_PERM);
        m_running_list.push_front(system);
        return true;
    }

    // Implements deep-first sorting
    void SystemManager::ProcessOrder()
    {
        m_running_list.clear(); // Start by reseting the current list of running systems
        ISystem *system = GetUnmarked();
        while(system != nullptr)
        {
            bool val = Visit(system);
            if(!val)
            {
                LOG_ERROR("Errors occurred while processing the system.\n")
                return;
            }
            system = GetUnmarked();
        }

        for(auto i : m_edges)
        {
            delete(i);
        }
        m_edges.clear();
    }

    bool SystemManager::HasSystem(const std::type_info* type)
    {
        return m_systems.find(type) != m_systems.end();
    }

    void SystemManager::FirstRun()
    {
        for(auto it : m_running_list)
        {
            it->FirstRun();
        }
    }

    void SystemManager::Run()
    {
        for(auto it : m_running_list)
        {
            it->Run();
        }
    }

    void SystemManager::LastRun()
    {
        for(auto it : m_running_list)
        {
            it->LastRun();
        }
    }

    void SystemManager::ReceiveEvent(Event::IEvent* event)
    {
        return;
    }
}