from SSTest import *

#-----------------------------------------------------------------------------
QuickTest('simpleClasses', OK,
"""
class X
{
}

class Y
{
}
""")

#-----------------------------------------------------------------------------
QuickTest('numbersNotNames', FAIL,
"""
class 9
{
}
""")

#-----------------------------------------------------------------------------
QuickTest('colonForSubclassing', FAIL,
"""
class X Y
{
}
""")

#-----------------------------------------------------------------------------
QuickTest('255CharName', OK,
"""
// 255-character name (max is 255)
class
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
{
}
""")

#-----------------------------------------------------------------------------
QuickTest('256CharName', FAIL,
"""
// 256-character name (max is 255)
class
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
{
}
""")

#-----------------------------------------------------------------------------
QuickTest('classNameConflict', FAIL,
"""
class X
{
}

class X
{
}
""")

#-----------------------------------------------------------------------------
QuickTest('classNameConflictDiffCase', FAIL,
"""
class X
{
}

class x
{
}
""")

#-----------------------------------------------------------------------------
QuickTest('classNameVarNameConflict', FAIL,
"""
class X
{
	int X;
}
""")

#-----------------------------------------------------------------------------
QuickTest('varNameConflict', FAIL,
"""
class X
{
	int Y;
	int Y;
}
""")

#-----------------------------------------------------------------------------
QuickTest('varNameConflictDiffCase', FAIL,
"""
class X
{
	int Y;
	int y;
}
""")

#-----------------------------------------------------------------------------
QuickTest('classNameFuncNameConflict', FAIL,
"""
class X
{
	X() {}
}
""")

#-----------------------------------------------------------------------------
QuickTest('classNameFuncNameConflictDiffCase', FAIL,
"""
class X
{
	X() {}
}
""")

#-----------------------------------------------------------------------------
QuickTest('funcNameConflict', FAIL,
"""
class X
{
	Y() {}
	Y() {}
}
""")

#-----------------------------------------------------------------------------
QuickTest('funcNameConflictDiffCase', FAIL,
"""
class X
{
	Y() {}
	y() {}
}
""")

#-----------------------------------------------------------------------------
QuickTest('varUsedAsType', FAIL,
"""
class Y
{
}

class X
{
	int Y;
	Y Z;
}
""")

#-----------------------------------------------------------------------------
QuickTest('varUsedAsTypeDiffOrder', FAIL,
"""
class Y
{
}

class X
{
	Y Z;
	int Y;
}
""")

#-----------------------------------------------------------------------------
# Regression test: Make sure inheritance doesn't cause name conflicts
QuickTest('varUsedAsTypeDiffOrder', OK,
"""
class Base
{
	int x;
}

class Derived : Base
{
}
""")

