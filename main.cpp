#include "avl.h"

#include <iostream>
int main(){
    Dictionary<int, int> x;
    x.insert(1,45454);
    x.insert(2,11111);
    x.insert(3,2222);
    std::cout << x.find(4) << std::endl;
try{
    std::cout << x.getInfo(2) << std::endl;
    std::cout << x.getInfo(4) << std::endl;
}
catch(...){
    std::cout << "Key not found\n";
}
}