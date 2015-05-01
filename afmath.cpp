
#include "afmath.hpp"
	
float afrsqrt( float x )
{
    //from http://fabiensanglard.net/doom3_documentation/37725-293747_293747.pdf
    long i;
    float y, r;
    y = x * 0.5f;
    i = *( long * )( &x );
    i = 0x5f3759df - ( i >> 1 );
    r = *( float * )( &i );
    r = r * ( 1.5f - r * r * y );
    return r;
}
