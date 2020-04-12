# Format

## Classes

### Normal Engine Class
```c++
// .h File
#pragma once

#include "Seek/Core.h"
#include "Seek/Log.h"

#include "Bar.h"

namespace Seek 
{
    class Foo : public Bar
    {
    // Structs, enum, usings
    public:
        enum class Test 
        {
            NONE, 
            TEST, 
            TEST2
        };
    // Constructors and Destructors
    public:
        Foo(Test test);
        ~Foo();

    // Methods
        void Hello();

    private:
        void World();

    // Variables
    private:
        int32 m_Variable = 0;

    // Static Methods
    public:
        static void StaticHello();
    private:
        static void StaticWorld();

    // Static Variables
    private:
        static float s_StaticVar;
    };
}

// .cpp File
#include "SeekPCH.h"
#include "Seek/Foo.h"

namespace Seek 
{
    // NOTE: Static Variables Initializtion
    float Foo::s_StaticVar;

    // NOTE: Constructor
    Foo(Test test) { }
    ~Foo() { }

    // NOTE: Methods

    void Foo::Hello() { }
    void Foo::World() { }

    // NOTE: Static Methods
    void Foo::StaticHello() { }
    void Foo::StaticWorld() { }
}
```

### Manager Class
- Todo Add

### Singleton Class
- Todo Add

### API Interface
- Todo Add