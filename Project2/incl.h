#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <set>
#include <map>


using namespace std;

extern bool imp;

enum Type { VAR, NOT, AND, OR, IMP, EQU, LPAREN, RPAREN };

struct Token { Type type; char c; };
struct Var { char c; bool bit; };
struct Step { string expr; bool bit; };
struct Implicant { int mask; int value; bool combined; };

inline bool CheckSetBitCount(int n) {
    int count = 0;
    while (n) {

        count += n & 1;
        n >>= 1;

    }
    return count != 1 ? false : true;
}

inline bool CanMatch(Implicant a, Implicant b){
    if (a.mask != b.mask)return false;
    int n = (a.value ^ b.value) & a.mask;
    if (!CheckSetBitCount(n))return false;
    return true;
}

inline vector<Implicant>GetPrimeImplicants(vector<unsigned int>& minterms, int numVars) {

    vector<Implicant> primeImplicants;
    vector<Implicant>first; 
    for (unsigned int m : minterms) {
        Implicant i; 
        i.mask = (1 << numVars) - 1;
        i.value = m;
        i.combined = false;
        first.push_back(i);
    }
   
    do {
        vector<Implicant>next; 
        for (unsigned int i = 0; i < first.size(); i++) {
            for (unsigned int j = i + 1; j < first.size(); j++) {
                if (CanMatch(first[i], first[j])) {
                    Implicant combined;
                    combined.mask = first[i].mask & ~((first[i].value ^ first[j].value) & first[i].mask);
                    combined.value = combined.mask & first[i].value;
                    combined.combined = false;
                    first[i].combined = first[j].combined = true;

                    bool duplicate = false;
                    for (auto& s : next)
                        if (s.mask == combined.mask && s.value == combined.value) { duplicate = true; break;};

                    if (!duplicate)next.push_back(combined);

                    



                }
            }
            

        }
        for (unsigned int i = 0; i < first.size(); i++) {
            if (!first[i].combined)primeImplicants.push_back(first[i]);
        }

        first = next;





    } while (!first.empty());






      return primeImplicants;
}

inline bool CheckCanon(vector<Implicant>& implicants) {
    if (implicants.size() != 2) return false;

    Implicant& a = implicants[0];
    Implicant& b = implicants[1];

    
    if (a.mask != b.mask) return false;

    
    bool cond1 = (a.value == 0 && b.value == a.mask);
    bool cond2 = (b.value == 0 && a.value == a.mask);

    return cond1 || cond2;
}

inline string MinExpressionToString(vector<Implicant>& implicants, vector<char>& vars) {

    if (implicants.empty()) return "false";
    

    int varsCount = vars.size();
    vector<string> terms;
    
    bool c = CheckCanon(implicants);

    for (auto& imp : implicants) {
        vector<string> literals;
        ImGui::DebugLog("%d", imp.value);
        for (int i = 0; i < varsCount; i++) {
            int n = varsCount - 1 - i;
            if (!((imp.mask >> n) & 1)) continue;
            if ((imp.value >> n) & 1)literals.push_back(string(1, vars[i]));
            else literals.push_back("~" + string(1, vars[i]));
            
        }
        if (literals.empty()) { terms.push_back("true"); continue; }
        string term = literals[0];
        for (int i = 1; i < literals.size(); i++)
            term += " & " + literals[i];
        terms.push_back(literals.size() > 1 ? "(" + term + ")" : term);
    }

    string result = terms[0];
    for (int i = 1; i < terms.size(); i++)result += " | " + terms[i];

    if (c) {
        vector<int> active;
        for (int i = 0; i < varsCount; i++) {
            int n = varsCount - 1 - i;
            if ((implicants[0].mask >> n) & 1)
                active.push_back(i);
        }

        if (active.size() == 2) {
            return string(1, vars[active[0]]) + "==" + string(1, vars[active[1]]);
        }
    }

    return result;
}




//realiza a operação bit a bit dependendo do operador
inline bool BitWiser(Type type, bool a, bool b) {
    switch (type) {
    case AND: return a && b;
    case OR:  return a || b;
    case IMP: return !a || b;
    case EQU: return a == b;
    default:  return false;
    }
};

