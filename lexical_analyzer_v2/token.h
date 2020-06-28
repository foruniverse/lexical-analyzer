#ifndef TOKEN_H
#define TOKEN_H

#include"common.h"

class Token{
    //打印最终结果
    friend void printToken(const Token &token) ;
    private :
        std::string value;
        TokenType type;
    public:
        // C++11 新特性,默认构造函数
        Token()=default;
        //使用初始化列表
        Token(std::string value,TokenType type):value(value),type(type) {}
        //通过词法分析过程中的中间字符串构造一个token
        Token(std::string line,int left,int right,TokenType type)
        {
            std::string temp(begin(line)+left,begin(line)+right+1);
            this->value=temp;
            this->type=type;
            //检查关键字是否
            if(this->type==ID)
                this->checkToken();
        }
        ~Token(){}        
        //检查token是Identifier还是关键字
        void checkToken();
        //inline 函数
        std::string getValue()
        {
            return value;
        }
        TokenType getType()
        {
            return type;
        }
        Token & setValue(const std::string  &temp)
        {
            this->value=temp;
            return *this;
        }
};

//定义静态保留字数组,方便在identifier中查找
static const Token reservedWords[MAXRESERVED]
={
    Token("if",IF),Token("else",ELSE),Token("return",RETURN),Token("int",INT),
    Token("char",CHAR),Token("cin",CIN),Token("cout",COUT),Token("while",WHILE),
    Token("for",FOR)
};

void Token::checkToken()
{
    for(auto temp:reservedWords)
    {
    //    if(strcmp(this->value.c_str(),temp.getValue().c_str)==0) // c风格
        if(this->value==temp.getValue())
            this->type=temp.getType();
    }
};
void printToken(const Token & token) 
{
    cout<<token.value<<"-----";
    switch(token.type)
    {
        case INT:
            cout<<"INT";
            break;
        case ID:
            cout<<"ID";
            break;
        case ELSE:
            cout<<"ELSE";
            break;
        case RETURN:
            cout<<"RETURN";
            break;
        case CHAR:
            cout<<"CHAR";
            break;
        case CIN:
            cout<<"CIN";
        case COUT:
            cout<<"COUT";
            break;
        case WHILE:
            cout<<"WHILE";
            break;
        case FOR:
            cout<<"FOR";
            break;
        case ENDL:
            cout<<"ENDL";
            break;
        case SLASH:
            cout<<"SLASH";
            break;
        case NUMBER:
            cout<<"NUMBER";
            break;
        case ADD:
            cout<<"ADD";
            break;
        case MINUS:
            cout<<"MINUS";
            break;
        case ASSIGN:
            cout<<"ASSIGN";
            break;
        case LBRACKET:
            cout<<"LBRACKET";
            break;
        case RBRACKET:
            cout<<"RBACKET";
            break;
        case LPAREN:
            cout<<"LPAREN";
            break;
        case RPAREN:
            cout<<"RPAREN";
            break;
        case GREATER:
            cout<<"GREATER";
            break;
        case LESS:
            cout<<"LESS";
            break;
        case SEMICOLON:
            cout<<"SEMICOLON";
            break;
        case MULTIPLY:
            cout<<"MULTIPLY";
            break;
        case COMMA:
            cout<<"COMMA";
            break;
        case ERROR:
            cout<<"ERROR";
            break;
        case STRING:
            cout<<"STRING";
            break;
        case CHAR_TOKEN:
            cout<<"CHAR_TOKEN";
            break;
        default:
            cout<<"not found in the table";// this should never happen;
    }
    cout<<endl;
};

#endif
