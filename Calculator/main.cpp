#include <format>
#include <iostream>
#include <set>
#include <vector>

namespace  {
    struct OpStack{
        double coeff=1;
        double value=0;
        bool finished=false; 
        OpStack(){};   
        OpStack(double c,double v): coeff(c),value(v){}    
    };

    class SerialCalculator {
    private:
        double total=0;

        double working_value;
        char previous_state;
        std::vector<OpStack> stack;
    public:
        SerialCalculator(){
            previous_state='+';
            stack.emplace_back();
        }    
        const std::set<char> supported_operations = {'+','-','*','/','(',')'};
        
        bool Next(const char operation){

            previous_state=operation;
            switch (operation)
            {                
            case '-':
            case '+':
                //adding finishes the previous working operation
                stack.back().value+= working_value;
                working_value=0;
                previous_state=operation;
                break;

            case '*':
            case '/':
                //I don't need to take any action 
                break;
            case '(':
                if (working_value==0.0) 
                    stack.emplace_back(1,0);
                else 
                    stack.emplace_back(working_value,0);

                working_value=0;
                break;
            case ')':
                OpStack& st=stack.back();
                st.value+=working_value;
                working_value=st.coeff*st.value;
                stack.pop_back();
                stack.back().value+=working_value;
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
            return true;
        }

        int End() {
            stack.back().value+=working_value;
            
            while (stack.size() > 1) {
                OpStack& st=stack.back();
                working_value=st.coeff*st.value;
                stack.pop_back();
                
                stack.back().value+=working_value;
            }

            total=stack.back().value;

            stack.back().value=0;

            return total;            
        };
    };
}

int main() {
    SerialCalculator sc;
    sc.Next(2);
    sc.Next('(');
    sc.Next(1);
    sc.Next('+');
    sc.Next('(');
    sc.Next(2);
    sc.Next('*');
    sc.Next(3);

    std::cout<<sc.End()<<std::endl;
}