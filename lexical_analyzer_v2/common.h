#ifndef COMMON_H
#define COMMON_H


#define MAXRESERVED 9 //

#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>

using std::vector;
using std::ifstream;
using std::string;
using std::for_each;
using std::cout;
using std::endl;
//定义保留字类型
typedef enum{
    IF,ELSE,RETURN,INT,CHAR,CIN,COUT,WHILE,FOR,ENDL,
    // reserverd words
    SLASH,NUMBER,ADD,MINUS,ASSIGN,LBRACKET,RBRACKET,LPAREN,RPAREN,GREATER,LESS,SEMICOLON,MULTIPLY,COMMA,ERROR,
    //operator
    STRING,CHAR_TOKEN,
    //id
    ID
} TokenType;

////定义词法分析中的最小单元 TOKEN
//typedef struct TOKEN {
//    std::string str; //TOKEN 在输入源码中的原始字符串
//    TokenType token; //TOKEN 种类
//} TOKEN;
//
//定义静态保留字数组,方便在identifier中查找
//static const TOKEN reservedWords[MAXRESERVED]
//={
//    {"if",IF},{"else",ELSE},{"return",RETURN},{"int",INT},
//    {"char",CHAR},{"cin",CIN},{"cout",COUT},{"while",WHILE},
//    {"for",FOR}
//};
//


#endif
