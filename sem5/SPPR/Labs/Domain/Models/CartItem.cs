using Domain.Entities;

namespace Domain.Models;
public class CartItem
{
	public Clothes Clothes { get; set; }
	public int Quantity { get; set; }
}
