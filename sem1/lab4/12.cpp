

#include <iostream>
void delete_dup(int a[], int &n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (a[i] == a[j])
			{
				for (int k = i; k < n; k++)
				{
					a[k] = a[k + 1];
				}
				n--;
				i--;
			}
		}
	}
}

int main()
{
	int n;
	std::cout << "Enter n(size of array = n)\n";
	while (!(std::cin >> n) || n <= 0)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data.\n";
		std::cout << "Enter n(size of array = n)\n";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	int k = n;

	int *a = new int[n];
	for (int i = 0; i < n; i++)
	{

		std::cout << "Enter a[" << i << "]" << std::endl;
		while (!(std::cin >> a[i]) || a[i] < 0 || a[i] > 1e6)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nPlease check the entered data.\n";
			std::cout << "Enter a[" << i << "]" << std::endl;
		}
	}
	std::cout << std::endl;

	delete_dup(a, n);
	std::cout << "New size of array = " << n << std::endl;
	for (int i = 0; i < k; i++)
	{
		std::cout << a[i] << " ";
	}
	std::cout << std::endl;

	delete[] a;
	return 0;
}
