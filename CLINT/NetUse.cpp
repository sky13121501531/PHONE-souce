#include "NetUse.h"

UpLoad::UpLoad()
{

}

UpLoad::~UpLoad()
{

}

void UpLoad::start( void )
{
    this->open();
}

int UpLoad::svc( void )
{

    return 0;
}

void UpLoad::open( void )
{
    this->activate();
}
