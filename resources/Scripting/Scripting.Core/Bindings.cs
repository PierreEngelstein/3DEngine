using System.Runtime.InteropServices;

namespace Scripting.Core
{
    public static class Bindings
    {
        [DllImport("libengine.so")]
        public static extern void Log(string filePath, int line, int verbose, string message);
    }
}