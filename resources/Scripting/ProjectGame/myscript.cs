
using System;
using Scripting.Core;

public class myscript : Script
{
    public override void OnInit()
    {
        Logger.Fatal("On Init Entity !");
    }

    public override void OnUpdate()
    {
        Logger.Debug("On update entity !");
    }

    public override void OnEnding()
    {
    }
}