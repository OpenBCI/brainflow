﻿using System;

namespace brainflow
{
    public class BrainFlowExceptioin : Exception
    {
        public int exit_code;
        public BrainFlowExceptioin (int code) : base (String.Format ("{0}:{1}", Enum.GetName (typeof (CustomExitCodes), code), code))
        {
            exit_code = code;
        }
    }
}
