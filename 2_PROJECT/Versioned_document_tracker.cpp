#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

class VersionedDocument {
private:
    string currentContent;
    stack<string> undoStack;
    stack<string> redoStack;

public:
    void insertText(const string& newText) {
        undoStack.push(currentContent);
        while (!redoStack.empty()) redoStack.pop();
        currentContent += newText + "\n";
        cout << "✅ Text inserted.\n";
    }

    void deleteLastLine() {
        if (currentContent.empty()) {
            cout << "⚠️ Document is already empty.\n";
            return;
        }
        
        undoStack.push(currentContent);
        while (!redoStack.empty()) redoStack.pop();

        // Remove trailing newline if exists
        if (!currentContent.empty() && currentContent.back() == '\n') {
            currentContent.pop_back();
        }

        size_t lastNewLine = currentContent.find_last_of('\n');
        if (lastNewLine != string::npos) {
            currentContent = currentContent.substr(0, lastNewLine + 1);
        } else {
            currentContent = "";
        }
        cout << "✅ Last line deleted.\n";
    }

    void undo() {
        if (!undoStack.empty()) {
            redoStack.push(currentContent);
            currentContent = undoStack.top();
            undoStack.pop();
            cout << "↩️ Undo successful.\n";
        } else {
            cout << "⚠️ Nothing to undo.\n";
        }
    }

    void redo() {
        if (!redoStack.empty()) {
            undoStack.push(currentContent);
            currentContent = redoStack.top();
            redoStack.pop();
            cout << "↪️ Redo successful.\n";
        } else {
            cout << "⚠️ Nothing to redo.\n";
        }
    }

    void display() {
        cout << "\n📄 --- Current Document ---\n";
        if (currentContent.empty()) {
            cout << "[Empty Document]\n";
        } else {
            // Display without extra newline at end
            string displayContent = currentContent;
            if (!displayContent.empty() && displayContent.back() == '\n') {
                displayContent.pop_back();
            }
            cout << displayContent << "\n";
        }
        cout << "--------------------------\n";
        cout << "📊 Total Lines: " << countLines() << "\n";
    }

    void clear() {
        if (currentContent.empty()) {
            cout << "⚠️ Document is already empty.\n";
            return;
        }
        undoStack.push(currentContent);
        while (!redoStack.empty()) redoStack.pop();
        currentContent.clear();
        cout << "🧹 Document cleared.\n";
    }

    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            // Save without extra newline at end
            string saveContent = currentContent;
            if (!saveContent.empty() && saveContent.back() == '\n') {
                saveContent.pop_back();
            }
            outFile << saveContent;
            outFile.close();
            cout << "💾 Document saved to " << filename << "\n";
        } else {
            cout << "❌ Failed to create file: " << filename << "\n";
        }
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            undoStack.push(currentContent);
            while (!redoStack.empty()) redoStack.pop();

            stringstream buffer;
            buffer << inFile.rdbuf();
            currentContent = buffer.str();
            
            // Ensure content ends with newline for consistency
            if (!currentContent.empty() && currentContent.back() != '\n') {
                currentContent += '\n';
            }
            
            inFile.close();
            cout << "📂 Document loaded from " << filename << "\n";
        } else {
            cout << "❌ File not found: " << filename << "\n";
        }
    }

    int countLines() {
        if (currentContent.empty()) return 0;
        int lines = count(currentContent.begin(), currentContent.end(), '\n');
        // If last line doesn't end with newline, add 1
        if (!currentContent.empty() && currentContent.back() != '\n') {
            lines++;
        }
        return lines;
    }
};

void showMenu() {
    cout << "\n=================================\n";
    cout << "📚 Versioned Document Tracker\n";
    cout << "=================================\n";
    cout << "1. Insert Text ||         ";
    cout << "2. Delete Last Line ||    ";
    cout << "3. Undo ||               ";
    cout << "4. Redo ||             ";
    cout << "5. Display Document ||  \n";
    cout << "6. Clear Document ||      ";
    cout << "7. Save to File ||        ";
    cout << "8. Load from File ||      ";
    cout << "9. Exit \n";
    cout << "---------------------------------\n";
    cout << "Enter your choice (1-9): ";
}

int main() {
    VersionedDocument doc;
    int choice;
    string text, filename;

    cout << "🚀 Versioned Document Tracker Started!\n";
    
    while (true) {
        showMenu();
        cin >> choice;
        cin.ignore(); // Clear newline character from buffer

        switch (choice) {
            case 1:
                cout << "Enter text to insert: ";
                getline(cin, text);
                if (!text.empty()) {
                    doc.insertText(text);
                } else {
                    cout << "❌ Cannot insert empty text.\n";
                }
                break;
                
            case 2:
                doc.deleteLastLine();
                break;
                
            case 3:
                doc.undo();
                break;
                
            case 4:
                doc.redo();
                break;
                
            case 5:
                doc.display();
                break;
                
            case 6:
                doc.clear();
                break;
                
            case 7:
                cout << "Enter filename to save: ";
                getline(cin, filename);
                if (!filename.empty()) {
                    doc.saveToFile(filename);
                } else {
                    cout << "❌ Filename cannot be empty.\n";
                }
                break;
                
            case 8:
                cout << "Enter filename to load: ";
                getline(cin, filename);
                if (!filename.empty()) {
                    doc.loadFromFile(filename);
                } else {
                    cout << "❌ Filename cannot be empty.\n";
                }
                break;
                
            case 9:
                cout << "👋 Thank you for using Document Tracker!\n";
                cout << "Exiting program...\n";
                return 0;
                
            default:
                cout << "❌ Invalid choice! Please enter 1-9.\n";
        }
        
        cout << endl; // Add spacing between operations
    }
}