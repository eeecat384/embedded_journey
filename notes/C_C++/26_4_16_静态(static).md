# 静态（static）

## 在类和结构体外的static

在类外的static，意味着声明为static的符号，其链接只在文件内部，仅对翻译单元（单个cpp文件）可见

比如我们有两个源文件main.cpp和static.cpp

```cpp
//static.cpp
static int s_var ＝ 5;
//s_前缀表名为静态变量
//类外static表明其链接只在static.cpp中，链接器不会在翻译单元作用域外寻找定义
```

```cpp
//main.cpp
#include<iostream>
static int s_var ＝ 10;
//由于两个变量没有被链接器链接，故没有错误
int main(){
    std: :cout << s_var <<endl; 
    //正常打印出10
}
```

如果将static.cpp中的变量使用int s_var定义，则链接器将会链接两翻译单元的变量，运行后会出现链接错误

如果在此基础上，将main.cpp的变量使用extern int s_var定义，则extern关键字会告诉编译器去其他文件寻找s_var变量，程序运行正常，打印出来5

如果继续改动，改为

```cpp
//static.cpp
static int s_var;
```

```cpp
//main.cpp
extern int s_var; 
//略
```

则extern让编译器在全局作用域下寻找s_var变量时，编译器将无法找到s_var，因为s_var被static限制为不会和其他翻译单元链接，就像类中的私有

上面这些对函数的定义来说是相似的

## 在类和结构体内的static
在类中的static，意味着该变量与类的所有实例共享内存（这个规律也适用于类中的静态方法）
