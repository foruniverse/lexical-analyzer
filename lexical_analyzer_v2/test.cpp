#include"lexical.cpp"
using namespace std;
int main()
{
    Scan scan("input.txt");
    scan.lexicalAnalyzer();
    scan.printRes();
    return 0;
}


