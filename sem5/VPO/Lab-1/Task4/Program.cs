namespace Task4;

internal class Program
{
	static void Main(string[] args)
	{
		int rowCount = 200;
		int columnCount = 5;

		string html = GenerateHtmlTable(rowCount, columnCount);
		File.WriteAllText("table.html", html);
	}

	static string GenerateHtmlTable(int rowCount, int columnCount)
	{
		string html = "<!DOCTYPE html>\n";
		html += "<html>\n<head>\n";
		html += "</head>\n<body>\n";
		html += "<style\n>" +
			   "table { border-collapse: collapse; width: 100%; }\n" +
			   "th, td { padding: 5px;}\n" +
			   "</style>\n";
		html += "<table>\n";

		for (int row = rowCount; row >= 0; row--)
		{
			html += "<tr>\n";
			for (int col = 0; col < columnCount; col++)
			{
				int grayValue = (int)Math.Round((row / (double)rowCount) * 255);
				string backgroundColor = $"rgb({grayValue}, {grayValue}, {grayValue})";

				html += $"<td style=\"background-color: {backgroundColor};\"></td>\n";
			}
			html += "</tr>\n";
		}

		html += "</table>\n";
		html += "</body>\n</html>";

		return html;
	}
}
