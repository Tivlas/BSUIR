public static class Program
{
	public static void Main()
	{
		Random rnd = new();
		var s = new string('!', rnd.Next(5, 51));
		Console.WriteLine("Hello, world!");
		Console.WriteLine("Andhiagain!");
		Console.WriteLine(s);
	}
}