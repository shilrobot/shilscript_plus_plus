//==================================================================
// Test.ss:
// Test file for SS++ compiler development.
//==================================================================

class Foo
{	
	static int foobar(int y)
	{
		int accum = y;
		while(accum < 10000) {
			accum = accum + accum;
		}
		return accum;
	}
}
