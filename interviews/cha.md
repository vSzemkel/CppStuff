
# Coding Home Assignment

## Prefeace

The Actor Model in computer science is a mathematical model of concurrent\
computation that treats an actor as the basic building block of concurrent\
computation. In response to a message it receives, an actor can: make local\
decisions, create more actors, send more messages, and determine how to \
respond to the next message received. Actors may modify their own private\
state, but can only affect each other indirectly through messaging\
(removing the need for lock-based synchronization).

## User Story

- You are required to implement an Electronic Mail Service that is modeled\
  as an actor system. Accounts are modeled as actors and email addresses as\
  actor addresses.
- Each email account should be able to send, receive and forward email messages.
- An email message shall contain the address of the recipients and string email body
- The mail can be normal, spam or suspicious. Each type of mail should have\
  dedicated handling
- The email is suspicious if it contains a malware pattern provided by a security\
  department. Pattern can be long and can be XOR'ed with 42 or E1 inside message\
  body. it must be automatically forwarded by the receiving account to the Security\
  Department's email address, and the sender should be blocked
- If an email is suspected as spam (contains the words: discount, inheritance,\
  lottery), it should be discarded
- Normal mails should be written to a log file
- Each email account created, should receive an address book with the email address\
  of all the accounts in the organization (security department included)
- No network transfer mechanism implementation is required. Emails can be delivered\
  by invoking an API call implemented by each of the accounts.
- The product of this exercise should be a unit test testing the above requirements.

## **The use of modern C++, smart pointers and STL is encouraged.**
