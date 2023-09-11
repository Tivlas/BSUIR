using Task2;

namespace TestTask2;

public class ReadTest
{
	[Fact]
	public void Read_WhenNoInputProvided_ShouldReturnEmptyList()
	{
		var input = string.Empty;
		var expected = new List<Person>();

		Console.SetIn(new StringReader(input));
		var actual = ConsoleReader.Read();

		Assert.Equal(expected, actual);
	}

	[Fact]
	public void Read_WhenValidInputProvided_ShouldReturnPopulatedList()
	{
		var input = "Ivanov, Ivan, 30\nIvanov2, Ivan2, 40\n";
		var expected = new List<Person>
		{
			new Person { LastName = "Ivanov", FirstName = "Ivan", Age = 30 },
			new Person { LastName = "Ivanov2", FirstName = "Ivan2", Age = 40 }
		};

		Console.SetIn(new StringReader(input));
		var actual = ConsoleReader.Read();

		Assert.Equal(expected, actual);
	}

	[Fact]
	public void Read_WhenInvalidInputProvided_ShouldSkipInvalidLinesAndReturnValidPersons()
	{
		var input = "Ivanov, Ivan, 30\nInvalid Input\nLast, First, InvalidAge\nIvanov2, Ivan2, 40\n";
		var expected = new List<Person>
		{
			new Person { LastName = "Ivanov", FirstName = "Ivan", Age = 30 },
			new Person { LastName = "Ivanov2", FirstName = "Ivan2", Age = 40 }
		};

		Console.SetIn(new StringReader(input));
		var actual = ConsoleReader.Read();

		Assert.Equal(expected, actual);
	}
}