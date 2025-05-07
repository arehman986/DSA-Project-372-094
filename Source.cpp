// DSA == PROJECT 
//2023372 Muhammad Ali Rehman
//2023094 Ali Bin Farooq



#define NOMINMAX
#define BYTE_DEFINED
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <Windows.h>
#include <iomanip>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;


/////////////////////////////
//       String Class      //
/////////////////////////////

// A custom String class that handles basic string operations
// Made this because std::string was giving us headaches with the file handling
// Plus it's good practice to understand how strings work under the hood
class String
{
    char* data;      // Pointer to our character array
    int lengthof;    // Keeps track of string length
public:
    // Helper functions that mimic classic C string functions
    int Strlen(const char* str);
    void Strcpy(char* dest, const char* src);

    // Basic constructors & destructor
    String();                        // Default constructor - creates empty string
    String(const char* str);         // Creates string from char array
    String(const String& str);       // Copy constructor
    ~String();                       // Cleanup memory

    // Operator overloads for basic string operations
    String& operator=(const String& other);     // Assignment operator
    int length() const;                         // Get string length
    char& operator[](int index);               // Access individual chars
    String operator+(const String& other) const;  // Concatenate strings
    String& operator+=(const String& other);    // Append another string
    const char* string() const;                 // Get raw char array
    
    // Comparison operators
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;
    
    // Stream operators for I/O
    friend std::ostream& operator<<(std::ostream& os, const String& str);
    operator const char* () const;              // Convert to char array
    
    // String state checking
    bool empty() const;
    
    // Input operations
    friend std::istream& getline(std::istream& is, String& str);
    friend std::istream& operator>>(std::istream& is, String& str);
    
    // Search operations
    int find(const String& str) const;          // Find substring
    int find(char ch) const;                    // Find character
    
    // More comparison operators
    bool operator>(const String& other) const;
    bool operator<(const String& other) const;
    
    // String manipulation
    void clear();                               // Reset string to empty
    String& operator+=(char c);                 // Append single character
    
    // Conversion utilities
    String To_string(int value);                // Convert int to string
    static String to_string(const std::string& value) // Convert std::string
    {
        return String(value.c_str());
    }
    
    // Display utilities
    void printLine(int n, char c);              // Print n copies of char c
    
    // Conversion helpers for compatibility
    const char* c_str() const {                 // Get C-style string
        return data;
    }
    std::string to_std_string() const {         // Convert to std::string
        return std::string(c_str());
    }
};

// Creates an empty string - just null pointer and zero length
String::String()
{
    data = nullptr;
    lengthof = 0;
}

// Creates string from char array - allocates memory and copies data
String::String(const char* str)
{
    lengthof = Strlen(str);
    data = new char[lengthof + 1];  // +1 for null terminator
    Strcpy(data, str);
}

// Copy constructor - deep copy of another String
String::String(const String& str)
{
    lengthof = str.lengthof;
    data = new char[lengthof + 1];
    Strcpy(data, str.data);
}

// Cleanup our memory to avoid leaks
String::~String()
{
    delete[] data;
}

// Assignment operator - handles self-assignment and deep copy
String& String::operator=(const String& other)
{
    if (this == &other)
        return *this;

    delete[] data;
    lengthof = other.lengthof;
    data = new char[lengthof + 1];
    Strcpy(data, other.data);
    return *this;
}

// Returns current string length
int String::length() const
{
    return lengthof;
}

// Access individual characters - no bounds checking for speed
char& String::operator[](int index)
{
    return data[index];
}

// Concatenate two strings and return new String
String String::operator+(const String& other) const
{
    String newString;
    newString.lengthof = lengthof + other.lengthof;
    newString.data = new char[newString.lengthof + 1];

    // Copy first string
    for (int i = 0; i < lengthof; ++i) {
        newString.data[i] = data[i];
    }

    // Append second string
    for (int i = 0; i < other.lengthof; ++i) {
        newString.data[lengthof + i] = other.data[i];
    }

    newString.data[newString.lengthof] = '\0';
    return newString;
}

// Append another string to this one
String& String::operator+=(const String& other)
{
    char* temp = data;
    lengthof += other.lengthof;
    data = new char[lengthof + 1];
    Strcpy(data, temp);
    Strcpy(data + lengthof, other.data);
    delete[] temp;
    return *this;
}

// Check if two strings are exactly the same
bool String::operator==(const String& other) const
{
    if (lengthof != other.lengthof)
        return false;

    for (int i = 0; i < lengthof; i++)
    {
        if (data[i] != other.data[i])
            return false;
    }

    return true;
}

// Check if strings are different
bool String::operator!=(const String& other) const
{
    return !(*this == other);
}

// Print string to output stream
std::ostream& operator<<(std::ostream& os, const String& str)
{
    os << str.data;
    return os;
}

