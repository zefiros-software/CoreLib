
#include "common/program.h"

int main( int argc, char **argv )
{
    Program p( argc, argv );

    while ( p.IsRunning() )
    {
        p.Update();
    }

    return 0;
}