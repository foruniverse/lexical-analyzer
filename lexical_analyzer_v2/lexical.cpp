#include"lexical.h"

void Scan::printRes()
{
    for_each(token_list.begin(),token_list.end(),printToken);
}

void Scan::insertToken(std::string line,int left,int right,TokenType type)
{
    Token token(line,left,right,type);
    this->token_list.push_back(token);
};
 
//对源代码按行处理
void Scan::lexicalAnalyzer()
{
    //每行,大概用line标识更贴切
    string program;
    //dfa 初始状态位0
    int state=0;
    //读取的每行行数
    int plength;
    //记录当前处理到每行的第几位
    int currentpos;
    //记录上次token处理后的位置,即下个token的开始位置
    int tokenpos;
    //错误行号,遇到不符合词法规则就退出,并显示错误行号
    int lineno=0;
    //当前字符
    char now;
    //用于处理多行注释的状态位
    bool commentnotfinished=false;
    //错误状态位,为真则退出
    bool iserror=false;
    //按行处理
    /*
     *一下部分为自动机的主流程,按照自动机的经典结构实现
        while()
        {
        switch()
            state a:
                jump c;
                break
            state b:

            state c:
                jump b;
        }
        自动机的状态索引没有采用状态表,效率有所下降,但随之而来的是代码可阅读性大幅加强
        
        具体的状态转移参考附带的DFA状态转移图

        代码阅读应仔细参考附带的画的DFA,否则可谓是鬼神莫识(  ^ ω ^  )
        
        DFA代码均手工编写,未采用任何自动工具,这点从编码风格也可看出
        
        但实际上,这类DFA代码均可由"正则表达式"->thompson算法->子集构造法->最小化DFA得到
     */
    while(getline(input,program))
    {
        if(iserror)// 检测到错误词法就跳出
            break;
        plength=program.size();//使用临时变量储存行大小.节省函数调用开销
        //初始位置为0,由于大部分时候解析都在行尾结束,每次进入新循环时都要重置
        currentpos=0;
        //每次解析出一个token后,就将tokenpos置为token后的下一索引处
        tokenpos=0;
        //储存行号,便于报错
        lineno++;
        //初始状态,这是状态机中最重要的变量,所有的状态跳转都由它来中转
        state=0; 
        //用于多行注释,这就是上文提到的特殊情况,在这种情况下,解析并不以行尾结束而结束
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
                //初始状态为0,按照DFA构建第一层状态转化
                case 0:
                    currentpos++;
                    if(isdigit(now))
                    {
                    //状态转移
                        state=6;
                    }
                    else if(now==' ')
                    {
                        tokenpos++;
                    }else if(now=='\t')
                    {
                        //tab 和 \n 符都需要单独避开
                        tokenpos++;
                    }else if(now=='\n')
                    {
                        state=100;
                    }else if(isalpha(now)|| now=='_')
                    {
                        state=7;
                    }else if(now=='/')
                    {
                        state=8;
                        //到达行尾,这是'/'不会再向当行注释和多行注释转换
                        if(currentpos==plength)
                            insertToken(program,tokenpos,currentpos-1,SLASH);
                    }else if(now=='\'')
                    {
                        state=1;
                    }else if(now=='"')
                    {
                        state=4;
                    }else
                    {
                        //排除上述后，即是单目运算符,直接插入token
                        state=0;
                        switch(now)
                        {
                            case '+':
                                insertToken(program,tokenpos,currentpos-1,ADD);
                                break;
                            case '-':
                                insertToken(program,tokenpos,currentpos-1,MINUS);
                                break;
                            case '*' :
                                insertToken(program,tokenpos,currentpos-1,MULTIPLY);
                                break;
                            case '(':
                                insertToken(program,tokenpos,currentpos-1,LPAREN);
                                break;
                            case ')':
                                insertToken(program,tokenpos,currentpos-1,RPAREN);
                                break;
                            case '{':
                                insertToken(program,tokenpos,currentpos-1,LBRACKET);
                                break;
                            case '}':
                                insertToken(program,tokenpos,currentpos-1,RBRACKET);
                                break;
                            case ';':
                                insertToken(program,tokenpos,currentpos-1,SEMICOLON);
                                break;
                            case ',':
                                insertToken(program,tokenpos,currentpos-1,COMMA);
                                break;
                            case '>':
                                insertToken(program,tokenpos,currentpos-1,GREATER);
                                break;
                            case '<':
                                insertToken(program,tokenpos,currentpos-1,LESS);
                                break;
                            case '=':
                                insertToken(program,tokenpos,currentpos-1,ASSIGN);
                                break;
                            default:
                                state=100;
                                insertToken(program,tokenpos,currentpos-1,ERROR);
                        }
                        tokenpos=currentpos;
                    }
                    break;
                //错误状态位
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
                //这些状态位要参考DFA图,重要重要重要重要
                    if(now=='\'')
                    {
                        insertToken(program,tokenpos,currentpos,CHAR_TOKEN);
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
                        insertToken(program,tokenpos,currentpos,STRING);
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
                        insertToken(program,tokenpos,currentpos-1,NUMBER); 
                        tokenpos=currentpos;
                    }else currentpos++;
                    if(currentpos==plength)//在这种情况下,遇到行尾就终止解析,但不是所有情况
                        insertToken(program,tokenpos,currentpos-1,NUMBER);//in case line ends;
                    break;
                case 7:
                    if(isalpha(now)||isdigit(now)||now=='_')
                        currentpos++;
                    else {

                        insertToken(program,tokenpos,currentpos-1,ID);   
                        state=0;
                        tokenpos=currentpos;
                    }
                    if(currentpos==plength)
                        insertToken(program,tokenpos,currentpos-1,ID);   
                    break;
                case 8://解析多行注释
                    if(program[currentpos]!='*'&& program[currentpos]!='/')
                    {
                        insertToken(program,tokenpos,currentpos-1,SLASH);
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
                    }//遍历到行尾,或者遇到'*'字符,
                    if(now=='*')
                    {//跳转到下一个状态
                        state=10;
                        currentpos++;
                    }else {
                        //多行注释,无疑,在下一行继续上一行的解析 
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
        //错误处理则显示行号
    }
    if(iserror)
        std::cout<<"error happened in line :"<<lineno<<std::endl;
};