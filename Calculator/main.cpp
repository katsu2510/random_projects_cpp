#include <format>
#include <iostream>
#include <set>
#include <set>

namespace  {
    class SerialCalculator {
    private:
        int total={};
        std::string message;
        int stack={};
    public:
        const std::set<char> supported_operations = {'+','-','*','/'};

        //returns True if the number was successfully counter towards the total.
        bool NextOp(const char operation,const int number) {
            if (!message.empty()) {message.clear();}

            if (!supported_operations.contains(operation)) {
                message =  std::format("Operation {} is not supported.",operation);
                return false;
            }
            switch (operation) {
                case '+':
                    total+=stack;
                    stack=number;
                    break;
                case '-':
                    total+=stack;
                    stack=-number;
                    break;
                case '*':
                    stack*=number;
                    break;
                case '/':
                    stack/=number;
                    break;
                default:
                    message =  std::format("Operation {} is not supported. Unknown error.",operation);
                    return false;
            }
            return true;
        }

        int Total(){
            message.clear();
            total+=stack;
            stack=0;
            return total;
        }
        int End() {
            int total_result=Total();
            total=0;
            message.clear();
            return total;
        };
    };
}

int main() {
    SerialCalculator sc;
    sc.NextOp('+',2);
    sc.NextOp('*',3);
    sc.NextOp('*',2);
    sc.NextOp('/',2);
    std::cout<<sc.Total()<<std::endl;
}