// Get length of C-style string
int String::Strlen(const char* str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

// Copy C-style string
void String::Strcpy(char* dest, const char* src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Convert to C-style string
String::operator const char* () const
{
    return data;
}

// Check if string is empty
bool String::empty() const
{
    return lengthof == 0 || data == nullptr || data[0] == '\0';
}

// Read line from input stream
std::istream& getline(std::istream& is, String& str)
{
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    if (is.getline(buffer, BUFFER_SIZE))
    {
        str = buffer;
    }
    else
    {
        str = "";
    }

    return is;
}

// Read word from input stream
std::istream& operator>>(std::istream& is, String& str)
{
    std::string temp;
    is >> temp;
    str = temp.c_str();
    return is;
}

// Find substring in string
int String::find(const String& str) const
{
    int n = strlen(data);
    int m = strlen(str.data);

    for (int i = 0; i <= n - m; ++i)
    {
        bool found = true;
        for (int j = 0; j < m; ++j)
        {
            if (data[i + j] != str.data[j])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return i;
        }
    }

    return -1;
}

// Find character in string
int String::find(char ch) const
{
    for (int i = 0; data[i] != '\0'; ++i)
    {
        if (data[i] == ch)
        {
            return i;
        }
    }
    return -1;
}

// Compare strings (greater than)
bool String::operator>(const String& other) const
{
    int i = 0;
    while (data[i] != '\0' && other.data[i] != '\0')
    {
        if (data[i] > other.data[i]) return true;
        if (data[i] < other.data[i]) return false;
        ++i;
    }
    return strlen(data) > strlen(other.data);
}

// Compare strings (less than)
bool String::operator<(const String& other) const
{
    int i = 0;
    while (data[i] != '\0' && other.data[i] != '\0')
    {
        if (data[i] < other.data[i]) return true;
        if (data[i] > other.data[i]) return false;
        ++i;
    }

    return strlen(data) < strlen(other.data);
}

// Reset string to empty state
void String::clear()
{
    delete[] data;  
    data = nullptr; 
    lengthof = 0;    
}

// Append single character
String& String::operator+=(char c)
{
    int len = Strlen(data);  
    char* newData = new char[len + 2]; 

    Strcpy(newData, data);
    newData[len] = c;
    newData[len + 1] = '\0';

    delete[] data;
    data = newData;

    return *this;
}

// Get underlying C string
const char* String::string() const
{
    return static_cast<const char*>(data);
}

// Convert integer to string
String String::To_string(int value)
{
    char buffer[12];  // Enough for 32-bit int
    int i = 0;

    // Handle negative numbers
    if (value < 0)
    {
        buffer[i++] = '-';
        value = -value;
    }

    // Count digits
    int temp = value;
    int length = 0;
    while (temp > 0)
    {
        length++;
        temp /= 10;
    }

    // Handle zero specially
    if (value == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        // Convert digits
        for (int j = length - 1; j >= 0; --j)
        {
            buffer[i + j] = (value % 10) + '0';
            value /= 10;
        }
        i += length;
    }

    buffer[i] = '\0';
    return String(buffer);
}

// Print a line of characters
void String::printLine(int n, char c)
{
    for (int i = 0; i < n; i++)
    {
        cout << c;
    }
}

/////////////////////////////////
//         AVL Class           //
/////////////////////////////////

class AVLNode
{
public:
    String filename;
    String key;
    String row;
    int height;
    String leftChildPath;
    String rightChildPath;

    // Constructor for AVLNode
    AVLNode(String f, String k, String r, const String& leftPath = "", const String& rightPath = "")
    {
        filename = f;
        key = k;
        row = r;
        height = 0;
        leftChildPath = leftPath;
        rightChildPath = rightPath;
    }

    // Overloaded constructor for AVLNode with height
    AVLNode(String f, String k, String r, const int& h, const String& leftPath = String(""), const String& rightPath = String(""))
    {
        filename = f;
        key = k;
        row = r;
        height = h;
        leftChildPath = leftPath;
        rightChildPath = rightPath;
    }
};

class AVLTree {
private:
    AVLNode* root;
    String current_branch;
    String repo_name;

    // Helper function to get the height of a node
    int height(AVLNode* node);

    // Helper function to create a file for a node
    void createFile(const String& filename, const String& key, const String& row, const String& leftChildPath, const String& rightChildPath, const int& height);

    // Helper function to perform a right rotation
    AVLNode* rightRotate(AVLNode* y);

    // Helper function to perform a left rotation
    AVLNode* leftRotate(AVLNode* x);

    // Helper function to get a node from its file path
    AVLNode* getNodeFromPath(const String& path);

    // Helper function to insert a node
    AVLNode* insert(AVLNode* node, const String& filename, const String& key, const String& row);

    // Helper function to delete a node
    AVLNode* deleteNode(AVLNode* node, const String& key);

    // Helper function to find the node with the minimum value
    String minValueNodePath(const String& path);

    // Helper function to get the balance factor of a node
    int getBalanceFactor(AVLNode* node);

public:
    // Constructor for AVLTree
    AVLTree();

    // Function to insert a node
    void insert(const String& filename, const String& key, const String& row);

    // Function to remove a node
    void removeNode(const String& key);

    // Function to update a node
    void updateNode(const String& oldKey, const String& newRow);

    // Helper function to search for a node by its key and return its file path
    String searchNodePath(AVLNode* node, const String& key);

    // Set the current branch
    void setCurrentBranch(const String& branch) {
        current_branch = branch;
    }

    // Set the repository name
    void setRepoName(const String& repo) {
        repo_name = repo;
    }

    // Get the current branch
    String getCurrentBranch() const {
        return current_branch;
    }

    // Get the repository name
    String getRepoName() const {
        return repo_name;
    }
};

// Get the height of a node
int AVLTree::height(AVLNode* node)
{
    return node ? node->height : -1;
}

// Constructor for AVLTree
AVLTree::AVLTree()
{
    root = nullptr;
    current_branch = "";
    repo_name = "";
}

// Create a file for a node
void AVLTree::createFile(const String& filename, const String& key, const String& row,
    const String& leftChildPath, const String& rightChildPath, const int& height) {

    fs::path full_path = fs::current_path() / "repositories" / static_cast<const char*>(repo_name) /
        static_cast<const char*>(current_branch) / "trees" / "avl" / static_cast<const char*>(filename);

    ofstream file(full_path);
    if (file.is_open()) {
        file << "Key: " << key << "\n";
        file << "Row: " << row << "\n";
        file << "Height: " << height << "\n";
        file << "LeftChildPath: " << leftChildPath << "\n";
        file << "RightChildPath: " << rightChildPath << "\n";
        file.close();
    }
    else {
        cerr << "Error creating file: " << full_path << endl;
    }
}

// Perform a right rotation
AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = getNodeFromPath(y->leftChildPath);
    AVLNode* T2 = getNodeFromPath(x->rightChildPath);

    // Perform rotation
    x->rightChildPath = y->filename;
    y->leftChildPath = T2 ? T2->filename : String("");

    // Update heights
    y->height = std::max(height(getNodeFromPath(y->leftChildPath)), height(getNodeFromPath(y->rightChildPath))) + 1;
    x->height = std::max(height(getNodeFromPath(x->leftChildPath)), height(getNodeFromPath(x->rightChildPath))) + 1;

    // Update files for all nodes involved
    if (T2) {
        createFile(T2->filename, T2->key, T2->row, T2->leftChildPath, T2->rightChildPath, T2->height);
    }
    createFile(y->filename, y->key, y->row, y->leftChildPath, y->rightChildPath, y->height);
    createFile(x->filename, x->key, x->row, x->leftChildPath, x->rightChildPath, x->height);

    return x; // New root
}

// Perform a left rotation
AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = getNodeFromPath(x->rightChildPath);
    AVLNode* T2 = getNodeFromPath(y->leftChildPath);

    // Perform rotation
    y->leftChildPath = x->filename;
    x->rightChildPath = T2 ? T2->filename : String("");

    // Update heights
    x->height = std::max(height(getNodeFromPath(x->leftChildPath)), height(getNodeFromPath(x->rightChildPath))) + 1;
    y->height = std::max(height(getNodeFromPath(y->leftChildPath)), height(getNodeFromPath(y->rightChildPath))) + 1;

    // Update files for all nodes involved
    if (T2) {
        createFile(T2->filename, T2->key, T2->row, T2->leftChildPath, T2->rightChildPath, T2->height);
    }
    createFile(x->filename, x->key, x->row, x->leftChildPath, x->rightChildPath, x->height);
    createFile(y->filename, y->key, y->row, y->leftChildPath, y->rightChildPath, x->height);

    return y; // New root
}

// Get a node from its file path
AVLNode* AVLTree::getNodeFromPath(const String& path) {
    if (path.empty()) {
        return nullptr; // No node exists for an empty path
    }

    if (path.empty()) return nullptr;

    // Build proper path: repositories/repo_name/branch/trees/avl/path
    fs::path full_path = fs::current_path() / "repositories" / static_cast<const char*>(repo_name) /
        static_cast<const char*>(current_branch) / "trees" / "avl" / static_cast<const char*>(path);

    ifstream file(full_path);
    if (!file.is_open()) {
        cerr << "Error opening file at path: " << full_path << endl;
        return nullptr;
    }

    String key, row, leftChildPath, rightChildPath;
    int height = 0;

    // Parse the file
    String line;
    while (getline(file, line)) {
        stringstream ss(static_cast<const char*>(line));
        String label;

        if (line.find("Key:") == 0) {
            ss >> label >> key;
        }
        else if (line.find("Row:") == 0) {
            ss >> label >> row;
        }
        else if (line.find("Height:") == 0) {
            ss >> label >> height;
        }
        else if (line.find("LeftChildPath:") == 0) {
            ss >> label >> leftChildPath;
        }
        else if (line.find("RightChildPath:") == 0) {
            ss >> label >> rightChildPath;
        }
    }
    file.close();

    return new AVLNode(path, key, row, height, leftChildPath, rightChildPath);
}

