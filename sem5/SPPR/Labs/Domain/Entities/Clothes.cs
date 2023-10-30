namespace Domain.Entities;
public class Clothes
{
	public int Id { get; set; }
	public int CategoryId { get; set; }
	public ClothesCategory? Category { get; set; }
	public string Name { get; set; } = string.Empty;
	public string Description { get; set; } = string.Empty;
	public string? ImagePath { get; set; }
	public string Mime { get; set; } = string.Empty;
	public decimal Price { get; set; }
}
