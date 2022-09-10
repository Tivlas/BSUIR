#include "Stack.h"

void Stack::pop()
{
	if (top) {
		Node* newTop = top->next;
		delete top;
		top = newTop;
	}
}

void Stack::clear() {
	while (top) {
		pop();
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
