using Task2;

namespace TestTask2;
public class MinMaxAvgTest
{
	[Fact]
	public void Min_WhenListIsEmpty_ShouldReturnNegativeOne()
	{
		var people = new List<Person>();

		var result = AgeCalculator.Min(people);

		Assert.Equal(-1, result);
	}

	[Fact]
	public void Min_WhenListIsNotEmpty_ShouldReturnMinimumAge()
	{
		var people = new List<Person>()
		{
			new Person { Age = 30 },
			new Person { Age = 25 },
			new Person { Age = 40 }
		};

		var result = AgeCalculator.Min(people);

		Assert.Equal(25, result);
	}

	[Fact]
	public void Max_WhenListIsEmpty_ShouldReturnNegativeOne()
	{
		var people = new List<Person>();

		var result = AgeCalculator.Max(people);

		Assert.Equal(-1, result);
	}

	[Fact]
	public void Max_WhenListIsNotEmpty_ShouldReturnMaximumAge()
	{
		var people = new List<Person>()
		{
			new Person { Age = 30 },
			new Person { Age = 25 },
			new Person { Age = 40 }
		};

		var result = AgeCalculator.Max(people);

		Assert.Equal(40, result);
	}

	[Fact]
	public void Avg_WhenListIsEmpty_ShouldReturnNegativeOne()
	{
		var people = new List<Person>();

		var result = AgeCalculator.Avg(people);

		Assert.Equal(-1, result);
	}

	[Fact]
	public void Avg_WhenListIsNotEmpty_ShouldReturnAverageAge()
	{
		var people = new List<Person>()
		{
			new Person { Age = 30 },
			new Person { Age = 25 },
			new Person { Age = 40 }
		};

		var result = AgeCalculator.Avg(people);

		Assert.Equal(31.67, result, 2);
	}
}