// Insert a node
AVLNode* AVLTree::insert(AVLNode* node, const String& filename, const String& key, const String& row)
{
    if (node == nullptr)
    {
        createFile(filename, key, row, "", "", 0); // Height of new node is 0
        return new AVLNode(filename, key, row, 0);
    }

    if (key < node->key)
    {
        AVLNode* leftChild = getNodeFromPath(node->leftChildPath);
        AVLNode* newLeft = insert(leftChild, filename, key, row);
        node->leftChildPath = newLeft ? newLeft->filename : String("");
    }
    else if (key > node->key)
    {
        AVLNode* rightChild = getNodeFromPath(node->rightChildPath);
        AVLNode* newRight = insert(rightChild, filename, key, row);
        node->rightChildPath = newRight ? newRight->filename : String("");
    }
    else {
        // Duplicate key detected
        if (node->row != row)
        { // If the row is different
            // Handle the insertion of the new row under the same key
            // For example, you can append a new entry to the current node or create a new file for the new row
            cerr << "Duplicate key detected: " << key << ". Rows are different. Inserting new row." << endl;
            String new_filename = filename;
            createFile(new_filename, key, row, node->leftChildPath, node->rightChildPath, node->height);

            // Insert the new row as a new node in the AVL tree (if that's the desired behavior)
            AVLNode* duplicateNode = new AVLNode(new_filename, key, row, 0);

            // Inserting the duplicate node into the tree
            if (key < node->key)
            {
                // The duplicate should be inserted as a left child of the current node
                node->leftChildPath = duplicateNode->filename;
            }
            else
            {
                // The duplicate should be inserted as a right child of the current node
                node->rightChildPath = duplicateNode->filename;
            }
        }
        else
        {
            cerr << "Duplicate key detected: " << key << " with identical row. No insertion performed." << endl;
        }
        return node; // No duplicate keys with identical rows, so we don't insert anything
    }

    // Recalculate height
    node->height = 1 + std::max(
        height(getNodeFromPath(node->leftChildPath)),
        height(getNodeFromPath(node->rightChildPath))
    );

    // Balance the node
    int balance = getBalanceFactor(node);
    if (balance > 1 && key < getNodeFromPath(node->leftChildPath)->key)
    {
        return rightRotate(node);
    }
    if (balance < -1 && key > getNodeFromPath(node->rightChildPath)->key)
    {
        return leftRotate(node);
    }
    if (balance > 1 && key > getNodeFromPath(node->leftChildPath)->key)
    {
        node->leftChildPath = leftRotate(getNodeFromPath(node->leftChildPath))->filename;
        return rightRotate(node);
    }
    if (balance < -1 && key < getNodeFromPath(node->rightChildPath)->key)
    {
        node->rightChildPath = rightRotate(getNodeFromPath(node->rightChildPath))->filename;
        return leftRotate(node);
    }

    // Recreate the file for the updated node
    createFile(node->filename, node->key, node->row, node->leftChildPath, node->rightChildPath, node->height);
    return node;
}

// Insert a node into the AVL tree
void AVLTree::insert(const String& filename, const String& key, const String& row)
{
    root = insert(root, filename, key, row);
}

// Get the balance factor of a node
int AVLTree::getBalanceFactor(AVLNode* node)
{
    if (node == nullptr) {
        return 0; // Balance factor is 0 for null nodes
    }

    int leftHeight = height(getNodeFromPath(node->leftChildPath));
    int rightHeight = height(getNodeFromPath(node->rightChildPath));

    return leftHeight - rightHeight;
}

// Delete a node from the AVL tree
AVLNode* AVLTree::deleteNode(AVLNode* root, const String& key)
{
    if (root == nullptr)
    {
        cerr << "Key not found: " << key << endl;
        return nullptr;
    }

    if (key < root->key)
    {
        AVLNode* leftChild = getNodeFromPath(root->leftChildPath);
        if (!leftChild)
        {
            cerr << "Error: Invalid left child path: " << root->leftChildPath << endl;
            return nullptr;
        }
        AVLNode* newLeft = deleteNode(leftChild, key);
        root->leftChildPath = newLeft ? newLeft->filename : String("");
    }
    else if (key > root->key)
    {
        AVLNode* rightChild = getNodeFromPath(root->rightChildPath);
        if (!rightChild)
        {
            cerr << "Error: Invalid right child path: " << root->rightChildPath << endl;
            return nullptr;
        }
        AVLNode* newRight = deleteNode(rightChild, key);
        root->rightChildPath = newRight ? newRight->filename : String("");
    }
    else
    {
        if (root->leftChildPath.empty() || root->rightChildPath.empty())
        {
            AVLNode* temp = getNodeFromPath(
                root->leftChildPath.empty() ? root->rightChildPath : root->leftChildPath
            );

            if (!root->filename.empty() && remove(root->filename) != 0)
            {
                cerr << "Error: Unable to delete file: " << root->filename << endl;
            }

            delete root;
            return temp;
        }

        AVLNode* successor = getNodeFromPath(minValueNodePath(root->rightChildPath));
        if (!successor)
        {
            cerr << "Error: Unable to find successor for key: " << key << endl;
            return nullptr;
        }

        root->key = successor->key;
        root->row = successor->row;

        AVLNode* rightChild = getNodeFromPath(root->rightChildPath);
        AVLNode* updatedRight = deleteNode(rightChild, successor->key);
        root->rightChildPath = updatedRight ? updatedRight->filename : String("");
    }

    root->height = 1 + max(
        height(getNodeFromPath(root->leftChildPath)),
        height(getNodeFromPath(root->rightChildPath))
    );

    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(getNodeFromPath(root->leftChildPath)) >= 0)
    {
        return rightRotate(root);
    }
    if (balance > 1 && getBalanceFactor(getNodeFromPath(root->leftChildPath)) < 0)
    {
        root->leftChildPath = leftRotate(getNodeFromPath(root->leftChildPath))->filename;
        return rightRotate(root);
    }
    if (balance < -1 && getBalanceFactor(getNodeFromPath(root->rightChildPath)) <= 0)
    {
        return leftRotate(root);
    }
    if (balance < -1 && getBalanceFactor(getNodeFromPath(root->rightChildPath)) > 0)
    {
        root->rightChildPath = rightRotate(getNodeFromPath(root->rightChildPath))->filename;
        return leftRotate(root);
    }

    createFile(root->filename, root->key, root->row, root->leftChildPath, root->rightChildPath, root->height);
    return root;
}

// Find the node with the minimum value
String AVLTree::minValueNodePath(const String& path)
{
    AVLNode* current = getNodeFromPath(path);

    while (current && !current->leftChildPath.empty())
    {
        current = getNodeFromPath(current->leftChildPath);
    }

    return current ? current->filename : String("");
}

// Remove a node from the AVL tree
void AVLTree::removeNode(const String& key)
{
    root = deleteNode(root, key);
}

// Update a node in the AVL tree
void AVLTree::updateNode(const String& oldKey, const String& newRow)
{
    // Step 1: Remove the node with the old key
    AVLNode* nodeToUpdate = getNodeFromPath(searchNodePath(root, oldKey));
    if (nodeToUpdate == nullptr)
    {
        cerr << "Error: Key " << oldKey << " not found in the tree." << endl;
        return;
    }

    nodeToUpdate->row = newRow;
}

// Search for a node by its key and return its file path
String AVLTree::searchNodePath(AVLNode* node, const String& key)
{
    if (node == nullptr)
    {
        return ""; // Node not found
    }

    if (key < node->key)
    {
        return searchNodePath(getNodeFromPath(node->leftChildPath), key);
    }
    else if (key > node->key)
    {
        return searchNodePath(getNodeFromPath(node->rightChildPath), key);
    }
    else {
        return node->filename; // Node found
    }
}

//////////////////////////////////////
//         RedBlack Trees           //
//////////////////////////////////////

class RBNode {
public:
    String filename;
    String key;
    String row;
    bool isRed;  // true for red, false for black
    String leftChildPath;
    String rightChildPath;

    // Constructor for RBNode
    RBNode(String f, String k, String r, bool red = true) {
        filename = f;
        key = k;
        row = r;
        isRed = red;
        leftChildPath = "";
        rightChildPath = "";
    }
};



class RedBlackTree {
private:
    RBNode* root;
    String current_branch;
    String repo_name;

    // Helper function to create a file for a node
    void createFile(const String& filename, const String& key, const String& row,
        bool isRed, const String& leftPath, const String& rightPath);

    // Helper function to get a node from its file path
    RBNode* getNodeFromPath(const String& path);

    // Helper function to perform a left rotation
    RBNode* rotateLeft(RBNode* node);

    // Helper function to perform a right rotation
    RBNode* rotateRight(RBNode* node);

