#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
//multiply by 5
static unsigned char func[] = {

        0x55,
        0x48 ,0x89,0xe5,
        0x89 ,0x7d ,0xec,
        0xc7,0x45,0xfc,0x05 ,0x00 ,0x00 ,0x00 ,
        0x8b, 0x45, 0xec,
        0x0f,0xaf,0x45,0xfc,
        0x5d,
        0xc3
};
const size_t  indexOfMul= 10;
constexpr  size_t size = sizeof(func);
void * data;
void  allocation(){
    data = mmap(nullptr,size,PROT_READ|PROT_EXEC|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    if(data == MAP_FAILED){
        std::cerr<<"Couldn't allocate the memory for function"<<"\n";
        return;
    }
}

void writeFunction(){
    memcpy(data,func,size);
}
void evaluate(size_t x){
    if(mprotect(data,size,PROT_WRITE|PROT_READ|PROT_EXEC)==-1){
        std::cerr<<"Memory can't contain executable code"<<"\n";
        return;
    }
    auto result = reinterpret_cast<size_t (*) (size_t)> (data);
    size_t ans = result(x);
    std::cout<<ans<<"\n";
}
void freeMemory(){
    if(munmap(data,size)==-1){
        std::cerr<<"Couldn't free memory"<<"\n";
    };
}
void change(size_t x){
    auto newValue = static_cast<unsigned char>(x);
    static_cast<unsigned char*>(data)[indexOfMul] = newValue;
}
int main() {
    std::cout<<"Function in multiplying the value by 5 in default";
    size_t x;
    std::cout<<"Please,write a value for the function"<<"\n";
    std::cin>>x;
    allocation();
    writeFunction();
    evaluate(x);
    std::cout<<"Do you want to change the function?[Y|N]"<<"\n";
    std::string confirmation;
    std::cin>>confirmation;
    if(confirmation=="Y"){
        std::cout<<"Please,write a value to change multiplication"<<"\n";
        std::cin>>x;
        std::cout<<"Please,write a value for the function"<<"\n";
        change(x);
        std::cin>>x;
        evaluate(x);
    }
    else {
        std::cout<<"Ok,bye";
    }
    freeMemory();
    return 0;
}
