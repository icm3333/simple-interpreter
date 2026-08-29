/*
	Trying to build an interpreter for LISP Style (prefix notation) Simple arithmetic.

	Steps to do this project:
		- Lexical Analysis (Tokenizer)
		- Syntactic Analysis (Parsing)
		- Evaluation (Execution)
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <memory>

enum TokenType : uint8_t{
	TOKEN_OP = 0,
	TOKEN_NUM = 1,
	TOKEN_PAREN = 2
};

struct Token{
	TokenType type;
	int value;
};

struct Node{
	virtual ~Node() = default;
	virtual int evaluate() const = 0;
};

struct NumNode : public Node{
	int value;
	NumNode(int val) : value(val){}

	int evaluate() const override {return value;}
};

struct OpNode : public Node{
	char op;
	std::vector<std::unique_ptr<Node>> children;
	OpNode(char operation) : op(operation){}

	int evaluate() const override {

		if(children.empty()) return 0;

		switch(op){
			case '+': {
				int sum = 0;
				for(const auto& c : children){
					sum += c->evaluate();
				}
				return sum;
			} case '-': {
				int total = children[0]->evaluate();
				for(size_t i=1; i<children.size(); ++i){
					int next = children[i]->evaluate();
					total -= next;
				}
				return total;
			} case '/': {
				int total = children[0]->evaluate();
				for(size_t i = 1; i < children.size(); ++i){
					int next = children[i]->evaluate();

					// TODO: properly handle division by 0
					if(next != 0) total /= next;
				}
				return total;
			} case '*': {
				int total = 1;
				for(const auto& c : children) total *= c->evaluate();	
				return total;

			} default :
				  return 0;
		}
	}
};


class Interpreter{
	private:
	  std::string source;
	  std::vector<Token> tokens;

	public:
	  Interpreter(const std::string& text) : source(text){}

	  void tokenize(){
	  	for(size_t i = 0; i < source.size(); i++){
			char c = source[i];

			if(c != ' '){
				if(c >= '0' && c <= '9'){
					// check future indexes till space or brackets.
					int j = i;
					while( j+1 < source.size() &&
					       source[j+1] != ' '  && 
					       source[j+1] != '('  &&
					       source[j+1] != ')'     ) j++;

					// transform the string into int.
					int num = std::stoi(source.substr(i, (j-i+1)));
					
					// push the number into the token vector
					Token tkn = {TOKEN_NUM, num};
					tokens.push_back(tkn);
					i = j;

				}else if(c == '(' || c == ')'){
					Token tkn = {TOKEN_PAREN, (int)c};
					tokens.push_back(tkn);
				}else if(c == '+' || c == '-' || c == '*' || c == '/'){
					Token tkn = {TOKEN_OP, (int)c};
					tokens.push_back(tkn);
				}
			}
		}
	  }
	  
	  void printTokens(){
	  	for(const auto& t : tokens){
			std::cout << "Type: " << (int) t.type << ", Value: " << t.value;
			if(t.type == TOKEN_OP || t.type == TOKEN_PAREN){
				std::cout << " ('" << (char)t.value << "') ";
			}
			std::cout << "\n";
		}
	  }

	  // TODO: Create an Abstract Syntax Tree to evaluate the expression

	  void interpret(){
	  	tokenize();
		printTokens();
	  }

};

int main(){
	std::string code = "(+ 1 2(* 1 33))";
	Interpreter myInterpreter(code);
	myInterpreter.interpret();
}