    // Helper function to flip the colors of a node and its children
    void flipColors(RBNode* node);

    // Helper function to check if a node is red
    bool isRed(RBNode* node);

    // Helper function to insert a node
    RBNode* insert(RBNode* node, const String& filename, const String& key, const String& row);

    // Helper function to move a red node to the left
    RBNode* moveRedLeft(RBNode* node);

    // Helper function to move a red node to the right
    RBNode* moveRedRight(RBNode* node);

    // Helper function to balance the tree
    RBNode* balance(RBNode* node);

    // Helper function to find the minimum node
    String min(RBNode* node);

    // Helper function to delete the minimum node
    RBNode* deleteMin(RBNode* node);

    // Helper function to delete a node
    RBNode* deleteNode(RBNode* node, const String& key);

public:
    // Constructor for RedBlackTree
    RedBlackTree();

    // Function to insert a node
    void insert(const String& filename, const String& key, const String& row);

    // Function to remove a node
    void remove(const String& key);

    // Function to check if a node exists
    bool contains(const String& key);

    // Function to update a node
    void updateNode(const String& key, const String& newRow);

    // Set the current branch
    void setCurrentBranch(const String& branch) {
        current_branch = branch;
    }

    // Set the repository name
    void setRepoName(const String& repo) {
        repo_name = repo;
    }

    // Get the current branch
    String getCurrentBranch() const {
        return current_branch;
    }

    // Get the repository name
    String getRepoName() const {
        return repo_name;
    }
};

// Constructor for RedBlackTree
RedBlackTree::RedBlackTree() {
    root = nullptr;
}

// Create a file for a node
void RedBlackTree::createFile(const String& filename, const String& key, const String& row,
    bool isRed, const String& leftPath, const String& rightPath) {
    // Build proper path: repositories/repo_name/branch/trees/redblack/filename
    fs::path full_path = fs::current_path() / "repositories" / static_cast<const char*>(repo_name) /
        static_cast<const char*>(current_branch) / "trees" / "redblack" / static_cast<const char*>(filename);

    ofstream file(full_path);
    if (file.is_open()) {
        file << "Key: " << key << "\n";
        file << "Row: " << row << "\n";
        file << "Color: " << (isRed ? "RED" : "BLACK") << "\n";
        file << "LeftChildPath: " << leftPath << "\n";
        file << "RightChildPath: " << rightPath << "\n";
        file.close();
    }
    else {
        cerr << "Error creating file: " << full_path << endl;
    }
}

// Get a node from its file path
RBNode* RedBlackTree::getNodeFromPath(const String& path) {
    if (path.empty()) return nullptr;

    // Build proper path: repositories/repo_name/branch/trees/redblack/path
    fs::path full_path = fs::current_path() / "repositories" / static_cast<const char*>(repo_name) /
        static_cast<const char*>(current_branch) / "trees" / "redblack" / static_cast<const char*>(path);

    ifstream file(full_path);
    if (!file.is_open()) {
        cerr << "Error opening file at path: " << full_path << endl;
        return nullptr;
    }

    String key, row, leftPath, rightPath;
    bool isRed = true;
    String line;

    while (getline(file, line)) {
        stringstream ss(static_cast<const char*>(line));
        string label;

        if (line.find("Key:") == 0) {
            ss >> label >> key;
        }
        else if (line.find("Row:") == 0) {
            ss >> label >> row;
        }
        else if (line.find("Color:") == 0) {
            string color;
            ss >> label >> color;
            isRed = (color == "RED");
        }
        else if (line.find("LeftChildPath:") == 0) {
            ss >> label >> leftPath;
        }
        else if (line.find("RightChildPath:") == 0) {
            ss >> label >> rightPath;
        }
    }
    file.close();

    RBNode* node = new RBNode(path, key, row, isRed);
    node->leftChildPath = leftPath;
    node->rightChildPath = rightPath;
    return node;
}

// Check if a node is red
bool RedBlackTree::isRed(RBNode* node) {
    if (node == nullptr) return false;
    return node->isRed;
}

// Perform a left rotation
RBNode* RedBlackTree::rotateLeft(RBNode* h) {
    RBNode* x = getNodeFromPath(h->rightChildPath);
    h->rightChildPath = x->leftChildPath;
    x->leftChildPath = h->filename;
    x->isRed = h->isRed;
    h->isRed = true;

    createFile(h->filename, h->key, h->row, h->isRed, h->leftChildPath, h->rightChildPath);
    createFile(x->filename, x->key, x->row, x->isRed, x->leftChildPath, x->rightChildPath);

    return x;
}

// Perform a right rotation
RBNode* RedBlackTree::rotateRight(RBNode* h) {
    RBNode* x = getNodeFromPath(h->leftChildPath);
    h->leftChildPath = x->rightChildPath;
    x->rightChildPath = h->filename;
    x->isRed = h->isRed;
    h->isRed = true;

    createFile(h->filename, h->key, h->row, h->isRed, h->leftChildPath, h->rightChildPath);
    createFile(x->filename, x->key, x->row, x->isRed, x->leftChildPath, x->rightChildPath);

    return x;
}

// Flip the colors of a node and its children
void RedBlackTree::flipColors(RBNode* h) {
    h->isRed = !h->isRed;
    RBNode* left = getNodeFromPath(h->leftChildPath);
    RBNode* right = getNodeFromPath(h->rightChildPath);

    if (left) {
        left->isRed = !left->isRed;
        createFile(left->filename, left->key, left->row, left->isRed,
            left->leftChildPath, left->rightChildPath);
    }
    if (right) {
        right->isRed = !right->isRed;
        createFile(right->filename, right->key, right->row, right->isRed,
            right->leftChildPath, right->rightChildPath);
    }

    createFile(h->filename, h->key, h->row, h->isRed, h->leftChildPath, h->rightChildPath);
}

// Insert a node
RBNode* RedBlackTree::insert(RBNode* h, const String& filename, const String& key, const String& row) {
    if (h == nullptr) {
        createFile(filename, key, row, true, "", "");
        return new RBNode(filename, key, row, true);
    }

    if (key < h->key) {
        RBNode* left = getNodeFromPath(h->leftChildPath);
        h->leftChildPath = insert(left, filename, key, row)->filename;
    }
    else if (key > h->key) {
        RBNode* right = getNodeFromPath(h->rightChildPath);
        h->rightChildPath = insert(right, filename, key, row)->filename;
    }
    else {
        if (h->row != row) {
            cerr << "Duplicate key with different row. Updating row." << endl;
            h->row = row;
            createFile(h->filename, h->key, h->row, h->isRed,
                h->leftChildPath, h->rightChildPath);
        }
        return h;
    }

    // Fix-up any right-leaning links
    if (isRed(getNodeFromPath(h->rightChildPath)) && !isRed(getNodeFromPath(h->leftChildPath))) {
        h = rotateLeft(h);
    }
    if (isRed(getNodeFromPath(h->leftChildPath)) &&
        isRed(getNodeFromPath(getNodeFromPath(h->leftChildPath)->leftChildPath))) {
        h = rotateRight(h);
    }
    if (isRed(getNodeFromPath(h->leftChildPath)) && isRed(getNodeFromPath(h->rightChildPath))) {
        flipColors(h);
    }

    createFile(h->filename, h->key, h->row, h->isRed, h->leftChildPath, h->rightChildPath);
    return h;
}

// Insert a node into the Red-Black tree
void RedBlackTree::insert(const String& filename, const String& key, const String& row) {
    root = insert(root, filename, key, row);
    root->isRed = false;
    createFile(root->filename, root->key, root->row, root->isRed,
        root->leftChildPath, root->rightChildPath);
}

// Check if a node exists in the Red-Black tree
bool RedBlackTree::contains(const String& key) {
    RBNode* x = root;
    while (x != nullptr) {
        if (key < x->key) {
            x = getNodeFromPath(x->leftChildPath);
        }
        else if (key > x->key) {
            x = getNodeFromPath(x->rightChildPath);
        }
        else {
            return true;
        }
    }
    return false;
}

