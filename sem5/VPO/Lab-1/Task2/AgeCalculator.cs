namespace Task2;
public static class AgeCalculator
{
	/// <summary>
	/// Calculates minimum person age.
	/// </summary>
	/// <param name="people">List of <c>Person</c>.</param>
	/// <returns>Min age. If list is empty returns -1.</returns>
	public static int Min(List<Person> people)
	{
		return people.Count > 0 ? people.Min(p => p.Age) : -1;
	}

	/// <summary>
	/// Calculates maximum person age.
	/// </summary>
	/// <param name="people">List of <c>Person</c>.</param>
	/// <returns>Max age. If list is empty returns -1.</returns>
	public static int Max(List<Person> people)
	{
		return people.Count > 0 ? people.Max(p => p.Age) : -1;
	}

	/// <summary>
	/// Calculates average person age.
	/// </summary>
	/// <param name="people">List of <c>Person</c>.</param>
	/// <returns>Average age rounded to 2 fractional digits. If list is empty returns -1.</returns>
	public static double Avg(List<Person> people)
	{
		return people.Count > 0 ? Math.Round(people.Average(p => p.Age), 2) : -1;
	}
}
