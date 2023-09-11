namespace Task2;

public static class ConsoleReader
{
	/// <summary>
	/// Reads input from the console to create a list of <c>Person</c> objects.
	/// Each input line should contain the last name, first name, and age of a person, separated by commas.
	/// The method continues reading input until an empty line is entered, indicating the end of input.
	/// </summary>
	/// <returns>A list of <c>Person</c> objects.</returns>
	public static List<Person> Read()
	{
		List<Person> people = new();
		var lineCounter = 1;
		while (true)
		{
			Console.Write($"{lineCounter}. ");
			var input = Console.ReadLine();
			if (string.IsNullOrWhiteSpace(input))
			{
				break;
			}
			string[]? personInfo = input.Split(',');
			if (personInfo?.Length != 3)
			{
				Console.WriteLine("Invalid input.");
				continue;
			}

			string lastName = personInfo[0].Trim();
			string firstName = personInfo[1].Trim();

			if (string.IsNullOrWhiteSpace(lastName) || string.IsNullOrWhiteSpace(firstName))
			{
				Console.WriteLine("Invalid input.");
				continue;
			}

			if (!int.TryParse(personInfo[2], out int age) || age < 0 || age > 125)
			{
				Console.WriteLine("Invalid age.");
				continue;
			}

			people.Add(new Person
			{
				Age = age,
				FirstName = firstName,
				LastName = lastName,
			});
			++lineCounter;
		}
		return people;
	}
}