// Remove a node from the Red-Black tree
void RedBlackTree::remove(const String& key) {
    if (!contains(key)) return;

    if (!isRed(getNodeFromPath(root->leftChildPath)) &&
        !isRed(getNodeFromPath(root->rightChildPath))) {
        root->isRed = true;
    }

    root = deleteNode(root, key);
    if (root != nullptr) {
        root->isRed = false;
        createFile(root->filename, root->key, root->row, root->isRed,
            root->leftChildPath, root->rightChildPath);
    }
}

// Move a red node to the left
RBNode* RedBlackTree::moveRedLeft(RBNode* h) {
    flipColors(h);
    RBNode* rightChild = getNodeFromPath(h->rightChildPath);
    if (rightChild && isRed(getNodeFromPath(rightChild->leftChildPath))) {
        h->rightChildPath = rotateRight(rightChild)->filename;
        h = rotateLeft(h);
        flipColors(h);
    }
    return h;
}

// Move a red node to the right
RBNode* RedBlackTree::moveRedRight(RBNode* h) {
    flipColors(h);
    RBNode* leftChild = getNodeFromPath(h->leftChildPath);
    if (leftChild && isRed(getNodeFromPath(leftChild->leftChildPath))) {
        h = rotateRight(h);
        flipColors(h);
    }
    return h;
}

// Balance the Red-Black tree
RBNode* RedBlackTree::balance(RBNode* h) {
    if (h == nullptr) return nullptr;

    if (isRed(getNodeFromPath(h->rightChildPath)))
        h = rotateLeft(h);

    RBNode* leftChild = getNodeFromPath(h->leftChildPath);
    if (leftChild && isRed(leftChild) && isRed(getNodeFromPath(leftChild->leftChildPath)))
        h = rotateRight(h);

    if (isRed(getNodeFromPath(h->leftChildPath)) && isRed(getNodeFromPath(h->rightChildPath)))
        flipColors(h);

    createFile(h->filename, h->key, h->row, h->isRed, h->leftChildPath, h->rightChildPath);
    return h;
}

// Find the minimum node
String RedBlackTree::min(RBNode* node) {
    if (node == nullptr) return "";
    while (!node->leftChildPath.empty()) {
        node = getNodeFromPath(node->leftChildPath);
    }
    return node->filename;
}

// Delete the minimum node
RBNode* RedBlackTree::deleteMin(RBNode* h) {
    if (h->leftChildPath.empty()) {
        remove(h->filename); // Delete the file
        delete h;
        return nullptr;
    }

    if (!isRed(getNodeFromPath(h->leftChildPath)) &&
        !isRed(getNodeFromPath(getNodeFromPath(h->leftChildPath)->leftChildPath)))
        h = moveRedLeft(h);

    RBNode* leftChild = getNodeFromPath(h->leftChildPath);
    RBNode* leftResult = deleteMin(leftChild);
    h->leftChildPath = leftResult ? leftResult->filename : String("");

    h = balance(h);
    createFile(h->filename, h->key, h->row, h->isRed, h->leftChildPath, h->rightChildPath);
    return h;
}

// Delete a node from the Red-Black tree
RBNode* RedBlackTree::deleteNode(RBNode* h, const String& key) {
    if (h == nullptr) return nullptr;

    if (key < h->key) {
        if (!isRed(getNodeFromPath(h->leftChildPath)) &&
            !isRed(getNodeFromPath(getNodeFromPath(h->leftChildPath)->leftChildPath)))
            h = moveRedLeft(h);

        RBNode* leftChild = getNodeFromPath(h->leftChildPath);
        RBNode* leftResult = deleteNode(leftChild, key);
        h->leftChildPath = leftResult ? leftResult->filename : String("");
    }
    else {
        if (isRed(getNodeFromPath(h->leftChildPath)))
            h = rotateRight(h);

        // Check for node with key and no right child
        if (key == h->key && h->rightChildPath.empty()) {
            remove(h->filename); // Delete the file
            delete h;
            return nullptr;
        }

        if (!isRed(getNodeFromPath(h->rightChildPath)) &&
            !isRed(getNodeFromPath(getNodeFromPath(h->rightChildPath)->leftChildPath)))
            h = moveRedRight(h);

        if (key == h->key) {
            // Replace with min node from right subtree
            RBNode* rightChild = getNodeFromPath(h->rightChildPath);
            String minFilename = min(rightChild);
            RBNode* minNode = getNodeFromPath(minFilename);
            h->key = minNode->key;
            h->row = minNode->row;

            RBNode* newRight = deleteMin(rightChild);
            h->rightChildPath = newRight ? newRight->filename : String("");

            // Update the current node's file
            createFile(h->filename, h->key, h->row, h->isRed, h->leftChildPath, h->rightChildPath);
        }
        else {
            RBNode* rightChild = getNodeFromPath(h->rightChildPath);
            RBNode* rightResult = deleteNode(rightChild, key);
            h->rightChildPath = rightResult ? rightResult->filename : String("");
        }
    }

    // Balance the tree and update the node's file
    h = balance(h);
    createFile(h->filename, h->key, h->row, h->isRed, h->leftChildPath, h->rightChildPath);
    return h;
}

// Update a node in the Red-Black tree
void RedBlackTree::updateNode(const String& key, const String& newRow) {
    // Ensure the node exists
    if (!contains(key)) {
        cerr << "Node with key \"" << key << "\" not found. Update aborted." << endl;
        return;
    }

    // Find the node and update its row
    RBNode* node = root;
    while (node != nullptr) {
        if (key < node->key) {
            node = getNodeFromPath(node->leftChildPath);
        }
        else if (key > node->key) {
            node = getNodeFromPath(node->rightChildPath);
        }
        else {
            // Update the node's row
            node->row = newRow;

            // Save the updated node to the file
            createFile(node->filename, node->key, node->row, node->isRed, node->leftChildPath, node->rightChildPath);
            return;
        }
    }

    // If the node is not found, output an error
    cerr << "Node with key \"" << key << "\" not found. Update aborted." << endl;
}



//////////////////////////////////////
//        CSV Handler Class         //
//////////////////////////////////////

class CSV_Handle {
private:
    String file_name;           // Name of the CSV file being handled
    String column_name;         // Name of the currently selected column
    ifstream file;              // File stream for reading the CSV file
    String current_branch;      // Name of the current branch in the repository
    String repo;                // Name of the repository

public:
    AVLTree tree;               // Instance of AVLTree to manage data
    RedBlackTree rbtree;        // Instance of RedBlackTree to manage data

    // Default constructor
    CSV_Handle();

    // Parameterized constructor to initialize filename and repository name
    CSV_Handle(const String& filename, const String& repo_name);

    // Reads a specified column based on user choice of tree (AVL or Red-Black)
    void readColumn(int choice);

    // Creates a new branch with the given name
    void createBranch(const String& branch_name);

    // Switches to the specified branch
    void switchBranch(const String& branch_name);

    //void commitchanges(); // Existing commit-related functionality (Preserved)

    // Displays all available branches in the repository
    void displayBranches();

    // Deletes the specified branch if it's not the main or current branch
    void deleteBranch(const String& branch_name);

    // Merges source_branch into target_branch by copying relevant files
    void mergeBranches(const String& source_branch, const String& target_branch);

    // Destructor to close the file if it's open
    ~CSV_Handle();

    // Retrieves the name of the CSV file
    String getFileName();

    // Retrieves the name of the repository
    String getRepoName();

    // Retrieves the name of the currently selected column
    String getColumnName();

    // Retrieves the name of the current branch
    String getCurrentBranch();

    // Retrieves the file stream for the CSV file
    ifstream& getFile();

