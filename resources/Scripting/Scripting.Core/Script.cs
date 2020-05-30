namespace Scripting.Core
{
    /** Base Script class. */
    public abstract class Script
    {
        public virtual void OnInit(){}
        
        public virtual void OnUpdate(){}
        
        public virtual void OnEnding(){}
    }
}