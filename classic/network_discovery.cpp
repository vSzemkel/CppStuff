
// Each node can communicate only with its direct neighbours
// Nodes have no unique identifiers. Design Message and receive 
// to count nodes in this network given:
//
// struct Message;
// const std::vector<Neighbor>& Neighbors()
// void send(const Neighbor& destination, const Message& msg);
// void receive(const Neighbor& sender, const Message& msg);

#include <iostream>
#include <chrono>
#include <thread>

int g_counter{1};
int g_received_count{0};
Neighbor g_ack_to;
bool g_search_reseived{false}

struct Message {
  enum {
    SEARCH,
    ACK,
  } type; 
  int count; // in ACK message this is responder network nodes count
}

void receive(const Neighbor& sender, const Message& msg) {
  switch (msg.type) {
    case ACK:
      g_counter += msg.count;
      if (++g_received_count == Neighbors.size() - 1)
        send(g_ack_to, Message{ACK, g_counter});
      break;
    case SEARCH:
      if (g_search_received) return;
      g_search_received = true;
      g_ack_to = sender;
      for (const auto& n : Neighbors)
        if (n != sender)
        	send(n, msg);
   }
}

int main(int, char**) {
  for (const auto& n : Neighbors)
    send(n, Message{SEARCH, 0});

  while (g_received_count < Neighbors.size())
    std::this_thread::sleep_for(1s);

  std::out << "Identified " << g_counter << " nodes\n";
}
