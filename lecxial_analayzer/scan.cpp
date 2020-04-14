#include<iostream>
#include<vector>
#include<string>
#include<cstring>
#include<fstream>
using namespace std;

#define  MAXRESERVED 9
#define  input "test.txt"
#define  output "output.txt"
typedef enum{
    IF,ELSE,RETURN,INT,CHAR,CIN,COUT,WHILE,FOR,
    // reserverd words
    SLASH,NUMBER,ADD,MINUS,ASSIGN,LBRACKET,RBRACKET,LPAREN,RPAREN,GREATER,LESS,SEMICOLON,MULTIPLY,COMMA,ERROR,
    //operator
    STRING,CHAR_TOKEN,
    //id
    ID
} TokenType;

typedef struct TOKEN {
    string str;
    TokenType token;
} TOKEN;

static const TOKEN reservedWords[MAXRESERVED]
={
    {"if",IF},{"else",ELSE},{"return",RETURN},{"int",INT},
    {"char",CHAR},{"cin",CIN},{"cout",COUT},{"while",WHILE},
    {"for",FOR}
};

TOKEN getToken(void);
void inserttoken(string,int,int,TokenType);
TokenType checkword(string);
string program;
vector<pair<string,TokenType>> Token;
void printtokentype(TokenType temp)
{
    switch(temp)
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
}
int main()
{
    ifstream file(input,ios::in);
    if(!file.is_open())
    {
        cout<<"file open failed"<<endl;
        return 1;
    }
    
    int state=0;
    int plength;
    int currentpos;
    int tokenpos;
    int lineno=0;
    char now;
    bool commentnotfinished=false;
    bool iserror=false;
    while(getline(file,program))
    {
        if(iserror)
            break;
        plength=program.size();
        currentpos=0;
        tokenpos=0;
        lineno++;
        state=0; 
        if(commentnotfinished)
        {
            state=9;
        }
        while(currentpos<plength)
        {
            now=program[currentpos];
            if(iserror)
                break;
            switch(state)
            {
                case 0:
                    currentpos++;
                    if(isdigit(now))
                    {
                        state=6;
                    }
                    else if(now==' ')
                    {
                        tokenpos++;
                    }else if(isalpha(now)|| now=='_')
                    {
                        state=7;
                    }else if(now=='/')
                    {
                        state=8;
                        if(currentpos==plength)
                            inserttoken(program,tokenpos,currentpos-1,SLASH);
                    }else if(now=='\'')
                    {
                        state=1;
                    }else if(now=='"')
                    {
                        state=4;
                    }else
                    {
                        state=0;
                        switch(now)
                        {
                            case '+':
                                inserttoken(program,tokenpos,currentpos-1,ADD);
                                break;
                            case '-':
                                inserttoken(program,tokenpos,currentpos-1,MINUS);
                                break;
                            case '*' :
                                inserttoken(program,tokenpos,currentpos-1,MULTIPLY);
                                break;
                            case '(':
                                inserttoken(program,tokenpos,currentpos-1,LPAREN);
                                break;
                            case ')':
                                inserttoken(program,tokenpos,currentpos-1,RPAREN);
                                break;
                            case '{':
                                inserttoken(program,tokenpos,currentpos-1,LBRACKET);
                                break;
                            case '}':
                                inserttoken(program,tokenpos,currentpos-1,RBRACKET);
                                break;
                            case ';':
                                inserttoken(program,tokenpos,currentpos-1,SEMICOLON);
                                break;
                            case ',':
                                inserttoken(program,tokenpos,currentpos-1,COMMA);
                                break;
                            case '>':
                                inserttoken(program,tokenpos,currentpos-1,GREATER);
                                break;
                            case '<':
                                inserttoken(program,tokenpos,currentpos-1,LESS);
                                break;
                            case '=':
                                inserttoken(program,tokenpos,currentpos-1,ASSIGN);
                                break;
                            default:
                                state=100;
                                inserttoken(program,tokenpos,currentpos-1,ERROR);
                        }
                        tokenpos=currentpos;
                    }
                    break;
                case 100:
                    iserror=true;
                    break;
                case 1:
                    currentpos++;
                    if(now=='"')
                        state=3;
                    else if(now=='\\')
                        state=2;
                    else if(now=='\'')
                    {
                        //error;
                        state=100;
                        iserror=true;
                    }else  state=3;
                    break;
                
                case 2:
                    currentpos++;
                    if(now=='"')
                    {
                        //error;
                        state=100;
                        iserror=true;
                    }else state=3;
                    break;
                case 3:
                
                    if(now=='\'')
                    {
                        inserttoken(program,tokenpos,currentpos,CHAR_TOKEN);
                        state=0;
                        tokenpos=currentpos+1;
                    }
                    else { 
                        //error;
                        state=100;
                        iserror=true;
                    }
                    currentpos++;
                    break;

                case 4:
                    if(now=='\\')
                    {
                        state=5;
                    }else if(now=='"')
                    {
                        inserttoken(program,tokenpos,currentpos,STRING);
                        state=0;
                        tokenpos=currentpos+1;
                    }
                    currentpos++;
                    break;
                case 5:
                    if(now=='\'')
                    {
                        //error
                        state=100;
                        iserror=true;
                    }
                    else {
                        state=4;
                    }
                    currentpos++;
                    break;

                case 6:
                    if(!isdigit(program[currentpos]))
                    {
                        state=0;
                        inserttoken(program,tokenpos,currentpos-1,NUMBER); 
                        tokenpos=currentpos;
                    }else currentpos++;
                    if(currentpos==plength)
                        inserttoken(program,tokenpos,currentpos-1,NUMBER);//in case line ends;
                    break;
                case 7:
                    if(isalpha(now)||isdigit(now)||now=='_')
                        currentpos++;
                    else {

                        inserttoken(program,tokenpos,currentpos-1,checkword(program.substr(tokenpos,currentpos-tokenpos)));   
                        state=0;
                        tokenpos=currentpos;
                    }
                    if(currentpos==plength)
                        inserttoken(program,tokenpos,currentpos-1,checkword(program.substr(tokenpos,currentpos-tokenpos)));   
                    break;
                case 8:
                    if(program[currentpos]!='*'&& program[currentpos]!='/')
                    {
                        inserttoken(program,tokenpos,currentpos-1,SLASH);
                        tokenpos=currentpos;
                        state=0;
                    }else if(now=='/')
                    {
                        currentpos=plength; 
                    }else {
                        state=9;
                        currentpos++;
                    }
                    break;
                case 9:
                    while(now!='*'&& currentpos<plength)
                    {
                        now=program[++currentpos];
                    }
                    if(now=='*')
                    {
                        state=10;
                        currentpos++;
                    }else {
                        commentnotfinished=true;
                    }
                    break;
                case 10:
                    if(now=='*')
                    {

                    }
                    else if(now=='/')
                    {
                        state=0;
                        commentnotfinished=false;
                        tokenpos=currentpos+1;
                    }else{
                        state=9;
                    }
                    currentpos++;
                    break;
            }
        }
    }
    vector<pair<string,TokenType>>::iterator ite=Token.begin();
    for(;ite!=Token.end();ite++)
    {
        cout<<ite->first<<"----";
        printtokentype(ite->second);
        cout<<endl;
    }
    if(iserror)
        cout<<"error happened in line:"<<lineno<<endl;
    file.close();
    return 0;
}

void inserttoken(string program,int tokenpos, int currentpos,TokenType type)
{
    pair<string,TokenType> token;
    string temp(begin(program)+tokenpos,begin(program)+currentpos+1);
    token.first=temp;
    token.second=type;
    Token.push_back(token);
}

TokenType checkword(string a)
{
    for(int i=0;i<MAXRESERVED;i++)
    {
        if(strcmp(a.c_str(),reservedWords[i].str.c_str())==0)
            return reservedWords[i].token;
    }
    return ID;
}
