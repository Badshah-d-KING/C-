#include <iostream>

class ShallowClass {
public:
    int* data;      // ❌ DANGEROUS: Heap pointer (causes shallow copy problems)
    int safeValue;  // ✅ SAFE: Simple variable (copied by value, completely safe)

    // Constructor: Initializes both variables
    ShallowClass(int pointerVal, int normalVal) {
        data = new int(pointerVal);
        safeValue = normalVal; // Plain assignment
    }

    // Destructor: Only deletes the heap pointer
    ~ShallowClass() {
        delete data; 
        // 'safeValue' needs no manual deletion; it cleans up automatically!
    }
};

int main() {
    std::cout << "--- Creating obj1 ---" << std::endl;
    ShallowClass obj1(42, 100);

    std::cout << "--- Creating obj2 (Shallow Copy) ---" << std::endl;
    ShallowClass obj2 = obj1; 

    // --- PROVING 'safeValue' IS COMPLETELY SAFE ---
    std::cout << "\n--- Checking the Simple Variable ---" << std::endl;
    std::cout << "obj1 safeValue: " << obj1.safeValue << std::endl; // Outputs 100
    std::cout << "obj2 safeValue: " << obj2.safeValue << std::endl; // Outputs 100

    // Modifying obj2's simple variable does NOT affect obj1
    obj2.safeValue = 500;
    std::cout << "\nAfter modifying obj2.safeValue to 500:" << std::endl;
    std::cout << "obj1 safeValue: " << obj1.safeValue << " (Stays 100! Safe) ✅" << std::endl;
    std::cout << "obj2 safeValue: " << obj2.safeValue << " (Becomes 500) ✅" << std::endl;

    // --- REMINDER OF THE POINTER PROBLEM ---
    std::cout << "\n--- Checking the Heap Pointer ---" << std::endl;
    *obj2.data = 99;
    std::cout << "obj1 data value: " << *obj1.data << " (Unexpectedly changed! Problematic) ❌" << std::endl;

    std::cout << "\n--- Exiting scope (Will still crash due to 'data' double-free) ---" << std::endl;
    return 0; 
}