inline vector<Step> EvaluateRPN(vector<Token>& ptokens, vector<Var>& vars) {

    vector<Step> stack;
    vector<Step> steps;

    for (auto& t : ptokens) {

        if (t.type == VAR) {
            for (auto& v : vars) {
                if (t.c == v.c) {
                    stack.push_back({ string(1, v.c), v.bit });

                    break;
                }
            }
        }

        else if (t.type == NOT) {
            auto a = stack.back(); stack.pop_back();

            Step res;
            res.expr = "~" + a.expr;
            res.bit = !a.bit;

            stack.push_back(res);
            steps.push_back(res);
        }

        else {
            auto b = stack.back(); stack.pop_back();
            auto a = stack.back(); stack.pop_back();

            string op;
            switch (t.type) {
            case AND: op = "&"; break;
            case OR:  op = "|"; break;
            case IMP: op = "->"; break;
            case EQU: op = "=="; break;
            default:  op = "?";
            }

            Step res;
            res.expr = "(" + a.expr + " " + op + " " + b.expr + ")";
            res.bit = BitWiser(t.type, a.bit, b.bit);

            stack.push_back(res);
            steps.push_back(res);
        }
    }

    return steps;
}

inline int Precedence(Type type) {
    switch (type) {
    case NOT: return 4;
    case AND: return 3;
    case OR:  return 2;
    case IMP: return 1;
    case EQU: return 0;
    default:  return -1;
    }
}

//Conversão para notação pós-fixa pois usaremos o algoritmo Shunting Yard.
inline vector<Token> RPN(const vector<Token>& tokens) {

    vector<Token> output; 
    vector<Token> stack; //array dinamico temporario com o contexto de pilha. operadores ficam aqui até serem avaliados.

    for (auto& token : tokens) {

        if (token.type == VAR)
            output.push_back(token); //variavel sempre vai para o vetor de saída pois não possui ordem de precedencia

        else if (token.type == AND || token.type == OR || (token.type == IMP && !imp) || token.type == EQU) {

            while (!stack.empty() && Precedence(stack.back().type) >= Precedence(token.type)) {  //checa se os operadores recentemente adicionados possuem maior precedencia que o atual. 
                output.push_back(stack.back()); //se sim, enviamos para a saída e tiramos do topo da pilha 
                stack.pop_back(); 
            }

            stack.push_back(token); //adicionamos o atual.
        }

        //nots consecutivos deu zika quando estava na condicional acima. Como o ~ vem antes da variavel, ele é avaliado primeiro 2 vezes, a condição >= enviava um not para o output antes da variavel
        //Ex do problema citado. ~~a, primeiro ~ vai para a stack. Segundo ~ é comparado com o da stack, >= valida e manda o da stack para o output. no fim o output fica ~a~. aí quebra o algoritmo kk

        else if (token.type == NOT)stack.push_back(token);

        else if (token.type == IMP && imp) {

            while (!stack.empty() && Precedence(stack.back().type) > Precedence(token.type)) {  
                output.push_back(stack.back()); 
                stack.pop_back();
            }

            stack.push_back(token); 
        }


          
        
        //como tem a precedencia -1, o operador anterior entra em espera até encontrar o fechamento de parenteses e descarta-los.
        else if (token.type == LPAREN)
            stack.push_back(token);

        //econtrou o fechamento de parenteses. pega tudo antes até achar a abertura. envia imediatamente para o output. 
        else if (token.type == RPAREN) { 
            while (!stack.empty() && stack.back().type != LPAREN) {
                output.push_back(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
        }
    }
    //o que permaneceu vai embora para o output
    while (!stack.empty()) {
        output.push_back(stack.back());
        stack.pop_back();
    }

    return output;
}


//tokeniza as variaveis e os operadores de acordo com o enum Type
//retorna o array dinamico com cada elemento contendo o tipo (e o caractere se for uma variavel).
//Ex de retorno: in (a & b) out {{6,0}, {0, a}, {2, 0}, {0, b}, {7, 0}
inline vector<Token> Tokenize(string expr) {

    vector<Token> tokens;

    for (unsigned int i = 0; i < expr.size(); i++) {

        char c = expr[i]; //cada caractere encontrado na expressão

        if (isblank(c)) continue; //ignoramos espaçamentos
        else if (isalpha(c)) tokens.push_back({ VAR, c }); //se for uma letra do alfabeto, é uma variavel. incluir caractere
        else if (c == '~') tokens.push_back({ NOT, 0 });
        else if (c == '&') tokens.push_back({ AND, 0 });
        else if (c == '|') tokens.push_back({ OR, 0 });
        else if (c == '(') tokens.push_back({ LPAREN, 0 });
        else if (c == ')') tokens.push_back({ RPAREN, 0 });

        else if (c == '-' && i + 1 < expr.size() && expr[i + 1] == '>') { tokens.push_back({ IMP, 0 }); i++; } //se encontrar '-' verificar se proximo caractere é '>'
        else if (c == '=' && i + 1 < expr.size() && expr[i + 1] == '=') { tokens.push_back({ EQU, 0 }); i++; }; //se encontrar '=' verificar se proximo caractere é '='


    }

    return tokens;
}