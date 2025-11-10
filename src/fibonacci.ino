#include <inttypes.h>

register float ctrl_matrix[2][2] = 
{
    { 1.0, 1.0 },
    { 1.0, 1.0 }
};


void __attribute__((__always_inline__))
reverse ( void )
{
    ctrl_matrix[0][0] |= 0x80000000;
    ctrl_matrix[0][1] |= 0x80000000;
    ctrl_matrix[1][0] |= 0x80000000;
    ctrl_matrix[1][1] |= 0x80000000;
}

void __attribute__((__always_inline__))
forward ( void )
{
    ctrl_matrix[0][0] &= (~0x80000000);
    ctrl_matrix[0][1] &= (~0x80000000);
    ctrl_matrix[1][0] &= (~0x80000000);
    ctrl_matrix[1][1] &= (~0x80000000);
}

void __attribute__((__always_inline__))
park ( void )
{
    ctrl_matrix[0][0]  = 0;
    ctrl_matrix[0][1]  = 0;
    ctrl_matrix[1][0]  = 0;
    ctrl_matrix[1][1]  = 0;
}

void setup ()
{

}

void loop ()
{

}