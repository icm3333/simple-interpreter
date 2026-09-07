/*
	TODO:
		- Error Handling
		- Division by 0
		- Input Validation
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
	  int current_token = 0;

	public:
	  Interpreter(const std::string& text) : source(text){}

	  void tokenize(){
	  	for(size_t i = 0; i < source.size(); i++){
			char c = source[i];

			if(c != ' '){
				if((c >= '0' && c <= '9') || (c == '-' && source[i+1] >= '0' && source[i+1] <= '9')){
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

	  std::unique_ptr<Node> parse(){
	  	if(current_token >= tokens.size()) return nullptr;

		Token current = tokens[current_token++];

		if(current.type == TOKEN_NUM) return std::make_unique<NumNode>(current.value);

		if(current.type == TOKEN_PAREN && current.value == '('){
			Token operatorToken = tokens[current_token++];
			auto node = std::make_unique<OpNode>((char)operatorToken.value);
			while(current_token < tokens.size() && !(tokens[current_token].type == TOKEN_PAREN && tokens[current_token].value == ')')){
				node->children.push_back(parse());
			}
			current_token++;
			return node;
		}
		return nullptr;
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

	  void interpret(){
	  	tokenize();
		printTokens();
		current_token = 0;

		std::unique_ptr<Node> root = parse();

		if(root){
			std::cout << root ->evaluate() << "\n";
		}else{
			std::cout << "Error \n";
		}
	  }

};

int main(){
	std::string buffer;
		
	while(true){
		std::cout << "> ";

		if(!std::getline(std::cin, buffer)) break;
		
		Interpreter a(buffer);
		a.interpret();

		std::cout << "\n";	
	}

	return 0;
}
