using Task5;

namespace TestTask5;

public class FileSearcherTest
{
	[Fact]
	public void Search_WhenValidParameters_ShouldReturnCorrectFilePaths()
	{
		// Arrange
		string testFolderPath = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString());
		Directory.CreateDirectory(testFolderPath);

		try
		{
			string[] expectedFilePaths = new string[]
			{
				Path.Combine(testFolderPath, "file1.txt"),
				Path.Combine(testFolderPath, "subdir1","file2.txt"),
				Path.Combine(testFolderPath, "subdir2","file3.txt"),
				Path.Combine(testFolderPath, "subdir2","subdir3","file3.txt"),
			};

			string[] notExpectedFilePaths = new string[]
			{
				Path.Combine(testFolderPath, "file1.exe"),
				Path.Combine(testFolderPath, "subdir1","file2.docs"),
				Path.Combine(testFolderPath, "subdir2","subdir3","file3.pdf"),
			};

			foreach (string filePath in expectedFilePaths)
			{
				Directory.CreateDirectory(Path.GetDirectoryName(filePath)!);
				File.Create(filePath).Dispose();
			}

			foreach (string filePath in notExpectedFilePaths)
			{
				Directory.CreateDirectory(Path.GetDirectoryName(filePath)!);
				File.Create(filePath).Dispose();
			}

			// Act
			string[] result = FileSearcher.Search(testFolderPath, "txt");

			// Assert
			Assert.Equal(expectedFilePaths.Order(), result.Order());
		}
		finally
		{
			Directory.Delete(testFolderPath, true);
		}
	}

	[Fact]
	public void Search_WhenInvalidParameters_ShouldThrowAnException()
	{
		// Arrange
		string testFolderPath = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString());
		Directory.CreateDirectory(testFolderPath);

		// Assert
		Assert.ThrowsAny<Exception>(() => FileSearcher.Search(testFolderPath + "invalid", "txt"));
	}
}