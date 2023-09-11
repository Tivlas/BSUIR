namespace Task2;
public class Person
{
	public int Age { get; set; }
	public string FirstName { get; set; } = string.Empty;
	public string LastName { get; set; } = string.Empty;

	public override bool Equals(object? obj)
	{
		if (obj is Person p)
		{
			return p.LastName.Equals(LastName) && p.FirstName.Equals(FirstName) && p.Age.Equals(Age);
		}
		return base.Equals(obj);
	}
}
