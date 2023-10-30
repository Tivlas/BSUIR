namespace Domain.Entities;
public class ClothesCategory
{
	public int Id { get; set; }
	public string Name { get; set; } = string.Empty;
	public string NormalizedName { get; set; } = string.Empty;
	public List<Clothes> Clothes { get; set; } = new();
}