    // Sets the name of the column to be handled
    void setColumnName(const String& column_name);

    // Sets the name of the repository
    void setRepoName(const String& repo_name);

    // Sets the current branch name
    void setCurrentBranch(const String& branch_name);

    // Sets and opens the CSV file to be handled
    void setFile(const String& filename);

    // Parses a line from the CSV and splits it into fields
    int parseCSVLine(const String& line, String result[], int max_fields);

    // Displays all commit histories for the current branch
    void viewCommits();
};

// Utility function to replace spaces with underscores in a string
String replaceSpacesWithUnderscores(const String& input) {
    String result = input;
    for (int i = 0; i < strlen(result); ++i)
    {
        if (result[i] == ' ') {
            result[i] = '_';  // Replace spaces with underscores
        }
    }
    return result;
}

// Default constructor implementation
CSV_Handle::CSV_Handle()
{
    // Initialize variables if necessary
}

// Retrieves the file name
String CSV_Handle::getFileName()
{
    return file_name;
}

// Retrieves the repository name
String CSV_Handle::getRepoName()
{
    return repo;
}

// Retrieves the column name
String CSV_Handle::getColumnName()
{
    return column_name;
}

// Retrieves the current branch name
String CSV_Handle::getCurrentBranch()
{
    return current_branch;
}

// Retrieves the file stream
ifstream& CSV_Handle::getFile()
{
    return file;
}

// Sets the column name
void CSV_Handle::setColumnName(const String& column_name)
{
    this->column_name = column_name;
}

// Sets the CSV file to be handled and opens it
void CSV_Handle::setFile(const String& filename) {
    this->file_name = filename;

    if (file.is_open())
    {
        file.close(); // Close the file if it's already open
    }

    file.open(filename); // Open the new file

    if (!file.is_open())
    {
        // Output an error message if the file fails to open
        cerr << "Error opening file: " << filename << "\nAbsolute path: " << fs::absolute(static_cast<const char*>(filename)) << endl;
    }
}

// Parameterized constructor implementation
CSV_Handle::CSV_Handle(const String& filename, const String& repo_name)
{
    repo = repo_name;
    fs::path project_dir = fs::path("D:/FAST") / static_cast<const char*>(repo_name);

    // Check if the project directory exists; if not, create it
    if (!fs::exists(project_dir))
    {
        if (fs::create_directory(project_dir))
        {
            cout << "Project repository '" << repo_name << "' created at " << project_dir << endl;
        }
        else
        {
            cerr << "Error creating project directory: " << project_dir << endl;
            return;
        }
    }

    file_name = filename;
    file.open(file_name); // Open the CSV file
    if (!file.is_open())
    {
        cerr << "Error opening file: " << file_name << std::endl;
    }

    current_branch = "main"; // Initialize to the main branch
    createBranch(current_branch); // Create the main branch
}

// Destructor implementation
CSV_Handle::~CSV_Handle()
{
    if (file.is_open())
    {
        file.close(); // Ensure the file is closed upon destruction
    }
}

// Creates a new branch with the specified name
void CSV_Handle::createBranch(const String& branch_name) {
    fs::path repo_root = fs::current_path() / "repositories" / static_cast<const char*>(repo);
    fs::path branch_dir = repo_root / static_cast<const char*>(branch_name);

    // Create branch directory structure if it doesn't already exist
    if (!fs::exists(branch_dir))
    {
        fs::create_directories(branch_dir / "trees" / "avl");
        fs::create_directories(branch_dir / "trees" / "redblack");
        fs::create_directories(branch_dir / "trees" / "btree");
        fs::create_directories(branch_dir / "commits");

        cout << "Branch " << branch_name << " created successfully." << endl;
    }

    switchBranch(branch_name); // Switch to the newly created branch
}

// Switches to the specified branch
void CSV_Handle::switchBranch(const String& branch_name)
{
    current_branch = branch_name;
    cout << "Switched to branch: " << current_branch << endl;
}

// Reads a specific column from the CSV based on user's choice of tree
void CSV_Handle::readColumn(int choice)
{
    if (!file.is_open())
    {
        cerr << "Error: Unable to read file: " << file_name << endl;
        return;
    }

    file.clear();
    file.seekg(0, std::ios::beg); // Reset file read position to the beginning
    String line;
    if (!getline(file, line)) // Read the header line
    {
        cerr << "Error: File is empty or no header row found." << endl;
        return;
    }

    cout << "Available column names:" << endl;

    string temp_col;
    String helper;
    int col_count = 0;

    // Display available columns by parsing the header
    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ',')
        {
            // Output the column index and name
            cout << "[" << col_count << "] " << replaceSpacesWithUnderscores(helper.to_string(temp_col)) << endl;
            temp_col.clear();  // Clear temp_col for the next column
            col_count++;       // Increment the column count
        }
        else
        {
            temp_col += line[i];  // Accumulate characters for the current column
        }
    }
    // Handle the last column after the loop ends
    if (!temp_col.empty()) {
        cout << "[" << col_count << "] " << replaceSpacesWithUnderscores(helper.to_string(temp_col)) << endl;
    }

    cout << "Enter the column name: ";
    cin.ignore(); // Ignore any leftover characters in the input buffer
    getline(cin, column_name);
    column_name = replaceSpacesWithUnderscores(column_name); // Format the column name

    // Find the index of the selected column by re-parsing the header
    int column_index = -1;
    int current_col = 0;
    temp_col.clear();

    for (int i = 0; i < line.length(); i++)
    {
        if (line[i] == ',')
        {
            if (replaceSpacesWithUnderscores(helper.to_string(temp_col)) == column_name)
            {
                column_index = current_col;
                break;
            }
            temp_col.clear();
            current_col++;
        }
        else
        {
            temp_col += line[i];
        }
    }

    if (!temp_col.empty() && replaceSpacesWithUnderscores(helper.to_string(temp_col)) == column_name)
    {
        column_index = current_col;
    }

    if (column_index == -1)
    {
        cerr << "Error: Column '" << column_name << "' not found in the header row." << endl;
        return;
    }

    fs::path project_dir = fs::current_path() / "repositories" / static_cast<const char*>(repo);
    fs::path branch_dir;

    // Determine which tree to use based on user choice
    switch (choice)
    {
    case 1:
        branch_dir = project_dir / static_cast<const char*>(current_branch) / "trees" / "avl";
        break;
    case 3:
        branch_dir = project_dir / static_cast<const char*>(current_branch) / "trees" / "redblack";
        break;
    default:
        return; // If choice doesn't match, exit the function
    }

    if (!fs::exists(branch_dir))
    {
        fs::create_directories(branch_dir); // Create the tree directory if it doesn't exist
    }

    // Process each data row in the CSV
    while (getline(file, line))
    {
        String key;
        int current_col = 0;
        temp_col.clear();

        // Extract the key from the specified column
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] == ',')
            {
                if (current_col == column_index)
                {
                    key = replaceSpacesWithUnderscores(helper.to_string(temp_col));
                    break;
                }
                temp_col.clear();
                current_col++;
            }
            else {
                temp_col += line[i];
            }
        }

        if (current_col == column_index && !temp_col.empty())
        {
            key = replaceSpacesWithUnderscores(helper.to_string(temp_col));
        }

        if (key.empty()) {
            continue; // Skip if the key is empty
        }

        // Create a unique filename for the entry to avoid conflicts
        String helper;
        std::string unique_filename = (branch_dir / static_cast<const char*>(key + ".txt")).string();
        int suffix = 0;
        while (fs::exists(unique_filename))
        {
            unique_filename = (branch_dir / (key + "_" + helper.To_string(suffix) + ".txt").to_std_string()).string();
            suffix++;
        }

        // Insert the entry into the selected tree (AVL or Red-Black)
        if (choice == 1) {
            tree.insert(helper.to_string(unique_filename), key, line);
        }
        else if (choice == 3) {
            rbtree.insert(helper.to_string(unique_filename), key, line);
        }
    }
}

