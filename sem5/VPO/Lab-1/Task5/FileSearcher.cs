namespace Task5;
public static class FileSearcher
{
	public static string[] Search(string folderPath, string extension)
		=> Directory.GetFiles(folderPath, $"*.{extension}", SearchOption.AllDirectories);
}
