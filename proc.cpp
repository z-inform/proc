#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <cerrno>
#include "../istack/stack.h"

#define EMPTYCMD 100
#define DONEXEC 101
#define WRNGCMD 102

#include "cmdCodes"

void printDouble(void* el);

Proc::Proc(int _stackSize):
    calcStack(stackElSize, _stackSize),
    asmFile(nullptr){
    calcStack.setPrint(*printDouble);
}

Proc::Proc():
    calcStack(stackElSize),
    asmFile(nullptr){
    calcStack.setPrint(*printDouble);
}

Proc::~Proc(){
    if( cmdPtr != nullptr ) free(cmdPtr);
}

void Proc::push(void* ptr){
    calcStack.push(ptr);
}

void Proc::pop(void* ptr){
    if( ptr == nullptr ){
        int* iptr = (int*) calloc(stackElSize, 1);
        calcStack.pop(iptr);
        free(iptr);
    }
    else calcStack.pop(ptr);
}

void Proc::setFile(char* name){
    struct stat st = {};
    size_t size = 0;
    FILE* desc = fopen(name, "rb");
    stat(name, &st);
    size = st.st_size / 4;
    cmdPtr = (int*) calloc(size, sizeof(int));
    fread(cmdPtr, size, sizeof(int), desc);
}

void Proc::mul(){
    int a = 0;
    int b = 0;
    calcStack.pop(&a);
    calcStack.pop(&b);
    a *= b;
    calcStack.push(&a);
}

void Proc::add(){
    int a = 0;
    int b = 0;
    calcStack.pop(&a);
    calcStack.pop(&b);
    a += b;
    calcStack.push(&a);
}

void Proc::sub(){
    int a = 0;
    int b = 0;
    calcStack.pop(&a);
    calcStack.pop(&b);
    a = a - b;
    calcStack.push(&a);
}

void Proc::div(){
    int a = 0;
    int b = 0;
    calcStack.pop(&a);
    calcStack.pop(&b);
    a = a / b;
    calcStack.push(&a);
}

void Proc::print(){
    int a = 0;
    try{
    calcStack.pop(&a);
    printf("%d\n", a);
    calcStack.push(&a);
    }
    catch(int err){
        if( err != EMPTYPOP ) throw;
    }


}

void Proc::calcDump(){
    calcStack.dump();
}

void printDouble(void* el){
    double* ptr = (double*) el;
    printf("%lg", *ptr);
}

void Proc::skip(int interval){
    cmdCnt += interval;
}

void Proc::startProc(){
    if( cmdPtr == nullptr ) throw EMPTYCMD;
    if( cmdCnt < 0 ) throw DONEXEC;

    while( cmdCnt >= 0 ) cmdExec();
}

void Proc::halt(){
    cmdCnt = -2;
}

void Proc::cmdExec(){
    switch (cmdPtr[cmdCnt]){
        case PUSH:
            push(cmdPtr + (++cmdCnt));
            break;

        case POP:
            if( cmdPtr[++cmdCnt] == -1 ){
               pop(nullptr);
            }
            else pop((void*) cmdPtr[cmdCnt]);
            break;

        case ADD:
            add();
            break;

        case MUL:
            mul();
            break;

        case SUB:
            sub();
            break;

        case DIV:
            div();
            break;

        case PRNT:
            print();
            break;

        case SKP:
            skip(cmdPtr[++cmdCnt]);
            break;

        case HLT:
            halt();
            return;
            break;

        default:
            halt();
            throw(WRNGCMD);
            break;
            
    }

    cmdCnt++;
}
