#ifndef LEXICAL_H
#define LEXICAL_H

#include"token.h"
//#include"token.cpp"

class Scan
{
    private :
        std::vector<Token> token_list;
        std::ifstream input;
        //向token_list中插入分析好的token
        void insertToken(std::string,int,int, TokenType);
    public :
        Scan() = default;
        //构造函数,直接读入文件
        Scan(std::string file)
        {
            input.open(file,std::ios::in);
            if(!input)
                std::cout<<"file open failed,check the path again";
        }
        ~Scan() {
            if(input)
                input.close();
            //关闭文件流
        }
        void lexicalAnalyzer();
        void printRes();
};

#endif
