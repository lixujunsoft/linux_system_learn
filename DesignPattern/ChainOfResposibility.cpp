#include <iostream>
#include <string>

using namespace std;

/*
使多个对象都有机会处理请求，从而避免请求的发送者和接收者之间的耦合关系。
将这些对象连成一条链，并沿着这条链传递请求，直到有一个对象处理它为止

要点总结：
Chain of Responsibility模式的应用场合在于“一个请求可能有多个接受者，但是
最后的真正接受着只有一个”，这时候请求的发送者与接受者的耦合有可能出现“变化脆弱的
症状”，职责链的目的就是将二者解耦，从而更好的应对变化。

应用了Chain of Responsibility模式后，对象的职责分派将更具灵活性。我们可以在
运行时动态添加、修改请求的处理职责

如果请求传递到职责链的末尾仍得不到处理，应该有一个合理的缺省机制。这也是每一个
接受对象的责任，而不是发出请求的对象的责任
*/

enum class RequestType {
    REQ_HANDLER1,
    REQ_HANDLER2,
    REQ_HANDLER3,
};

class Request {
public:
    Request(const string &desc, RequestType type) : description(desc), reqType(type) {}
    RequestType getReqType() const { return reqType; }
    const string &getDescription() const { return description; }

private:
    RequestType reqType;
    string description;  
};

class ChainHandler {
public:
    ChainHandler() { nextChain = nullptr; }
    void setNextChain(ChainHandler *next) { nextChain = next; }
    void handle(const Request &req) {
        if (canHandleRequest(req)) {
            processRequest(req);
        } else {
            sendRequestToNextHandler(req);
        }
    }

protected:
    virtual bool canHandleRequest(const Request &req) = 0;
    virtual void processRequest(const Request &req) = 0;

private:
    ChainHandler *nextChain;
    void sendRequestToNextHandler(const Request &req) {
        if (nextChain != nullptr) {
            nextChain->handle(req);
        }
    }
};

class Handler1 : public ChainHandler {
protected:
    bool canHandleRequest(const Request &req) {
        return req.getReqType() == RequestType::REQ_HANDLER1;
    }

    void processRequest(const Request &req) {
        cout << "Handler1 is handle request: " << req.getDescription() << endl;
    }
};

class Handler2 : public ChainHandler {
protected:
    bool canHandleRequest(const Request &req) {
        return req.getReqType() == RequestType::REQ_HANDLER2;
    }

    void processRequest(const Request &req) {
        cout << "Handler2 is handle request: " << req.getDescription() << endl;
    }
};

class Handler3 : public ChainHandler {
protected:
    bool canHandleRequest(const Request &req) {
        return req.getReqType() == RequestType::REQ_HANDLER3;
    }

    void processRequest(const Request &req) {
        cout << "Handler3 is handle request: " << req.getDescription() << endl;
    }
};

int main() {
    Handler1 h1;
    Handler2 h2;
    Handler3 h3;

    h1.setNextChain(&h2);
    h2.setNextChain(&h3);

    Request req("process task ...", RequestType::REQ_HANDLER3);

    h1.handle(req);
    return 0;
}
