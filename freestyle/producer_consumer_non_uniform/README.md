# Amarula Quest

General idea: producer and consumer are not related processes that can share piece of memory or TCP socket. If the task was meant to thread coordination, [here](https://github.com/vSzemkel/CppStuff/blob/master/classic/producer_consumer.cpp) is my old solution.

There is no metrix defined for this task, so I presume single producer and consumer sharing scarce memory with no payload waste. Synchronization will depend on std::atomic variables
