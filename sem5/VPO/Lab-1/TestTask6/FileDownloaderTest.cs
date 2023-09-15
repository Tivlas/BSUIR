using Task6;

namespace TestTask6;

public class FileDownloaderTest
{
	[Fact]
	public async Task DownloadAsync_WhenValidUrl_ShouldReturnFileData()
	{
		// Arrange
		string url = "https://yandex.by/images/search?pos=0&from=tabbar&img_url=https%3A%2F%2Fwww.nastol.com.ua%2Fdownload.php%3Fimg%3D202109%2F2560x1600%2Fnastol.com.ua-473542.jpg&text=picture&rpt=simage&lr=157";

		// Act
		byte[] fileData = await FileDownloader.DownloadAsync(url);

		// Assert
		Assert.NotNull(fileData);
		Assert.NotEmpty(fileData);
	}

	[Fact]
	public async Task DownloadAsync_WhenInvalidUrl_ShouldThrowAnException()
	{
		// Arrange
		string url = "https://yandex-invalid.by/images/search?pos=0&from=tabbar&img_url=https%3A%2F%2Fwww.nastol.com.ua%2Fdownload.php%3Fimg%3D202109%2F2560x1600%2Fnastol.com.ua-473542.jpg&text=picture&rpt=simage&lr=157";

		// Act and Assert
		await Assert.ThrowsAsync<HttpRequestException>(async () => await FileDownloader.DownloadAsync(url));
	}
}