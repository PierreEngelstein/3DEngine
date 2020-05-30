using System.Diagnostics;

namespace Scripting.Core
{
    internal enum Verbosity
    {
        Debug = 0,
        Info = 1,
        Warning = 2,
        Error = 3,
        Fatal = 4,
    }
    
    public static class Logger
    {
        /** Sends a debug log to the engine. */
        public static void Debug(string message,
            [System.Runtime.CompilerServices.CallerFilePath] string sourceFilePath = "",
            [System.Runtime.CompilerServices.CallerLineNumber] int sourceLineNumber = 0)
        {
            Bindings.Log(sourceFilePath, sourceLineNumber, (int)Verbosity.Debug, $"{message}\n");
        }
        
        /** Sends an info log to the engine. */
        public static void Info(string message,
            [System.Runtime.CompilerServices.CallerFilePath] string sourceFilePath = "",
            [System.Runtime.CompilerServices.CallerLineNumber] int sourceLineNumber = 0)
        {
            Bindings.Log(sourceFilePath, sourceLineNumber, (int)Verbosity.Info, $"{message}\n");
        }
        
        /** Sends a warning log to the engine. */
        public static void Warning(string message,
            [System.Runtime.CompilerServices.CallerFilePath] string sourceFilePath = "",
            [System.Runtime.CompilerServices.CallerLineNumber] int sourceLineNumber = 0)
        {
            Bindings.Log(sourceFilePath, sourceLineNumber, (int)Verbosity.Warning, $"{message}\n");
        }
        
        /** Sends an error log to the engine. */
        public static void Error(string message,
            [System.Runtime.CompilerServices.CallerFilePath] string sourceFilePath = "",
            [System.Runtime.CompilerServices.CallerLineNumber] int sourceLineNumber = 0)
        {
            Bindings.Log(sourceFilePath, sourceLineNumber, (int)Verbosity.Error, $"{message}\n");
        }
        
        /** Sends a fatal log to the engine. Results in an abort of the full program. Be careful with that ! */
        public static void Fatal(string message,
            [System.Runtime.CompilerServices.CallerFilePath] string sourceFilePath = "",
            [System.Runtime.CompilerServices.CallerLineNumber] int sourceLineNumber = 0)
        {
            Bindings.Log(sourceFilePath, sourceLineNumber, (int)Verbosity.Fatal, $"{message}\n");
        }
    }
}