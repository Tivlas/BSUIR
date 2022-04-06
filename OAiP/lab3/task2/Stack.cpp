#include "Stack.h"

void Stack::pop()
{
	if (top) {
		Node* newTop = top->next;
		delete top;
		top = newTop;
	}
}

void Stack::push(Node* newNode)
{
	newNode->next = top;
	top = newNode;
}

Node* Stack::peek()
{
	return top;
}
