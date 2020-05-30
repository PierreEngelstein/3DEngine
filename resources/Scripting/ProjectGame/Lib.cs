using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using Scripting.Core;

namespace ProjectGame
{
    
    public static class Lib
    {
        private static ScriptManager manager = new ScriptManager();
        public static int GetMemAtStart(IntPtr arg, int argLength)
        {
            Console.WriteLine("In CSharp!");
            return 0;
        }
        
        [StructLayout(LayoutKind.Sequential)]
        public struct ManagedScriptArguments
        {
            public uint id;
            public string scriptName;
        }

        public static int AddManagedScriptComponent(IntPtr arg, int argLength)
        {
            ManagedScriptArguments argument = Marshal.PtrToStructure<ManagedScriptArguments>(arg);
            
            Type t = Type.GetType(argument.scriptName);
            
            manager.AddScript(argument.id, t);
            
            Console.WriteLine($"new script {argument.scriptName} for entity {argument.id}");

            return 0;
        }

        public static int DeleteManagedScriptComponent(IntPtr arg, int argLength)
        {
            ManagedScriptArguments argument = Marshal.PtrToStructure<ManagedScriptArguments>(arg);
            Type t = Type.GetType(argument.scriptName);
            manager.RemoveScript(argument.id, t);
            Console.WriteLine($"delete script {argument.scriptName} for entity {argument.id}");
            return 0;
        }

        public static int OnInit(IntPtr arg, int argLength)
        {
            manager.OnInit();
            return 0;
        }
        
        public static int OnUpdate(IntPtr arg, int argLength)
        {
            manager.OnUpdate();
            return 0;
        }
        
        public static int OnEnding(IntPtr arg, int argLength)
        {
            manager.OnEnding();
            return 0;
        }
        
    }
}