#include <iostream>
#include <queue>
#include <unordered_set>
#include <string>
using namespace std;

class Message {
public:
    string messageId;
    Message(){}
    virtual ~Message() {} // Virtual destructor to enable polymorphic behavior
};

class ServerResponse : public Message{
public:
    string serverId;
    string metadata;
    ServerResponse(string messageId, string serverId, string metadata){
        this->messageId = messageId;
        this->serverId = serverId;
        this->metadata = metadata;
    }
};

class ClientRequest : public Message{
public:
    string clientId;
    int requestTime;
    ClientRequest(string messageId, string clientId, int requestTime){
        this->messageId = messageId;
        this->clientId = clientId;
        this->requestTime = requestTime;
    }
};

class customQueue {
private:
    queue<Message*> q; // Queue to store Message pointers
    unordered_set<string> clientIds; // Set to store distinct clientIds
    unordered_set<string> serverIds; // Set to store distinct serverIds

public:
    void enqueue(ServerResponse* e) {
        q.push(e);
        serverIds.insert(e->serverId);
    }

    void enqueue(ClientRequest* e) {
        q.push(e);
        clientIds.insert(e->clientId);
    }

    void dequeue() {
        if (!q.empty()) {
            Message* front = q.front();
            //Checking if the messages are from ClientRequest or ServerResponse to delete them from the set
            if (dynamic_cast<ClientRequest*>(front)) {
                clientIds.erase(static_cast<ClientRequest*>(front)->clientId);
            }
            else if (dynamic_cast<ServerResponse*>(front)) {
                serverIds.erase(static_cast<ServerResponse*>(front)->serverId);
            }
            q.pop();
        }
    }

    string getFront() {
        if (!q.empty()) {
            Message* front = q.front();
            //Checking if the messages are from ClientRequest or ServerResponse to format them correctly
            if (dynamic_cast<ClientRequest*>(front)) {
                ClientRequest* request = static_cast<ClientRequest*>(front);
                return request->messageId + " " + request->clientId + " " + to_string(request->requestTime);
            }
            else if (dynamic_cast<ServerResponse*>(front)) {
                ServerResponse* response = static_cast<ServerResponse*>(front);
                return response->messageId + " " + response->serverId + " " + response->metadata;
            }
        }
        return "";
    }

    unsigned int getCountClients() {
        return clientIds.size();
    }

    unsigned int getCountServers() {
        return serverIds.size();
    }
};

int main() {
    customQueue queue;
    queue.enqueue(new ClientRequest("m1", "C1", 10));
    queue.enqueue(new ServerResponse("m2", "S1", "4bytes,1second"));
    queue.enqueue(new ClientRequest("m3", "C2", 15));

    cout << queue.getCountClients() << endl;
    cout << queue.getCountServers() << endl;
    cout << queue.getFront() << endl;

    queue.dequeue();

    cout << queue.getFront() << endl;

    return 0;
}