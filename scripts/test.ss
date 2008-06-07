//==================================================================
// Test.ss:
// Test file for SS++ compiler development.
//==================================================================

class Foo
{	
	static float sqrt(float f)
	{
		return f < 1e-6 ? 0.0 : f**0.5;
	}
	
	static float dist(float x, float y)
	{
		return sqrt(x*x + y*y);
	}
	
	
	static void printBeerCount(int x)
	{	
	}
	
	void foobar() {
		int beers = 99;
		while(beers >= 0)
		{
			printBeerCount(beers);
			beers = beers - 1;
		}
		/*
		int accum = y;
		if(accum < 0)
			return -1;
		else
		{
			while(accum < 10000) {
				accum = accum + accum;
			}
			return accum;
		}*/
	}
}