// Displays all commit histories for the current branch
void CSV_Handle::viewCommits()
{
    fs::path project_dir = fs::current_path() / repo.c_str(); // Get current repository path
    fs::path commit_dir = project_dir / current_branch.c_str() / "commits"; // Path to commits directory

    if (!fs::exists(commit_dir))
    {
        cerr << "No commits found for branch: " << current_branch << endl;
        return;
    }

    cout << "Commits for branch: " << current_branch << endl;

    // Iterate through all files in the commits directory and display their names
    for (fs::directory_iterator it(commit_dir); it != fs::directory_iterator(); ++it)
    {
        if (fs::is_regular_file(it->status()))
        {
            cout << it->path().filename().string() << endl;
        }
    }
}

// Displays all available branches except the main branch
void CSV_Handle::displayBranches()
{
    string helper;
    fs::path project_dir = fs::current_path() / repo.to_std_string() / "repositories";
    
    // Iterate through each directory in the repositories folder
    for (const auto& entry : fs::directory_iterator(project_dir))
    {
        if (fs::is_directory(entry.status()))
        {
            string branch_name = entry.path().filename().string();
            if (branch_name != "main")
            {
                cout << "Branch: " << branch_name << endl; // Display branch name
            }
        }
    }
}

// Deletes the specified branch if it's not the main or currently active branch
void CSV_Handle::deleteBranch(const String& branch_name)
{
    // Prevent deletion of the main or currently active branch
    if (branch_name == String("main") || branch_name == current_branch)
    {
        cerr << "Error: Cannot delete the main branch or the currently active branch." << endl;
        return;
    }

    fs::path project_dir = fs::current_path() / static_cast<const char*>(repo); // Get repository path
    fs::path branch_dir = project_dir / static_cast<const char*>(branch_name); // Path to the branch directory

    if (!fs::exists(branch_dir))
    {
        cerr << "Error: Branch '" << branch_name << "' does not exist." << endl;
        return;
    }

    // Attempt to remove the branch directory and all its contents
    if (fs::remove_all(branch_dir) == 0)
    { // remove_all returns the number of files removed
        cerr << "Error: Failed to delete branch '" << branch_name << "'." << endl;
    }
    else
    {
        cout << "Branch '" << branch_name << "' deleted successfully." << endl;
    }
}

// Merges source_branch into target_branch by copying files from source to target
void CSV_Handle::mergeBranches(const String& source_branch, const String& target_branch)
{
    // Ensure source and target branches are different
    if (source_branch == target_branch)
    {
        cerr << "Error: Source and target branches are the same." << endl;
        return;
    }

    fs::path project_dir = fs::current_path() / static_cast<const char*>(repo); // Get repository path
    fs::path source_dir = project_dir / static_cast<const char*>(source_branch) / "AVL"; // Source AVL tree directory
    fs::path target_dir = project_dir / static_cast<const char*>(target_branch) / "AVL"; // Target AVL tree directory

    cout << "Source path: " << source_dir << endl;
    cout << "Target path: " << target_dir << endl;

    // Check if both source and target directories exist
    if (!fs::exists(source_dir))
    {
        cerr << "Error: Source branch does not exist." << endl;
        return;
    }

    if (!fs::exists(target_dir))
    {
        cerr << "Error: Target branch does not exist." << endl;
        return;
    }

    String helper;

    // Iterate through all files in the source AVL tree directory
    for (const auto& entry : fs::directory_iterator(source_dir))
    {
        if (fs::is_regular_file(entry.status()))
        {
            String source_file = helper.to_string(entry.path().filename().string());
            fs::path target_file_path = target_dir / static_cast<const char*>(source_file);

            // Copy the file from the source to the target, overwriting if it already exists
            if (fs::exists(target_file_path))
            {
                // If file exists, overwrite it with the source version
                if (fs::copy_file(entry.path(), target_file_path, fs::copy_options::overwrite_existing))
                {
                    cout << "Merged (overwritten) file: " << source_file << endl;
                }
                else
                {
                    cerr << "Error copying file: " << source_file << endl;
                }
            }
            else
            {
                // If file doesn't exist in the target, copy it
                if (fs::copy_file(entry.path(), target_file_path))
                {
                    cout << "Merged new file: " << source_file << endl;
                }
                else
                {
                    cerr << "Error copying file: " << source_file << endl;
                }
            }
        }
    }

    cout << "Branch '" << source_branch << "' merged into '" << target_branch << "' successfully." << endl;
}

// Sets the current branch and updates both AVL and Red-Black trees with the new branch
void CSV_Handle::setCurrentBranch(const String& branch_name)
{
    current_branch = branch_name;
    tree.setCurrentBranch(branch_name);    // Update AVLTree's current branch
    rbtree.setCurrentBranch(branch_name);  // Update RedBlackTree's current branch
}

// Sets the repository name and updates both AVL and Red-Black trees with the new repository name
void CSV_Handle::setRepoName(const String& repo_name)
{
    repo = repo_name;
    tree.setRepoName(repo_name);           // Update AVLTree's repository name
    rbtree.setRepoName(repo_name);         // Update RedBlackTree's repository name
}

//void CSV_Handle::commitchanges() {
//
//    String helper;
//    String commit_message;
//    cin.ignore();
//    getline(cin, commit_message);
//    commit_message = replaceSpacesWithUnderscores(commit_message);
//
//    fs::path repo_root = fs::current_path() / "repositories" / static_cast<const char*>(repo);
//    fs::path commit_dir = repo_root / static_cast<const char*>(current_branch) / "commits";
//
//    // Ensure commit directory exists
//    if (!fs::exists(commit_dir))
//    {
//        fs::create_directories(commit_dir);
//    }
//    String unique_data = current_branch + commit_message + helper.To_string(time(nullptr));
//    String commit_hash = helper.to_string(picosha2::hash256_hex_string(unique_data));
//    string commit_filename = (commit_dir / static_cast<const char*>(commit_message + "_" + commit_hash + ".txt")).string();
//
//    ofstream commit_file(commit_filename);
//    if (!commit_file.is_open()) {
//        cerr << "Error creating commit file: " << commit_filename << endl;
//        return;
//    }
//
//    commit_file << "Commit to branch: " << current_branch << "\n";
//    commit_file << "Commit Hash: " << commit_hash << "\n";
//    commit_file << "Changes made:\n";
//
//    // Write tree files to commit
//    fs::path trees_dir = repo_root / static_cast<const char*>(current_branch) / "trees";
//
//    const char* trees[] = { "avl", "redblack", "btree" };
//    for (int i = 0; i < 3; ++i)
//    {
//        fs::path tree_dir = trees_dir / trees[i];
//        if (fs::exists(tree_dir))
//        {
//            for (const auto& entry : fs::directory_iterator(tree_dir))
//            {
//                if (fs::is_regular_file(entry.status()))
//                {
//                    commit_file << trees[i] << "/" << entry.path().filename().string() << "\n";
//                }
//            }
//        }
//    }
//
//    commit_file.close();
//    cout << "Changes committed successfully to " << commit_filename << endl;
//}


//////////////////////////////////////
//        Git Handler Class         //
//////////////////////////////////////

class GitHandler
{
    CSV_Handle csv;
    String file_name;
public:
    GitHandler();
    void initialize();
    void commitChanges();
    void viewCommits();
    void createBranch();
    void switchBranch();
    void displayBranches();
    void deleteBranch();
    void mergeBranches();
    void displayCurrentBranch();
    void updateNode();
};

GitHandler::GitHandler()
{

}

