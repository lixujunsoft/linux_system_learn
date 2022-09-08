#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Command {
public:
    virtual void execute() = 0;
};

class ConcreteCommand1 : public Command {
public:
    ConcreteCommand1(const string &a) : arg(a) {}
    void execute() override {
        cout << "#1 process ..." << arg << endl;
    }
private:
    string arg;
};

class ConcreteCommand2 : public Command {
public:
    ConcreteCommand2(const string & a) : arg(a) {}
    void execute() override {
        cout << "#2 process ..." << arg << endl;
    }
private:
    string arg;
};

class MacroCommand : public Command {
public:
    void addCommand(Command *c) {
        commands.push_back(c);
    }
    void execute() override {
        for (auto &c : commands) {
            c->execute();
        }
    }

private:
    vector<Command*> commands;
};

int main() {
    ConcreteCommand1 command1("Arg ###");
    ConcreteCommand2 command2("Arg $$$");

    MacroCommand macro;
    macro.addCommand(&command1);
    macro.addCommand(&command2);

    macro.execute();
}