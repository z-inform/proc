#pragma once
#include <stdint.h>
#include "../istack/stack.h"


class Proc{
    int stackElSize = sizeof(int);
    int* cmdPtr = nullptr;
    int cmdCnt = 0;
    Stack calcStack;
    FILE* asmFile;
    void cmdExec();
    
    public:


    Proc(int _stackSize);
    Proc();
    ~Proc();

    void push(void* ptr);
    void pop(void* ptr);
    void setFile(char* name);
    void add();
    void mul();
    void sub();
    void div();
    void print();
    void calcDump();
    void skip(int interval);
    void startProc();
    void halt();

    







};








#include "proc.cpp"

