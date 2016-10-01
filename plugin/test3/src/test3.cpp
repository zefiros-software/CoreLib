#include "test3.h"

#include "test2.h"

void Test3::OnInit()
{
    assert( GetNumber() == 6 );
}

U32 Test3::GetNumber()
{
    return Controller::Get< Test2 >()->GetNumber() + 4;
}
