using Task3;
namespace TestTask3;


public class UnitTest1
{
	[Theory]
	[InlineData(5.4, 7.8, 42.12)]
	[InlineData(5.4, 10, 54.00)]
	public void Area_WhenValidArguments_ShouldReturnArea(double length, double width, double expected)
	{
		double actual = Program.Area(length, width);
		Assert.Equal(expected, actual, 2);
	}

	[Theory]
	[InlineData(5.4, -7.8)]
	[InlineData(-5.4, 7.8)]
	[InlineData(4, 0)]
	public void Area_WhenNegativeParameter_ShouldThrowArgumentException(double length, double width)
	{
		var exception = Assert.Throws<ArgumentException>(() => Program.Area(length, width));
	}

	[Fact]
	public void Area_WhenOverflow_ShouldThrowArgumentException()
	{
		double length = double.MaxValue;
		double width = 2.0;
		var exception = Assert.Throws<OverflowException>(() => Program.Area(length, width));
	}
}