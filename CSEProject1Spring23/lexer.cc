/*
 * Copyright (C) Rida Bazzi
 *
 * Do not share this file with anyone
 */
/*
Students that worked on Project 1 
Trevor Ransom
Tanner Ward

*/
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM", "BASE16NUM" // TODO: Add labels for new token types here (as string)
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };
void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

// int ispdigit16(char t) {
// 	if (t >= '0' && t <= '9') {
// 		return 1;
// 	} else if (t >= 'A' && t <= 'F') {
// 		return 1;
// 	} else {
// 		return 0;
// 	}
// }

Token LexicalAnalyzer::ScanNumber()
{
    char c;

    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0') {
            tmp.lexeme = "0";
        }
        
         else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {     // while input is digit add to lexeme - Trevor R.
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) { 
                input.UngetChar(c);                         // put character that is not a digit back into input - Trevor R.
            }
        }
        input.GetChar(c);      
        if (c == '.') {                                     // checks if DOT is in c if so then more than likely REALNUM - Trevor R.
            input.GetChar(c);
            if (isdigit(c)) {                               // if DOT is followed by a digit it is a REALNUM - Trevor R.
                tmp.lexeme += '.';
                while (isdigit(c)) {
                    tmp.lexeme += c;
                    input.GetChar(c);
                }
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = REALNUM;

            } 
            else {                                          // if not a DOT and not at the end of input put the char back - Trevor R.
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                input.UngetChar('.');
                tmp.token_type = NUM;
            }
            return tmp;
        }
        // else if(ispdigit16(c))
        // {
        //     tmp.lexeme += c;
        //     input.GetChar(c);
        //     while(ispdigit16(c))
        //     {
        //         tmp.lexeme += c;
        //         input.GetChar(c);
        //     }
        //     if(!input.EndOfInput())
        //     {
        //         input.UngetChar(c);
            }
        }
        else if(c == 'x')                                   // if character is an x than check if it is a BASE08 num - Trevor R.
        {
            input.GetChar(c);
            if(c == '0')
            {
                input.GetChar(c);
                if(c == '8')                                // If the x is followed by 08 then it is a BASE08NUM - Trevor R.
                {
                    tmp.lexeme += 'x';
                    tmp.lexeme += '0';
                    tmp.lexeme += '8';
                    tmp.token_type = BASE08NUM;
                }
                else if(c != '8'){                          // If x0 is followed by something other than 8 it is not a BASE08NUM and may be an ID - Trevor R.
                    input.UngetChar(c);
                    input.UngetChar(c);
                    tmp.token_type = NUM;
                    if (!input.EndOfInput()) {
                        input.UngetChar('x');
                    }     
                }     
            }
            else if(c == '1')                               // Check if x is followed by a 16 to see if it is a BASE16NUM - Trevor R.
            {
                input.GetChar(c);
                if(c == '6')
                {
                    tmp.lexeme += 'x';
                    tmp.lexeme += '1';
                    tmp.lexeme += '6';
                    input.GetChar(c);
                    tmp.token_type = BASE16NUM;
                }
                if(!input.EndOfInput())
                {
                    input.UngetChar(c);
                }
                
            }
            
            else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
            }
        }
        else{                        // if no condition is met above it is a NUM - Trevor R.
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = NUM;
        }
    } 
    return tmp;
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
