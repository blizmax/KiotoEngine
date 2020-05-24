﻿using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace ShaderInputsParserApp.Source.Types
{
    class Structure
    {
        public Structure(string name, List<Variable> members)
        {
            Name = name;
            Members = new List<Variable>(members);
        }

        public string Name { get; }

        public List<Variable> Members { get; }

        public int Bindpoint { get; set; } = 0;
    }
}
