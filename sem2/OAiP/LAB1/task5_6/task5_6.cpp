

#include <iostream>

class Expression
{
public:
	virtual double evaluate() const = 0;
	virtual ~Expression() {};
};


class Number : public Expression
{
private:
	double val;
public:
	Number(double val) : val(val) {};
	double evaluate() const
	{
		return val;
	}
};


class BinaryOperation : public Expression
{
private:
	Expression const* left, const* right;
	char op;
public:
	BinaryOperation(Expression const* left, char op, Expression const* right) : left(left), op(op), right(right) {}

	double evaluate() const;

	~BinaryOperation()
	{
		delete left;
		delete right;
	}

};
double BinaryOperation::evaluate() const
{
	if (op == '+')
	{
		return left->evaluate() + right->evaluate();
	}
	else if (op == '-')
	{
		return left->evaluate() - right->evaluate();
	}
	else if (op == '*')
	{
		return left->evaluate() * right->evaluate();
	}
	else {
		return left->evaluate() / right->evaluate();
	}
}

bool check_equals(BinaryOperation const* left, Expression const* right)
{
	return *(size_t**)left == *(size_t**)right;
}

int main()
{
	Expression* sube = new BinaryOperation(new Number(4.5), '*', new Number(5));
	Expression* expr = new BinaryOperation(new Number(3), '+', sube);
	std::cout << expr->evaluate() << std::endl;
	return 0;
}


