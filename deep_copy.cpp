#include <iostream>
#include <utility> // Needed for std::move

class ShallowClass { // Now a robust, safe class!
public:
    int* data;      
    int safeValue;  

    // 0. Standard Constructor
    ShallowClass(int pointerVal, int normalVal) {
        data = new int(pointerVal);
        safeValue = normalVal;
        std::cout << "[Constructor] Allocated memory at " << data << "\n";
    }

    // =========================================================================
    // THE RULE OF FIVE
    // =========================================================================

    // 1. Destructor
    ~ShallowClass() {
        if (data != nullptr) {
            std::cout << "[Destructor] Deleting memory at " << data << "\n";
            delete data;
            data = nullptr; // Clear to prevent accidental reuse
        } else {
            std::cout << "[Destructor] Nothing to delete (pointer is null)\n";
        }
    }

    // 2. Deep Copy Constructor
    ShallowClass(const ShallowClass& other) {
        safeValue = other.safeValue;
        if (other.data != nullptr) {
            data = new int(*other.data); // ✅ Allocate BRAND NEW heap memory
        } else {
            data = nullptr;
        }
        std::cout << "[Copy Constructor] Deep copied to new address " << data << "\n";
    }

    // 3. Deep Copy Assignment Operator
    ShallowClass& operator=(const ShallowClass& other) {
        std::cout << "[Copy Assignment] Executing...\n";
        if (this == &other) {
            return *this; // Protect against self-assignment (e.g., obj1 = obj1)
        }

        delete data; // Free existing heap resource to prevent memory leaks

        safeValue = other.safeValue;
        if (other.data != nullptr) {
            data = new int(*other.data); // ✅ Allocate brand new heap memory
        } else {
            data = nullptr;
        }
        return *this;
    }

    // 4. Move Constructor
    ShallowClass(ShallowClass&& other) noexcept {
        safeValue = other.safeValue;
        data = other.data;        // ⚡ Steal the pointer directly (no allocation needed!)
        
        other.data = nullptr;     // ⚡ CRITICAL: Nullify old pointer so its destructor won't free it
        other.safeValue = 0;
        std::cout << "[Move Constructor] Stole pointer address " << data << "\n";
    }

    // 5. Move Assignment Operator
    ShallowClass& operator=(ShallowClass&& other) noexcept {
        std::cout << "[Move Assignment] Executing...\n";
        if (this == &other) {
            return *this; // Self-assignment check
        }

        delete data; // Free current heap resource before taking the new one

        safeValue = other.safeValue;
        data = other.data;        // ⚡ Steal the pointer
        
        other.data = nullptr;     // ⚡ CRITICAL: Nullify old pointer
        other.safeValue = 0;
        return *this;
    }
};

int main() {
    std::cout << "--- Creating obj1 ---\n";
    ShallowClass obj1(42, 100);

    std::cout << "\n--- Creating obj2 (Deep Copy Constructor) ---\n";
    ShallowClass obj2 = obj1; // Calls Copy Constructor

    std::cout << "\n--- Verifying Separation ---\n";
    *obj2.data = 99;
    std::cout << "obj1 data value: " << *obj1.data << " (Stays 42! Safe) ✅\n";
    std::cout << "obj2 data value: " << *obj2.data << " (Becomes 99) ✅\n";

    std::cout << "\n--- Creating obj3 and using Move Constructor ---\n";
    // std::move forces the compiler to treat obj1 as a temporary rvalue, triggering a move
    ShallowClass obj3 = std::move(obj1); 

    std::cout << "\n--- Exiting scope (No crash, no double-free!) ---\n";
    return 0;
}
