namespace Task6;
public static class FileDownloader
{
	public static async Task<byte[]> DownloadAsync(string url)
	{
		using (HttpClient client = new())
		{
			var response = await client.GetAsync(url);
			response.EnsureSuccessStatusCode();
			return await response.Content.ReadAsByteArrayAsync();
		}
	}
}
