namespace Task6;

internal class Program
{
	static async Task Main(string[] args)
	{
		if (args.Length != 2) {
            Console.WriteLine("You have to pass two parameters: <url> <output file path>");
		}
		else
		{
			try
			{
				var url = args[0];
				var path = args[1];
				var data = await FileDownloader.DownloadAsync(url);
				await FileSaver.SaveAsync(data, path);
			}
			catch (Exception e)
			{
                Console.WriteLine(e.Message);
            }
		}
	}
}
