#include <format>
#include <iostream>
#include <set>
#include <vector>
#include <variant>

namespace  {
    struct OpStack{
        double workingValue=1;
        char prevState=' ';
        double value=0;
        bool finished=false; 
        OpStack(){};   
        OpStack(double c,double v): workingValue(c),value(v){}    
    };

    class SerialCalculator {
    private:
        double total=0;

        double working_value;
        char previous_state;
        std::vector<OpStack> stack;
    public:
        SerialCalculator(){
            working_value=0;
            previous_state='+';
            stack.emplace_back();
        }    
        const std::set<char> supported_operations = {'+','-','*','/','(',')'};
        
        bool Next(const char operation){

            switch (operation)
            {                
            case '-':
                stack.back().value+= working_value;
                working_value=0;
                previous_state=previous_state=='-'?'+':operation;
                break;
              
            case '+':
                //adding finishes the previous working operation
                stack.back().value+= working_value;
                working_value=0;
                previous_state=operation;
                break;

            case '*':
            case '/':
                previous_state=operation;
                break;
            case '(':                
                if (working_value==0.0) 
                    stack.back().workingValue=(previous_state=='-')?-1:1;
                else 
                    stack.back().workingValue=working_value;
                stack.back().prevState=previous_state;

                stack.emplace_back(0,0);
                previous_state=operation;
                working_value=0;
                break;
            case ')':
                OpStack& st=stack.back();
                st.value+=working_value;
                working_value=st.value;
                previous_state=operation;
                stack.pop_back();  
                if (stack.back().prevState=='/')
                    working_value= stack.back().workingValue/working_value;
                else 
                    working_value= stack.back().workingValue*working_value;
                stack.back().workingValue=0;
                break;
            }
            return true;
        }
        
        bool Next(const int number) {
            switch (previous_state)
            {
            case '-':
                working_value=-number;
                break;
            case '(': 
            case '+':
                working_value=number;
                break;
            
            case ')':
            case '*':
                working_value*=number;
                break;

            case '/':
                working_value/=number;
                break;
          
            default:
                break;
            }  
            previous_state=' ';
            return true;
        }

        int End() {
            stack.back().value+=working_value;
            
            while (stack.size() > 1) {
                OpStack& st=stack.back();
                working_value=st.value;
                stack.pop_back();
                
                if (stack.back().prevState=='/')
                    stack.back().value+=stack.back().workingValue/working_value;
                else
                    stack.back().value+=working_value*stack.back().workingValue;
            }

            total=stack.back().value;

            stack.back().value=0;
            working_value=0;

            return total;            
        };
    };

    struct ReadVal{
        bool isInt=true;
        int intVal=0;
        char charVal=' ';
    };

    class InputLexer{
    private:
        std::istream& stream;
        int ReadInt(){
            int i;
        }
    public:        
        ReadVal val={};
        
        InputLexer(std::istream& streamObject):stream(streamObject){}
        bool end;
        bool nextToken(){
            end=false;
            char c=static_cast<char>(stream.peek());
            if (c == 'q'){
                end=true;
                return false;
            }
            if (c == '\n' ){
                stream.get();
                return false;
            }
            if(c == EOF){
                return false;
            }
                
            while (c==' ' || c=='\t'){
                stream.get();
                c=static_cast<char>(stream.peek());
            };

            if ((c >= '0' && c <= '9')){
                val.isInt=true;
                stream>>val.intVal;
                return true;
            } 
            else if (c == '+' || c == '-' ||
            c == '*' || c == '/' ||
            c == '(' || c == ')') {
                val.isInt=false;
                stream>>val.charVal;
                return true;
            }
            else throw std::runtime_error("Invalid character in input.");
        }
    };
    
    class Dispacher{
    private:
        SerialCalculator sc;
        InputLexer il;
    public:
        Dispacher(): sc(),il(std::cin){}
        
        bool Run(){
            std::cout<<"Expression: ";
            try{
                 while (il.nextToken()){                
                    if (il.val.isInt) {
                        sc.Next(il.val.intVal);
                    }                
                    else {
                        sc.Next(il.val.charVal);                   
                    }    
                }

            }
            catch (const std::exception& e) {            
                std::cout << e.what() << '\n';
                return false;
            }            
            if (il.end) return false;
            
            std::cout<<"Result: "<<sc.End()<<'\n';
            return true;

        }


    };
}

int main() {
    Dispacher d;
    while (d.Run());
    
}