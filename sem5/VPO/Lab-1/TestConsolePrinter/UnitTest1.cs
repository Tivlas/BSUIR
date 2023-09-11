namespace TestConsolePrinter;

public class UnitTest1
{
	[Fact]
	public void Main_WhenValidOutput()
	{
        for (int numberOfRuns = 0; numberOfRuns < 100; numberOfRuns++)
        {
			var stringWriter = new StringWriter();
			Console.SetOut(stringWriter);
			Program.Main();

			var actual = stringWriter.ToString().Trim().Split(Environment.NewLine);

			Assert.True(actual.Length == 3);
			Assert.Equal("Hello, world!", actual[0]);
			Assert.Equal("Andhiagain!", actual[1]);

			int exclamationMarkCount = actual[2].Count(c => c == '!');
			Assert.True(exclamationMarkCount >= 5 && exclamationMarkCount <= 50 && exclamationMarkCount == actual[2].Length); 
		}
	}
}