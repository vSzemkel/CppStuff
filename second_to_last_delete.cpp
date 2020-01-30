#include <stdio.h>

struct Node
{
    char name[64];
    Node* next;
};

void delete_last(Node* list)
{
    if (list == nullptr)
        return;

    Node* stl = nullptr;
    while (list->next != nullptr) {
        stl = list;
        list = list->next;
    }

    if (stl != nullptr) {
        printf("deleting: %s\n", list->name);
        // free(list) if allocated dynamically
        stl->next = nullptr;
    }
}

void print_list(Node* list)
{
    int i = 1;
    while (list != nullptr) {
        printf("%i: %s\n", i++, list->name);
        list = list->next;
    }
}
int main()
{
    Node n1{"Rus", nullptr};
    Node n2{"Czech", &n1};
    Node head{"Lech", &n2};

    print_list(&head);
    delete_last(&head);
    print_list(&head);
    delete_last(&head);
    print_list(&head);
    delete_last(&head);
    print_list(&head);
    delete_last(&head);
}