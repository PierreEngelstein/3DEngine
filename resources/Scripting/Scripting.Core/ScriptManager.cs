using System;
using System.Collections.Generic;
using System.Linq;

namespace Scripting.Core
{
    public class ScriptManager
    {
        public ScriptManager()
        {
            
        }

        public void OnInit()
        {
            foreach (var id in m_scripts)
            {
                foreach (var script in id.Value)
                {
                    script.OnInit();
                }
            }
        }

        public void OnUpdate()
        {
            foreach (var id in m_scripts)
            {
                foreach (var script in id.Value)
                {
                    script.OnUpdate();
                }
            }
        }

        public void OnEnding()
        {
            foreach (var id in m_scripts)
            {
                foreach (var script in id.Value)
                {
                    script.OnEnding();
                }
            }
        }
        
        /** Adds a script to the entity. */
        public void AddScript(uint EntityID, Type T)
        {
            if(T == null) throw new ArgumentNullException(nameof(T));
            // Entity first registered : add it and add the script
            if (!m_scripts.ContainsKey(EntityID))
            {
                Script sc = (Script)Activator.CreateInstance(T);
                m_scripts.Add(EntityID, new List<Script>(){sc});
            }
            // Entity already registered : check it does not already have a script of that name, if so throw exception else add new script.
            else
            {
                List<string> currentScripts = m_scripts[EntityID].Select(s => s.GetType().Name).ToList();
                if(!currentScripts.Contains(T.Name))
                    m_scripts[EntityID].Add((Script)Activator.CreateInstance(T));
                else
                    throw new InvalidOperationException($"Entity {EntityID} already contains script {T.Name}");
            }
        }
        
        /** Removes a script from the entity. */
        public void RemoveScript(uint EntityID, Type T)
        {
            if (!m_scripts.ContainsKey(EntityID))
            {
                throw new ArgumentException($"The entity {EntityID} does not contain any scripts.");
            }
            List<string> currentScripts = m_scripts[EntityID].Select(s => s.GetType().Name).ToList();
            if(!currentScripts.Contains(T.Name)) throw new ArgumentException($"The entity {EntityID} does not contain a script named {T.Name}");
            m_scripts[EntityID].RemoveAll(s => s.GetType().Name.Equals(T.Name));
        }

        private Dictionary<uint, List<Script>> m_scripts = new Dictionary<uint, List<Script>>();
    }
}