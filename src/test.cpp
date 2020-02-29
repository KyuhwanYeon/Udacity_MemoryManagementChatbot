#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <iostream>
// rule of three 는 constructor 나 , copy constructor 등을 사용하기 위해서는 constructor, copy constructor, copy assignment operator가 모두 필요하다는 것이다.
// rule of five 는 move constructor, move assignment operator를 추가한다.
// Move constructor 는, source의 object를 target의 object에 복사하는 것이 아니라, target 이 가리키는 위치만 기존의 source의 heap resouce 가리키는 것이다.
/// object의 ownership 이 target 쪽으로 옮겨진 것이다. 복사 없이.

// Because copying objects can be an expensive operation which involves creating,
// copying and destroying temporary objects, rvalue references are used to bind to an rvalue.
// Using this mechanism, the move constructor transfers the ownership of a resource from a (temporary) rvalue object to a permanent lvalue object(target 함수에서의 lvalue object).

// Move semantic을 쓰는 첫번째 이유는 heavy-weight를 가지는 object를 복사해야할 때다. move semantic은 이를 위해 실제로 복사하는 것이 아니고  object를 source에서 target으로 move 할 뿐이다. r value reference를
// 두번 째 이유는 ownership 을 바꿔야할 때이다. unique pointer 처럼
class MyMovableClass
{
private:
    int _size;
    int *_data;

public:
    MyMovableClass(size_t size) // constructor
    {
        _size = size;
        _data = new int[_size];
        std::cout << "CREATING instance of MyMovableClass at " << this << " allocated with size = " << _size * sizeof(int) << " bytes" << std::endl;
    }
    MyMovableClass(const MyMovableClass &source) // 2 : copy constructor
    {
        _size = source._size;
        _data = new int[_size];
        *_data = *source._data; // deep copy / instance가 아무리 여러개 생겨도, 동일한 메모리를 가르키지는 않으니 deep copy 다
        std::cout << "COPYING content of instance " << &source << " to instance " << this << std::endl;
    }
    MyMovableClass &operator=(const MyMovableClass &source) // 3 : copy assignment operator
    {
        std::cout << "ASSIGNING content of instance " << &source << " to instance " << this << std::endl;
        if (this == &source) // obj1 = obj1 과 같은 상황이 발생했을 때 사용된다.
            return *this;
        delete[] _data;
        _data = new int[source._size];
        *_data = *source._data;
        _size = source._size;
        return *this;
    }
    MyMovableClass(MyMovableClass &&source) // 4 : move constructor
    {
        std::cout << "MOVING (c’tor) instance " << &source << " to instance " << this << std::endl;
        _data = source._data;
        _size = source._size;
        source._data = nullptr;
        source._size = 0;
    }
    MyMovableClass &operator=(MyMovableClass &&source) // 5 : move assignment operator
    {
        std::cout << "MOVING (assign) instance " << &source << " to instance " << this << std::endl;
        if (this == &source)
            return *this;

        delete[] _data;

        _data = source._data;
        _size = source._size;

        source._data = nullptr;
        source._size = 0;

        return *this;
    }
    ~MyMovableClass() // 1 : destructor
    {
        std::cout << "DELETING instance of MyMovableClass at " << this << std::endl;
        delete[] _data;
    }
    void printOwnAddress() { std::cout << "Own address on the stack is " << this << std::endl; }
    void printMemberAddress() { std::cout << "Managing memory block on the heap at " << _data << std::endl; }
};

int main()
{
    // std::cout << "--------------- 1. regular constructor-------------------" << std::endl;
    // MyMovableClass obj1(10); // regular constructor
    // std::cout << "obj1:   ";
    // obj1.printMemberAddress();
    // std::cout << std::endl;

    // std::cout << "---------------2. copy constructor-------------------" << std::endl;
    // MyMovableClass obj2(obj1); // copy constructor
    // std::cout << "obj1:   ";
    // obj1.printMemberAddress();
    // std::cout << "obj2:   ";
    // obj2.printMemberAddress();
    // // MyMovable obj3 = obj1; // 이것 또한 copy constructor 이다.
    // std::cout << std::endl;
    // //그런데 생각해보면, copy constructor 를 하기 위해서는 &source = obj1 를 하는 과정에서 obj1 클래스의 메모리와 source의 메모리까지 만들어야 되기 때문에, 큰 클래스를 복사할 경우 비효율적이다. -> 이에 따라 Rule of five 가 나왔다.

    // std::cout << "---------------3. copy assignment operator-------------------" << std::endl;
    // obj2 = obj1; // copy assignment operator
    // std::cout << "obj1:   ";
    // obj1.printMemberAddress();
    // std::cout << "obj2:   ";
    // obj2.printMemberAddress();
    // std::cout << std::endl;

    // std::cout << "---------------4. move constructor (src: obj1, target: obj3-------------------" << std::endl;
    // MyMovableClass obj3(std::move(obj1));
    // std::cout << "obj1:   ";
    // obj1.printMemberAddress();
    // std::cout << "obj3:   ";
    // obj3.printMemberAddress();
    // std::cout << std::endl;

    // std::cout << "---------------5. move assignment operator (src: obj2, target: obj3-------------------" << std::endl;
    // obj3 = (std::move(obj2));
    // std::cout << "obj2:   ";
    // obj2.printMemberAddress();
    // std::cout << "obj3:   ";
    // obj3.printMemberAddress();
    // std::cout << std::endl;

    int id = 4;
    std::vector<std::unique_ptr<MyMovableClass>> _edges; 
     std::unique_ptr<MyMovableClass> edge(new MyMovableClass(id));
   // std::unique_ptr<MyMovableClass> edge = std::make_unique<MyMovableClass>(id);
    _edges.emplace_back(std::move(edge));

    return 0;
}