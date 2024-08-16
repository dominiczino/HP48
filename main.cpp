#include <iostream>
#include <stack>
#include <functional>
#include <vector> // Include for std::vector
#include <math.h>

// MyClass definition (same as before)
class MyClass {
private:
    float value; // Example attribute

public:
    MyClass(float val) : value(val) {}
    float getValue() const { return value; }
    // You can define other methods as needed
};

// Template class definition for MyClass
template<typename T>
class LineItemSpecific {
private:
    T value; // Store a value of type T

public:
    LineItemSpecific(T val) : value(val) {} // Constructor accepting a value of type T

    T getValue() const { return value; } // Getter method for value
    // You can define other methods as needed
};

    // Create instances of MyClass with different types
    //MyClass<int> obj1(42);

// MyStack definition (same as before)
class MyStack {
private:
    std::stack<MyClass> items;

public:
    bool isEmpty() const {
        return items.empty();
    }

    void push(const MyClass& item) {
        items.push(item);
    }

    MyClass pop() {
        if (!isEmpty()) {
            MyClass topItem = items.top();
            items.pop();
            return topItem;
        } else {
            throw std::out_of_range("pop from empty stack");
        }
    }

    MyClass& peek() {
        if (!isEmpty()) {
            return items.top();
        } else {
            throw std::out_of_range("peek from empty stack");
        }
    }

    void printStack() const {
        std::cout << "Stack Contents (top to bottom): ";
        std::stack<MyClass> temp = items; // Create a copy of items
        while (!temp.empty()) {
            std::cout << temp.top().getValue() << " ";
            temp.pop();
        }
        std::cout << std::endl;
    }

    void swapTopTwo() {
        if (items.size() < 2) {
            std::cerr << "Error: Stack has fewer than two elements." << std::endl;
            return;
        }

        // Pop the top two elements
        MyClass top1 = pop();
        MyClass top2 = pop();

        // Push them back in swapped order
        push(top1);
        push(top2);
    }

    size_t size() const {
        return items.size();
    }
};

// execute function using std::vector
void execute(MyStack* stackPtr, int N, std::function<MyClass(std::vector<MyClass>&)> func) {
    if (stackPtr->size() < N) {
        std::cerr << "Error: Stack does not have enough items to pop." << std::endl;
        return;
    }

    // Prepare arguments by popping N items from the stack into a vector
    std::vector<MyClass> args;
    for (int i = 0; i < N; ++i) {
        args.push_back(stackPtr->pop());
    }

    // Call the provided function with popped items as arguments
    MyClass result = func(args);

    // Push the returned value back onto the stack
    stackPtr->push(result);
}



// Example function that takes std::vector<MyClass> and returns a MyClass instance
MyClass add(std::vector<MyClass>& args) {
    float sum = 0;
    for (const auto& obj : args) {
        sum += obj.getValue();
    }
    return MyClass(sum);
}

MyClass mult(std::vector<MyClass>& args) {
    float prod = 1;
    for (const auto& obj : args) {
        prod *= obj.getValue();
    }
    return MyClass(prod);
}

MyClass exponentiate(std::vector<MyClass>& args) {
    return std::pow(args[0].getValue(),args[1].getValue());
}


float getInputNumber() {
    float num;
    std::cin >> num;
    return num;
}


int main() {
    MyStack stack;
    stack.push(MyClass(1));
    stack.push(MyClass(2));
    stack.push(MyClass(3));
    stack.printStack();

    // Execute exampleFunction with N = 3
    execute(&stack, 2, add);
    stack.printStack();
    stack.swapTopTwo();
    stack.printStack();
    execute(&stack, 2, mult);
    stack.printStack();
    stack.push(-1);
    stack.push(getInputNumber());
    stack.swapTopTwo();
    stack.printStack();
    execute(&stack, 2, exponentiate);
    stack.printStack();

    // Print the result (should be 6 if exampleFunction sums the values)
    std::cout << "Result after execution: " << stack.pop().getValue() << std::endl;

    return 0;
}