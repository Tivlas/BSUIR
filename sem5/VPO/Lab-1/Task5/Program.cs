namespace Task5;

public class Program
{
	static void Main(string[] args)
	{
		if (args.Length != 2)
		{
			Console.WriteLine("You have to pass 2 arguments: <folder path> <extension>");
		}
		else
		{
			try
			{
				var fileNames = FileSearcher.Search(args[0], args[1]);
				foreach(var file in fileNames)
				{
					Console.WriteLine(file);
				}
			}
			catch (Exception e)
			{
                Console.WriteLine(e.Message);
            }
		}
	}
}
