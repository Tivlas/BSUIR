namespace Task2;

internal class Program
{
	static void Main(string[] args)
	{
		Console.WriteLine("Input template: last name, first name, age. Enter empty line to stop.");
		var people = ConsoleReader.Read();
		foreach (var person in people)
		{
			Console.WriteLine($"{person.LastName} {person.FirstName} {person.Age}");
		}
		Console.WriteLine($"{AgeCalculator.Min(people)} {AgeCalculator.Max(people)} {AgeCalculator.Avg(people)}");
	}
}
