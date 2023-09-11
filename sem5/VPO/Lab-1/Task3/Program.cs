namespace Task3;

public class Program
{
	public static void Main(string[] args)
	{
		if (args.Length != 2)
		{
			Console.WriteLine("Invalid number of arguments");
		}
		else
		{
			try
			{
				double length = double.Parse(args[0]);
				double width = double.Parse(args[1]);
				Console.WriteLine(Area(length, width));
			}
			catch (Exception e)
			{
				Console.WriteLine(e.Message);
				return;
			}
		}
	}

	public static double Area(double length, double width)
	{
		if (length <= 0 || width <= 0)
		{
			throw new ArgumentException("Length and width must be greater than zero");
		}
		var area = length * width;
		if (double.IsInfinity(area))
		{
			throw new OverflowException("Overflow");
		}
		return area;
	}
}
