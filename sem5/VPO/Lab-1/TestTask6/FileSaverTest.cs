using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Task6;

namespace TestTask6;
public class FileSaverTest
{
	[Fact]
	public async Task SaveAsync_WhenValidDataAndPath_ShouldSaveSuccessfully()
	{
		// Arrange
		byte[] data = new byte[] { 0x1, 0x2, 0x3 };
		string testFolderPath = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString());
		Directory.CreateDirectory(testFolderPath);
		var path = Path.Combine(testFolderPath, Guid.NewGuid().ToString());

		// Act
		await FileSaver.SaveAsync(data, path);

		// Assert
		Assert.True(File.Exists(path));

		// Clean up
		Directory.Delete(testFolderPath, true);
	}

	[Fact]
	public async Task SaveAsync_WhenInvalidPath_ShouldThrowAnException()
	{
		// Arrange
		byte[] data = new byte[] { 0x1, 0x2, 0x3 };
		string testFolderPath = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString());
		Directory.CreateDirectory(testFolderPath);
		var path = Path.Combine(testFolderPath + "invalid", Guid.NewGuid().ToString());

		// Assert
		_ = await Assert.ThrowsAnyAsync<Exception>(async () => await FileSaver.SaveAsync(data, path));

		// Clean up
		Directory.Delete(testFolderPath, true);
	}
}