void GitHandler::initialize() {
    cout << "Enter the file name: ";
    cin >> file_name;

    // Get the executable directory path
    fs::path exe_path = fs::current_path();
    fs::path file_path = exe_path / static_cast<const char*>(file_name);

    cout << "Enter the Repository Name: ";
    String repo_name;
    cin >> repo_name;

    fs::path repo_root = fs::current_path() / "repositories" / static_cast<const char*>(repo_name);

    if (!fs::exists(repo_root))
    {
        fs::create_directories(repo_root);
    }

    String helper;

    csv.setFile(helper.to_string(file_path.string())); // Pass the full path

    if (!csv.getFile().is_open())
    {
        cerr << "Error opening file: " << file_path << endl;
        return;
    }

    csv.setRepoName(repo_name);
    csv.setCurrentBranch("main");
    csv.createBranch(csv.getCurrentBranch());

    cout << "\nWhat tree do you want the data to be stored in?" << endl;
    cout << "1. AVL Tree" << endl;
    cout << "2. B Tree" << endl;
    cout << "3. Red Black Tree" << endl;
    cout << "4. Return to Main Menu" << endl;

    cout.flush();
    int choice = 0;
    cin >> choice;
    csv.readColumn(choice);
}

void GitHandler::commitChanges()
{
    //csv.commitchanges();  // Call the commitchanges function from CSV_Handle to commit the changes
    cout << "Changes have been committed successfully." << endl;
}


void GitHandler::viewCommits()
{
    csv.viewCommits();
}

void GitHandler::createBranch()
{

    String branch_name;
    cin >> branch_name;
    csv.createBranch(branch_name); // Call the createBranch method from CSV_Handle
}

// Switch to an existing branch
void GitHandler::switchBranch()
{
    String branch_name;
    cin >> branch_name;
    csv.switchBranch(branch_name); // Call the switchBranch method from CSV_Handle
}

// Display all branches in the project
void GitHandler::displayBranches()
{
    csv.displayBranches(); // Call the displayBranches method from CSV_Handle
}

// Delete an existing branch
void GitHandler::deleteBranch()
{
    String branch_name;
    cin >> branch_name;
    csv.deleteBranch(branch_name); // Call the deleteBranch method from CSV_Handle
}

void GitHandler::mergeBranches()
{
    String source;
    cout << "Enter the source branch: ";
    getline(cin, source);

    String destination;
    cout << "Enter the destination branch: ";
    getline(cin, destination);
    // Proceed with the merge
    csv.mergeBranches(source, destination);
    cout << "Successfully merged branch '" << source << "' into '" << destination << "'." << endl;
}

void GitHandler::displayCurrentBranch()
{
    cout << "You are on Branch: " << csv.getCurrentBranch() << endl;
}

/////////////////////////////////////
//         Menu System             //
/////////////////////////////////////

class MenuSystem {
private:
    GitHandler git;
    HANDLE hConsole;
    const int MENU_WIDTH = 60;

    void displayMainMenu();
    void displayRepositoryMenu();
    void displayBranchMenu();
    void displayCommitMenu();

    void handleRepositoryManagement();
    void handleBranchOperations();
    void handleCommitManagement();

    void setConsoleColor(int color);
    void centerText(const std::string& text);
    void drawLine(char symbol = '=');
    void clearScreen();
    void setCursorPosition(int x, int y);

public:
    MenuSystem();
    ~MenuSystem();
    void run();
};

MenuSystem::MenuSystem() 
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // Set console window size
    COORD bufferSize = { 80, 25 };
    SMALL_RECT windowSize = { 0, 0, 79, 24 };
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

MenuSystem::~MenuSystem() {
    setConsoleColor(7); // Reset to default color
}

void MenuSystem::setConsoleColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void MenuSystem::centerText(const std::string& text) {
    int spaces = (MENU_WIDTH - text.length()) / 2;
    std::cout << std::setw(spaces + text.length()) << text << std::endl;
}

void MenuSystem::drawLine(char symbol) {
    std::cout << std::setfill(symbol) << std::setw(MENU_WIDTH) << "" << std::endl;
    std::cout << std::setfill(' '); // Reset fill character
}

void MenuSystem::clearScreen() {
    system("cls");
}

void MenuSystem::setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

void MenuSystem::displayMainMenu() {
    clearScreen();
    setConsoleColor(11); // Light cyan
    drawLine();
    centerText("GIT MANAGEMENT SYSTEM");
    drawLine();
    std::cout << "\n";

    setConsoleColor(7); // White
    centerText("[1] Repository Management");
    centerText("[2] Branch Operations");
    centerText("[3] Commit Management");
    centerText("[4] Exit Program\n");

    setConsoleColor(11);
    drawLine('-');
    setConsoleColor(7);
    centerText("Enter your choice: ");
}

void MenuSystem::displayRepositoryMenu() {
    clearScreen();
    setConsoleColor(14); // Yellow
    drawLine();
    centerText("REPOSITORY MANAGEMENT");
    drawLine();
    std::cout << "\n";

    setConsoleColor(7);
    centerText("[1] Initialize New Repository");
    centerText("[2] Return to Main Menu\n");

    setConsoleColor(14);
    drawLine('-');
    setConsoleColor(7);
    centerText("Enter your choice: ");
}

void MenuSystem::displayBranchMenu() {
    clearScreen();
    setConsoleColor(10); // Light green
    drawLine();
    centerText("BRANCH OPERATIONS");
    drawLine();
    std::cout << "\n";

    setConsoleColor(7);
    centerText("[1] Create New Branch");
    centerText("[2] Switch Branch");
    // Removed "[3] List All Branches"
    centerText("[3] View Current Branch");
    centerText("[4] Return to Main Menu\n");

    setConsoleColor(10);
    drawLine('-');
    setConsoleColor(7);
    centerText("Enter your choice: ");
}

void MenuSystem::displayCommitMenu() {
    clearScreen();
    setConsoleColor(13); // Light magenta
    drawLine();
    centerText("COMMIT MANAGEMENT");
    drawLine();
    std::cout << "\n";

    setConsoleColor(7);
    centerText("[1] Make New Commit");
    centerText("[2] View Commit History");
    centerText("[3] Return to Main Menu\n");

    setConsoleColor(13);
    drawLine('-');
    setConsoleColor(7);
    centerText("Enter your choice: ");
}

void MenuSystem::handleRepositoryManagement() {
    char choice;
    do {
        displayRepositoryMenu();
        std::cin >> choice;

        switch (choice) {
        case '1':
            git.initialize();
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        case '2':
            return;
        default:
            std::cout << "\nInvalid option! Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
    } while (true);
}

void MenuSystem::handleBranchOperations() {
    char choice;
    do {
        displayBranchMenu();
        std::cin >> choice;

        switch (choice) {
        case '1':
            std::cout << "\nEnter branch name: ";
            git.createBranch();
            break;
        case '2':
            std::cout << "\nEnter branch name to switch to: ";
            git.switchBranch();
            break;
        case '3':
            git.displayCurrentBranch();
            break;
        case '4':
            return;
        default:
            std::cout << "\nInvalid option!";
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    } while (true);
}

void MenuSystem::handleCommitManagement() {
    char choice;
    do {
        displayCommitMenu();
        std::cin >> choice;

        switch (choice) {
        case '1':
            std::cout << "\nEnter commit message: ";
            git.commitChanges();
            break;
        case '2':
            git.viewCommits();
            break;
        case '3':
            return;
        default:
            std::cout << "\nInvalid option!";
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    } while (true);
}

void MenuSystem::run() {
    char choice;
    do {
        displayMainMenu();
        std::cin >> choice;

        switch (choice) {
        case '1':
            handleRepositoryManagement();
            break;
        case '2':
            handleBranchOperations();
            break;
        case '3':
            handleCommitManagement();
            break;
        case '4':
            setConsoleColor(7);
            std::cout << "\nExiting program...\n";
            return;
        default:
            std::cout << "\nInvalid option! Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
    } while (true);
}


//////////////////////////////////////
//        Main Function             //
//////////////////////////////////////

int main() {
    MenuSystem menu;
    menu.run();
    return 0;
}


