namespace Task6;
public static class FileSaver
{
	/// <summary>
	/// Creates a file and writes all data, closes the file. If the target file already exists, it is overwritten. Directory must exist.
	/// </summary>
	/// <param name="data"></param>
	/// <param name="path"></param>
	/// <returns></returns>
	public static async Task SaveAsync(byte[] data, string path)
	{
		await File.WriteAllBytesAsync(path, data);
	}
}
